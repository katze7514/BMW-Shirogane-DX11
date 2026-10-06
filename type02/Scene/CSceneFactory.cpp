#include "stdafx.h"

#include "IDScene.h"
#include "Scene.h"
#include "CSceneFactory.h"

namespace BMW{
namespace Scene{

smart_ptr<Task::ITaskList> CSceneFactory::createTaskList(int nID)
{
	Task::ITaskList* pList=NULL;

	switch(nID)
	{
	case ID::INIT_LOAD:	pList = new InitLoad::CInitLoadScene(); break;
	case ID::LOGO:		pList = new Logo::CLogoScene();			break;
	case ID::TITLE:		pList = new Title::CTitleScene();		break;
	case ID::DATA:		pList = new Data::CDataScene();			break;
	case ID::HERO:		pList = new Hero::CHeroScene();			break;
	case ID::TUTORIAL:	pList = new Tutorial::CTutorialScene();	break;
	case ID::GAME:		pList = new Game::CGameScene();			break;
	case ID::ADV:		pList = new ADV::CADVScene();			break;
	case ID::SLG:		pList = new SLG::CSLGScene();			break;
	case ID::INTER:		pList = new Inter::CInterScene();		break;
	case ID::DEMO:		pList = new Demo::CDemoScene();			break;
	case ID::ED:		pList = new ED::CEdScene();				break;
	case ID::MOVIE:		pList = new Movie::CMovieScene();		break;
	case ID::END:		pList = new END::CEndScene();			break;

	// 以下三つはトライアルモードの時は遷移しない
#ifndef BMW_TRIAL
	case ID::DICT_CHARA:pList = new Dict::CDictCharaScene();	break;
	case ID::DICT_SOUND:pList = new Dict::CDictSoundScene();	break;
	case ID::HANDOVER:	pList = new Handover::CHandoverScene();	break;
#endif // #ifndef BMW_TRIAL

	default: break;
	}

	return smart_ptr<Task::ITaskList>(pList);
}

} // namespace Scene end
} // namespace BMW end