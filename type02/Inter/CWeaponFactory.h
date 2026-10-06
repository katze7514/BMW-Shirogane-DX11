/*
	katze 05/06/23
	武器データ生成関数
*/
#pragma once

namespace BMW{

namespace Chara{
class CDataCharaInter;
} // namespace Chara end

namespace Weapon{
class CDataWeaponBattle;
} // namespace Weapon end

namespace Inter{

class CWeaponFactory
{
public:
	// デストラクタ
	~CWeaponFactory();
	// 武器データ生成
	Weapon::CDataWeaponBattle* createWeapon(int nID, BMW::Chara::CDataCharaInter* pChara, Task::CTaskContext& p);
	// ランク反映
	void updateRank();

	int getOut()const{ return nOut_; }
	void setOut(int nOut){ nOut_=nOut; }

private:
	map<int, Weapon::CDataWeaponBattle*> mapWeapon_;
	// ランク計算用マップ
	map<int, Weapon::CDataWeaponBattle*> strMap_; // 格闘用
	map<int, Weapon::CDataWeaponBattle*> mgcMap_; // 魔術用

	// カウント
	// 武器養成時に使う、養成に関係ない武器数
	int nOut_;
};

} // namespace Inter end
} // naemsapce BMW end