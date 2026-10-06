/*
	katze 05/03/09
	•ŠíƒNƒ[ƒWƒƒ
*/
#pragma once

namespace BMW{
namespace Weapon{

struct weapon_info_closure : public boost::spirit::closure<weapon_info_closure, int, string>
{
	member1 val;
	member2 name;
};

struct collab_closure : public boost::spirit::closure<collab_closure, int, int>
{
	member1 val;
	member2 weapon;
};

} // namespace Weapon end
} // namespace BMW end