/*
	katze 05/06/29
	ADVのユーザー定義サブルーチンのMAP
*/
#pragma once

#include "IDADV.h"

namespace BMW{
namespace ADV{

class CAdvMap : public katzeSDK::Misc::CStringMap
{/**
	ADVのユーザー定義サブルーチンのMAP
 */
public:
	CAdvMap()
	{
		writeMap("MAIN",Rule::MAIN);
	}

	int addMap(const string& sID){ writeMap(sID, Rule::MAIN+map_.size()); return Rule::MAIN+map_.size()-1;}
};

} // namespace ADV end
} // namespace BMW end