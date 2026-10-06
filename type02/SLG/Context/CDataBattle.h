/*
	katze 05/05/04
	戦闘用データクラス
*/
#pragma once

#include "CDataBattleAttack.h"
#include "CDataBattleDefence.h"

namespace BMW{
namespace SLG{
class CDataCharaSLG;

class CDataBattleBase
{/**
	戦闘用データクラスのベース

	アタックは、こちらの攻撃時の行動
	デフェンスは、こちらが防御時の行動

	デモを構築する時は、攻撃側と反撃側で
	attackデータとDefenceデータがクロスする感じで
 */
public:
	// 設定・取得
	smart_ptr<CDataCharaSLG>&	getChara(){ return pChara_; }
	void						setChara(const smart_ptr<CDataCharaSLG>& chara){ pChara_=chara; }

	CDataBattleAttack&			getAttack(){ return attackData_; }
	CDataBattleDefence&			getDefence(){ return defenceData_; }

	// 操作
	void						clearData(){ attackData_.clearData(); defenceData_.clearData(); }

private:
	smart_ptr<CDataCharaSLG>	pChara_;
	CDataBattleAttack			attackData_;
	CDataBattleDefence			defenceData_;
};

class CDataBattle
{/**
	戦闘用データクラス
	攻撃系ルールは、計算して、これを構築する
	また、これに適切な値を設定することで戦闘イベントなどを
	演出できる（ようにする）
 */
public:
	enum eData{
		ATTACK,
		COUNTER,
		ATTACK_BACK,
		COUNTER_BACK,
	};
	enum eSide{
		LEFT,
		RIGHT,
	};

	// 設定・取得
	CDataBattleBase&	getBattleData(int nData){ return battleData_[nData]; }
	CDataBattleBase*	getBattleDataPtr(int nData){ return &battleData_[nData]; }
	int					getSide() const { return nSide_; }
	void				setSide(int nSide){ nSide_=nSide; }
	const string&		getBack()const{ return sBack_; }
	void				setBack(const string& sBack){ sBack_=sBack; }
	bool				IsEvent()const{ return bEvent_; }
	void				event(bool b){ bEvent_=b; }

	//　操作
	void			clearBattleData()
					{
						battleData_[0].clearData();
						battleData_[1].clearData();
						battleData_[2].clearData();
						battleData_[3].clearData();
						bEvent_=false;
					}

private:
	// 援護キャラも含んでしまう
	CDataBattleBase	battleData_[4];
	// 攻撃側がどちらサイドなのか
	int nSide_;
	// 背景ファイル
	string	sBack_;
	// イベントモード？
	// イベントモード時はキャンセルできない
	bool	bEvent_;
};

} // namespace SLG end
} // namespace BMW end