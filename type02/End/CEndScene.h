/*
	katze 08/06/21
	エンドシーン
*/
#pragma once

#include "../Scene/CScene.h"

namespace BMW{
namespace END{

class CEndScene : public Scene::CScene<>
{/**
	エンドシーン

	クリア後の特典解禁したりとかそういうことをする
 */
public:
	enum eState{
		FADE,
		FADE_END,
		WAIT,
		END,
	};
	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	
	// イベントハンドラ
	void eventFade(Task::CTaskContext*);

private:
	// 表示グラフィック
	GUI::CGraphic release_;

	// 表示するグラフィックリスト
	list<string> listGuiID_;
	// 表示するIDのイテレータ
	list<string>::iterator it_;

	// フレーム数
	int nFrame_;
};

} // namespace END end
} // namespace BMW end