/*
	katze 05/06/15
	シナリオクロージャ
*/
#pragma once

#include "CDataScenario.h"

namespace BMW{
namespace Scenario{

struct scenario_base_closure : public boost::spirit::closure<scenario_base_closure, CDataScenarioBase, int>
{
	member1 val;
	member2 src;
};

} // namespace Scenario end
} // namespace BMW end