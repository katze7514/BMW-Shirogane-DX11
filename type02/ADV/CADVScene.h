/*
	katze 05/05/18
	ADVシーン
*/
#pragma once

#include "../Scene/CScene.h"
#include "CADVContext.h"

namespace BMW{
namespace ADV{

class CADVScene : public Scene::CScene<CADVContext>
{/**
	ADVシーン
 */
public:
	enum eState{
		NORMAL,
		END,
	#ifdef BMW_DEBUG
		CONTINUE,
	#endif
	};
	enum ePriority{
		BACK,
		MSG_L,
		MSG_R,
		VM,
	};

	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

#ifdef BMW_DEBUG
	void eventFade(Task::CTaskContext*);

private:
	Input::CTaskInput* pInput_;
#endif
};

} // namespace ADV end
} // namespace BMW end