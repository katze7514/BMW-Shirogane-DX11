/*
	katze 06/07/10
	ムービーシーン
*/
#pragma once

namespace BMW{
namespace Movie{

class CMovieScene : public Task::ITaskList
{/**
	ムービーシーン

	ムービーを再生するだけ
 */
public:
	enum ePriority{
		BACK,
		MOVIE,
		YESNO,
	};
	enum eState{
		FADE,
		PLAY,
		END,
		WAIT,
	};
	// コンストラクタ・デストラクタ
	CMovieScene();
	~CMovieScene();
	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	// イベントリスナ
	void eventFade(Task::CTaskContext*);
	
private:
	Movie::CMovieClip* pMovie_;
	Movie::CSymbolDB*  symbol_;
};

} // namespace ED end
} // namespace BMW end