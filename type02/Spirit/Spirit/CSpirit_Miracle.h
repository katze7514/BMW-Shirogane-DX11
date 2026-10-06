/*
	katze 06/03/14
	MIRACLEコマンド
*/
#pragma once

#include "../../Ability/IDataAbility.h"

namespace BMW{
namespace Spirit{
class CSpirit_Acc;
class CSpirit_Jump;
class CSpirit_Fireball;
class CSpirit_Hit;
class CSpirit_Avoid;
class CSpirit_Power;
class CSpirit_Fortune;
class CSpirit_Effort;

class CSpirit_Miracle : public Ability::IDataAbility
{/**
	奇跡
*/
public:
	// コンストラクタ・デストラクタ
	CSpirit_Miracle(const string& sGuiDefID);
	~CSpirit_Miracle();
	
	// 適用
	// ステータス適用
	void			applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void			backStatus(SLG::CDataCharaSLG& slg, int nAttr);
	// SLGデータ補正値計算
	// 個別対応
	void applyOffset(SLG::COffsetMove& data);
	void applyOffset(SLG::COffsetBattle& data, int nDamage);
	// 使用可能かのチェック
	bool			enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext);
	// 使用
	void			use(Chara::CDataCharaBattle& battle, int nAttr, SLG::CSLGContext& p);

private:
	// 加速
	CSpirit_Acc* acc_;
	// 跳躍
	CSpirit_Jump* jump_;
	// 熱血
	CSpirit_Fireball* fire_;
	// 必中
	CSpirit_Hit* hit_;
	// ひらめき
	CSpirit_Avoid* avoid_;
	// 気合
	CSpirit_Power* power_;
	// 幸運
	CSpirit_Fortune* for_;
	// 努力
	CSpirit_Effort* effort_;
};

} // namespace Spirit end
} // namespace BMW end

