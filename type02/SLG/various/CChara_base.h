/*
	katze 05/07/20
	キャラ判定系
*/
#pragma once

#include "CScript_base.h"

namespace BMW{
namespace SLG{
class CSLGContext;

namespace Chara{

class CChara_base : public Script::CScript_base
{/**
	キャラ判定系
 */
public:
	// デストラクタ
	virtual ~CChara_base(){}

	// 判定
	// nIndexにいるキャラのフェーズが返ってくる
	// いない場合は-1
	static int  IsHere(int nIndex, CSLGContext& context);
	static bool IsHere(int nID, int nIndex, CSLGContext& context);
	static bool IsHere(const string& sID, int nIndex, CSLGContext& context);
};

} // namespace Chara end
} // namespace SLG end
} // namespace BMW end