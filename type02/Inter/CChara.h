/*
	katze 05/06/16
	update 06/02/27
	インターミッションキャラ
*/
#pragma once

namespace BMW{

namespace Status{
class CStatusWeaponPanel;
} // namespace Status end

namespace Inter{
class CInterContext;

namespace Chara{

class CChara : public Rule::CRuleList
{/**
	インターミッションキャラ
 */
public:
	enum eState{
		NORMAL,
		CANCEL,
	};
	enum ePriority{
		CANCEL_T,
		PANEL,
	};
	enum eChara{
		C_BACK,
		C_FORWARD,
	};
	enum eMenu{
		M_BASE,
		M_WEAPON,
		T_FND,
		T_SKILL,
		T_BATTLE,
		T_WEAPON,
		I_EQUP,
		I_EXCHANGE,
		M_BACK,
	};
	enum eStatus{
		S_HEAD,
		S_EASY,
		S_BATTLE,
		S_BASE,
		S_WEAPON,
		S_ITEM_START, // アイテム交換準備（キャンセルの扱いが変わる）
		S_ITEM_END, // アイテム交換終了（キャンセルの扱いが変わる
		S_BP,
		S_FP,
	};
	// デストラクタ
	~CChara();
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// アクション
	void actionChange(Task::CTaskContext*);

	// イベントハンドラ
	// キャラ替え
	void eventChara(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	// パネル選択ボタン用
	void eventMenu(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	// 武器パネル切り替え用
	void eventWeapon(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	// 養成反応用
	void eventStatus(int nStatus, Task::CTaskContext*);

private:
	// ステータス更新
	void updateHeader(CInterContext* pContext);
	void updateEasy(CInterContext* pContext);
	void updateBattle(CInterContext* pContext);
	void updateWeapon(BMW::Chara::CDataCharaInter* pChara, Task::CTaskContext& p);

	BMW::Rule::CRuleCancel* pCancel_;
	// インターフェイス
	// 全体
	GUI::CPanel*		pPanel_;
	// ステータス
	GUI::CPanel*		pHeader_;
	GUI::CPanel*		pEasy_;
	GUI::CPanel*		pBattle_;
	// パネルCtrl
	GUI::CPanelCtrl*	pCtrl_;
	int					nItemExchange_;

	GUI::CPanel*				pBase_;
	GUI::CPanel*				pWeapon_;
	Status::CStatusWeaponPanel* pWeaponPanel_[2];
	int							nWeapon_;
	
};

} // namespace Chara end
} // namespace Inter end
} // namespace BMW end