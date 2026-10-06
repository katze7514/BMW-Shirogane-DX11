/*
	katze 05/06/15
	シナリオシンボル
*/
#pragma once

#include "CDataScenario.h"

namespace BMW{
namespace Scenario{

struct scene_sym : public boost::spirit::symbols<int>
{
	scene_sym()
	{
		add
			("ADV",	CDataScenarioBase::ADV)
			("SLG",	CDataScenarioBase::SLG)
		;
	}
};

} // namespace Scenario end
} // namespace BMW end