/*
	katze 06/07/01
	移動攻撃で対象ランダム
	アカバイン用
*/
#pragma once

#include "CActionNormalParam.h"

namespace BMW{
namespace SLG{
namespace Action{

class CActionAkabine : public CActionNormalParam
{/*
	移動攻撃で対象ランダム
	アカバイン用
 */
public:
	virtual ~CActionAkabine(){}

	virtual void Serialize(ISerialize& s);
	virtual void getActionParam(int& nActionID, list<int>& listParam);

	virtual void action(SLG::CDataCharaSLG& chara, CSLGContext& p);	
};

} // namespace Action end
} // namespace SLG end
} // namespace BMW end