#include "stdafx.h"

#include "CDataSymbolMovieClip.h"

namespace BMW{
namespace Movie{

CDataSymbolMovieClip::~CDataSymbolMovieClip()
{
	beginLayer();
	while(!endLayer()) DELETE_SAFE(*nextLayer());
}

} // namespace Movie end
} // namespace BMW end