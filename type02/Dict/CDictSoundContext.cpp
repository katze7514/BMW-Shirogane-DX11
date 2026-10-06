#include "stdafx.h"

#include "CDictSoundContext.h"

namespace BMW{
namespace Dict{

CDictSoundContext::~CDictSoundContext()
{
	sound_item_map::iterator it;
	for(it=mapSoundItem_.begin(); it!=mapSoundItem_.end(); ++it)
		DELETE_SAFE(it->second);

	mapSoundItem_.clear();
}

} // namespace Dict end
} // namespace BMW end
