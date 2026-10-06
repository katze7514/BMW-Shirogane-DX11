#include "stdafx.h"

#include "../../Chara/CValidSpirit.h"

#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"
#include "../../Weapon/CDataWeaponBattleCollab.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"
#include "../Map/CMapChipChara2.h"

#include "CAttack_range2.h"
#include "CAttack_calc.h"
#include "CAttack_action.h"

namespace BMW{
namespace SLG{
namespace Attack{

void CAttack_action::calcAndSetHit(int nState, int nTarget, CSLGContext* p, bool bFriend)
{// 命中計算と設定
	int nHit,nOffHit;
	if(!bFriend)
	{// 普通に攻撃じゃ
		bool bBackUp = (nState==CBattleState::ATTACK_BACK ? true : false);
		Weapon::CDataWeaponBattle* pWeapon = p->getWeaponData(state_.getWeaponID(nState));
		if(pWeapon!=NULL)
		{// 武器あり、ってことは通常攻撃
			nHit = CAttack_calc::calcHit(*state_.getCharaData(nState),
										 pWeapon, 
										 *state_.getCharaData(nTarget),
										 (nState==CBattleState::ATTACK || bBackUp) ? nDist_ : nCounterDist_,
										 (nState==CBattleState::ATTACK || bBackUp) ? nHeight_ : nCounterHeight_,
										 *p,bBackUp);
			// 精神が掛かってたか？
			nOffHit=0;
			if(nHit==CAttack_calc::AVOID)
				nHit=0;
			ef(nHit==CAttack_calc::HIT)
				nHit=200;
			else
				nOffHit = CAttack_calc::calcOffHit(*state_.getCharaData(nState),
												   *state_.getCharaData(nTarget),
												   *p,
												   bBackUp);
		}
		else
		{// 出てこないってことは、反撃不能時
			nHit=nOffHit=0;
		}
	}
	else
	{// 仲間からってことは、能力アップの予感
		nHit=100;
		nOffHit=0;
	}
	state_.setHit(nHit,nState);
	state_.setOffHit(nOffHit,nState);
}

void CAttack_action::calcBackAttack(CDataCharaSLG& attack, CSLGContext& p, int nHP)
{// 援護攻撃可能キャラ選択
	Map::CMapChip*			pMapBase = p.getMapChip(attack.getIndex());
	Map::CMapChip*			pMap;
	Task::ITaskBase*		pBase;
	Map::CMapChipChara2*	pMapChara;
	CDataCharaSLG*			pChara;
	int						nPos=0;
	int						nWeapon;

	for(int i=Way::TOP; i<=Way::RIGHT; i++)
	{// まずは、攻撃キャラの隣接キャラを取得
		pMap = p.getMapChip(pMapBase->getMapInfo().getOnMap(i));
		if(pMap==NULL) continue;
		pBase = pMap->getTask(Map::CMapChip::CHARA);
		if(pBase==NULL) continue;
		// キャラがいた
		pMapChara = static_cast<Map::CMapChipChara2*>(pBase);
		pChara = p.getCharaData(pMapChara->getID());
		// そいつは仲間？
		if(attack.getPhase()!=pChara->getPhase()) continue;
		// 合体攻撃のメンバーじゃないよね？
		if((p.getCtrlWeaponData()->getKind()==Weapon::Kind::FIGHT_COLLAB
		|| p.getCtrlWeaponData()->getKind()==Weapon::Kind::MAGIC_COLLAB)
		&& static_cast<Weapon::CDataWeaponBattleCollab*>(p.getCtrlWeaponData())->IsCollabChara(pChara->getID()))
			continue;
		// 行動不能じゃないよね？
		if(pChara->getBattle().IsCond(Chara::CValidCond::ACTION)) continue;
		// 行動できる？
		if(pChara->getState().getAct()!=Act::BEFORE) continue;
		// 援護攻撃できる？
		if(!pChara->getBattle().IsAttack()) continue;
		// 援護対象の位置に進入可能？
		if(!IsMove(*pChara,pMap,pMapBase,p)) continue;
		// 援護攻撃可能武器を持ってる？
		// 補正値計算
		CAttack_range2::calcAllRange(pChara, &p);

#ifdef BMW_DEBUG
	CDbg().Out("ActBack Height %d",nHeight_);
#endif

		nWeapon=pChara->actionCounter(nDist_,nRealDist_,abs(nHeight_),p,nHP,true);
		if(nWeapon<0) continue;
		// よーし、援護攻撃キャラ認定！
		pairBackUp_[nPos].first=pChara->getID();
		pairBackUp_[nPos].second=nWeapon;
		++nPos;
	}
}

void CAttack_action::calcBackDef(CDataCharaSLG& def, CSLGContext& p)
{// 援護防御可能キャラ選択
	Map::CMapChip*			pMapBase = p.getMapChip(def.getIndex());
	Map::CMapChip*			pMap;
	Task::ITaskBase*		pBase;
	Map::CMapChipChara2*	pMapChara;
	CDataCharaSLG*			pChara;
	int						nPos=0;

	// 敵の武器が合体攻撃じゃない
	if(p.getCtrlWeaponData()->getKind()==Weapon::Kind::FIGHT_COLLAB
	|| p.getCtrlWeaponData()->getKind()==Weapon::Kind::MAGIC_COLLAB
	// 相手に直撃が掛かっている
	|| p.getCtrlCharaData()->getBattle().IsSpirit(Chara::CValidSpirit::DIRECT))
		return;
	
	for(int i=Way::TOP; i<=Way::RIGHT; i++)
	{// まずは、攻撃キャラの隣接キャラを取得
		pMap = p.getMapChip(pMapBase->getMapInfo().getOnMap(i));
		if(pMap==NULL) continue;
		pBase = pMap->getTask(Map::CMapChip::CHARA);
		if(pBase==NULL) continue;
		// キャラがいた
		pMapChara = static_cast<Map::CMapChipChara2*>(pBase);
		pChara = p.getCharaData(pMapChara->getID());
		// そいつは仲間？
		if(def.getPhase()!=pChara->getPhase()) continue;
		// 行動不能じゃないよね？
		if(pChara->getBattle().IsCond(Chara::CValidCond::ACTION)) continue;
		// 援護防御できる？
		if(!pChara->getBattle().IsDefence()) continue;
		// 援護対象の位置に進入可能？
		if(!IsMove(*pChara,pMap,pMapBase,p)) continue;
		// ピンチでは無い？
		//if(pChara->getState().IsPinch()) continue;
		// よーし、援護攻撃キャラ認定！
		pairBackUp_[nPos++].first=pChara->getID();
	}
}

bool CAttack_action::IsMove(CDataCharaSLG& chara, Map::CMapChip* pBase, Map::CMapChip* pMap, CSLGContext& p)
{
	// ようは高さが足りればOKなわけで
	int nJump = chara.getBattle().getJump();
	// 飛行使い？
	if(chara.getBattle().IsTalent(Ability::FLY))
	{// 持ってる
		// じゃ、使える？
		if(p.getApp()->getAbility().enable(chara,0,p,Ability::FLY))
			nJump=INT_MAX;
	}
	
	return nJump >= abs(pMap->getMapInfo().getHeight()-pBase->getMapInfo().getHeight());
}

int CAttack_action::selectBackUpAtk(CSLGContext& context)
{
	// 攻撃の時は、一番攻撃力の高いやつ
	int nPos=-1;
	int nMaxAttack=INT_MIN;
	int nAttack;
	CDataCharaSLG* pChara;

	for(int i=0; i<4; i++)
	{
		if(pairBackUp_[i].first>=0)
		{// 援護キャラがいたら
			pChara = context.getCharaData(pairBackUp_[i].first);
			nAttack	= context.getWeaponData(pairBackUp_[i].second)->getAttack();
			if(nMaxAttack<=nAttack)
			{
				nPos=i;
				nMaxAttack=nAttack;
			}
		}
		else
		{// 頭から設定されるので、なくなったら終了
			break;
		}
	}

	return nPos;
}

int CAttack_action::selectBackUpDef(CSLGContext& context)
{
	// 防御の時は、一番HPの高いやつ
	int nPos=-1;
	int nMaxHP=INT_MIN;
	int nHP;
	CDataCharaSLG* pChara;

	for(int i=0; i<4; i++)
	{
		if(pairBackUp_[i].first>=0)
		{// 援護キャラがいたら
			pChara = context.getCharaData(pairBackUp_[i].first);
			nHP = pChara->getBattle().getHP();
			if(nMaxHP<=nHP)
			{// 最強情報を更新
				nPos=i;
				nMaxHP=nHP;
			}
		}
		else
		{// 頭から設定されるので、なくなったら終了
			break;
		}
	}
	
	// 決定されたやつを返す
	return nPos;
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end