#include "stdafx.h"

//#include "../Scene/IDGui.h"
#include "../Scene/IDScene.h"
#include "../Scene/ConstScene.h"

#include "../Chara/ConstChara.h"
#include "../Weapon/ConstWeapon.h"
#include "../Item/ConstItem.h"

#include "../SLG/Map/CMapChipState.h"
#include "CInitLoadScene.h"

namespace BMW{
namespace InitLoad{

void CInitLoadScene::OnInit(Task::CTaskContext* pContext)
{// 初期ロード画面表示初期化
	setState(FIRST);
	pContext->getApp()->getFoward()->visibleLoad(true);
}

void CInitLoadScene::OnAction(Task::CTaskContext* pContext)
{// ゲーム全体にかかわるデータの初期化
	switch(getState())
	{
	case FIRST: 
		// 一回、画面を描画するために、一回スルーする
		setState(INIT);
	break;

	case INIT: Init(pContext); break;

	default:
		getTaskListCtrl()->jumpTaskList(Scene::ID::LOGO);
		//getTaskListCtrl()->jumpTaskList(Scene::ID::TITLE);
		//pContext->getInput()->cursolVisible(true);
		pContext->getApp()->getFoward()->visibleLoad(false);
	break;
	}
}

void CInitLoadScene::Init(Task::CTaskContext* pContext)
{
	// Face Map
	pContext->getApp()->getFaceMap().setFaceMap(Config::Const::configDB_.getConfigFileStr("FACE_MAP"));

	// キャラID
	Chara::Const::charaID_.readMapFile(Config::Const::configDB_.getConfigFileStr("CHARA_ID"));
	// 武器ID
	Weapon::Const::weaponID_.readMapFile(Config::Const::configDB_.getConfigFileStr("WEAPON_ID"));
	// フラグID
	Scene::Const::flagID_.readMapFile(Config::Const::configDB_.getConfigFileStr("FLAG_ID"));
	
	// シナリオDB
	pContext->getApp()->getScenario().setScenarioDB(Config::Const::configDB_.getConfigFileStr("SCENARIO"));
	// キャラ DB
	const_cast<Chara::CCharaDB&>(pContext->getApp()->getChara()).setCharaDB(Config::Const::configDB_.getConfigFileStr("CHARA_INIT"));
	const_cast<Chara::CCharaDB&>(pContext->getApp()->getChara()).setStatusDB(Config::Const::configDB_.getConfigFileStr("CHARA_GROWTH"));
	// 武器 DB
	const_cast<Weapon::CWeaponDB&>(pContext->getApp()->getWeapon()).setWeaponDB(Config::Const::configDB_.getConfigFileStr("WEAPON_INIT"));
	// アビリティ DB
	const_cast<Ability::CAbilityDB&>(pContext->getApp()->getAbility()).setAbilityDB(Config::Const::configDB_.getConfigFileStr("ABILITY"));
	// 精神DB
	pContext->getApp()->getSpirit().setAbilityDB(Config::Const::configDB_.getConfigFileStr("SPIRIT"));
	// アイテム名
	Item::Const::itemName_.readMapFile(Config::Const::configDB_.getConfigFileStr("ITEM_NAME"));
	// アイテムDB
	pContext->getApp()->getItem().setAbilityDB(Config::Const::configDB_.getConfigFileStr("ITEM"));

	// 終了したら、jumpScene
	setState(END);
}

} // namespace InitLoad end
} // namespace BMW end