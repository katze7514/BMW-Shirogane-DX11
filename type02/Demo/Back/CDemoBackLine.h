/*
	katze 06/03/23
	戦闘デモ背景の1ライン
*/
#pragma once

#include "IDemoBackLine.h"

namespace BMW{
namespace Demo{
using katzeSDK::Misc::CFixedNum;

class CDemoBackLine : public IDemoBackLine
{/**
	戦闘デモ背景の1ライン
 */
public:
	// デストラクタ
	virtual ~CDemoBackLine();

	// タスク
	void Task(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// 設定
	void setSymbol(Task::ITaskBase* pSymbol, int nIndex){ pSymbol_[nIndex]=pSymbol; pSymbol_[nIndex]->setParent(smart_ptr<Task::ITaskBase>(this,false)); }
	void setLeft(int nLeft, int nSide){ point_[nSide].nLeft_=nLeft<<16; }
	void setRight(int nRight, int nSide){ point_[nSide].nRight_=nRight<<16; }
	void setMove(int nMove, int nSide){ point_[nSide].nMove_=nMove<<16; }

	void setSymbolX(int nX, int nIndex){ nX_[nIndex]=nX; pSymbol_[nIndex]->setX(nX<<16); }
	void setY(int nY){ drawInfo_.setY(nY<<16); }

private:
	Task::ITaskBase* pSymbol_[2];
	// ↑のX座標
	CFixedNum nX_[2];

	// 切り替えポイントとその行き先
	struct change_point{
		int nLeft_;
		int nRight_;
		int nMove_;
	} point_[2];
};

} // namespace Demo end
} // namespace BMW end