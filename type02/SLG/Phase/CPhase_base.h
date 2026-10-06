/*
	katze 05/07/20
	フェーズ系のベース
*/
#pragma once

#include "../various/CScript_base.h"

namespace BMW{
namespace SLG{
class CSLGContext;

namespace Phase{

class CPhase_base : public Script::CScript_base
{/**
	フェーズ系のベース
 */
public:
	// デストラクタ
	virtual ~CPhase_base(){}

	// フェーズ動作実行
	void actionPhase(Task::CTaskContext*);

	// 判定
	static bool IsTurn(int nTurn, Task::CTaskContext& context);
	static bool IsPhase(int nPhase, Task::CTaskContext& context);
	static bool IsTurnPhase(int nTurn, int nPhase, Task::CTaskContext& context);
};

} // namespace Phase end
} // namespace SLG end
} // namespace BMW end