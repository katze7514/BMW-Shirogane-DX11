/*
	katze 05/04/27
	update 07/02/17
	SLGシーンのAPI
*/
#pragma once

// コード系
#include "Code/CCode_chara2slg.h"
#include "Code/CCode_slg2chara.h"
#include "Code/CCode_get_target.h"
#include "Code/CCode_set_target.h"
#include "Code/CCode_get_chara.h"
#include "Code/CCode_get_ctrl_chara.h"
#include "Code/CCode_set_ctrl_chara.h"
#include "Code/CCode_get_phase.h"
#include "Code/CCode_get_turn.h"
#include "Code/CCode_cond_call.h"
#include "Context/CCode_chara.h"
#include "Code/CCode_slg_end.h"
#include "Code/CCode_slg2valid.h"

// メニュー系
#include "Menu/CMenu_select.h"
#include "Menu/CMenu_select_player.h"
#include "Menu/CMenu_select_cpu.h"

//#include "Menu/CMenu_chara.h"
#include "Menu/CMenu_chara2.h"
#include "Menu/CMenu_turn.h"
#include "Menu/CMenu_status.h"

// フェーズ
#include "Phase/CPhase_init.h"

// システム
#include "System/CSystem_rule.h"

// 中断
#include "Save_Exit/CSave_rule.h"
#include "Save_Exit/CExit_rule.h"

// 移動系
#include "Move/CMove_rule.h"
#include "Move/CMove_rule_cpu.h"
#include "Move/CMove_range.h"
#include "Move/CMove_view.h"
#include "Move/CMove_select.h"
#include "Move/CMove_road.h"
//#include "Move/CMove_exec.h"
#include "Move/CMove_exec2.h"
#include "Move/CMove_dist.h"

// 攻撃系
//#include "Attack/CAttack_rule.h"
#include "Attack/CAttack_rule2.h"
#include "Attack/CAttack_rule_cpu.h"
#include "Attack/CAttack_weapon.h"
//#include "Attack/CAttack_range.h"
#include "Attack/CAttack_range2.h"
//#include "Attack/CAttack_select.h"
#include "Attack/CAttack_select2.h"
#include "Attack/CAttack_action.h"
#include "Attack/CAttack_demo.h"
#include "Attack/CAttack_apply.h"
#include "Attack/CAttack_del.h"
#include "Attack/CAttack_result.h"
// マップ攻撃系
#include "Attack/CField_rule.h"
#include "Attack/CField_select.h"
#include "Attack/CField_attack.h"
//#include "Attack/CMap_effect.h"

// 精神系
#include "Spirit/CSpirit_rule.h"
#include "Spirit/CSpirit_menu.h"
#include "Spirit/CSpirit_select.h"
#include "Spirit/CSpirit_apply.h"

// アイテム系
#include "Item/CItem_rule.h"
#include "Item/CItem_menu.h"
#include "Item/CItem_apply.h"

// 治癒
#include "Refill/CCure_rule.h"
// 補給
#include "Refill/CRefill_rule.h"
#include "Refill/CRefill_apply.h"

// ステータス
#include "Status/CStatus_rule.h"

// 告白
#include "various/CLove_rule.h"

// 待機
#include "various/CWait_rule.h"

// デモ
#include "Demo/CDemo_map.h"

// 出撃系
#include "Sally/CSally_select.h"
#include "Sally/CSally_add_chara.h"
#include "Sally/CSally_del_chara.h"
#include "Sally/CSally_change_chara.h"
#include "Sally/CSally_add_chara_map.h"
#include "Sally/CSally_add_chara_map2.h"
//#include "Sally/CSally_add_chara_map3.h"
#include "Sally/CSally_del_chara_map.h"
#include "Sally/CSally_setup_weapon.h"
#include "Sally/CSally_set_pers.h"
#include "Sally/CSally_view.h"
#include "Sally/CSally_spirit.h"
//#include "Sally/CWait_sally.h"

// 勝利条件系
#include "Victory/CVictory_view.h"
#include "Victory/CVictory_check.h"
#include "Victory/CVictory_check2.h"
#include "Victory/CVictory_change.h"

// ペナルティ
#include "Penalty/CPenalty_rule.h"

// エフェクト系
#include "../../ADV/Code/CCode_fade.h"
#include "../../ADV/Code/CWait_fade.h"
#include "Effect/Code/CWait_symbol.h"
#include "various/CWait_frame.h"

// イベント系
#include "Event/CEvent_base.h"
#include "Event/CEvent_Msg.h"
#include "Event/CEvent_Msg_state.h"
#include "../../ADV/Code/CWait_input.h"
#include "EVent/CEvent_Battle2.h"
#include "Event/CEvent_Msg_back.h"

// マップ系
#include "Map/CMap_cursol.h"
#include "Map/CMap_scroll.h"