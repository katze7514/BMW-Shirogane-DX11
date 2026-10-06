/*
	katze 05/03/25
	顔クロージャー
*/
#pragma once

#include "CDataFace.h"

namespace BMW{
namespace Face{

struct face_closure : boost::spirit::closure<face_closure, CDataFace,int>
{
	member1 val;
	member2 count;
};

} // namespace Face end
} // namespace BMW end