/*
	katze 06/04/19
	戦闘イベントを表現する
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Event{

class CBattleEventAbility
{/**
	戦闘データ基本
 */
public:
	// コンストラクタ・デストラクタ
	CBattleEventAbility():nAbilityEn_(0){}
	virtual ~CBattleEventAbility(){}

	// 設定・取得
	set<int>&				getAbilitySet(){ return setAbility_; }
	void					setAbility(int nID){ setAbility_.insert(nID); }
	
	int						getEnAbility() const { return nAbilityEn_; }
	void					setEnAbility(int nEn){ nAbilityEn_=nEn; }
	void					calcEnAbility(int nEn){ nAbilityEn_+=nEn; }

	const string&			getMsgList()const{ return sMsgList_; }
	void					setMsgList(const string& sMsgList){ sMsgList_=sMsgList; }
	const string&			getMsgListBackup()const{ return sMsgListBackup_; }
	void					setMsgListBackup(const string& sMsgListBackup){ sMsgListBackup_=sMsgListBackup; }

protected:
	set<int>	setAbility_;	// 発生する技能
	int			nAbilityEn_;	// ↑によって消費されるEN
	string		sMsgList_;		// メッセージID
	string		sMsgListBackup_;// メッセージID for 援護
};

class CBattleEventAttack : public CBattleEventAbility
{/**
	攻撃ベース
 */
public:
	enum eDamage{
		ABS,	// 絶対値
		RATIO,	// 比率
	};
	// コンストラクタ
	CBattleEventAttack():nWeapon_(-1),nEn_(0),nType_(ABS),nDamage_(0),bCT_(false),bDeath_(false){}

	// 設定・取得
	int		getWeaponID(){ return nWeapon_; }
	void	setWeaponID(int nWeapon){ nWeapon_ = nWeapon; }

	int		getEN() const { return nEn_; }
	void	setEN(int nEN){ nEn_=nEN; }
	bool	IsCT() const { return bCT_; }
	void	ct(bool bCT){ bCT_=bCT; }
	bool	IsDeath() const { return bDeath_; }
	void	death(bool bDeath){ bDeath_=bDeath; }

	int		getType() const { return nType_; }
	void	setType(int nType){ nType_=nType; }
	int		getDamage() const { return nDamage_; }
	void	setDamage(int nDamage){ nDamage_=nDamage; }

private:
	int		nWeapon_;	// 武器ID
	int		nEn_;		// ↑によって消費されるEN
	bool	bCT_;		// CTが発生するかどうか
	bool	bDeath_;	// この攻撃で相手を倒すのか？

	int		nType_;		// ダメージタイプ
	int		nDamage_;	// ダメージ量
};

class CBattleEventDefence : public CBattleEventAbility
{/**
	防御側戦闘データクラス

	こつが、戦闘デモとかに渡される
 */
public:
	// コンストラクタ
	CBattleEventDefence():nAction_(-4){}

	// 設定・取得
	int		getAction() const { return nAction_; }
	void	setAction(int nAction){ nAction_=nAction; }

private:
	// 行動
	// 回避したとか、防御とか、HITとか
	int nAction_;
};

class CBattleEventBase
{/**
	戦闘イベントの基礎
 */
public:
	// コンストラクタ
	CBattleEventBase():nCharaID_(-1){}

	// アクセッサ
	int						getChara()const{ return nCharaID_; }
	void					setChara(int nChara){ nCharaID_=nChara; }
	CBattleEventAttack&		getAtk(){ return atk_; }
	CBattleEventAttack*		getAtkPtr(){ return &atk_; }
	void					setAtk(const CBattleEventAttack& atk){ atk_=atk; }
	CBattleEventDefence&	getDef(){ return def_; }
	CBattleEventDefence*	getDefPtr(){ return &def_; }
	void					setDef(const CBattleEventDefence& def){ def_=def; }

private:
	int nCharaID_; // SLG IDの方
	CBattleEventAttack	atk_;
	CBattleEventDefence def_;
};

class CBattleEventData
{/*
	戦闘イベントを表現する
*/
public:
	enum eBattle{
		ATTACK,
		COUNTER,
		ATTACK_BACK,
		COUNTER_BACK,
	};

	CBattleEventBase& getEventBase(int nIndex){ return base_[nIndex]; }
	CBattleEventBase* getEventBasePtr(int nIndex){ return &base_[nIndex]; }

private:
	CBattleEventBase base_[4];
};

} // namespace Event end
} // namespace SLG end
} // namespace BMW end