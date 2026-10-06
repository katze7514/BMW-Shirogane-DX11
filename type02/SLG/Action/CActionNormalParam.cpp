#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"
//#include "../Map/CMapChipChara2.h"

#include "../Move/CMove_dist.h"
#include "../Attack/CAttack_range2.h"

#include "IDAction.h"
#include "CActionNormalParam.h"

namespace BMW{
namespace SLG{
namespace Action{

void CActionNormalParam::Serialize(ISerialize& s)
{// 書き出しだけ
	if(s.IsStoring())
	{
		int nID = Action::NORMAL_PARAM;
		s << nID;
		nID = 3;
		s << nID;
		nID = getWait();
		s << nID;
		nID = getMove();
		s << nID;
		nID = getSnipeChara();
		s << nID;
	}
}

void CActionNormalParam::getActionParam(int& nActionID, list<int>& listParam)
{
	nActionID=Action::NORMAL_PARAM;
	listParam.push_back(getWait());
	listParam.push_back(getMove());
	listParam.push_back(getSnipeChara());
}

void CActionNormalParam::action(CDataCharaSLG& chara, CSLGContext& p)
{
	// 状況をリセット
	setUseWeapon(-1);	// 攻撃せず
	setMapIndex(-1);	// 移動もしない

	// 行動不能だったら、このまま終了
	if(chara.getBattle().IsCond(Chara::CValidCond::ACTION)) return;

	// 挑発されてたらそれを設定
	swapProvo(chara);

	// 攻撃優先キャラがいればそいつを狙う
	if(!(getSnipeChara()>=0
	&& p.getCharaData(getSnipeChara())!=NULL
	&& p.getCharaData(getSnipeChara())->IsExist()))
	// 狙うキャラがいなくなってた
		setSnipeChara(-1);

	if(getWait()<=0)
	{// WAITはしない
		// その場で攻撃できるか？
		// 攻撃範囲をとりあえず計算
		calcAttack(chara,p);

		// 狙うキャラが-2だったら、毎回狙うキャラを計算し直す
		if(getSnipeChara()==CHANGE) setTargetChara(-1);
		// ターゲット検索～
		// 必ず移動なら、actionAttackはスキップ
		if(getMove()==MOVE_ONLY
		|| getMove()==MOVE_ONLY_ATK
		|| (!actionAttack(chara,p) 
			&& getMove()!=MOVE_NO
			// 二回攻撃時はactionAttackだけ呼べば良い
			&& chara.getState().getAct()!=Act::MOVE))
		{// 移動後攻撃できるか？
		// もしくは、次のターン攻撃できそうな位置に移動
			// 移動砲台モードだったら、最低射程内にキャラがいるかを検索
			if(getMove()==MOVE_BATTERY) bInner_ = p.IsRangeCharaInner(nMin_-1,nReach_,chara.getPhase());
			// 移動範囲計算
			calcMove(chara,p);
			if(!actionMoveAttack(chara,p))
			{// それでもだめなら、攻撃できそうなやつに近づくだけ
				if(getMove()==MOVE
				|| getMove()==MOVE_ONLY)
					actionMove(chara,p);
			}
		}

		// 砲台モードか、移動砲台モードで内側にキャラがいなければ
		// 移動せず
		if(getMove()==MOVE_ATK
		|| getMove()==MOVE_ONLY_ATK
		|| (getMove()==MOVE_BATTERY && !bInner_))
		{// 武器が設定されてないなら、動かない
			if(getUseWeapon()<0) setMapIndex(-1);
		}
		
		// 移動範囲が設定されてなかったら、移動範囲をクリア
		if(getMapIndex()<0) p.clearMove();
	}
	else
	{ --nWait_; }

	// 挑発
	swapProvo(chara);
	endProvo(chara);
}

////////////////////////////////////////////////////////////
// いってみればヘルパ
////////////////////////////////////////////////////////////
void CActionNormalParam::calcAttack(CDataCharaSLG& chara, CSLGContext& p, CDataCharaSLG* pAbility)
{
	p.clearRange();
	// 攻撃範囲の取得
	Attack::CAttack_range2 range;
	// 計算状況設定
	range.setSLGContext(&p);
	Map::CMapChip* pMap = p.getMapChip(chara.getIndex());
	range.actionAbility(pAbility==NULL ? &chara : pAbility);
	range.setIndex(chara.getIndex());

	// 計算
	range.calcAttack(pMap,0,0,0,Way::NO);

	nMin_ = range.getMin();
	nReach_ = range.getReach();
}

////////////////////////////////////////////////////////
// 思考ルーチン本体
////////////////////////////////////////////////////////
bool CActionNormalParam::actionAttack(SLG::CDataCharaSLG& chara, CSLGContext& p)
{
	if(getSnipeChara()<0)
		return CActionNormal::actionAttack(chara,p);
	else
		return actionAttackSnipe(chara,p);
}

bool CActionNormalParam::actionMoveAttack(SLG::CDataCharaSLG& chara, CSLGContext& p)
{
	if(getSnipeChara()<0)
		return CActionNormal::actionMoveAttack(chara,p);
	else
		return actionMoveAttackSnipe(chara,p);
}

void CActionNormalParam::actionMove(SLG::CDataCharaSLG& chara, CSLGContext& p)
{
	if(getSnipeChara()<0)
		CActionNormal::actionMove(chara,p);
	else
		actionMoveSnipe(chara,p);
}

bool CActionNormalParam::actionAttackSnipe(SLG::CDataCharaSLG& chara, CSLGContext& p)
{
	// とりあえず、設定されているキャラに攻撃できるか？
	SLG::CDataCharaSLG* pChara = p.getCharaData(getSnipeChara());
	int nID = selectWeapon(chara, pChara, false, p);
	if(nID>=0)
	{// 攻撃できる武器あったよー
		setTargetChara(getSnipeChara());
		setUseWeapon(nID);
		return true;
	}

	// できねえし
	// 次は移動後攻撃ですよ
	return false;
}	
bool CActionNormalParam::actionMoveAttackSnipe(SLG::CDataCharaSLG& chara, CSLGContext& p)
{
	// フェイズリスト取得
	CDataCharaSLG *pTargetChara;
	int nWeaponID=-1;
	int nIndex=-1;
	Map::CMapChip* pChip;
	// そいつを軸に攻撃範囲を計算
	pTargetChara = p.getCharaData(getSnipeChara());
	// 計算のために一度自分自身をはずす
	Task::ITaskBase* pSelf = p.getMapChip(chara.getIndex())->removeTask(Map::CMapChip::CHARA);
	// そのキャラを軸に攻撃範囲計算
	calcAttack(*pTargetChara,p,&chara);
	// 終わったので戻す
	p.getMapChip(chara.getIndex())->addTask(pSelf,Map::CMapChip::CHARA);

	set<int>& setMove = p.getIndexSet();
	set<int>::iterator mit;
	for(mit=setMove.begin(); mit!=setMove.end(); ++mit)
	{// 移動範囲を見つつ判定をしていく
		pChip = p.getMapChip(*mit);
		// 攻撃範囲に入ってない、もしくはそこにキャラがいたら次へ
		if(pChip->getMapChipState()->getAttack()<=0
		|| pChip->getTask(Map::CMapChip::CHARA)!=NULL) continue;
		// そこに攻撃できる武器があるか
		int nID = selectWeaponAttack(chara,
								     pChip->getMapChipState()->getAttack(),
									 pChip->getMapChipState()->getRealDist(),
									 pChip->getMapChipState()->getAtkHeight(),
									 true, p);
		if(nID>=0)
		{// あるなら、攻撃
			Weapon::CDataWeaponBattle* pWeapon = p.getWeaponData(nID);
			nIndex = *mit;
			// 武器はP属性が無いと攻撃できない
			// こうしておくことで、攻撃できそうな場所に
			// 移動だけはするようになる
			if(pWeapon->IsP()) nWeaponID=nID;

			setTargetChara(getSnipeChara());
			setUseWeapon(nWeaponID);
			setMapIndex(nIndex);
			return true;
		}
		
	}

	// 移動後攻撃もできねー
	return false;
}

void CActionNormalParam::actionMoveSnipe(SLG::CDataCharaSLG& chara, CSLGContext& p)
{
	Move::CMove_dist dist;
	dist.setSLGContext(&p);
	dist.setJump(chara.getBattle().getJump());
	dist.setPhase(chara.getPhase());

	// 移動範囲Indexを一端コピー
	set<int> setMove(p.getIndexSet().begin(), p.getIndexSet().end());
	set<int> moveOut;
	set<int>::iterator it_m;
	int nMove=0;
	// 外縁範囲を取得
	while(moveOut.empty() && nMove<=chara.getBattle().getMove())
	{
		for(it_m=setMove.begin(); it_m!=setMove.end(); it_m++)
		{
			if(p.getMapChip(*it_m)->getMapChipState()->getMove()==nMove)
			{
				moveOut.insert(*it_m);
			}
		}
		nMove++;
	}

	// 移動先判定
	int nIndex=-1;
	int nMinDist=INT_MAX;
	SLG::CDataCharaSLG* pTarget;
	// 対象取得
	pTarget = p.getCharaData(getSnipeChara());
	dist.setEndIndex(pTarget->getIndex());
	for(it_m=moveOut.begin(); it_m!=moveOut.end(); it_m++)
	{// 距離計算
		// そこに誰かいたら次へ
		if(p.getMapChip(*it_m)->getTask(Map::CMapChip::CHARA)!=NULL) continue;
		p.clearDist();
		p.getIndexSet().insert(*it_m);
		dist.setDist(-1);
		dist.calcDist(p.getMapChip(*it_m),0);
		if(dist.getDist()>=0 && nMinDist>dist.getDist())
		{
			nMinDist = dist.getDist();
			nIndex=*it_m;
		}
	}

	p.clearDist();

	setMapIndex(nIndex);

	// IndexSetを戻す
	p.getIndexSet().insert(setMove.begin(), setMove.end());
}

void CActionNormalParam::swapProvo(CDataCharaSLG& chara)
{
#ifdef BMW_DEBUG
	CDbg().Out("Provo %d %d",chara.getID(),chara.getBattle().IsSpirit(Chara::CValidSpirit::PROVO));
#endif
	// 挑発後攻撃だったら、一端TargetCharaもリセット
	if(bProvo_)
	{ 
		setTargetChara(-1);
		bProvo_=false;
	}

	// 挑発IDとSnipeCharaを入れ替える
	if(chara.getBattle().IsSpirit(Chara::CValidSpirit::PROVO))
	{
		int nID = getSnipeChara();
		setSnipeChara(chara.getBattle().getValidSpirit().getProvoID());
		chara.getBattle().getValidSpirit().setProvoID(nID);
	}
}

void CActionNormalParam::endProvo(CDataCharaSLG& chara)
{
	// 攻撃相手が挑発と同じだったら、挑発効果を消す
	if(chara.getBattle().IsSpirit(Chara::CValidSpirit::PROVO)
	&& chara.getBattle().getValidSpirit().getProvoID()==getTargetChara())
	{
		chara.getBattle().spirit(false, Chara::CValidSpirit::PROVO);
		chara.getBattle().getValidSpirit().setProvoID(-1);
		bProvo_=true; // 挑発攻撃だよ
	}
}

void CActionNormalParam::responseAbility(SLG::CDataCharaSLG& chara, CSLGContext& context)
{// 技能による思考ルーチンの影響
	// 二回行動 二回実行されるのでwaitを倍にする
	if(chara.getBattle().IsTalent(Ability::TWICE_ACTION))
		setWait(getWait()*2);
}


} // namespace Action end
} // namespace SLG end
} // namespace BMW end