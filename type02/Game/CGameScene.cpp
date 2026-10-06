#include "stdafx.h"

#include "../mode.h"
#include "../Scene/IDScene.h"
#include "../SLG/IDSLG.h"

#include "IDGame.h"
#include "CGameScene.h"

// SLGスキップモード
//#define SLG_SKIP

namespace BMW{
namespace Game{

namespace{
void setContFileName(string& sFile)
{
	sFile.clear();
	sFile=BMW::sSaveFolder + "\\";
	sFile+=sContinue;
}
}

void CGameScene::OnInit(Task::CTaskContext* pContext)
{// スタックトップに呼ばれた時のモードが積まれている
	nMode_ = pContext->top();
	pContext->pop();
	setState(NORMAL);

	switch(nMode_)
	{
	case Scene::Mode::NEW: // ニューゲーム
		// 一番はじめの話を設定する
		pScenario_ = pContext->getApp()->getScenario().getScenario(pContext->getApp()->getExec().getStory());
		pContext->setScenarioData(smart_ptr<Scenario::CDataScenario>(pScenario_,false));
		pScenario_->beginBase();
		setState(NEXT);
	break;

	case Scene::Mode::LOAD: // データロード後
	{
		// クリア済みのシナリオを設定しておく
		pScenario_ = pContext->getApp()->getScenario().getScenario(pContext->getApp()->getExec().getStory());
		pContext->setScenarioData(smart_ptr<Scenario::CDataScenario>(pScenario_,false));
		// とりあえず、インターミッションを呼んでおく
		getTaskListCtrl()->callTaskList(Scene::ID::INTER,true);
	}
	break;

	case Scene::Mode::CONTINUE: // コンティニュー
	{		
		pScenario_ = pContext->getScenarioData().getPointer();
		// SLG IDを負の値にして、SLGシーンを呼び出す
		pContext->push(-1);
		getTaskListCtrl()->callTaskList(Scene::ID::SLG,true);
	}
	break;

	default:
		getTaskListCtrl()->returnTaskList();
	break;
	}
}

void CGameScene::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case NEXT:
	{// 次のシナリオ呼び出し
		setState(NORMAL);

		Scenario::CDataScenario::base_list::iterator it = pScenario_->nextBase();
		switch(it->getScene())
		{
		case Scenario::CDataScenarioBase::ADV:
		{// ADV呼び出し
		#ifdef BMW_DEBUG
			CDbg().Out("Call ADV %d",it->getID());
		#endif
			pContext->push(it->getID());
			getTaskListCtrl()->callTaskList(Scene::ID::ADV,true);
		}
		break;

		case Scenario::CDataScenarioBase::SLG:
		{// SLG呼び出し
		#ifdef BMW_DEBUG
			CDbg().Out("Call SLG %d",it->getID());
		#endif

		#ifndef SLG_SKIP
			pContext->push(0);
			pContext->push(it->getID());
			getTaskListCtrl()->callTaskList(Scene::ID::SLG,true);
		#else
			// SLGスキップモード
			actionNext(pContext);
		#endif

			// SLGに来たら、現在のシナリオとしてID設定
			pContext->getApp()->getExec().setStory(pContext->getApp()->getExec().getNextScenario());
		}
		break;

		case Scenario::CDataScenarioBase::ED:
		case Scenario::CDataScenarioBase::ED_RETURN:
		{// ED呼び出し、つうかジャンプ
		#ifdef BMW_DEBUG
			CDbg().Out("Call ED %d",it->getID());
		#endif
			pContext->push(it->getID());
			if(it->getScene()==Scenario::CDataScenarioBase::ED)
			{
				pContext->push(0);
				getTaskListCtrl()->jumpTaskList(Scene::ID::ED);
			}
			else
			{// ED_RETURNの時はまたGameシーンに戻ってくる
				pContext->push(1);
				getTaskListCtrl()->callTaskList(Scene::ID::ED,true);
			}
		}
		break;

		case Scenario::CDataScenarioBase::END:
		// 終了
		// クリア状況に応じて色々解禁する場所に飛ぶ
		#ifdef BMW_DEBUG
			CDbg().Out("END");
		#endif
			getTaskListCtrl()->callTaskList(Scene::ID::END,true);
		break;

		default:

		// 知らんIDじゃ、とりあえず、タイトルにもどっちゃえ
		#ifdef BMW_DEBUG
			CDbg().Out("UnknownScene");
		#endif
			getTaskListCtrl()->jumpTaskList(Scene::ID::TITLE);
		break;
		}
	}
	break;

	default: break;
	}
}

void CGameScene::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	// シーンの切り替わりにはキャッシュクリア
	pContext->getApp()->clearSeCache();
	
	switch(nID)
	{
	case Scene::ID::ADV:
	// ADVが終わったなら次へ
	#ifndef BMW_DEBUG
		actionNext(pContext);
	#else
		if(pContext->top()>=0) // 0以上ってことはやり直しもじゃ
			getTaskListCtrl()->callTaskList(Scene::ID::ADV,true);
		ef(pContext->top()==-2) // -2ってことはタイトルへ戻るもじゃ
			getTaskListCtrl()->jumpTaskList(Scene::ID::TITLE);
		else // そうじゃなければ普通に終了
			actionNext(pContext);
	#endif
	break;

	case Scene::ID::SLG:
	{
		// スタックトップは終了したSLG ID
		int nSlgID = pContext->top();
		pContext->pop();
		int nFlag = pContext->top();
		pContext->pop();
		// SLG後の分岐
		afterSLG(nSlgID, nFlag, pContext);
	}
	break;

	case Scene::ID::INTER:
	// インターミッションから呼ばれた
	// スタックトップが1だったら、次へ
		if(pContext->top())
		{
		#ifdef BMW_DEBUG
			CDbg().Out("NEXT %d",pContext->getApp()->getExec().getNextScenario());
		#endif

			pScenario_ = pContext->getApp()->getScenario().getScenario(pContext->getApp()->getExec().getNextScenario());
			pContext->setScenarioData(smart_ptr<Scenario::CDataScenario>(pScenario_,false));
			pScenario_->beginBase();
			setState(NEXT);
		}
		else
		{// そうじゃなかったら、タイトルへ
			getTaskListCtrl()->jumpTaskList(Scene::ID::TITLE);
			setState(NORMAL);
		}
		pContext->pop();
	break;

	case Scene::ID::ED:
		actionNext(pContext);
	break;

	case Scene::ID::END:
		#ifdef BMW_DEBUG
			CDbg().Out("RETURN END %d",pContext->top());
		#endif
		// スタックトップが1だったら、次へ
		if(pContext->top())
		{
			actionNext(pContext);
		}
		else
		{// そうじゃなかったら、タイトルへ
			getTaskListCtrl()->jumpTaskList(Scene::ID::TITLE);
			setState(NORMAL);
		}
		pContext->pop();
	break;

	default: break;
	}
}

void CGameScene::actionNext(Task::CTaskContext* pContext)
{
	// 次のシナリオ判定
	if(pScenario_->endBase())
	{// 次に行くシナリオがなかったら
	 // インターミッションへ
		getTaskListCtrl()->callTaskList(Scene::ID::INTER,true);
		setState(NORMAL);
	}
	else
	{	setState(NEXT); }
}

void CGameScene::afterSLG(int nSlgID, int nFlag, Task::CTaskContext* pContext)
{// SLG後の分岐
	// 終了した時のフラグによって分岐
	switch(nFlag)
	{
	case SLG::Victory::VICTORY:
	{// 勝ったなら次へ
		if(nMode_==Scene::Mode::CONTINUE)
		{// コンテニューからだと、pScenario_が設定されてないので、設定する
			nMode_=Scene::Mode::LOAD;
			pScenario_->progBase(nSlgID);
		}
		actionNext(pContext);
	}
	break;

	case SLG::Victory::NO:
	{// 何もなしなら、中断されたので、タイトルへ戻る
		getTaskListCtrl()->jumpTaskList(Scene::ID::TITLE);
		setState(NORMAL);
	}
	break;

#ifdef BMW_DEBUG
	case SLG::Victory::RESTART:
	{// リスタート
		// 熟練度
		pContext->push(0);
		pContext->push(nSlgID);
		getTaskListCtrl()->callTaskList(Scene::ID::SLG,true);
	}
	break;
#endif

	default:
	{// 負けたらもう一回SLGか、どうかはFlag次第
		if(nSlgID>=0)
		{// やり直しだってさ
			pContext->getApp()->getExec().incContinue();
			// 熟練度無効
			pContext->push(-1); 
		}
		else
		{// 即コンテニューだったら、セーブデータ復元
			CSerialize s;
			s.SetStoring(false);
			string sFile;
			setContFileName(sFile);
			s.Load(sFile);
			Save::CExecData& save = pContext->getApp()->getExec();
			save.clear();
			s << save;
			Scenario::CDataScenario* pScn = pContext->getApp()->getScenario().getScenario(save.getStory());
			pContext->setScenarioData(smart_ptr<Scenario::CDataScenario>(pScn,false));
			// コンテニューモードにしておく
			nMode_=Scene::Mode::CONTINUE;
		}
		// もし、セーブデータからなら、nIDは負であるので、コンテニューモードで立ち上がる
		pContext->push(nSlgID);
		getTaskListCtrl()->callTaskList(Scene::ID::SLG,true);
	}
	break;
	}
}

} // namespace Game end
} // namespace BMW end