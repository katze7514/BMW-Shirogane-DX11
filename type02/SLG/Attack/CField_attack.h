/*
	katze 06/06/11
	MAP兵器用攻撃計算および確認
*/
#pragma once

namespace BMW{

namespace Movie{
class CSymbolDB;
} // namespace Movie end

namespace SLG{
class CDataBattleMapAtk;
class CDataBattleMap;

namespace Attack{

class CField_attack : public Task::ITaskList
{/**
	MAP兵器用攻撃計算および確認
 */
public:
	enum eState{
		INTRO,
		NORMAL,
		CANCEL,
		END,
	};
	enum eButton{
		ON,OFF,GO
	};
	// コンストラクタ・デストラクタ
	CField_attack();
	~CField_attack();
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// アクション
	void actionGo(Task::CTaskContext* pContext);
	// リスナー
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);

private:
	// 攻撃計算
	void calcBattle(CDataBattleMapAtk& atk, int nDef, CDataBattleMap& map, CSLGContext& p);
	void calcDamage(CDataBattleMapAtk& atk, CDataBattleMapDef& def, int nDist, CSLGContext& p);

	BMW::Rule::CRuleCancel* pCancel_;
	Movie::CMovieClip* pCutIn_;
	// パネル
	GUI::CPanel* pPanel_;
	GUI::CPanelCtrl* pOnOff_;
	void callTaskAction(Task::CTaskContext*);
	void callTaskDraw(Task::CTaskContext*);

	// 武器ロード用シンボルDB
	Movie::CSymbolDB* pSymbol_;
	// ↑こいつがロードしてる武器ID
	int nWeaponID_;
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end