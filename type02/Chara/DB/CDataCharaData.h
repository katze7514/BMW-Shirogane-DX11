/*
	katze 05/03/05
	update 06/01/18
	キャラDBの要素
*/
#pragma once

#include "../CDataCharaInit.h"
#include "../CDataCharaGrowthAbility.h"

namespace BMW{
namespace Chara{

class CDataCharaBase;

class CDataCharaData
{/*
	キャラDBの要素クラスの一つ

	キャラごとの初期状態と成長情報をもつ
*/
public:
	// 設定・取得
	const CDataCharaInit&			getInit() const { return init_; }
	CDataCharaInit*					getInitPtr(){ return &init_; }
	void							setInit(const CDataCharaInit& init){ init_=init; }
	const CDataCharaGrowthAbility&	getAbility() const { return ability_; }
	void							setAbility(const CDataCharaGrowthAbility& ability){ ability_=ability; }

	const smart_ptr<CDataCharaData>&	getParentData()const{ return pParent_; }
	void								setParentData(const smart_ptr<CDataCharaData>& pParent){ pParent_=pParent; }

	// 操作
	// ↓bContがコンテニュー時のデータ復元フラグ
	void			applyInit(CDataCharaBase* base, bool bCont=false);
	void			applyInitSub(CDataCharaBase* base, bool bCont=false); // 差分適用
	void			applyAbility(CDataCharaBase* base, int nLv=0);

private:
	CDataCharaInit			init_;
	CDataCharaGrowthAbility ability_;
	// 親キャラデータ
	smart_ptr<CDataCharaData> pParent_;
};

} // namespace Chara end
} // namespace BMW end