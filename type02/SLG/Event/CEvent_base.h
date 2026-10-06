/*
	katze 06/04/09
	イベントハンドラ
*/
#pragma once

namespace BMW{
namespace SLG{

class CEvent_base : public Task::ITaskList
{/**
	イベントハンドラ
 */
public:
	enum eState{
		NORMAL,
		END,
		WAIT,
	};
	// コンストラクタ
	CEvent_base(int nID):nID_(nID){}
	virtual ~CEvent_base(){}
	// アクション
	void OnInit(Task::CTaskContext*){ setState(NORMAL); }
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// アクセッサ
	int		getID()const{ return nID_; }
	void	setID(int nID){ nID_=nID; }
	
#ifdef BMW_DEBUG
	void	setEventName(const string& sEventName){ sEventName_ = sEventName; }
#endif

private:
	int nID_;

#ifdef BMW_DEBUG
	string sEventName_;
#endif
};

} // namespace SLG end
} // namespace BMW end