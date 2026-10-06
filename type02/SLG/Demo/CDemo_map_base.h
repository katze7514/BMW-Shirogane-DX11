/*
	katze 06/03/17
	マップ上デモの基底クラス
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Demo{
class CDemo_map;

class CDemo_map_base : public Task::ITaskBase
{/**
	マップ上デモの基底クラス
 */
public:
	enum eSide{
		LEFT,
		RIGHT,
	};
	// デストラクタ
	virtual ~CDemo_map_base(){};

	// タスク
	virtual void OnInit(Task::CTaskContext* pContext);

protected:
	// アクション
	void actionEnd(Task::CTaskContext*);

	// デモデータ
	smart_ptr<CDemo_map> pDemo_;
};

} // namespace Demo end
} // namespace SLG end
} // namespace BMW end