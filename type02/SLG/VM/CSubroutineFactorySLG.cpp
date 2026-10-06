#include "stdafx.h"

#include "../../mode.h"

#include "../../Chara/IDChara.h"
#include "../../Sound/Code/CCode_se_wait.h"
#include "../../ADV/CAdvCmdFactory.h"

#include "../IDRule.h"
#include "../SlgApi.h"
#include "../Context/CSLGDef.h"
#include "../Context/CSLGContext.h"
#include "../Action/IDAction.h"

#include "../various/CContinue_start.h"
#include "../various/CData_ref.h"

#include "CSubroutineFactorySLG.h"

namespace BMW{
namespace SLG{

smart_ptr<Task::ITaskList> CSubroutineFactorySLG::createTaskList(int nID)
{
	if(nID>=0 && nID<Rule::USER_DEF)
	{// API
		return mapApi_[nID];
	}
	else
	{// USER_DEF以降は文字通りユーザー定義スクリプト
		return createTaskListUser(nID);
	}
}

smart_ptr<Task::ITaskList> CSubroutineFactorySLG::createTaskListUser(int nID)
{// 基本的には普通にスクリプト定義なので、そうする
	VM::CScriptExec* pExec = new VM::CScriptExec();
	pExec->setScript(pSlgDef_->getScript(nID));
	return smart_ptr<Task::ITaskList>(pExec);
}

//////////////////////////////////////////////////
// 初期化
//////////////////////////////////////////////////
void CSubroutineFactorySLG::OnInit(CSLGContext* pContext)
{// SLGイベントスクリプトファイルから、スクリプトを展開する
	// API系は、ここでnewしておく
	// OnInitはCVMから呼ばれるので、代わりにOnResetで初期化してしまう
	// つまり、OnInitとOnResetがCtrlChacheに対して逆動作する

	// エントリ関数を設定
	VM::CScriptExec* pMain = new VM::CScriptExec();
	VM::CScript* pMainScript = new VM::CScript();
	pMain->setScript(smart_ptr<VM::CScript>(pMainScript));
	VM::Code::CCode_call* pCall;

	// SLG_STARTから始まって
	pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::SLG_START);
	pMainScript->addCode(pCall);

	// MENU_SELECTってのは、ようはユーザが操作できる状態
	pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::MENU_SELECT);
	pMainScript->addCode(pCall);

	// PENALTY_RULE。MENU_SELECTが終了ってことは、SLGが終わり
	pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::PENALTY_RULE);
	pMainScript->addCode(pCall);

	// DATA_REF。ってわかりにくいけど、ようはセーブ
	pMainScript->addCode(new Data::CData_ref());

	// SLG_END。最後のイベント！
	pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::SLG_END);
	pMainScript->addCode(pCall);
	pMainScript->addCode(new VM::Code::CCode_ret());

	// エントリポイントとして設定ー
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MAIN,smart_ptr<Task::ITaskList>(pMain)));

	// コンテニュースタート
	CContinue_start* pContStart = new CContinue_start();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::CONTINUE_START,smart_ptr<Task::ITaskList>(pContStart)));

	// メニュー系
	// メニューセレクト
	Menu::CMenu_select* pSelect = new Menu::CMenu_select();
	pSelect->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MENU_SELECT,smart_ptr<Task::ITaskList>(pSelect)));
	// メニューセレクト for player
	Menu::CMenu_select_player* pSelectPlayer = new Menu::CMenu_select_player();
	pSelectPlayer->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MENU_SELECT_PLAYER,smart_ptr<Task::ITaskList>(pSelectPlayer)));
	// メニューセレクト for cpu
	Menu::CMenu_select_cpu* pSelectCpu = new Menu::CMenu_select_cpu();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MENU_SELECT_CPU,smart_ptr<Task::ITaskList>(pSelectCpu)));

	// キャラメニュー
	//Menu::CMenu_chara* pChara = new Menu::CMenu_chara();
	//pChara->OnReset(pContext);
	//mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MENU_CHARA,smart_ptr<Task::ITaskList>(pChara)));
	// キャラメニュー2
	Menu::CMenu_chara2* pChara2 = new Menu::CMenu_chara2();
	pChara2->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MENU_CHARA,smart_ptr<Task::ITaskList>(pChara2)));
	// ターンメニュー
	Menu::CMenu_turn* pTurn = new Menu::CMenu_turn();
	pTurn->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MENU_TURN,smart_ptr<Task::ITaskList>(pTurn)));
	// ステータスメニュー
	Menu::CMenu_status* pStatus = new Menu::CMenu_status();
	pStatus->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MENU_STATUS,smart_ptr<Task::ITaskList>(pStatus)));

	// フェーズ
	Phase::CPhase_init* pPhase = new Phase::CPhase_init();
	pPhase->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::PHASE_INIT,smart_ptr<Task::ITaskList>(pPhase)));

	// システム
	System::CSystem_rule* pSystem = new System::CSystem_rule();
	pSystem->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::SYSTEM_RULE,smart_ptr<Task::ITaskList>(pSystem)));

	// 中断
	// セーブルール
	Save::CSave_rule* pSaveRule = new Save::CSave_rule();
	pSaveRule->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::SAVE_RULE,smart_ptr<Task::ITaskList>(pSaveRule)));
	// 終了ルール
	Exit::CExit_rule* pExitRule = new Exit::CExit_rule();
	pExitRule->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::EXIT_RULE,smart_ptr<Task::ITaskList>(pExitRule)));

	// 移動系
	// 移動ルール
	Move::CMove_rule* pMoveRule = new Move::CMove_rule();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MOVE_RULE,smart_ptr<Task::ITaskList>(pMoveRule)));
	// 移動ルール for CPU
	Move::CMove_rule_cpu* pMoveRuleCpu = new Move::CMove_rule_cpu();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MOVE_RULE_CPU,smart_ptr<Task::ITaskList>(pMoveRuleCpu)));
	// 移動範囲計算
	Move::CMove_range* pMoveRange = new Move::CMove_range();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MOVE_RANGE,smart_ptr<Task::ITaskList>(pMoveRange)));
	// 移動範囲表示 こいつはニーモニック扱い
	//Move::CMove_view* pMoveView = new Move::CMove_view();
	//mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MOVE_VIEW,smart_ptr<Task::ITaskList>(pMoveView)));
	// 移動位置選択
	Move::CMove_select* pMoveSelect = new Move::CMove_select();
	pMoveSelect->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MOVE_SELECT,smart_ptr<Task::ITaskList>(pMoveSelect)));
	// 移動ルート計算
	Move::CMove_road* pMoveRoad = new Move::CMove_road();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MOVE_ROAD,smart_ptr<Task::ITaskList>(pMoveRoad)));
	// 移動実行
	Move::CMove_exec2* pMoveExec = new Move::CMove_exec2();
	//Move::CMove_exec* pMoveExec = new Move::CMove_exec();
	pMoveExec->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MOVE_EXEC,smart_ptr<Task::ITaskList>(pMoveExec)));

	// 攻撃系
	// 攻撃ルール
	//Attack::CAttack_rule* pAttackRule = new Attack::CAttack_rule();
	//mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ATTACK_RULE,smart_ptr<Task::ITaskList>(pAttackRule)));
	// 攻撃ルール2
	Attack::CAttack_rule2* pAttackRule2 = new Attack::CAttack_rule2();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ATTACK_RULE,smart_ptr<Task::ITaskList>(pAttackRule2)));
	// 攻撃ルール for CPU
	Attack::CAttack_rule_cpu* pAttackRuleCpu = new Attack::CAttack_rule_cpu();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ATTACK_RULE_CPU,smart_ptr<Task::ITaskList>(pAttackRuleCpu)));
	// 武器選択
	Attack::CAttack_weapon* pAttackWeapon = new Attack::CAttack_weapon();
	pContext->push(0); // 通常用と援護用で若干動作が違うその切り分け
	pAttackWeapon->OnReset(pContext);
	pContext->pop();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ATTACK_WEAPON,smart_ptr<Task::ITaskList>(pAttackWeapon)));
	// 攻撃範囲計算
	//Attack::CAttack_range*	pAttackRange = new Attack::CAttack_range();
	//mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ATTACK_RANGE,smart_ptr<Task::ITaskList>(pAttackRange)));
	// 攻撃範囲計算2
	Attack::CAttack_range2*	pAttackRange2 = new Attack::CAttack_range2();
	pAttackRange2->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ATTACK_RANGE,smart_ptr<Task::ITaskList>(pAttackRange2)));
	// 攻撃対象選択
	//Attack::CAttack_select*	pAttackSelect = new Attack::CAttack_select();
	//pAttackSelect->OnReset(pContext);
	//mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ATTACK_SELECT,smart_ptr<Task::ITaskList>(pAttackSelect)));
	// 攻撃対象選択2
	Attack::CAttack_select2* pAttackSelect2 = new Attack::CAttack_select2();
	pAttackSelect2->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ATTACK_SELECT,smart_ptr<Task::ITaskList>(pAttackSelect2)));
	// 攻撃行動表
	Attack::CAttack_action*	pAttackAction = new Attack::CAttack_action();
	pAttackAction->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ATTACK_ACTION,smart_ptr<Task::ITaskList>(pAttackAction)));
	// 攻撃デモ
	Attack::CAttack_demo*	pAttackDemo = new Attack::CAttack_demo();
	pAttackDemo->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ATTACK_DEMO,smart_ptr<Task::ITaskList>(pAttackDemo)));
	// 攻撃適用
	Attack::CAttack_apply*	pAttackApply = new Attack::CAttack_apply();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ATTACK_APPLY,smart_ptr<Task::ITaskList>(pAttackApply)));
	// 攻撃後のキャラマップ削除
	Attack::CAttack_del*	pAttackDel = new Attack::CAttack_del();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ATTACK_DEL,smart_ptr<Task::ITaskList>(pAttackDel)));
	// 攻撃結果
	Attack::CAttack_result*	pAttackResult = new Attack::CAttack_result();
	pAttackResult->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ATTACK_RESULT,smart_ptr<Task::ITaskList>(pAttackResult)));

	// フィールド攻撃系
	// フィールドルール
	Attack::CField_rule* pFieldRule = new Attack::CField_rule();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::FIELD_RULE,smart_ptr<Task::ITaskList>(pFieldRule)));
	// フィールド選択
	Attack::CField_select* pFieldSelect = new Attack::CField_select();
	pFieldSelect->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::FIELD_SELECT,smart_ptr<Task::ITaskList>(pFieldSelect)));
	// フィールド攻撃計算
	Attack::CField_attack* pFieldAttack = new Attack::CField_attack();
	pFieldAttack->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::FIELD_ATTACK,smart_ptr<Task::ITaskList>(pFieldAttack)));
	// フィールドエフェクト
	//Attack::CField_effect* pFieldEffect = new Attack::CField_effect();
	//mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::FIELD_EFFECT,smart_ptr<Task::ITaskList>(pFieldEffect)));

	// 精神系
	// 精神ルール
	Spirit::CSpirit_rule* pSpiritRule = new Spirit::CSpirit_rule();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::SPIRIT_RULE,smart_ptr<Task::ITaskList>(pSpiritRule)));
	// 精神メニュー
	Spirit::CSpirit_menu*	pSpiritMenu = new Spirit::CSpirit_menu();
	pSpiritMenu->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::SPIRIT_MENU,smart_ptr<Task::ITaskList>(pSpiritMenu)));
	// 精神セレクト
	Spirit::CSpirit_select*	pSpiritSelect = new Spirit::CSpirit_select();
	pSpiritSelect->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::SPIRIT_SELECT,smart_ptr<Task::ITaskList>(pSpiritSelect)));
	// 精神適用
	Spirit::CSpirit_apply*	pSpiritApply = new Spirit::CSpirit_apply();
	pSpiritApply->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::SPIRIT_APPLY,smart_ptr<Task::ITaskList>(pSpiritApply)));

	// アイテム系
	// アイテムルール
	Item::CItem_rule*	pItemRule = new Item::CItem_rule();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ITEM_RULE,smart_ptr<Task::ITaskList>(pItemRule)));
	// アイテムメニュー
	Item::CItem_menu*	pItemMenu = new Item::CItem_menu();
	pItemMenu->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ITEM_MENU,smart_ptr<Task::ITaskList>(pItemMenu)));
	// アイテム適用
	Item::CItem_apply*	pItemApply = new Item::CItem_apply();
	pItemApply->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ITEM_APPLY,smart_ptr<Task::ITaskList>(pItemApply)));

	// 治癒ルール
	Cure::CCure_rule* pCureRule = new Cure::CCure_rule();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::CURE_RULE,smart_ptr<Task::ITaskList>(pCureRule)));

	// 補給ルール
	Refill::CRefill_rule* pRefillRule = new Refill::CRefill_rule();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::REFILL_RULE,smart_ptr<Task::ITaskList>(pRefillRule)));
	// 補給適用
	Refill::CRefill_apply* pRefillApply = new Refill::CRefill_apply();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::REFILL_APPLY,smart_ptr<Task::ITaskList>(pRefillApply)));

	// ステータスルール
	Status::CStatus_rule* pStatusRule = new Status::CStatus_rule();
	pStatusRule->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::STATUS_RULE,smart_ptr<Task::ITaskList>(pStatusRule)));

	// 告白ルール
	Love::CLove_rule* pLoveRule = new Love::CLove_rule();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::LOVE_RULE,smart_ptr<Task::ITaskList>(pLoveRule)));

	// 待機
	Wait::CWait_rule* pWaitRule = new Wait::CWait_rule();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::WAIT_RULE,smart_ptr<Task::ITaskList>(pWaitRule)));

	// デモ
	Demo::CDemo_map* pDemoMap = new Demo::CDemo_map();
	pDemoMap->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::DEMO_MAP,smart_ptr<Task::ITaskList>(pDemoMap)));

	// 勝利条件確認
	Victory::CVictory_view* pVictView = new Victory::CVictory_view();
	pVictView->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::VICTORY_VIEW,smart_ptr<Task::ITaskList>(pVictView)));
	// 条件変更告知
	Victory::CVictory_change* pVicChange = new Victory::CVictory_change();
	pVicChange->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::VICTORY_CHANGE,smart_ptr<Task::ITaskList>(pVicChange)));

	// 出撃系
	// 出撃キャラ選択
	Sally::CSally_select* pSallySelect = new Sally::CSally_select();
	pSallySelect->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::SALLY_SELECT,smart_ptr<Task::ITaskList>(pSallySelect)));
	// キャラ追加
	Sally::CSally_add_chara* pAddChara = new Sally::CSally_add_chara();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ADD_CHARA,smart_ptr<Task::ITaskList>(pAddChara)));
	// キャラ削除
	Sally::CSally_del_chara* pDelChara = new Sally::CSally_del_chara();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::DEL_CHARA,smart_ptr<Task::ITaskList>(pDelChara)));
	// キャラ交代
	Sally::CSally_change_chara* pChangeChara = new Sally::CSally_change_chara();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::CHANGE_CHARA,smart_ptr<Task::ITaskList>(pChangeChara)));
	// キャラのマップへの追加
	//Sally::CSally_add_chara_map* pAddCharaMap = new Sally::CSally_add_chara_map();
	Sally::CSally_add_chara_map2* pAddCharaMap = new Sally::CSally_add_chara_map2();
	//Sally::CSally_add_chara_map3* pAddCharaMap = new Sally::CSally_add_chara_map3();
	pAddCharaMap->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ADD_CHARA_MAP,smart_ptr<Task::ITaskList>(pAddCharaMap)));
	// キャラのマップからの削除
	Sally::CSally_del_chara_map* pDelCharaMap = new Sally::CSally_del_chara_map();
	pDelCharaMap->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::DEL_CHARA_MAP,smart_ptr<Task::ITaskList>(pDelCharaMap)));
	// 武器のセットアップ
	Sally::CSally_setup_weapon* pSetWeapon = new Sally::CSally_setup_weapon();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::SETUP_WEAPON,smart_ptr<Task::ITaskList>(pSetWeapon)));
	// 説得キャラの設定（ニーモニック扱い）
	//Sally::CSally_set_pers* pSetPers = new Sally::CSally_set_pers();
	//mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::SET_PERS,smart_ptr<Task::ITaskList>(pSetPers)));

	// 出撃キャラ一覧
	Sally::CSally_view* pSallyView = new Sally::CSally_view();
	pSallyView->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::SALLY_VIEW,smart_ptr<Task::ITaskList>(pSallyView)));

	// 精神検索
	Sally::CSally_spirit* pSallySpirit = new Sally::CSally_spirit();
	pSallySpirit->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::SALLY_SPIRIT,smart_ptr<Task::ITaskList>(pSallySpirit)));

	// ペナルティ
	Penalty::CPenalty_rule* pPena = new Penalty::CPenalty_rule();
	pPena->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::PENALTY_RULE,smart_ptr<Task::ITaskList>(pPena)));

	// メッセージボード(ニーモニック扱い)
	//Event::CEvent_Msg* pEventMsg = new Event::CEvent_Msg();
	//pEventMsg->OnReset(pContext);
	//mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MES_BOARD,smart_ptr<Task::ITaskList>(pEventMsg)));
	// 入力ウェイト
	ADV::API::CWait_input* pWaitInput = new ADV::API::CWait_input();
	pWaitInput->setState(Rule::MSG_BACK);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::WAIT_INPUT,smart_ptr<Task::ITaskList>(pWaitInput)));
	// フェードウェイト
	ADV::API::CWait_fade* pWaitFade = new ADV::API::CWait_fade();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::WAIT_FADE,smart_ptr<Task::ITaskList>(pWaitFade)));
	// SEウェイト
	Sound::Code::CCode_se_wait* pWaitSe = new Sound::Code::CCode_se_wait();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::WAIT_SE,smart_ptr<Task::ITaskList>(pWaitSe)));
	// 出撃WAIT
	//Sally::CWait_sally* pWaitSally = new Sally::CWait_sally();
	//mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::WAIT_SALLY,smart_ptr<Task::ITaskList>(pWaitSally)));
	// エフェクトウェイト
	Effect::CWait_symbol* pWaitEffect = new Effect::CWait_symbol();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::WAIT_EFFECT,smart_ptr<Task::ITaskList>(pWaitEffect)));
	// イベント戦闘
	Event::CEvent_Battle2* pEventBattle = new Event::CEvent_Battle2();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::EVENT_BATTLE,smart_ptr<Task::ITaskList>(pEventBattle)));
	// フレームウェイト
	CWait_frame* pWaitFrame = new CWait_frame();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::WAIT_FRAME,smart_ptr<Task::ITaskList>(pWaitFrame)));
	// バックログ
	Event::CEvent_Msg_back* pEventMsgBack = new Event::CEvent_Msg_back();
	pEventMsgBack->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MSG_BACK,smart_ptr<Task::ITaskList>(pEventMsgBack)));

	// マップ系
	// カーソル誘導
	Map::CMap_cursol* pCursol =new Map::CMap_cursol();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MAP_CURSOL,smart_ptr<Task::ITaskList>(pCursol)));
	// マップスクロール
	Map::CMap_scroll* pScroll =new Map::CMap_scroll();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MAP_SCROLL,smart_ptr<Task::ITaskList>(pScroll)));
}

void CSubroutineFactorySLG::setScript(CSLGContext* pContext)
{
	// 勝利条件チェック
	Victory::CVictory_check2* pVictCheck = new Victory::CVictory_check2();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::VICTORY_CHECK,smart_ptr<Task::ITaskList>(pVictCheck)));
	// イベント系
	// SLGスタート
	CEvent_base* pEventBase = new CEvent_base(pContext->getSLGDef().getScriptID("SLG_START_EVENT"));
#ifdef BMW_DEBUG
	pEventBase->setEventName("SLG_START_EVENT");
#endif
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::SLG_START,smart_ptr<Task::ITaskList>(pEventBase)));

	// SLGエンド
	pEventBase = new CEvent_base(pContext->getSLGDef().getScriptID("SLG_END_EVENT"));
#ifdef BMW_DEBUG
	pEventBase->setEventName("SLG_END_EVENT");
#endif
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::SLG_END,smart_ptr<Task::ITaskList>(pEventBase)));

	// フェーズスタート
	pEventBase = new CEvent_base(pContext->getSLGDef().getScriptID("PHASE_START_EVENT"));
#ifdef BMW_DEBUG
	pEventBase->setEventName("PHASE_START_EVENT");
#endif
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::PHASE_START,smart_ptr<Task::ITaskList>(pEventBase)));

	// フェーズエンド
	pEventBase = new CEvent_base(pContext->getSLGDef().getScriptID("PHASE_END_EVENT"));
#ifdef BMW_DEBUG
	pEventBase->setEventName("PHASE_END_EVENT");
#endif
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::PHASE_END,smart_ptr<Task::ITaskList>(pEventBase)));

	// バトルスタート
	pEventBase = new CEvent_base(pContext->getSLGDef().getScriptID("BATTLE_START_EVENT"));
#ifdef BMW_DEBUG
	pEventBase->setEventName("BATTLE_START_EVENT");
#endif
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::BATTLE_START,smart_ptr<Task::ITaskList>(pEventBase)));

	// バトルエンド
	pEventBase = new CEvent_base(pContext->getSLGDef().getScriptID("BATTLE_END_EVENT"));
#ifdef BMW_DEBUG
	pEventBase->setEventName("BATTLE_END_EVENT");
#endif
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::BATTLE_END,smart_ptr<Task::ITaskList>(pEventBase)));

	// キャラエンド
	pEventBase = new CEvent_base(pContext->getSLGDef().getScriptID("CHARA_END_EVENT"));
#ifdef BMW_DEBUG
	pEventBase->setEventName("CHARA_END_EVENT");
#endif
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::CHARA_END,smart_ptr<Task::ITaskList>(pEventBase)));
	// 告白スタート
	pEventBase = new CEvent_base(pContext->getSLGDef().getScriptID("LOVE_START_EVENT"));
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::LOVE_START,smart_ptr<Task::ITaskList>(pEventBase)));
}

} // namespace SLG end
} // namespace BMW end