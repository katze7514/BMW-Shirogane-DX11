/*
	katze 06/04/06
	SLGパートの判定系のベース
*/
#pragma once

namespace BMW{
namespace SLG{
class CSLGContext;

class ISlgCond
{/**
	SLGパートの判定系のベース
 */
public:
	// デストラクタ
	~ISlgCond(){}
	// 判定
	virtual bool judg(CSLGContext* p){ return false; }
};

} // namespace SLG end
} // namespace BMW end