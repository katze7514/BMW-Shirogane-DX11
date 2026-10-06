/*
	katze 05/07/10
	計算というか設定とか
*/
#pragma once

namespace BMW{

namespace SLG{
class CSLGContext;
} // namespace SLG end

namespace Weapon{
class CDataWeaponBattle;
class CDataWeaponBattleCollab;

// 武器データのインスタンス生成
CDataWeaponBattle* createWeapon(int nID);

// 武器データの設定
// これを呼ぶ前にpBattle内のstatus_は設定しておくこと
void setWeaponData(CDataWeaponBattle* pBattle, SLG::CDataCharaSLG& chara, SLG::CSLGContext& context, bool bCont=false);
void setWeaponData(CDataWeaponBattleCollab* pBattle, SLG::CDataCharaSLG& chara, SLG::CSLGContext& context, bool bCont=false);

// 武器アイコンとのマップ関数
int getIconID(int nKind, GUI::CPanelCtrl* pCtrl);
int getAttrID(bool bP, bool bM, bool bT, bool bF, GUI::CPanelCtrl* pCtrl);

} // namespace Weapon end
} // namespace BMW end