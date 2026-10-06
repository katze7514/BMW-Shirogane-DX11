/*
	katze 06/01/20
	インターフェイスイベント
*/
#pragma once

namespace BMW{
namespace GUI{

class IEvent
{/**
	インターフェイスのイベントデータを表現する基底クラス
 */
public:
	// デストラクタ
	virtual ~IEvent(){}
	// インターフェイス
	// イベント種別
	int		getKind()const{ return -1; }
	// イベントを発生させたGUI ID
	const smart_ptr<Task::ITaskBase>&	getEventTask()const{ return pTask_; }
	void								setEventTask(const smart_ptr<Task::ITaskBase>& pTask){ pTask_=pTask; }

protected:
	smart_ptr<Task::ITaskBase> pTask_; // イベントを発生させたタスク
};

} // namespace GUI end
} // namespace BMW end