/*
	katze 05/05/19
	ADVシーンの各種ID
*/
#pragma once

namespace BMW{
namespace ADV{

namespace Rule{
enum eAPI
{
	MSG,
	BACK,
	WAIT_INPUT,
	WAIT_BGM,
	WAIT_SE,
	WAIT_FADE,
	WAIT_FRAME,
	SELECT,		// 選択肢
	SCENARIO,	// シナリオの自動選択
	ITEM_CTRL,
	MSG_BACK,	// バックログ
	MAIN,
};
} // namespace Rule end

namespace Flag{
enum eFlag{
	ID,
};
} // namespace Flag end

namespace Wait{
enum eWait{
	INPUT,
	BGM,
	SE,
	FADE,
	// SLGでのみ
	//SALLY,
};
} // namespace Wait end

namespace Scenario{
enum eScenario{
	NEXT,
	MEKAHISUI,
};
} // namespace Scenario end

} // namespace ADV end
} // namespace BMW end