/*
	katze 05/03/24
	マップチップのオブジェクトを表現する
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Map{

class CMapChipObj : public Task::CTaskListDraw
{/**
	マップチップのオブジェクトを表現するクラス

	ま、実質的にCMapChipのスプライトクラスやね
 */
public:
	enum ePriority
	{// タスクプライオリティ
		BASE,
		OBJ,
	};
	// デストラクタ
	virtual ~CMapChipObj(){}
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end