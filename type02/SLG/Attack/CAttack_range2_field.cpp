#include "stdafx.h"

#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"
#include "../Map/CMapChipChara2.h"

#include "CAttack_range2.h"

namespace BMW{
namespace SLG{
namespace Attack{

//////////////////////////////////////////
// フィールドライン武器範囲計算
//////////////////////////////////////////
void CAttack_range2::calcField(Map::CMapChip* pMapChip)
{// フィールド武器計算
	if(pFieldWeapon_->getField()==Weapon::Field::LINE)
		calcFieldLine(pMapChip);
	else
		calcFieldCircle(pMapChip, 0, Way::NO, pFieldWeapon_->getMax() + (pFieldWeapon_->getField()==Weapon::Field::THROW ? pFieldWeapon_->getFieldSize() : 0 ));
}

//////////////////////////////////////////
// 円形
// キャラが居ても射程減衰しないので、
// あらこちらも書いた
//////////////////////////////////////////
void CAttack_range2::calcFieldCircle(Map::CMapChip* pMapChip, int nAttack, int nToward, int nMax)
{
	// マップが存在しなくても終了
	if(pMapChip==NULL) return;

	// 最大射程を超えたら終了
	if(nAttack+1>nMax) return;

	Map::CMapChip* pOn;
	// 上へいけるか
	if(nToward!=Way::TOP
	&& getIndex()!=pMapChip->getMapInfo().getOnMap(Way::TOP))
	{
		pOn = p->getMapChip(pMapChip->getMapInfo().getOnMap(Way::TOP));
		switch(IsFieldAttack(pOn))
		{
		case ENABLE:
			setFieldIndex(pOn, nAttack+1);
		case THROUGH:
			calcFieldCircle(pOn, nAttack+1, Way::BOTTOM, nMax);
		break;

		default: break;
		}
	}

	// 左へいけるか
	if(nToward!=Way::LEFT
	&& getIndex()!=pMapChip->getMapInfo().getOnMap(Way::LEFT))
	{
		pOn = p->getMapChip(pMapChip->getMapInfo().getOnMap(Way::LEFT));
		switch(IsFieldAttack(pOn))
		{
		case ENABLE:
			setFieldIndex(pOn, nAttack+1);
		case THROUGH:
			calcFieldCircle(pOn, nAttack+1, Way::RIGHT, nMax);
		break;

		default: break;
		}
	}

	// 下へいけるか
	if(nToward!=Way::BOTTOM
	&& getIndex()!=pMapChip->getMapInfo().getOnMap(Way::BOTTOM))
	{
		pOn = p->getMapChip(pMapChip->getMapInfo().getOnMap(Way::BOTTOM));
		switch(IsFieldAttack(pOn))
		{
		case ENABLE:
			setFieldIndex(pOn, nAttack+1);
		case THROUGH:
			calcFieldCircle(pOn, nAttack+1, Way::TOP, nMax);
		break;

		default: break;
		}
	}

	// 右へいけるか
	if(nToward!=Way::RIGHT
	&& getIndex()!=pMapChip->getMapInfo().getOnMap(Way::RIGHT))
	{
		pOn = p->getMapChip(pMapChip->getMapInfo().getOnMap(Way::RIGHT));
		switch(IsFieldAttack(pOn))
		{
		case ENABLE:
			setFieldIndex(pOn, nAttack+1);
		case THROUGH:
			calcFieldCircle(pOn, nAttack+1, Way::LEFT, nMax);
		break;
			
		default: break;
		}
	}
}

int CAttack_range2::IsFieldAttack(Map::CMapChip* pMap)
{
	if(pMap==NULL) return NOTENABLE;
	// いるマップの攻撃検索マップの高さ差が、到達度以上かどうか
	int nHeight = pMap->getMapInfo().getHeight() - nHeight_;
	// 到達できないとこはスルー
	if(abs(nHeight) > pFieldWeapon_->getHeight()) return THROUGH;
	
	// 基本的に範囲内として扱う
	return ENABLE;
}

void CAttack_range2::setFieldIndex(Map::CMapChip* pMap, int nField)
{
	Map::CMapChipState* pState = pMap->getMapChipState();

	int nMapField = pState->getFieldAttack();
	if(nMapField<0 || nMapField > nField)
	{
		// 値更新前に前の値を削除
		if(nMapField>0)
			p->delRangeField(nMapField-1, pMap->getIndex());

		// Field更新
		pState->setFieldAttack(nField);
		pState->setFieldToward(Way::ALL);
		pState->setAtkHeight(abs(pMap->getMapInfo().getHeight()-nHeight_));
		Map::CMapChipChara2* pChip = static_cast<Map::CMapChipChara2*>(pMap->getTask(Map::CMapChip::CHARA));
		if(pChip!=NULL)
			p->addRangeField(nField-1, pChip->getID(), pMap->getIndex());
		else
			p->addRangeField(nField-1, -1, pMap->getIndex());
	}
}

///////////////////////////////////////////////////
// ラインタイプ
///////////////////////////////////////////////////
void CAttack_range2::calcFieldLine(Map::CMapChip* pMapChip)
{// こいつの範囲は矩形×4なので、通常の円計算にはできない
	// トップ方向
	calcFieldLineToward(pMapChip, Way::TOP, pFieldWeapon_->getMin(), pFieldWeapon_->getMax());
	// レフト方向
	calcFieldLineToward(pMapChip, Way::LEFT, pFieldWeapon_->getMin(), pFieldWeapon_->getMax());
	// ボトム方向
	calcFieldLineToward(pMapChip, Way::BOTTOM, pFieldWeapon_->getMin(), pFieldWeapon_->getMax());
	// ライト方向
	calcFieldLineToward(pMapChip, Way::RIGHT, pFieldWeapon_->getMin(), pFieldWeapon_->getMax());
}

namespace{
__inline Map::CMapChip* getMapChipLineBase(Map::CMapChip* pMapChip, int nToward, int nWidth, CSLGContext& p)
{// 現在の場所からToward方向にWidthだけ離れたマップチップを取得する
	// 直角方向に進む
	if(nToward==Way::TOP)	 nToward=Way::LEFT;
	ef(nToward==Way::LEFT)	 nToward=Way::BOTTOM;
	ef(nToward==Way::BOTTOM) nToward=Way::RIGHT;
	else					 nToward=Way::TOP;

	if(nWidth<0)
	{// nWidthが負だったら逆方向に進む
		nWidth = -nWidth;
		// towardがTOP/LEFTの時は、2足せば反転
		if(nToward<=1) nToward+=2;
		// towardがBOTTOM/RIGHTの時は、2引けば反転
		ef(nToward>=2) nToward-=2;
	}
	Map::CMapChip* pMapChip2=pMapChip;
	for(int i=1; i<=nWidth; ++i)
	{
		pMapChip2 = p.getMapChip(pMapChip2->getMapInfo().getOnMap(nToward));
		// pMapChip2がNULLになったら終了
		if(pMapChip2==NULL) break;
	}

	return pMapChip2;
}
} // namespace end

void CAttack_range2::calcFieldLineToward(Map::CMapChip* pMapChip, int nToward, int nMin, int nMax)
{// 一ラインずつ処理していく
	// 現在いるラインを中心にどれだけ広げるかを計算
	// なお、↓の処理のため幅は奇数にすること
	int nWidth = (nMin-1)/2;
	// まずは自分がいるライン
	calcFieldLineTowardLine(pMapChip, nToward, nMax, 1);
	// 負の方向
	for(int i=-1; i>=-nWidth; --i)
		calcFieldLineTowardLine(getMapChipLineBase(pMapChip,nToward,i,*p), nToward, nMax, 1+(-i));
	// 正の方向
	for(int i=1; i<=nWidth; ++i)
		calcFieldLineTowardLine(getMapChipLineBase(pMapChip,nToward,i,*p), nToward, nMax, 1+i);
}

void CAttack_range2::calcFieldLineTowardLine(Map::CMapChip* pMapChip, int nToward, int nMax, int nWidth)
{// pMapChipから、nToward方向に、nMaxの範囲を攻撃可能範囲とする
	if(pMapChip==NULL) return;
	Map::CMapChipState* pState;
	for(int i=nWidth; i<=nMax; ++i)
	{
		pMapChip = p->getMapChip(pMapChip->getMapInfo().getOnMap(nToward));
		if(pMapChip==NULL) break;
		pState = pMapChip->getMapChipState();

		// 高さ差が到達度以下だったらOK
		if(abs(pMapChip->getMapInfo().getHeight()-nHeight_) <= pFieldWeapon_->getHeight())
		{
			// 高さ差
			pState->setAtkHeight(abs(pMapChip->getMapInfo().getHeight()-nHeight_));
			// グルーピング
			pState->setFieldToward(nToward);
			// 距離
			if(pState->getFieldAttack()<0 || pState->getFieldAttack()>i)
			{
				if(pState->getFieldAttack()>0) p->delRangeField(pState->getFieldAttack()-1,pMapChip->getIndex());
				pState->setFieldAttack(i);
			}
			// 攻撃範囲領域に突っ込む
			Map::CMapChipChara2* pChip = static_cast<Map::CMapChipChara2*>(pMapChip->getTask(Map::CMapChip::CHARA));
			if(pChip!=NULL)
				p->addRangeField(i-1, pChip->getID(), pMapChip->getIndex());
			else
				p->addRangeField(i-1, -1, pMapChip->getIndex());
		}
	}
}

} // namespace Attack end
} // namespace SLG end
} // mamespace BMW end