/*
	katze 05/03/13
	Int2String‚ÆString2Int‚ª‰Â”\‚ÈMap
*/
#pragma once

#include "CIntMap.h"
#include "CStringMap.h"

namespace katzeSDK{
namespace Misc{

class CIntBiStringMap
{/**
	Int©¨String Map
 */
public:
	// ’l‚Ìæ“¾
	const string&	getValueStr(int nKey, const string& s="") const
					{
						return int2string_.getValue(nKey,s);
					}

	int				getValueInt(const string& sKey) const
					{
						return string2int_.getValue(sKey);
					}

	// ’l‚Ìİ’è
	void			readMapFile(const string& sFile)
					{
						int2string_.readMapFile(sFile);
						string2int_.readMapFile(sFile);
					}

private:
	CIntMap		int2string_;
	CStringMap	string2int_;
};

} // namespace Misc end
} // namespace katzeSDK end