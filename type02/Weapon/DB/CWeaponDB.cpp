#include "stdafx.h"

#include "CWeaponDB.h"

namespace BMW{
namespace Weapon{

CWeaponDB::~CWeaponDB()
{
	weapon_map::iterator it;
	for(it=mapWeapon_.begin(); it!=mapWeapon_.end(); it++)
		DELETE_SAFE(it->second);

	mapWeapon_.clear();
}

void CWeaponDB::setWeaponDB(const string& sFile)
{
	using namespace boost::spirit;
	using namespace phoenix;

	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(sFile);
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	// 構文解析
	CWeaponParser ps(*this);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r =
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full)	CDbg().Out("%s 読み込み失敗！！", r.stop);
#endif
}

/////////////////////////////////////////////
// 各データの設定メソッド
/////////////////////////////////////////////
/*
void CWeaponDB::setBattle(CDataWeaponBattle* pBattle,int nID) const
{
}

void CWeaponDB::setBattleCollab(CDataWeaponBattleCollab* pCollab,int nID) const
{
}

void CWeaponDB::setBattleStatus(CDataWeaponBattleStatus* pStatus,int nID) const
{
}

void CWeaponDB::setBattleCond(CDataWeaponBattleCond* pCond,int nID) const
{
}
*/
} // namespace Weapon end
} // namespace BMW end