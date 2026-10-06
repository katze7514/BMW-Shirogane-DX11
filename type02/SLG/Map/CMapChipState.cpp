#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/COffsetWeapon.h"

#include "CMapChip.h"
#include "CMapChipState.h"

namespace BMW{
namespace SLG{
namespace Map{
// static定義
bool CMapChipState::bAction_;
bool CMapChipState::bGrid_;
bool CMapChipState::bMove_;
bool CMapChipState::bAttack_;
int	 CMapChipState::nMax_;
int	 CMapChipState::nMin_;
int	 CMapChipState::nCoreMax_;
int	 CMapChipState::nCoreMin_;
int	 CMapChipState::nHeight_;
int	 CMapChipState::nField_;
//int	 CMapChipState::nHeightMin_;
bool CMapChipState::bEnable_;
GUI::CGraphic* CMapChipState::sprite_[5];

void CMapChipState::setRangeData(Weapon::CDataWeaponBattle* pWeapon, int nToward)
{
	setMin(pWeapon->getMin());
	setMax(pWeapon->getMax());
	setCoreMin(pWeapon->getCoreMin());
	setCoreMax(pWeapon->getCoreMax());
	setHeight(pWeapon->getHeight());
	if(pWeapon->IsF())
	{// フィールド武器だったら、FieldToward設定
		if(pWeapon->getField()==Weapon::Field::LINE)
		{
			// LINE時のminは幅なので、スタートは1
			setMin(1);
			setCoreMin(1);
			if(nToward>=-1) setField(nToward);
		}
		else
		{	setField(Way::ALL); }
	}
	else // そうじゃなければ、無効化
	{	setField(Way::NO);	}
}

void CMapChipState::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsAction() && IsValid())
			OnAction(pContext);
	}
	else
	{
		if(IsVisible())
			OnDraw(pContext);
	}
}

bool CMapChipState::IsAtkRange()
{
	return getRealDist()<=getAttack() && getMin()<=getAttack() && getMax()>=getAttack();
}

bool CMapChipState::IsHeight()
{
	return getHeight()>=getAtkHeight();
}

bool CMapChipState::IsCore()
{
	return getField()>=0
		 ? (getCoreMin()<=getFieldAttack() && getCoreMax()>=getFieldAttack())
		 : (getCoreMin()<=getAttack() && getCoreMax()>=getAttack())
		 ;
}

bool CMapChipState::IsField()
{
	return (getField()==Way::ALL || getField()==getFieldToward(0) || getField()==getFieldToward(1))
			&& getMin()<=getFieldAttack() && getMax()>=getFieldAttack();
}

void CMapChipState::OnDraw(Task::CTaskContext* pContext)
{
	Draw::CDrawInfo info = getDrawInfo();

	// 移動・攻撃領域
	// 移動と攻撃領域は同時に描画される時は移動が上に来る
	if(IsAttack())
	{// 攻撃範囲
		if((getField()<0 && IsAtkRange() && IsHeight())
		|| IsField())
		{
			int nState = IsCore() ? MapChip::ATTACK_CORE : MapChip::ATTACK;
			sprite_[nState]->setDrawInfo(info);
			sprite_[nState]->setAlpha(IsMapValid() ? 255 : 255/2);
			sprite_[nState]->OnDraw(pContext);
		}
	}
	if(IsMove() && nMove_>=0)
	{// 移動範囲
		sprite_[MapChip::MOVE]->setDrawInfo(info);
		sprite_[MapChip::MOVE]->OnDraw(pContext);
	}
	
	// マウスオーバー
	#ifdef BMW_DEBUG_MAP
	if(bOver_)
	{
		sprite_[MapChip::ACTIVE]->setDrawInfo(info);
		sprite_[MapChip::ACTIVE]->OnDraw(pContext);
		bOver_=false;
	}
	#endif

	if(IsAction() && getState()!=CButton::NORMAL)
	{
		sprite_[MapChip::ACTIVE]->setDrawInfo(info);
		sprite_[MapChip::ACTIVE]->OnDraw(pContext);

		#ifdef BMW_DEBUG_MAP
		// ついでに隣も表示する
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		const CMapChipInfo& map = smart_ptr_static_cast<CMapChip>(getParent())->getMapInfo();
		for(int i=0; i<4; ++i)
		{
			if(p->getMapChip(map.getOnMap(i))!=NULL)
			{
				p->getMapChip(map.getOnMap(i))->getMapChipState()->bOver_=true;
			}
		}
		#endif
	}

	// グリッド
	if(IsGrid())
	{
		sprite_[MapChip::GRID]->setDrawInfo(info);
		sprite_[MapChip::GRID]->OnDraw(pContext);
	}
}

void CMapChipState::actionOverIn(Task::CTaskContext* pContext)
{
	GUI::CButton::actionOverIn(pContext);
	getParent()->setState(CMapChip::OVER);
}

} // namespace Map end
} // namespace SLG end
} // namespace BMW end