#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"
#include "../../Scene/IScene.h"

#include "../IDSLG.h"
#include "../Action/IAction.h"
#include "../Map/CMap.h"
#include "../Map/CMapChip.h"
#include "../Map/CMapChipChara2.h"
#include "../Map/CMapChipState.h"

#include "CMapSymbolDB.h"
#include "CDataCharaSLG.h"
#include "CSLGDef.h"
#include "CSLGContext.h"

#include "SlgFunctor.h"

namespace BMW{
namespace SLG{

CSLGContext::CSLGContext():bQuickLoad_(true)
{ 
	setBgm(-1);
	nCircleX_=nCircleY_=0;
	clearFlag();
	clearCharaData();
	clearWeaponData();

	slgDef_ = new CSLGDef();
}

CSLGContext::~CSLGContext()
{
	clearCharaData();
	clearWeaponData();

	DELETE_SAFE(slgDef_);

	for(int i=0; i<5; ++i)
	{ 
		DELETE_SAFE(pMapChip_[i]);
		Map::CMapChipState::setGraphic(NULL,MapChip::GRID+i);
	};
}

void CSLGContext::clearFlag()
{
	setValue(0,Flag::WIPEOUT);
	setID(-1);

	setTurn(1);
	setValue(0,Flag::BP);
	setValue(0,Flag::FP);
	setValue(0,Flag::EXPERT);
	setPhase(Phase::PLAYER);

	setValue(0,Flag::PHASE_CHANGE);
	setValue(0,Flag::ENEMY_RESET);

	setValue(0,Flag::VICTORY);
	setValue(0,Flag::LOSE);
	setValue(0,Flag::EXPERT_C);

	setValue(1,Flag::DEMO);
	setValue(0,Flag::NEXT_WEAPON_ID);

	setTargetMap(-1);
	setTargetWeapon(-1);
	setCtrlWeapon(-1); 
	setTargetChara(-1);
	setCtrlChara(-1); 

	setValue(INT_MAX,Flag::NEXT_SALLY_ID);
	//setValue(1,Flag::SALLY_INIT);
}

void CSLGContext::initValue(int nValue, int nID)
{
	// コンテニュー時は設定しない
	if(getValue(Flag::CONTINUE)) setValue(nValue,nID);
}

//////////////////////////////////////////////////
// 養成データ
//////////////////////////////////////////////////
Chara::CDataCharaTrain& CSLGContext::getTrain(int nID)
{ 
	return slgDef_->getTrain(nID);
}

void CSLGContext::setTrain(int nID, const Chara::CDataCharaTrain& pTrain)
{ 
	slgDef_->setTrain(nID,pTrain);
}

//////////////////////////////////////////////////
// マップ
//////////////////////////////////////////////////
Map::CMapChip* CSLGContext::getMapChip(int nIndex)
{ 
	return nIndex>=0 ? pMap_->getMapChip(nIndex) : NULL; 
}

Map::CMapChip* CSLGContext::getTargetMapChip()
{ 
	int nTarget = getValue(Flag::TARGET_MAP);
	return nTarget>=0 ? pMap_->getMapChip(nTarget) : NULL; 
}

void CSLGContext::createMapChip()
{
	const string sID[]={"SQU_GRID_G","SQU_ACTIVE_G","SQU_BLUE_G","SQU_RED_G","SQU_YELLOW_G"};
	for(int i=0; i<5; ++i)
	{
		pMapChip_[i] = new BMW::GUI::CGraphic();
		getScene()->getGuiDefDB().getSymbolDB().setGraphicGui(pMapChip_[i],	sID[i]);
		Map::CMapChipState::setGraphic(pMapChip_[i],MapChip::GRID+i);
	}
}

//////////////////////////////////////////////////
// キャラ
//////////////////////////////////////////////////
CDataCharaSLG* CSLGContext::getCharaData(int nID)
{ 
	chara_map::iterator it = mapChara_.find(nID);
	if(it==mapChara_.end()) return NULL;
	return it->second;
}

CDataCharaSLG* CSLGContext::getCharaData(const string& sID)
{ 
	return getCharaData(slgDef_->getSlgID(sID));
}

void CSLGContext::setCharaData(int nID, CDataCharaSLG* pData, bool bPhase)
{ 
	//CDbg().Out("SetChara %d %d",nID,pData->getBattle().getID());
	mapChara_.insert(pair<int, CDataCharaSLG*>(nID, pData));
	if(bPhase)
	{
		switch(pData->getPhase())
		{
		case Phase::PLAYER:		listPlayer_.push_back(nID);		break;
		case Phase::ENEMY:		listEnemy_.push_back(nID);		break;
		case Phase::NEUTRAL:	listNeutral_.push_back(nID);	break;
		default: break;
		}
	}
}

void CSLGContext::delCharaData(int nID)
{// これを呼び出すと単に消されるためセーブデータに残らない
	// マップデータから削除
	chara_map::iterator it = mapChara_.find(nID);
	if(it==mapChara_.end()) return;

//	int				nPhase	= it->second->getPhase();
	CDataCharaSLG*	pChara	= it->second;

	// もし、死亡セットにいたら削除
	// セーブデータ対象外は、ペナルティ対象外でもある
	setDeath_.erase(pChara->getID());

	// フェーズから削除
	//delPhase(nID,nPhase);

	// 武器がロード済みなら、武器データの削除
	if(pChara->IsWeaponLoad())
		delCharaWeaponData(pChara);

	// キャラデータは最後に削除
	DELETE_SAFE(pChara);

	// マップから削除
	mapChara_.erase(it);
}

CDataCharaSLG* CSLGContext::delMapCharaData(int nID, bool bPhase)
{// マップから削除する
	chara_map::iterator it = mapChara_.find(nID);
	if(it==mapChara_.end()) return NULL;

	CDataCharaSLG* pChara = it->second;
	// マップから削除
	mapChara_.erase(it);
	// フェーズからも削除
	if(bPhase)
		delPhase(pChara->getID(), pChara->getPhase());

	return pChara;
}

void CSLGContext::clearCharaData()
{
	chara_map::iterator it_c;
	for(it_c=mapChara_.begin(); it_c!=mapChara_.end(); ++it_c)
		DELETE_SAFE(it_c->second);

	mapChara_.clear();
}

//////////////////////////////////////////////////
// 武器
//////////////////////////////////////////////////
Weapon::CDataWeaponBattle* CSLGContext::getWeaponData(int nID)
{ 
	weapon_map::iterator it = mapWeapon_.find(nID);
	if(it==mapWeapon_.end()) return NULL;
	return it->second;
}

void CSLGContext::setWeaponData(int nID, Weapon::CDataWeaponBattle* pData)
{ 
	mapWeapon_.insert(pair<int, Weapon::CDataWeaponBattle*>(nID,pData));
}

void CSLGContext::delWeaponData(int nID, CDataCharaSLG* pChara)
{
	weapon_map::iterator it;
	it = mapWeapon_.find(nID);
	if(it!=mapWeapon_.end())
	{
		(it->second)->delWeapon(pChara, *this);
		DELETE_SAFE(it->second);
		mapWeapon_.erase(it);
	}
}

void CSLGContext::delCharaWeaponData(CDataCharaSLG* pChara)
{
	pChara->getBattle().beginWeapon();
	while(!pChara->getBattle().endWeapon())
		delWeaponData(*pChara->getBattle().nextWeapon(), pChara);
}

void CSLGContext::clearWeaponData()
{
	weapon_map::iterator it_w;
	for(it_w=mapWeapon_.begin(); it_w!=mapWeapon_.end(); ++it_w)
		DELETE_SAFE(it_w->second);

	mapWeapon_.clear();
}

void CSLGContext::delRange(int nRange, int nIndex)
{
	if((int)rangeWeapon_.size()<=nRange) return;
	range_list& listRange = getRangeList(nRange);
	range_list::iterator it;
	for(it=listRange.begin(); it!=listRange.end(); ++it)
	{//	指定されたnIndexを削除
		if(it->bAttack_ && it->nIndex_==nIndex)
		{
			listRange.erase(it);
			break;
		}
	}
}

void CSLGContext::clearRange()
{
	Map::CMapChipState* pState;
	weapon_range::iterator it;
	range_list::iterator lit;
	for(it=rangeWeapon_.begin(); it!=rangeWeapon_.end(); ++it)
	{
		for(lit=it->begin(); lit!=it->end(); ++lit)
		{
			pState = getMapChip(lit->nIndex_)->getMapChipState();
			pState->setAttack(-1);
			pState->setRealDist(-1);
			pState->setAtkHeight(-1);
			pState->setFieldAttack(-1);
			pState->resetFieldToward();
		}
		it->clear();
	}
	
	rangeWeapon_.clear();
}

bool CSLGContext::IsRangeChara(int nHeight, bool bF)
{
	Map::CMapChipState* pState;
	weapon_range::iterator it;
	range_list::iterator rit;
	for(it=rangeWeapon_.begin(); it!=rangeWeapon_.end(); ++it)
	{
		if(!it->empty())
		{
			rit=it->begin();
			pState = getMapChip(rit->nIndex_)->getMapChipState();
			// この範囲は攻撃対象外なのでスキップ
			if(bF ? pState->getField()<0 : pState->getAttack()<0) continue;
			for(; rit!=it->end(); ++rit)
			{// 相手がいて、ちゃんと攻撃できる高さいる？
				if(rit->nID_>=0 
				&& pState->getAtkHeight()<=nHeight)
					return true;
			}
		}
	}

	return false;
}

bool CSLGContext::IsRangeCharaInner(int nMin, int nHeight, int nPhase)
{// 最低射程内にキャラがいるぞな？
	range_list::iterator rit;
	for(int i=0; i<=nMin; ++i)
	{
		if(rangeWeapon_[i].empty()) continue;
		rit=rangeWeapon_[i].begin();
		for(; rit!=rangeWeapon_[i].end(); ++rit)
		{// 相手がいて、敵で、ちゃんと攻撃できる高さいる？
			if(getCharaData(rit->nID_)!=NULL
			&& getCharaData(rit->nID_)->getPhase()!=nPhase
			&& getMapChip(rit->nIndex_)->getMapChipState()->getAtkHeight() <= nHeight)
				return true;
		}
	}

	return false;
}

bool CSLGContext::IsRangeChara(int nRange, int nPhase, bool bFriend, int nHeight, bool bF, bool bSmart)
{
	#ifdef BMW_DEBUG
		CDbg().Out("Range %d %d",rangeWeapon_.size(),nRange);
	#endif
	if((int)rangeWeapon_.size()<=nRange) return false;
	range_list& listRange = getRangeList(nRange);
	CDataCharaSLG* pChara;
	range_list::iterator it;
	for(it=listRange.begin(); it!=listRange.end(); ++it)
	{// フェーズによって攻撃できる相手を選択
	#ifdef BMW_DEBUG
		CDbg().Out("IsRange %d %d %d %d",it->nID_,it->nIndex_,it->bAttack_,it->bField_);
	#endif
	
		if(it->nID_<0) continue;
		pChara = getCharaData(it->nID_);
		// 相手が死んでたら、当然攻撃不可能
		if(!pChara->IsLive()) continue;

		// 相手がちゃんと攻撃できる高さいる？
		Map::CMapChipState* pState = getMapChip(it->nIndex_)->getMapChipState();
		// フィールド武器かそうじゃないかなで、若干判定が変わる
		if(pState->getAtkHeight()<=nHeight)
		{
			if(!bF)
			{// 通常武器
				if(it->bAttack_
				&& pState->getAttack()>0
				&& (bFriend ? nPhase==pChara->getPhase() : nPhase!=pChara->getPhase()))
					return true;
			}
			// フィールド武器
			else if(it->bField_ 
				&& pState->getFieldAttack()>0
				 // 味方識別しないなら、とにかくキャラが居ればOK
				&& (bSmart ? (bFriend ? nPhase==pChara->getPhase() : nPhase!=pChara->getPhase()) : true))
					return true;
		}
	}
	return false;
}

int CSLGContext::hasRangeChara(int nRange, int nID)
{
	if((int)rangeWeapon_.size()<=nRange) return -1;
	range_list& listRange = getRangeList(nRange);
	range_list::iterator it;
	for(it=listRange.begin(); it!=listRange.end(); ++it)
		if(it->nID_==nID) return it->nIndex_;

	return -1;
}

int CSLGContext::hasRangeIndex(int nIndex, bool bF)
{
	int nAttack = bF ? getMapChip(nIndex)->getMapChipState()->getFieldAttack()
					 : getMapChip(nIndex)->getMapChipState()->getAttack();
	if(nAttack<=0) return -1;

	if((int)rangeWeapon_.size()<=nAttack-1) return -1;
	range_list& listRange = getRangeList(nAttack-1);
	range_list::iterator it;
	for(it=listRange.begin(); it!=listRange.end(); ++it)
		if((bF ? it->bField_ : it->bAttack_) && it->nIndex_==nIndex)
			return it->nID_;

	return -1;
}

void CSLGContext::getRangeChara(int nPhase, bool bFriend, list<int>& listChara)
{
	weapon_range::iterator it;
	range_list::iterator lit;
	for(it=rangeWeapon_.begin(); it!=rangeWeapon_.end(); ++it)
	{
		for(lit=it->begin(); lit!=it->end(); ++lit)
		{
			if(!lit->bAttack_
			|| lit->nID_<0) continue;
			CDataCharaSLG* pChara = getCharaData(lit->nID_);
			if(bFriend ? nPhase==pChara->getPhase() : nPhase!=pChara->getPhase())
				listChara.push_back(lit->nID_);
		}
	}
}

void CSLGContext::delRangeField(int nRange, int nIndex)
{
	if((int)rangeWeapon_.size()<=nRange) return;
	range_list& listRange = getRangeList(nRange);
	range_list::iterator it;
	for(it=listRange.begin(); it!=listRange.end(); ++it)
	{//	指定されたnIndexを削除
		if(it->bField_ && it->nIndex_==nIndex)
		{
			listRange.erase(it);
			break;
		}
	}
}

bool CSLGContext::IsRangeField(int nFieldToward, int nMin, int nMax, int nPhase)
{
	for(int i=nMin; i<=nMax; ++i)
	{
		range_list& listRange = getRangeList(i-1);
		range_list::iterator it;
		for(it=listRange.begin(); it!=listRange.end(); ++it)
		{
			if(it->bField_
			&& (nFieldToward==getMapChip(it->nIndex_)->getMapChipState()->getFieldToward(0)
			|| nFieldToward==getMapChip(it->nIndex_)->getMapChipState()->getFieldToward(1)))
			{// 指定のフィールド範囲
				if(it->nID_>=0)
				{// キャラがいる
					CDataCharaSLG* pCharaData = getCharaData(it->nID_);
					// 当然生きてないといけない
					if(!pCharaData->IsLive()) continue;
					// 味方認識しないなら、これでOK
					if(nPhase<0) return true;
					// するなら、フェーズチェック
					if(getCharaData(it->nID_)->getPhase()!=nPhase) return true;
				}
			}
		}
	}

	return false;
}

bool CSLGContext::IsRangeFieldThrow(int nPhase)
{// 投げ込む範囲内に対象が含まれているかどうか
	Map::CMapChip* pChip;
	set<int>::iterator it;
	for(it=setIndex_.begin(); it!=setIndex_.end(); ++it)
	{// Move領域が攻撃範囲
		pChip = getMapChip(*it);
		// 誰かいるか？
		Task::ITaskBase* pTask = pChip->getTask(Map::CMapChip::CHARA);
		if(pTask!=NULL)
		{// いた
			// するなら、フェーズチェック
			Map::CMapChipChara2* pChara = static_cast<Map::CMapChipChara2*>(pTask);
			CDataCharaSLG* pCharaData = getCharaData(pChara->getID());
			// 当然生きてないといけない
			if(!pCharaData->IsLive()) continue;
			// 味方認識しない、かつ、自分自身じゃないなら、OK
			if(nPhase<0	&& pChara->getID()!=getCtrlChara())	return true;
			ef(pCharaData->getPhase()!=nPhase)				return true;
		}
	}

	return false;
}

void CSLGContext::getRangeFieldChara(int nFieldType, int nPhase, int nMin, int nMax, int nFieldToward, set<int>& setChara)
{// タイプによって取得の仕方が少々違う
	switch(nFieldType)
	{
	case Weapon::Field::THROW:
	{// 投げ込み
		set<int>::iterator it;
		for(it=setIndex_.begin(); it!=setIndex_.end(); ++it)
		{// Move領域が攻撃範囲
			Map::CMapChip* pChip = getMapChip(*it);
			// 誰かいるか？
			Task::ITaskBase* pTask = pChip->getTask(Map::CMapChip::CHARA);
			if(pTask!=NULL)
			{// いた
				Map::CMapChipChara2* pChara = static_cast<Map::CMapChipChara2*>(pTask);
				CDataCharaSLG* pCharaData = getCharaData(pChara->getID());
				// すでに死んでるやつは当然対象外
				if(!pCharaData->IsLive()) continue;
				// smartじゃない、か、敵だったら対象にする
				if((nPhase<0 && pChara->getID()!=getCtrlChara())
				|| pCharaData->getPhase()!=nPhase)
					setChara.insert(pChara->getID());
			}
		}
	}
	break;

	case Weapon::Field::LINE:
		nMin=1;
	default:
	{
		for(int i=nMin; i<=nMax; ++i)
		{
			range_list& listRange = getRangeList(i-1);
			range_list::iterator it;
			for(it=listRange.begin(); it!=listRange.end(); ++it)
			{
				if(it->bField_
				&&(nFieldToward==getMapChip(it->nIndex_)->getMapChipState()->getFieldToward(0)
				|| nFieldToward==getMapChip(it->nIndex_)->getMapChipState()->getFieldToward(1)))
				{// 指定のフィールド範囲
					// キャラがいる
					// フェーズチェック
					if(it->nID_>=0)
					{
						CDataCharaSLG* pCharaData = getCharaData(it->nID_);
						// 生きててフェイズチェックを通ったやつだけ攻撃対象
						if(pCharaData->IsLive() && (nPhase<0 || pCharaData->getPhase()!=nPhase))
							setChara.insert(it->nID_);
					}
				}
			}
		}
	}
	break;
	}
}

void CSLGContext::getRangeFieldToward(int nFieldType, int nPhase, int nMin, int nMax, int anToward[], int nSnipe, bool abSnipe[])
{// 一番キャラのいる攻撃方向を取得する
 // タイプによって取得の仕方が少々違う
 // また、nSnipeが指定されてたら、そいつが居る方向は負になっている
	switch(nFieldType)
	{
	case Weapon::Field::LINE:
		nMin=1;
	default:
	{
		for(int i=nMin; i<=nMax; ++i)
		{
			range_list& listRange = getRangeList(i-1);
			range_list::iterator it;
			for(it=listRange.begin(); it!=listRange.end(); ++it)
			{
				if(it->bField_ && it->nID_>=0)
				{// フィールド範囲
				 // キャラがいる
					CDataCharaSLG* pChara = getCharaData(it->nID_);
					// 死んでたら意味なし
					if(!pChara->IsLive()) continue;
					// フェーズチェック
					if((nPhase<0 || pChara->getPhase()!=nPhase))
					{
						int nToward = (getMapChip(it->nIndex_))->getMapChipState()->getFieldToward(0);
						if(nToward==Way::ALL) nToward=0;
						if(nToward>=0){
							++anToward[nToward];
							if(pChara->getID()==nSnipe) abSnipe[nToward]=true;
						}

						nToward = (getMapChip(it->nIndex_))->getMapChipState()->getFieldToward(1);
						if(nToward==Way::ALL) nToward=0;
						if(nToward>=0){
							++anToward[nToward];
							if(pChara->getID()==nSnipe) abSnipe[nToward]=true;
						}
					}
				}
			}
		}
	}
	break;
	}
	
}



//////////////////////////////////////////////////
// フェーズ
//////////////////////////////////////////////////
list<int>::iterator CSLGContext::delPhase(int nSlgID, int nPhase, bool bNonPlayer)
{
	// もし、nPhaseが負だったら、検索する
	if(nPhase<0) nPhase=searchPhase(nSlgID);

	list<int>* pList;
	switch(nPhase)
	{
	case Phase::PLAYER:
		if(bNonPlayer)
			pList=&listNonPlayer_;
		else
			pList=&listPlayer_;
	break;

	case Phase::ENEMY:
		pList=&listEnemy_;
	break;

	default:
		pList=&listNeutral_;
	break;
	}

	list<int>::iterator it;
	for(it=pList->begin(); it!=pList->end(); it++)
	{
		if(nSlgID==*it)
		{
			return pList->erase(it);
		}
	}

	// 削除対象がなかったらendを返す
	return pList->end();
}

void CSLGContext::getMapExistList(list<int>& listChara)
{
	CDataCharaSLG* pChara;
	list<int>::iterator it;
	for(it=listPlayer_.begin(); it!=listPlayer_.end(); ++it)
	{
		pChara = getCharaData(*it);
		if(pChara != NULL
		&& pChara->IsExist()
		&& pChara->getIndex()>=0)
			listChara.push_back(*it);
	}
}

void CSLGContext::phasePer()
{
	phasePerList(getPlayerPhaseList());
	phasePerList(getEnemyPhaseList());
	phasePerList(getNeutralPhaseList());
}

void CSLGContext::phasePerList(list<int>& List)
{
	list<int>::iterator it;
	it=List.begin();
	while(it!=List.end())
	{
		if(getCharaData(*it)!=NULL)
			getCharaData(*it)->actionPhasePer(*this);
		++it;
	}
}

void CSLGContext::phaseStartPhase(int nPhase)
{
	// フェーズに属するキャラのactionPhaseStartを呼び出す
	switch(nPhase)
	{
	case Phase::PLAYER: phaseStartList(getPlayerPhaseList()); break;
	case Phase::ENEMY: phaseStartList(getEnemyPhaseList()); break;
	case Phase::NEUTRAL: phaseStartList(getNeutralPhaseList()); break;
	default: break;
	}
}

void CSLGContext::phaseStartList(list<int>& List)
{
	list<int>::iterator it;
	it=List.begin();
	while(it!=List.end())
	{
		getCharaData(*it)->actionPhaseStart(*this);
		++it;
	}
}

void CSLGContext::allActBefore()
{// 死んでいないキャラの状態をBEFOREにする

	actBefore(Phase::PLAYER);
	actBefore(Phase::ENEMY);
	actBefore(Phase::NEUTRAL);
}

void CSLGContext::actBefore(int nPhase)
{// 死んでいないキャラの状態をBEFOREにする
	// ついでにPhaseListを正常化する
	list<int>& listPhase = getPhaseList(nPhase);

	bool bPlayer = getPhase()==Phase::PLAYER && nPhase==Phase::PLAYER;
	if(bPlayer)
	{// Playerフェーズの時は同時にNPCを洗い出す
		listNonPlayer_.clear();
	}
	CDataCharaSLG* pChara;
	list<int>::iterator it;
	it=listPhase.begin();
	while(it!=listPhase.end())
	{
		pChara = getCharaData(*it);

		if(pChara==NULL)
		{
			it=listPhase.erase(it);
		}
		ef(pChara->getState().getAct()==Act::DEATH
		|| pChara->getState().getAct()==Act::DEATH_EVENT
		|| pChara->getState().getAct()==Act::REMOVE)
		{
			// 敵キャラ、もしくはNPCだったらマップから削除してしまう
			if(pChara->getPhase()!=Phase::PLAYER
			|| pChara->getAction()->IsNonPlayer()) delCharaData(*it);
			// フェーズリストから削除
			it=listPhase.erase(it);
		}
		else
		{
			if(bPlayer && pChara->getAction()->IsNonPlayer())
			{// NPCだったらNPCリストに追加
				listNonPlayer_.push_back(pChara->getID());
			}
			pChara->getState().setAct(Act::BEFORE);
			++it;
		}
	}
}

void CSLGContext::actAfter(int nPhase, set<int>& setOut)
{// 死んでいないキャラの状態をAFETERにする
	// 出撃選択で使う
	list<int>& listPhase = getPhaseList(nPhase);

	CDataCharaSLG* pChara;
	list<int>::iterator it;
	it=listPhase.begin();
	while(it!=listPhase.end())
	{
		if(setOut.find(*it)!=setOut.end()){ ++it; continue; }
		pChara = getCharaData(*it);
		if(pChara==NULL)
		{
			it=listPhase.erase(it);
		}
		ef(pChara->getState().getAct()==Act::DEATH
		|| pChara->getState().getAct()==Act::DEATH_EVENT
		|| pChara->getState().getAct()==Act::REMOVE)
		{// マップにないなら、スルー
			++it;
		}
		else
		{
			pChara->getState().setAct(Act::AFTER);
			++it;
		}
	}
}

int CSLGContext::getNoActionChara()
{
	return count_if(listPlayer_.begin(),listPlayer_.end(),IsNoAction(*this));
}

int CSLGContext::getMapExistChara()
{
	return count_if(listPlayer_.begin(),listPlayer_.end(),IsMapExist(*this));
}

//////////////////////////////////////////////////////
// データ検索系
//////////////////////////////////////////////////////
bool CSLGContext::IsList(int nSlgID, list<int>& List)
{
	list<int>::iterator it;
	for(it=List.begin(); it!=List.end(); it++)
		if(nSlgID==*it) return true;

	return false;
}

int CSLGContext::searchPhase(int nSlgID)
{
	if(IsList(nSlgID, listPlayer_)) return Phase::PLAYER;
	ef(IsList(nSlgID, listEnemy_)) return Phase::ENEMY;
	ef(IsList(nSlgID, listNeutral_)) return Phase::NEUTRAL;
	else return -1;
}

bool CSLGContext::searchPhase(int nSlgID,int nPhase)
{
	return IsList(nSlgID, getPhaseList(nPhase));
}

int CSLGContext::IsChara(int nCharaID, list<int>& List)
{
	list<int>::iterator it;
	for(it=List.begin(); it!=List.end(); ++it)
	{
		if(getCharaData(*it)==NULL) continue;
		if(nCharaID==getCharaData(*it)->getCharaID()) return *it;
	}

	return -1;
}

int CSLGContext::searchSlg(int nCharaID)
{
	int nID;
	nID=IsChara(nCharaID, listPlayer_);
	if(nID!=-1) return nID;
	nID=IsChara(nCharaID, listEnemy_);
	if(nID!=-1) return nID;
	nID=IsChara(nCharaID, listNeutral_);
	if(nID!=-1) return nID;

	return -1;
}

int CSLGContext::searchSlgChild(int nCharaID, int nPhase)
{
	list<int>& List = getPhaseList(nPhase);
	list<int>::iterator it;
	for(it=List.begin(); it!=List.end(); ++it)
		if(getCharaData(*it)!=NULL
		&& getApp()->getChara().IsChild(getCharaData(*it)->getCharaID(),nCharaID))
			return *it;

	return -1;
}

int CSLGContext::IsFace(int nFaceID, list<int>& List)
{
	list<int>::iterator it;
	for(it=List.begin(); it!=List.end(); ++it)
	{
		if(getCharaData(*it)==NULL) continue;
		// 変身対応！！
		if(getApp()->getFaceMap().IsFace(nFaceID,getCharaData(*it)->getBattle().getFaceID()))
			return *it;
	}

	return -1;
}

int CSLGContext::searchFace2Slg(int nFaceID)
{// 顔IDからSLG IDを検索する
	int nID;
	nID=IsFace(nFaceID, listPlayer_);
	if(nID!=-1) return nID;
	nID=IsFace(nFaceID, listEnemy_);
	if(nID!=-1) return nID;
	nID=IsFace(nFaceID, listNeutral_);
	if(nID!=-1) return nID;

	return -1;
}

int CSLGContext::IsMapSymbol(const string& sMapID, list<int>& List)
{
	list<int>::iterator it;
	CDataCharaSLG* pChara;
	for(it=List.begin(); it!=List.end(); ++it)
	{
		pChara = getCharaData(*it);
		if(pChara==NULL) continue;
		if(sMapID == pChara->getBattle().getMapSymbolID()
		&& !pChara->getMapSymbol().isNull()
		&& pChara->getMapSymbol()->IsRead())
			return *it;
	}

	return -1;
}

int CSLGContext::searchMapSymbol(const string& sMapID, int nPhase)
{// 敵・味方の入れ替わりは、まず無いので、指定されたフェイズだけでOK	
	if(nPhase==Phase::PLAYER) return IsMapSymbol(sMapID,listPlayer_);
	ef(nPhase==Phase::ENEMY) return IsMapSymbol(sMapID,listEnemy_);
	ef(nPhase==Phase::NEUTRAL) return IsMapSymbol(sMapID,listNeutral_);
	else return -1;
}

//////////////////////////////////////////////////////
// 気力
//////////////////////////////////////////////////////
void CSLGContext::allMental(int nPhase, int nID, int nSlg)
{
	list<int>& listPhase = getPhaseList(nPhase);

	CDataCharaSLG* pChara;
	list<int>::iterator it;
	it=listPhase.begin();
	while(it!=listPhase.end())
	{
		if(nSlg!=*it)
		{
			pChara = getCharaData(*it);
			if(pChara!=NULL
			&& pChara->getState().getAct()!=Act::DEATH
			&& pChara->getState().getAct()!=Act::REMOVE)
			{
				pChara->actionMental(nID);
			}
		}
		it++;
	}
}

//////////////////////////////////////////////////////////////
// マップインデックス
//////////////////////////////////////////////////////////////
void CSLGContext::clearMove()
{
	set<int>::iterator it;
	for(it=setIndex_.begin(); it!=setIndex_.end(); ++it)
		getMapChip(*it)->getMapChipState()->setMove(-1);

	setIndex_.clear();
}

void CSLGContext::clearDist()
{
	set<int>::iterator it;
	for(it=setIndex_.begin(); it!=setIndex_.end(); ++it)
		getMapChip(*it)->getMapChipState()->setDist(-1);

	setIndex_.clear();
}

void CSLGContext::clearAttack()
{
	set<int>::iterator it;
	for(it=setIndex_.begin(); it!=setIndex_.end(); ++it)
	{
		getMapChip(*it)->getMapChipState()->setAttack(-1);
		getMapChip(*it)->getMapChipState()->setRealDist(-1);
	}

	setIndex_.clear();
}

////////////////////////////////////////////////////////////
// 座標
////////////////////////////////////////////////////////////
namespace{
const string sStateTable_[2][4]=
{
	{"BEFORE_TOP","BEFORE_LEFT","BEFORE_BOTTOM","BEFORE_RIGHT"},
	{"AFTER_TOP","AFTER_LEFT","AFTER_BOTTOM","AFTER_RIGHT"}
};

int getAct(int nAct)
{
	switch(nAct)
	{
	case Act::BEFORE:
	case Act::MOVE:
	case Act::HIT_AWAY:
		return 0;

	default: 
		return 1;
	}
}
} // namespace end

void CSLGContext::setCirclePos(int nID, int& nX, int& nY)
{
	switch(nID)
	{
	case Pos::MOUSE:
		getInput()->getCursolPos(nX,nY);
	break;

	case Pos::CHARA:
	{
		CDataCharaSLG* pChara = getCtrlCharaData();
		// マップチップ位置取得
		getMap()->getMapChipPos(pChara->getIndex(),nX,nY);
		nX+=32;
		nY+=16;
		// そのマップチップに立っているキャラのスプライトの高さを取得
		Draw::CSpriteInfoBase base;
		pChara->getMapSymbol()->getSpriteDB().getSpriteData(sStateTable_[getAct(pChara->getState().getAct())][pChara->getState().getWay()], base);
		nY-=(base.getRect().bottom-base.getRect().top)/2;
	}
	break;

	case Pos::CHARA_TARGET:
	{
		CDataCharaSLG* pChara = getTargetCharaData();
		// マップチップ位置取得
		getMap()->getMapChipPos(pChara->getIndex(),nX,nY);
		nX+=32;
		nY+=16;
		// そのマップチップに立っているキャラのスプライトの高さを取得
		Draw::CSpriteInfoBase base;
		pChara->getMapSymbol()->getSpriteDB().getSpriteData(sStateTable_[getAct(pChara->getState().getAct())][pChara->getState().getWay()], base);
		nY-=(base.getRect().bottom-base.getRect().top)/2;
	}
	break;

	case Pos::CHIP_TARGET:
		// マップチップ位置取得
		getMap()->getMapChipPos(getTargetCharaData()->getIndex(),nX,nY);
		// マップほぼ中央に移動
		nX+=32;
		nY+=14;
	break;

	default:
		// マップチップ位置取得
		getMap()->getMapChipPos(getCtrlCharaData()->getIndex()<0 ? getTargetMap() : getCtrlCharaData()->getIndex(),nX,nY);
		// マップほぼ中央に移動
		nX+=32;
		nY+=14;
	break;
	}
}

void CSLGContext::setCirclePos(int nID)
{
	setCirclePos(nID,nCircleX_,nCircleY_);
}

//////////////////////////////////////////////////////////
// ヘルパ
//////////////////////////////////////////////////////////
bool IsAtk(CDataCharaSLG* pChara, CSLGContext* p, bool bP)
{
	// 攻撃可能武器を一個でももってればtrue
	Weapon::CDataWeaponBattle* pWeapon;
	Chara::CDataCharaBattle& battle = pChara->getBattle();
	battle.beginWeapon();
	while(!battle.endWeapon())
	{
		pWeapon = p->getWeaponData(*battle.nextWeapon());
		// 治癒と補給は関係無し
		if(pWeapon->getKind()==Weapon::Kind::CURE
		|| pWeapon->getKind()==Weapon::Kind::REFILL) continue;

		if(pWeapon->enable(*pChara,bP,*p)) return true;
	}
	return false;
}

} // namespace SLG end
} // namespace BMW end