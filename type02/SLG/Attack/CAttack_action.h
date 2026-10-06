/*
	katze 05/05/07
	update 06/02/17
	戦闘行動表
*/
#pragma once

#include "../../Scene/Event/IListenerCircleMenu.h"
#include "../Context/CBattleState.h"
#include "../Context/COffsetBattle.h"

#include "CAttack_weapon_support.h"

namespace BMW{
namespace SLG{
class CSLGContext;

namespace Map{
class CMapChip;
} // namespace Map end

namespace Attack{
class CAttack_weapon_support;

class CAttack_action : public BMW::Rule::CRuleList, public BMW::Event::IListenerCircleMenu
{/**
	戦闘行動表
 */
public:
	enum eState{
		NORMAL,
		CANCEL,
		COUNTER,		// 反撃メニュー選択中
		COUNTER_CANCEL, // ↑でキャンセル
		COUNTER_ATK,	// 反撃武器選択中
		SUPPORT_ATK,	// 援護武器選択中
	};
	enum ePriority{
		CANCEL_T,
		MAIN,
		SUPPORT,
		MENU,
		WEAPON_MENU,
	};
	enum eCounter{
		SELECT_B,
		COUNTER_B,
		AVOID_B,
		DEF_B,
	};
	enum eCtrl{
		ON_C,
		OFF_C,
		GO_C,
	};
	enum eSupport{
		CHARA1,
		CHARA2,
		CHARA3,
		CHARA4,
		SELECT,
	};
	// コンストラクタ・デストラクタ
	CAttack_action():pSupport_(NULL){}
	virtual ~CAttack_action();
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// アクション
	void actionGo(Task::CTaskContext*);
	void actionEnd(Task::CTaskContext*);

	// イベントハンドラ
	// 前回CAttack_ctrlとして定義していたやつ
	void eventCtrl(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	// 援護キャラ選択
	void eventBackUpAtk(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	void eventBackUpDef(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	// 反撃行動選択に対するもの↓
	void eventCircle(int nState, Task::CTaskContext* pContext);
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	// 武器選択メニュー
	// 呼び出しに合わせてどちらを呼ぶかを切り替える
	void eventWeaponCounter(int nState, Task::CTaskContext* pContext);
	void eventWeaponBackAtk(int nState, Task::CTaskContext* pContext);

	////////////////////////////////////////////////////////////
	// 戦闘系
	////////////////////////////////////////////////////////////
	// カウンター発動判定
	static bool IsCounter(CDataCharaSLG& counter, Weapon::CDataWeaponBattle& counterWeapon, CDataCharaSLG& attack, CSLGContext& p);
	// 必中扱いの判定
	static bool IsSpecialHit(int nAttack, CDataCharaSLG* pAttackChara, Weapon::CDataWeaponBattle* pAttackWeapon, CDataBattleBase* attackBattle,int nEN, int nDef, CBattleStateBase& defState, CDataCharaSLG* pDefChara, CSLGContext& p);
	// 分身発動計算
	static bool	calcAlterEgo(CDataCharaSLG& def, CDataBattleAbility& defData, int nEN, CSLGContext& p, CDataCharaSLG& atk, CDataBattleAbility& atkData);
	// バリア発動計算
	// 軽減したダメージ量が返ってくる
	static int	calcBarriar(int& nDamage, CDataCharaSLG&  def, CDataBattleAbility& defData, int nEN, bool bM, bool bT, CSLGContext& p, bool bD=false);

protected:
	//////////////////////////////////////////////////////
	// 内部利用メソッド群

	// nState→nTargetへの攻撃時の命中を計算し、CBattleStateに設定する
	void	calcAndSetHit(int nState, int nTarget, CSLGContext* p, bool bFriend=false);
	// 反撃行動選択表示準備
	void	readyCircleCounter(Task::CTaskContext* pContext);
	// 武器選択表示準備
	void	readyWeaponSelect(int nID, const CAttack_weapon_support::WeaponEvent& fun, bool bCounter, Task::CTaskContext* pContext);
	
	//////////////////////////////////////////////////////////
	// 援護系
	//////////////////////////////////////////////////////////
	// pAttackに援護攻撃に入れるかを計算し、援護配列にIDを設定
	void	calcBackAttack(CDataCharaSLG& attack, CSLGContext& p, int nHP);
	// 援護攻撃キャラ選択
	int		selectBackUpAtk(CSLGContext& context);
	// pDefに援護防御に入れるかを計算し、援護配列にIDを設定
	void	calcBackDef(CDataCharaSLG& def, CSLGContext& p);
	// 援護攻撃キャラの状態設定
	void	setStateBackUpAtk(int nPos, CSLGContext& p);
	// 援護防御キャラ選択
	int		selectBackUpDef(CSLGContext& context);
	// ↑らを判定するための補助メソッド
	bool	IsMove(CDataCharaSLG& chara, Map::CMapChip* pBase, Map::CMapChip* pMap, CSLGContext& p);

	/////////////////////////////////////////////////////////////
	// パネルの反映
	/////////////////////////////////////////////////////////////
	// nLeft,nRightがCBattleStateに対応する状態。bLeftは、どちら側が味方側か。
	void	initPanel(int nLeft, int nRight, bool bLeft, CSLGContext& p);
	void	initPanelSide(int nChara, bool bCounter, int nLeft, CSLGContext& p);
	void	updatePanelSide(int nChara, int nSide, GUI::CPanel* pPanel, CSLGContext& p);
	void	updatePanelSideWeapon(int nChara, int nTarget, GUI::CPanel* pPanel, CSLGContext& p);
	void	setRangeMark(int nWeaponID, GUI::CPanelCtrl* pPanel, CSLGContext& p, bool bCounter);
	////////////////////////////////////////////////////////////
	// 援護パネル系
	////////////////////////////////////////////////////////////
	void	initPanelBackUpAtk(bool bLeft, CSLGContext& p);
	void	updatePanelBackUpAtkWeapon(GUI::CPanel* pPanel, CSLGContext& p);
	void	updateBackUpAtk(CSLGContext& p);
	void	initPanelBackUpDef(bool bLeft, CSLGContext& p);
	void	updatePanelBackUpDefHP(GUI::CPanel* pPanel, CSLGContext& p);
	////////////////////////////////////////////////////////////
	// チップ
	////////////////////////////////////////////////////////////
	void	initPanelBackUpChip(GUI::CPanel* pPanel, bool bLeft, bool bAtk, CSLGContext& p);
	void	initPanelBackUpChipButton(GUI::CPanel* pPanel, CDataCharaSLG& chara, CSLGContext& p, bool bAtk, bool bEnemy);
	void	updatePanelBackUpChipVisible(bool bLeft, GUI::CPanel* pPanel, CSLGContext& p);
	void	changeValidChara(int nPos);
	////////////////////////////////////////////////////////////
	// 戦闘系
	////////////////////////////////////////////////////////////
	// 戦闘結果計算 nAttackとnDefにはBattleStateを指定する
	bool	calcBattle(int nAttack, int nDef, int nAttackEN, int nDefEN, CSLGContext& p, int nAtt=0);
	// 具体命中計算判定
	bool	calcBattleHit(CDataBattleBase*			attackBattle,
						  smart_ptr<CDataCharaSLG>&	pAttackChara,
						  const CBattleStateBase&	attackState,
						  CDataBattleBase*			defBattle,
						  smart_ptr<CDataCharaSLG>&	pDefChara,
						  const CBattleStateBase&	defState,
						  int						nDefEN,
						  bool						bFriend,
						  CSLGContext&				p);
	// 具体ダメージ計算
	void	calcAndSetBattleDamage(int							nAttack,
								   CDataBattleBase*				attackBattle,
								   smart_ptr<CDataCharaSLG>&	pAttackChara,
								   Weapon::CDataWeaponBattle*	pAttackWeapon,
								   const CBattleStateBase&		attackState,
								   int							nDef,
								   CDataBattleBase*				defBattle,
								   smart_ptr<CDataCharaSLG>&	pDefChara,
								   const CBattleStateBase&		defState,
								   int							nDefEN,
								   bool							bFriend,
								   int							nAtt,
								   CSLGContext&					p);
	// 援護防御計算
	bool	calcBattleBackDef(int nEN, CSLGContext& p);


	///////////////////////////////////////////////////////
	// 戦闘前状態
	CBattleState state_;
	// 攻撃キャラとの距離
	int nDist_;
	int nRealDist_;
	int nCounterDist_;	// 反撃側から見た距離
	// 攻撃キャラとの高さ
	int nHeight_;			// 攻撃側から見た高さ
	int nCounterHeight_;	// 反撃側から見た高さ
	// 武器選択を呼び出すための保存データ
	int nCtrl_;
	// 反撃不能フラグ
	bool bImCounter_;
	// 補正データ(魔力放出用)
	int nRelease_; // 魔力放出を使った場所
	COffsetBattle release_;
	// NPCキャラの攻撃？
	bool bNpc_;

	// 現在の援護キャラの位置
	int nBackUpPos_;
	// 援護キャラの位置とキャラIDのマップ
	// first:ID second:WeaponID
	pair<int,int> pairBackUp_[4];
	void resetBackUp()
	{ 
		nBackUpPos_=-1;
		for(int i=0; i<4; i++)
			pairBackUp_[i].first=pairBackUp_[i].second=-1;
	}

	// インターフェイス
	BMW::Rule::CRuleCancel* pCancel_;
	// HP表示部は、CEventにある
	// メインとなる上部分と戦闘開始など
	GUI::CPanel* pMain_;
	GUI::CPanelCtrl* pOnOff_;
	GUI::CPanelCtrl* pAct_;
	// 援護パネル
	GUI::CPanel* pSupport_;
	GUI::CPanel* pSupportR_;
	// ↓は援護パネルのホルダー
	GUI::CPanel *pPlayer_,*pEnemy_;
	// 反撃メニュー
	// GUI::CCircleMenu* pMenu_; IListenerCircleMenuより継承
	// 攻撃選択メニュー
	// 援護武器・反撃武器選択で使う
	CAttack_weapon_support* pWeaponMenu_;
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end