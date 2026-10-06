#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "IDAction.h"
#include "CActionAkabine.h"

namespace BMW{
namespace SLG{
namespace Action{

void CActionAkabine::Serialize(ISerialize& s)
{// 書き出しだけ
	if(s.IsStoring())
	{
		int nID = Action::AKABINE;
		s << nID;
		nID = 3;
		s << nID;
		nID = getWait();
		s << nID;
		nID = getMove();
		s << nID;
		nID = getSnipeChara();
		s << nID;
	}
}

void CActionAkabine::getActionParam(int& nActionID, list<int>& listParam)
{
	nActionID=Action::AKABINE;
	listParam.push_back(getWait());
	listParam.push_back(getMove());
	listParam.push_back(getSnipeChara());
}

void CActionAkabine::action(CDataCharaSLG& chara, CSLGContext& p)
{// アカバイン思考
 // この思考ルーチンは、対象をランダムに選ぶ

	if(getWait()<=0)
	{
		list<int>& listPhase = getPhaseList(p.getPhase(), p);
		int n = CApp::rand_.Get((int)listPhase.size());
		list<int>::iterator it;
		int i=0;
		for(it=listPhase.begin(); n!=i; ++it, ++i);
		// 選んだキャラを攻撃対象とする
		setSnipeChara(*it);
	}

	// 通常思考
	CActionNormalParam::action(chara,p);
}

} // namespace Action end
} // namespace SLG end
} // namespace BMW end