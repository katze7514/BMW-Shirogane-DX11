/*
	katze 05/07/25
	反撃はする壁動作
*/
#pragma once

#include "CActionNormal.h"

namespace BMW{
namespace SLG{
namespace Action{

class CActionWallCounter : public CActionNormal
{/**
	反撃はする壁動作
 */
public:
	// デストラクタ
	virtual ~CActionWallCounter(){}

	virtual void Serialize(ISerialize& s);
	virtual void action(SLG::CDataCharaSLG& chara, CSLGContext& p);
	virtual void getActionParam(int& nActionID, list<int>& listParam);

	virtual int	actionCounter(SLG::CDataCharaSLG& chara, int nDist, int nRealDist, int nHeight, CSLGContext& context, int nHP, bool bBackUp);
};

} // namespace Action end
} // namespace SLG end
} // namespace BMW end