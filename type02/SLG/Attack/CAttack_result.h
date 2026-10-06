/*
	katze 05/05/07
	update 06/02/19
	戦闘結果
*/
#pragma once

namespace BMW{
namespace SLG{
class CDataBattleMapAtk;
class CDataBattleMapDef;

namespace Attack{

class CAttack_result : public BMW::Rule::CRuleList
{/**
	戦闘結果
 */
public:
	enum eState{
		NORMAL,
		CLICK,
	};
	enum ePriority{
		OK_T,
		CANCEL_T,
		RESULT,
		STATUS,
		LVUP,
	};
	// デストラクタ
	virtual ~CAttack_result(){}
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

protected:
	// 計算結果
	int nExp_;
	int nBP_;
	int nFP_;
	// 取得アイテム
	list<int> listItem_;
	// LVUPした？
	int nLv_;

#ifdef BMW_DEBUG
	// フレームカウンタ
	int nFrame_;
#endif

	// インターフェイス
	GUI::CPanel* pResult_;
	GUI::CPanel* pLvUp_;
	GUI::CPanel* pStatus_;

	// 判定
	bool IsDeath(CDataCharaSLG* pChara, CSLGContext& p);

	// 結果計算
	// 通常攻撃
	void calcResult(CDataBattleBase& attack, CDataBattleBase& def, CSLGContext& p, int nLv);
	// マップ兵器
	void calcResult(CDataBattleMapAtk& atk, CDataBattleMapDef& def, CSLGContext& p);
	// いろんな計算補助
	void checkSpirit(Chara::CDataCharaBattle* pBattle);
	void getItem(CDataCharaSLG* pChara, CSLGContext* p);
	void LvUP(CDataCharaSLG* pChara, CSLGContext& p);

	// インターフェイス設定
	void setPanelChara(CDataCharaSLG& chara, CSLGContext& p);
	void setPanelLvUp(CDataCharaSLG& chara, CSLGContext& p);
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end