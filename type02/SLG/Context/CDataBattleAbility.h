/*
	katze 05/05/08
	update 06/03/25
	戦闘データの基礎
*/
#pragma once

#include "CValidAbility.h"

namespace BMW{
namespace SLG{

class CDataBattleAbility
{/**
	戦闘データの基礎クラス
 */
public:
	// コンストラクタ・デストラクタ
	CDataBattleAbility():nEn_(0){}
	virtual ~CDataBattleAbility(){}

	// 設定・取得
	const CValidAbility&	getAbility() const { return ability_; }
	set<int>&				getAbilitySet(){ return ability_.getAbilitySet(); }
	bool					IsAbility(int nID) const { return ability_.IsAbility(nID); }
	void					setAbility(int nID){ ability_.setAbility(nID); }
	void					delAbility(int nID){ ability_.delAbility(nID); }
	void					clearAbility(){ ability_.clearAbility(); }

	int						getEnAbility() const { return nEn_; }
	void					setEnAbility(int nEn){ nEn_=nEn; }
	void					calcEnAbility(int nEn){ nEn_+=nEn; }

	const string&			getMsgList()const{ return sMsgList_; }
	void					setMsgList(const string& sMsgList){ sMsgList_=sMsgList; }
	const string&			getMsgListBackup()const{ return sMsgListBackup_; }
	void					setMsgListBackup(const string& sMsgListBackup){ sMsgListBackup_=sMsgListBackup; }

	virtual void			clearData(){ ability_.clearAbility(); nEn_=0; sMsgList_.clear(); sMsgListBackup_.clear(); }

	// 判定
	// バリア技能発動してる？
	bool	IsBarriar();
	// 分身技能発動している？
	bool	IsAlterEgo();

protected:
	// 発動Ability
	CValidAbility	ability_;
	// AbilityのEN消費
	int	nEn_;
	// MsgList指定
	// これが空だったらランダム。そうじゃなかったら、これで指定したのを使う
	string	sMsgList_;
	// 援護MsgList指定
	// これが空だったらランダム。そうじゃなかったら、これで指定したのを使う
	string	sMsgListBackup_;
};

} // namespace SLG end
} // namespace BMW end