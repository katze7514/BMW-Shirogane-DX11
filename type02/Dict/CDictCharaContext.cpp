#include "stdafx.h"

#include "../SLG/IDSLG.h"
#include "CDictCharaContext.h"

namespace BMW{
namespace Dict{

void CDictCharaItem::setCharaID(const string& sCharaID)
{
	sCharaID_=sCharaID;

	// ƒLƒƒƒ‰ID‚Ìæ“ª‚ÅPhase”»’è
	if(sCharaID_.find("PLAYER_",0)!=string::npos) data_.setPhase(SLG::Phase::PLAYER);
	else										  data_.setPhase(SLG::Phase::ENEMY);
}

CDictCharaContext::~CDictCharaContext()
{
	for(chara_item_map::iterator it = mapCharaItem_.begin(); it != mapCharaItem_.end(); ++it)
		DELETE_SAFE(it->second);

	mapCharaItem_.clear();
}

CDictCharaItem* CDictCharaContext::getAgainstCharaData(int nPhase)
{
	size_t nSize = mapCharaItem_.size();
	CDictCharaItem* pAgainst=NULL;
	chara_item_map::iterator it;
	while(pAgainst==NULL || nPhase==pAgainst->getCharaData().getPhase())
	{
		int nRand = CApp::rand_.Get(nSize);
		int i=0;
		for(it=mapCharaItem_.begin(); it!=mapCharaItem_.end(); ++it)
			if(i++ == nRand) break; // nRandˆÊ’u‚Ü‚Åiterator‚ði‚ß‚é

		if(it==mapCharaItem_.end()) pAgainst=NULL;
		else						pAgainst=it->second;
	}

	return pAgainst;
}

} // namespace Dict end
} // namespace BMW end
