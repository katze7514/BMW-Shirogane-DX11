#include "stdafx.h"

#include "../../SLG/IDSLG.h"
#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CAbility_Counter.h"

namespace BMW{
namespace Ability{
namespace{
const int anFP[10]={0,20,25,30,35,40,45,50,55,60};
}
int CAbility_Counter::getGetFP(int nAttr)const
{
	if(nAttr<-1) return 0;
	return anFP[nAttr];
}

/////////////////////////////////////////////
// Žg—p
/////////////////////////////////////////////
bool CAbility_Counter::enable(const SLG::CDataCharaSLG& base, const SLG::CDataCharaSLG& target, int nAttr, SLG::CSLGContext& p)
{
	// LV‚Æ‹Z—Ê·‚É‚æ‚éŠm—¦
	// ”­“®Šm—¦‚ðŒvŽZ
	// ((Ž©•ª‚Ì‹Z—Ê-‘ŠŽè‚Ì‹Z—Ê)/10 + CounterLv)/16
	// ¨(Ž©•ª‚Ì‹Z—Ê-‘ŠŽè‚Ì‹Z—Ê + CounterLv*10)*5/8
	int nRate =  (base.getBattle().getSkill() - target.getBattle().getSkill())/10 + nAttr;
	nRate = (nRate*100)/16;
	if(nRate<0) nRate=0;
	// “G‚Ìê‡‚ÍA‚³‚ç‚É”¼•ª‚ÌŠm—¦
	if(base.getPhase()!=SLG::Phase::PLAYER) nRate /= 2;
	return (int)CApp::rand_.Get(100)+1<=nRate;
}

} // namespace Ability end
} // namespace BMW end
