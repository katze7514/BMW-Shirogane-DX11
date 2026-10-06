/*
	katze 05/05/15
	マップシンボル
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Map{

struct map_symbol : public boost::spirit::symbols<>
{
	map_symbol()
	{
		add
			("TEST",0)
			("STREET",1)
			("KOUEN",2)
			("KOHJO_OUT",3)
			("KOHJO_IN",4)
		;
	}
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end