#include "stdafx.h"

#include "../Hero/IDHero.h"
#include "../Scene/IDScene.h"

#include "CEndScene.h"

namespace BMW{
namespace END{

void CEndScene::Task(Task::CTaskContext* pContext)
{
	context_.action(pContext->IsAction());

	release_.Task(pContext);

	if(pContext->IsAction() && IsValid())
		OnAction(pContext);
}

void CEndScene::OnInit(Task::CTaskContext* pContext)
{
	// コンテキスト設定
	setContext(pContext);

	// GUI読み込み
	setGuiDefDB("END");

	// IDリセット
	listGuiID_.clear();
	// フレームリセット
	nFrame_=0;

	// ひとまず、フェード
	Scene::CFoward::FaderEvent funFade(this,&CEndScene::eventFade);
	context_.getApp()->getFoward()->setFaderHandler(funFade);

	// クリア情報保存
	Save::CExecData& exec = pContext->getApp()->getExec();
	Save::CGlobalData& global = pContext->getApp()->getGlobal();

	// 53話への遷移はここで判定
	Scenario::CScenarioDB& scn = pContext->getApp()->getScenario();
	// 次のシナリオが青子クリアのやつはアンバー戦にいくかも？
	if(exec.getNextScenario()==scn.getScenarioID("CLEAR_GOOD") // 青子戦クリア済み
	&& exec.getExpert()==EXPERT_MAX) // 熟練度全取り 
	{
		// 52話クリア済みと出すためにこうしておく
		exec.setStory(scn.getScenarioID("52_C"));
		// 52話にずらす
		pContext->setScenarioData(smart_ptr<Scenario::CDataScenario>(scn.getScenario("52_C"),false));
		// 53話へ
		exec.setNextScenario(scn.getScenarioID("53_C"));
		// 次へ
		pContext->push(1);
		setState(END);
	}
	else
	{// 終了！！
		// クリア状態に合わせて解放されるのが変わる
		if(// 匠はクリア済みだが陽菜はまだ
			(  global.IsTakumiClear()
			&& exec.getHero()==Hero::Target::HARUNA
			&& !global.IsHarunaClear()
			)
		||// 陽菜はクリア済みだが匠はまだ
			(  global.IsHarunaClear()
			&& exec.getHero()==Hero::Target::TAKUMI
			&& !global.IsTakumiClear()
			)
		)
		{// はじめて両ルートクリア時
		 // コメント解放
			listGuiID_.push_back("KOKUCHI2_G");
		}
		ef(!global.IsClear())
		{// 初めてのクリア！
			listGuiID_.push_back("KOKUCHI1_G");
		}
		
		if(exec.getNextScenario()==scn.getScenarioID("CLEAR_TRUE")
		&& !global.IsAmberClear())
		{// アンバー戦初めてクリア！
			listGuiID_.push_back("KOKUCHI3_G");
		}

		// タイトルへ戻るよ
		pContext->push(0);

		if(!listGuiID_.empty())
		{// 表示すべき何かがある
			it_ = listGuiID_.begin();

			Movie::CSymbolDB& symbolDB = getGuiDefDB().getSymbolDB();
			symbolDB.setGraphicGui(&release_,*it_);

			context_.getApp()->getFoward()->fadeOut();
			setState(FADE);
		}
		else
		{// なにもないなら、そのまま終了
			setState(END);
		}

		// クリアした主人公設定
		// 他のクリア時情報は引継ぎシーンでごちゃごちゃやる
		if(exec.getHero()==Hero::Target::TAKUMI)
			global.takumiClear(true);
		else
			global.harunaClear(true);

		// アンバー戦クリア
		if(exec.getNextScenario()==scn.getScenarioID("CLEAR_TRUE"))
			global.amberClear(true);
	}
}

void CEndScene::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case WAIT:
		if(++nFrame_>=120
		|| Input::releaseCancel(&context_)
		|| Input::releaseOK(&context_))
		{
			++it_;
			if(it_==listGuiID_.end())
			{// 表示するパネルないよ！
				context_.getInput()->guard(true);
				context_.getApp()->getFoward()->fadeIn();
				setState(FADE_END);
			}
			else
			{// 次のパネル
				Movie::CSymbolDB& symbolDB = getGuiDefDB().getSymbolDB();
				symbolDB.setGraphicGui(&release_,*it_);

				// 念のため入力リセット
				context_.getInput()->resetInputState();
			}
		}
	break;


	case END: // 一個戻る
		getTaskListCtrl()->returnTaskList();
	break;

	default: break;
	}
}

void CEndScene::eventFade(BMW::Task::CTaskContext* pContext)
{
	if(getState()==FADE)
	{
		context_.getInput()->guard(false);
		setState(WAIT);
	}
	else // 終了
	{
		setState(END);
	}
}

} // namespace END end
} // namespace BMW end