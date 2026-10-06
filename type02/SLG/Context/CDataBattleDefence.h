/*
	katze 05/05/05
	防御側戦闘データクラス
*/
#pragma once

#include "CDataBattleAbility.h"

namespace BMW{
namespace SLG{

class CDataBattleDefence : public CDataBattleAbility
{/**
	防御側戦闘データクラス

	こつが、戦闘デモとかに渡される
 */
public:
	// コンストラクタ
	CDataBattleDefence():nAction_(-4){}

	// 設定・取得
	int		getAction() const { return nAction_; }
	void	setAction(int nAction){ nAction_=nAction; }

	void	clearData(){ CDataBattleAbility::clearData(); nAction_=-4; }

private:
	// 行動
	// 回避したとか、防御とか、HITとか
	int nAction_;
};

} // namespace SLG end
} // namespace BMW end