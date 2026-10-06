/*
	katze 05/04/25
	genereted by code_gen.rb
	cursol_pos
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace VM{
namespace Code{

class CCode_cursol_pos : public Rule::IRuleTask
{/**
	cursol_pos
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);

	// 設定・取得
	int  getX()const{ return nX_; }
	void setX(int nX){ nX_=nX; }
	int  getY()const{ return nY_; }
	void setY(int nY){ nY_=nY; }

private:
	int nX_,nY_;
};

} // namespace Code end
} // namespace VM end
} // namespace BMW end
