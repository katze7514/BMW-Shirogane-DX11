#include "stdafx.h"

#include "../Scene/IDScene.h"
#include "CDictCharaFactory.h"
#include "CDictCharaParser.h"
#include "CDictCharaScene.h"

namespace BMW{
namespace Dict{

void CDictCharaScene::OnInit(Task::CTaskContext* pContext)
{
	// コンテキストのコピー
	setContext(pContext);
	context_.setBattleData(pContext->getBattleData());

	// 辞書ファイルの読み込み
	setDictChara();

	// GUI読み込み
	setGuiDefDB("DICT");

	// NOWLOADING表示
	pContext->getApp()->getFoward()->visibleDictCaution(true);

	// LOADING表示するため1フレ待つ
	setState(INIT_READY);
}

void CDictCharaScene::OnAction(Task::CTaskContext* pContext)
{
#ifdef BMW_DEBUG
	const DWORD INTERVAL=0;
#else
	const DWORD INTERVAL=2500;
#endif

	switch(getState())
	{
	case INIT_READY:
		loadTimer_.Reset();
		setState(INIT);
	break; 

	case INIT:
	{
		// キャラ辞典のListCtrl設定
		Task::CTaskListCtrlChache* pListCtrl = new Task::CTaskListCtrlChache(smart_ptr<Task::ITaskListFactory>(new CDictCharaFactory()));
		addTask(pListCtrl, CTRL);

		pListCtrl->createChache(SELECT,&context_);
		pListCtrl->createChache(VIEW,&context_);

	#ifdef BMW_DEBUG
		pListCtrl->createChache(FACE,&context_);
		pListCtrl->createChache(SYMBOL,&context_);
	#endif

		// キャラセレクトから
		pListCtrl->jumpTaskList(SELECT);

		setState(INIT_END);
	}
	break;

	case INIT_END:
		// 2.5秒はみておけ
		if(loadTimer_.Get()>=INTERVAL)
		{
			// ローディング消す
			pContext->getApp()->getFoward()->visibleDictCaution(false);
			// FadeOut
			setFadeOut();
		}
	break;

	case DEMO: // デモへ飛ぶ！
		context_.getApp()->getFoward()->fadeIn();
		setState(DEMO_JUMP);
	break;

	case END: // 終了ー
		context_.getApp()->getFoward()->fadeIn();
		context_.getBgmSound()->FadeOut(30);

		setState(FADE_END);
	break;

	default: break;
	}
}

void CDictCharaScene::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	// デモシーンから戻ってきたよ
	setFadeOut();
}

void CDictCharaScene::eventFade(Task::CTaskContext* pContext)
{
	if(getState()==FADE)
	{// FADE終了
		context_.getInput()->cursolVisible(true);
		context_.getInput()->guard(false);
		setState(NORMAL);
	}
	ef(getState()==DEMO_JUMP)
	{
		// FADE_INが終わったら、DEMOへ！
		context_.getApp()->getFoward()->clearPopUp();
		getTaskListCtrl()->callTaskList(Scene::ID::DEMO,true);
		setState(NORMAL);
	}
	else
	{// 終了ー
		pContext->getApp()->getFoward()->clearPopUp();
		getTaskListCtrl()->jumpTaskList(Scene::ID::TITLE);
	}
}

__inline void CDictCharaScene::setFadeOut()
{
	// イベントハンドラ設定
	Scene::CFoward::FaderEvent fun;
	fun.set(this,&CDictCharaScene::eventFade);
	context_.getApp()->getFoward()->setFaderHandler(fun);
	// FadeOut
	context_.getApp()->getFoward()->fadeOut();
	// BGM
	context_.getBgmSound()->change("STATUS");
	context_.getBgmSound()->FadeIn(30);

	setState(FADE);
}

void CDictCharaScene::setDictChara()
{
	string dictFile;

//#ifdef BMW_DEBUG
//	dictFile = Config::Const::configDB_.getConfigFileStr("DICT_CHARA_DEBUG");
//#else
//	if(!context_.getApp()->getGlobal().IsAmberClear())
//		dictFile = Config::Const::configDB_.getConfigFileStr("DICT_CHARA");
//	else
		dictFile = Config::Const::configDB_.getConfigFileStr("DICT_CHARA_AMBER");
//#endif
	
	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(dictFile);
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	using namespace boost::spirit;
	using namespace phoenix;

	// 構文解析
	CDictCharaParser ps(&context_);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("%s 読み込み失敗！！", r.stop);
#endif
}

} // namespace Dict end
} // namespace BMW end
