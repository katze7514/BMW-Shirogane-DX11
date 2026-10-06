#include "stdafx.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/CMapSymbolDB.h"

#include "../Map/CMap.h"
#include "../Map/CMapChip.h"
#include "../Map/CMapChipChara2.h"
#include "../Map/CMapChipCharaSprite2.h"

#include "CSally_add_chara_map2.h"

namespace BMW{
namespace SLG{
namespace Sally{

void CSally_add_chara_map2::setupCharaChip(CDataCharaSLG* pChara, int nIndex, CSLGContext* p)
{
	// マップチップキャラの生成
	Map::CMapChipChara2* pChip = new Map::CMapChipChara2();
	pChip->setID(p->getTargetChara());
	// 状態の共有
	pChip->setCharaState(smart_ptr<CCharaState>(pChara->getStatePtr(),false));

	// マップへの追加
	p->getMapChip(nIndex)->addTask(pChip, Map::CMapChip::CHARA);

	// マップチップキャラスプライトの生成
	Map::CMapChipCharaSprite2* pSprite = new Map::CMapChipCharaSprite2();
	pSprite_ = pSprite;
	// マップキャラへの追加
	pChip->addTask(pSprite,Map::CMapChipChara2::CHARA);
	// チップの設定
	smart_ptr<CMapSymbolDB>& pDB = pChara->getMapSymbol();

#ifdef BMW_DEBUG
	CDbg().Out("Target %d %d %d",p->getTargetChara(), nIndex, pChara->getCharaID());
#endif

	// マップチップのアニメ展開
	using namespace ChipMovie;
	// 歩き
	pSprite->setChipMovie(pDB->createSymbolStr("WALK_TOP"),WALK_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("WALK_LEFT"),WALK_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("WALK_BOTTOM"),WALK_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("WALK_RIGHT"),WALK_RIGHT);
	// ジャンプ
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_TOP1"),JUMP_READY_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_TOP2"),JUMP_UP_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_TOP3"),JUMP_DOWN_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_LEFT1"),JUMP_READY_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_LEFT2"),JUMP_UP_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_LEFT3"),JUMP_DOWN_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_BOTTOM1"),JUMP_READY_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_BOTTOM2"),JUMP_UP_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_BOTTOM3"),JUMP_DOWN_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_RIGHT1"),JUMP_READY_RIGHT);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_RIGHT2"),JUMP_UP_RIGHT);
	pSprite->setChipMovie(pDB->createSymbolStr("JUMP_RIGHT3"),JUMP_DOWN_RIGHT);
	// 静止
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_TOP"),BEFORE_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_LEFT"),BEFORE_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_BOTTOM"),BEFORE_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_RIGHT"),BEFORE_RIGHT);
	// 行動済み静止
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_TOP"),AFTER_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_LEFT"),AFTER_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_BOTTOM"),AFTER_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_RIGHT"),AFTER_RIGHT);
	// 静止ピンチ
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_PINCH_TOP"),BEFORE_PINCH_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_PINCH_LEFT"),BEFORE_PINCH_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_PINCH_BOTTOM"),BEFORE_PINCH_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("BEFORE_PINCH_RIGHT"),BEFORE_PINCH_RIGHT);
	// 行動済み静止ピンチ
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_PINCH_TOP"),AFTER_PINCH_TOP);
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_PINCH_LEFT"),AFTER_PINCH_LEFT);
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_PINCH_BOTTOM"),AFTER_PINCH_BOTTOM);
	pSprite->setChipMovie(pDB->createSymbolStr("AFTER_PINCH_RIGHT"),AFTER_PINCH_RIGHT);
}

} // namespace Sally end
} // namesapce SLG end
} // namespace BMW end