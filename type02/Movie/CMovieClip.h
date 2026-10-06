/*
	katze 05/05/10
	MovieClip
*/
#pragma once

namespace BMW{
namespace Movie{

class CMovieClip : public Rule::CRuleListDraw
{/**
	MovieClip
 */
public:
	enum eState{
		NORMAL,
		STOP,	// Movieの停止
		END,	// Movieの終了
	};
	// コンストラクタ・デストラクタ
	CMovieClip():nEndLayer_(0){}
	virtual ~CMovieClip(){}

	// タスク
	virtual void OnAction(Task::CTaskContext*);
	virtual void OnReset(Task::CTaskContext*);

	// 操作
	bool IsStop();
	bool IsEnd();

	void getSize(LONG& lWidth, LONG& lHeight);
	void getDrawSize(LONG& lWidth, LONG& lHeight){ getSize(lWidth,lHeight); }

private:
	// 動作を停止してるレイヤー数
	int nEndLayer_;
	void callTaskAction(Task::CTaskContext* pContext);
};

} // namespace Moive end
} // namespace BMW end