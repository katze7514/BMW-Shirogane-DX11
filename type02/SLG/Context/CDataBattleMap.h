/*
	katze 06/06/10
	マップ兵器用戦闘データクラス
*/
#pragma once

#include "CDataBattleAbility.h"

namespace BMW{
namespace SLG{

class CDataBattleMapAtk : public CDataBattleAbility
{/**
	マップ兵器用戦闘攻撃データ
 */
public:
	// コンストラクタ
	CDataBattleMapAtk():pChara_(NULL),pWeapon_(NULL),nWeaponEn_(0){}

	// アクセッサ
	CDataCharaSLG*				getChara(){ return pChara_; }
	void						setChara(CDataCharaSLG* pChara){ pChara_=pChara; }
	Weapon::CDataWeaponBattle*	getWeapon(){ return pWeapon_; }
	void						setWeapon(Weapon::CDataWeaponBattle* pWeapon){ pWeapon_=pWeapon; }
	
	int		getEnWeapon()const{ return nWeaponEn_; }
	void	setEnWeapon(int nWeaponEn){ nWeaponEn_=nWeaponEn; }

	void clearData()
	{ 
		pChara_=NULL;
		pWeapon_=NULL;
		nWeaponEn_=0;
	}

private:
	// キャラデータ
	CDataCharaSLG* pChara_;
	// 武器データ
	Weapon::CDataWeaponBattle* pWeapon_;
	// ↑の消費EN
	int nWeaponEn_;
};

class CDataBattleMapDef : public CDataBattleAbility
{/**
	マップ兵器用戦闘防御データ
 */
public:
	// コンストラクタ
	CDataBattleMapDef():pChara_(NULL),bHit_(false),nDamage_(0),bCT_(false),bDeath_(false){}

	// 設定・取得
	CDataCharaSLG*	getChara(){ return pChara_; }
	void			setChara(CDataCharaSLG* pChara){ pChara_=pChara; }

	bool	IsHit()const{ return bHit_; }
	void	hit(bool bHit){ bHit_=bHit; }
	int		getDamage()const{ return nDamage_; }
	void	setDamage(int nDamage){ nDamage_=nDamage; }
	bool	IsCT()const{ return bCT_; }
	void	ct(bool bCT){ bCT_=bCT; }
	bool	IsDeath()const{ return bDeath_; }
	void	death(bool bDeath){ bDeath_=bDeath; }

private:
	CDataCharaSLG* pChara_; // キャラデータ
	bool	bHit_;		// 当たった？
	int		nDamage_;	// 被ダメージ
	bool	bCT_;		// CTフラグ
	bool	bDeath_;	// DEATHフラグ
};

class CDataBattleMap
{/**
	マップ兵器用戦闘データ
 */
public:
	typedef list<CDataBattleMapDef> mapdef_list;

	CDataBattleMapAtk&	getMapAtk(){ return atk_; }
	void				clearMapAtk(){ atk_.clearData(); }
	mapdef_list&		getMapDefList(){ return listDef_; }
	void				addMapDef(const CDataBattleMapDef& def){ listDef_.push_back(def); }
	void				clearMapDef(){ listDef_.clear(); }

	// MapDefのイテレーション
	mapdef_list::iterator	beginMapDef(){ it=listDef_.begin(); return it; }
	bool					endMapDef(){ return it==listDef_.end(); }
	mapdef_list::iterator	nextMapDef(){ return it++; }
	mapdef_list::iterator	currentMapDef(){ return it; }

private:
	CDataBattleMapAtk	atk_;
	mapdef_list			listDef_;
	mapdef_list::iterator it;

};

} // namespace SLG end
} // namespace BMW end