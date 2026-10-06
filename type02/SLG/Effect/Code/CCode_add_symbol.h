/*
	katze 06/05/23
	シンボル投入
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Effect{

class CCode_add_event_symbol : public BMW::Rule::IRuleTask
{/*
	イベントレイヤーへのシンボル投入

	No
	対象
	(対象がマップ絡みの時は)index
	x
	y
	
	が積まれている

	注！　これだけではフレームを進めないので
		　再生開始するには、nopを挟むこと
 */
public:
	enum eTarget{
		EVENT,		// イベントレイヤー
		EVENT_CHARA, // イベントレイヤーだが、xyがCHARA
		EVENT_MAP,	// イベントレイヤーだが、xyがMAP
	};
	// タスク
	void OnAction(Task::CTaskContext*);
};

class CCode_add_map_symbol : public BMW::Rule::IRuleTask
{/*
	マップレイヤーへのシンボル投入

	No
	対象
	index
	x
	y
	
	が積まれている

	注！　これだけではフレームを進めないので
		　再生開始するには、nopを挟むこと
 */
public:
	enum eTarget{
		MAP,		
		MAP_CHARA,
	};
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Effect end
} // namepsace SLG end
} // namepsace BMW end