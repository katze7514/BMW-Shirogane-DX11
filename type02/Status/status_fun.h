/*
	katze 06/02/19
	各種ステータスパネルに対する設定子
*/
#pragma once

namespace BMW{

namespace Chara{
class CDataCharaInter;
} // namespace Chara end

namespace GUI{
class CGage;
} // namespace GUI end

namespace Weapon{
class CDataWeaponBattle;
} // namespace Weapon end

namespace SLG{
class CDataCharaSLG;
class CSLGContext;
} // namespace SLG end

namespace Inter{
class CInterChara;
} // namespace Inter end

namespace Status{
///////////////////////////////////////////////////
// クリアヘッダ
//////////////////////////////////////////////////
// 共通部分
void setClearHeaderBase(GUI::CPanel* pPanel, Task::CTaskContext& p, bool bInter);
// SLG版
void setClearHeader(GUI::CPanel* pPanel, SLG::CSLGContext& p);
// Inter版
void setClearHeaderInter(GUI::CPanel* pPanel, Task::CTaskContext& p);

//////////////////////////////////////////////////
// 簡易ステータス設定
//////////////////////////////////////////////////
// SLG版
void setEasyStatus(GUI::CPanel* pPanel, const SLG::CDataCharaSLG& chara, SLG::CSLGContext& p, bool bApper=true);
// Inter版
void setEasyStatus(GUI::CPanel* pPanel, Inter::CInterChara& chara, Task::CTaskContext& p);

//////////////////////////////////////////////////
// 戦闘ステータス設定
//////////////////////////////////////////////////
void setBattleStatus(GUI::CPanel* pPanel, const Chara::CDataCharaBase& chara);

//////////////////////////////////////////////////
// 基礎ステータス設定
//////////////////////////////////////////////////
// 共通部分
void addLvStr(int nID, int nAttr, string& sName);
void addLvStr(const Chara::CStatusAbility& skill, string& sName);
void setStatusBasicFund(GUI::CPanel* pPanel, const Chara::CDataCharaBase& chara);
void setStatusBasicTalent(GUI::CPanel* pPanel, const Chara::CDataCharaBase& chara, Task::CTaskContext& p);
void setStatusBasicSpirit(GUI::CPanel* pPanel, const Chara::CDataCharaBase& chara, Task::CTaskContext& p);
void setStatusBasicItem(GUI::CPanel* pPanel, const Chara::CDataCharaBase& chara, Task::CTaskContext& p);
// SLG版
void setStatusBasicSkill(GUI::CPanel* pPanel, const SLG::CDataCharaSLG& chara, SLG::CSLGContext& p);
void setStatusBasic(GUI::CPanel* pPanel, const SLG::CDataCharaSLG& chara, SLG::CSLGContext& p);
// Inter版
void setStatusBasicSkill(GUI::CPanel* pPanel, const Chara::CDataCharaInter& chara, Task::CTaskContext& p);
void setStatusBasic(GUI::CPanel* pPanel, const Inter::CInterChara& chara, Task::CTaskContext& p);

//////////////////////////////////////////////////
// 武器ステータス設定
//////////////////////////////////////////////////
// 共通部分
void setWeaponIcon(GUI::CPanel* pPanel, const Weapon::CDataWeaponBattle& weapon);
void setStatusWeaponLine(GUI::CPanel* pPanel, const Weapon::CDataWeaponBattle& weapon, bool bIcon=true);
void setStatusWeaponDetail(GUI::CPanel* pPanel, const Weapon::CDataWeaponBattle& weapon, int nEN, int nMental);

//////////////////////////////////////////////////
// チップソート設定
//////////////////////////////////////////////////
// SLG版
void setCharaSort(GUI::CPanelCtrl* pCtrl, int nID, SLG::CDataCharaSLG& chara);
void setChipGage(GUI::CGage* pGage, int nValue, int nMax);
// Intermisson版
void setChipSort(GUI::CPanel* pPanel, Inter::CInterChara& chara);
void setCharaSort(GUI::CPanelCtrl* pCtrl, int nID, Chara::CDataCharaInter& chara);
void setChipGage(GUI::CGage* pGage, int nValue);

} // namespace Status end
} // namespace BMW end