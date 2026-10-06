/*
	katze 05/03/09
	武器DB
*/
#pragma once

namespace BMW{
namespace Weapon{

class CDataWeaponInit;

class CWeaponDB
{/**
	武器DB
 */
public:
	typedef map<int, CDataWeaponInit*> weapon_map;
	// デストラクタ
	~CWeaponDB();

	// 操作
	void				setWeaponDB(const string& sFile);
	CDataWeaponInit*	getData(int nID){ return mapWeapon_[nID]; }
	void				addData(CDataWeaponInit* pData, int nID)
						{ 
							//if(mapWeapon_[nID]!=NULL) delete mapWeapon_[nID];
							mapWeapon_[nID]=pData;
						}

	// 各WeaponDataの生成
	/*
	CDataWeaponBattle* 			createBattle(int nID, int nSlg, SLG::CSLGContext* pContext) const;
	CDataWeaponBattleCollab*	createBattleCollab(int nID) const;
	CDataWeaponBattleStatus*	createBattleStatus(int nID) const;
	CDataWeaponBattleCond*		createBattleCond(int nID) const;
	*/
private:
	weapon_map mapWeapon_;
};

} // namespace Weapon end
} // namespace BMW end