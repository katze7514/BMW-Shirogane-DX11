/*
	katze 05/03/09
	update 06/01/17
	戦闘用合体攻撃武器
*/
#pragma once

#include "CDataWeaponBattle.h"

namespace BMW{
namespace Weapon{

class CDataWeaponBattleCollab : public CDataWeaponBattle
{/**
	戦闘用合体攻撃武器を表現するクラス
 */
public:
	// コンストラクタ・デストラクタ
	CDataWeaponBattleCollab():bCollabSetup_(false){}
	virtual ~CDataWeaponBattleCollab(){}

	// シリアライズ
	void Serialize(ISerialize& s);
	
	// デリートする際に呼ばれる
	void delWeapon(SLG::CDataCharaSLG* pChara, SLG::CSLGContext& p);

	// 取得
	set<int>&	getSlgIDSet(){ return setSlgID_; }
	void		setSlgID(int nID){ setSlgID_.insert(nID); }
	// 操作
	bool	enableNeed(const SLG::CDataCharaSLG& chara, bool bP, SLG::CSLGContext& context);
	void	use(SLG::CDataCharaSLG& chara, SLG::CSLGContext& context);

	bool	IsCollabLoad(){ return setSlgID_.size()==status_.getCollabCharaMap().size(); }
	bool	IsCollabChara(int nSlg){ return setSlgID_.find(nSlg)!=setSlgID_.end(); }

	bool	IsCollabSetup()const{ return bCollabSetup_; }
	void	collabSetup(bool bCollabSetup){ bCollabSetup_=bCollabSetup; }

	// ↓の構築関数
	static Weapon::CDataWeaponBattle* getCollabWeapon(SLG::CDataCharaSLG& chara, int nWeaponID, SLG::CSLGContext& context);
	static void chara2slgCollab(SLG::CDataCharaSLG& chara, Weapon::CDataWeaponBattleCollab& collab, SLG::CSLGContext& context);

private:
	// この合体攻撃を使用するキャラIDセット
	set<int> setSlgID_;
	// すでに設定されたかフラグ
	// 状況によっては重複してしまうため
	bool bCollabSetup_;

	// 与えられたIndexの周り8マスを取得する
	void getEightMapIndex(int nIndex, set<int>& setMap, SLG::CSLGContext& context);
};

} // namespace Weapon end
} // namespace BMW end