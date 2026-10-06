/**
	katze 07/03/14
	ヘルプID
*/
#pragma once

namespace BMW{
namespace Unit{

namespace Help{
enum eHelp
{
	// SLG
	SLG_START,			// はじめてのSLG
	SLG_START2,			// はじめてのSLG2
	SLG_CHARA_SELECT,	// キャラクタ選択説明（位置替え）
	SLG_CHARA_SELECT2,	// キャラクタ選択（待機キャラ出し）
	SLG_CHARA_METOR,	// 変身説明
	SLG_CHARA_MENU,		// キャラメニュー
	SLG_MOVE_SELECT,	// 移動選択
	SLG_ATK,			// 攻撃
	SLG_ATK_SELECT,		// 攻撃選択
	SLG_ATK_CHECK,		// 攻撃開始
	SLG_ATK_STATUS,		// ステータスアップ武器
	SLG_ATK_COND,		// 状態変化武器
	SLG_ATK_COLLAB,		// 合体攻撃
	SLG_ATK_FIELD_CENTER,// フィールド武器（自分中心型）
	SLG_ATK_FIELD_LINE,	// フィールド武器（ライン型）
	SLG_ATK_FIELD_THROW,// フィールド武器（投げ込み型）
	SLG_SPIRIT,			// 精神
	//SLG_ITEM,			// アイテム
	SLG_CURE,			// 回復
	SLG_PIT,			// 補給
	SLG_TURN_MENU,		// ターンメニュー
	SLG_ICHIRAN,		// キャラ一覧
	SLG_SAVE,			// セーブ
	// インターミッション
	INTER_SELECT,		// インターミッション説明
	INTER_FUND,			// 基礎養成説明
	INTER_SKILL,		// 技能養成説明
	INTER_BATTLE,		// 戦闘養成説明
	INTER_WEAPON,		// 武器養成説明
	INTER_ITEM,			// アイテム装備説明
	INTER_ITEM_CHANGE,	// アイテム交換説明
	// 番兵
	HELP_END,			
};

// ヘルプフラグリセット
__inline void resetHelpFlag(Save::CExecData& save)
{// ヘルプIDフラグをリセット
	save.setFlag("SLG_START",0);
	save.setFlag("SLG_START2",0);
	save.setFlag("SLG_CHARA_SELECT",0);
	save.setFlag("SLG_CHARA_SELECT2",0);
	save.setFlag("SLG_CHARA_METOR",0);
	save.setFlag("SLG_CHARA_MENU",0);
	save.setFlag("SLG_MOVE_SELECT",0);
	save.setFlag("SLG_ATK",0);
	save.setFlag("SLG_ATK_SELECT",0);
	save.setFlag("SLG_ATK_CHECK",0);
	save.setFlag("SLG_ATK_STATUS",0);
	save.setFlag("SLG_ATK_COND",0);
	save.setFlag("SLG_ATK_COLLAB",0);
	save.setFlag("SLG_ATK_FIELD_CENTER",0);
	save.setFlag("SLG_ATK_FIELD_LINE",0);
	save.setFlag("SLG_ATK_FIELD_THROW",0);
	save.setFlag("SLG_SPIRIT",0);
	//save.setFlag("SLG_ITEM",0);
	save.setFlag("SLG_CURE",0);
	save.setFlag("SLG_PIT",0);
	save.setFlag("SLG_TURN_MENU",0);
	save.setFlag("SLG_ICHIRAN",0);
	save.setFlag("SLG_SAVE",0);
	save.setFlag("INTER_SELECT",0);
	save.setFlag("INTER_FUND",0);
	save.setFlag("INTER_SKILL",0);
	save.setFlag("INTER_BATTLE",0);
	save.setFlag("INTER_WEAPON",0);
	save.setFlag("INTER_ITEM",0);
	save.setFlag("INTER_ITEM_CHANGE",0);
}
// ヘルプモードロード時リセット

namespace{
__inline void resetLoadHelpFlagOne(const string& sID, Save::CExecData& save)
{
	if(save.getFlag(sID,-1)	|| !save.getFlag(sID,1))
		save.setFlag(sID,0);
}
} // namespce end

__inline void resetLoadHelpFlag(Save::CExecData& save)
{
	// キャラ出せるよ
	resetLoadHelpFlagOne("SLG_CHARA_SELECT2",save);
	// 変身とかできちゃうよ
	resetLoadHelpFlagOne("SLG_CHARA_METOR",save);
	// 合体攻撃
	resetLoadHelpFlagOne("SLG_ATK_COLLAB",save);
	// フィールド武器系だよ
	resetLoadHelpFlagOne("SLG_ATK_FIELD_CENTER",save);
	resetLoadHelpFlagOne("SLG_ATK_FIELD_LINE",save);
	resetLoadHelpFlagOne("SLG_ATK_FIELD_THROW",save);

#ifdef BMW_DEBUG
//	resetLoadHelpFlagOne("SLG_ATK_STATUS",save);
//	resetLoadHelpFlagOne("SLG_ATK_COND",save);
//	resetLoadHelpFlagOne("SLG_CURE",save);
//	resetLoadHelpFlagOne("SLG_PIT",save);
//	resetLoadHelpFlagOne("SLG_SPIRIT",save);
#endif
}

// ヘルプモード呼び出し
__inline void callHelp(int nHelp, const string& sFlag, Task::CTaskContext* pContext)
{
	if(pContext->getApp()->getExec().getFlag(sFlag,0))
	{// まだ呼び出したことがなかったら呼び出す
		pContext->getApp()->getExec().setFlag(sFlag,1);
		pContext->getApp()->getFoward()->createHelp(nHelp,pContext);
	}
}

} // namesapce Help end

} // namespace Unit end
} // namesapce BMW end