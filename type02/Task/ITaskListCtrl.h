/*
	katze 05/02/17
	update 06/02/01 親クラスをITaskBaseに
	TaskListを制御するクラス
*/
#pragma once

#include "ITaskBase.h"

namespace BMW{
namespace Task{

class ITaskList;
class ITaskListFactory;

class ITaskListCtrl : public ITaskBase
{/**
	タスクListコントローラインターフェイス
 */
public:
	// 処理メッセージID
	enum eMessageID{
		MES_NO,
		MES_CALL,
		MES_CALL_FAST,
		MES_RETURN,
		MES_JUMP,
		MES_EXIT,
	};
	// コンストラクタ・デストラクタ
	ITaskListCtrl(){ visible(true); }
	virtual ~ITaskListCtrl(){}

	// 設定・取得
	virtual smart_ptr<ITaskListFactory>&	getTaskListFactory()=0;
	virtual void							setTaskListFactory(const smart_ptr<ITaskListFactory>& pFactory)=0;
	
	// 操作
	// 指定されたIDのTaskListをコールする
	// bFastフラグが立ってる時は現在のTaskListはDeleteされない
	virtual void callTaskList(int nNextID,bool bFast=false)=0;
	// 現在のTaskListからリターンする
	virtual void returnTaskList()=0;
	// 指定されたIDのTaskListにジャンプする
	virtual void jumpTaskList(int nNextID)=0;
	// 動作を終了する
	virtual void exitTaskList()=0;

	// 現在の動作TaskList情報を取得
	virtual int						getCurrentTaskListID()=0;
	virtual smart_ptr<ITaskList>&	getCurrentTaskList()=0;
	virtual bool					IsEnd() const=0;

protected:
	// returnTaskListの際に戻るTaskListが無い時に呼び出される
	virtual void noExist(CTaskContext*)=0;
};

} // namespace Task end
} // namespace BMW end
