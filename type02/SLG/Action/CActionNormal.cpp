#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"
//#include "../Map/CMapChipChara2.h"

#include "../Move/CMove_dist.h"
#include "../Move/CMove_range.h"
#include "../Attack/CAttack_range2.h"

#include "IDAction.h"
#include "CActionNormal.h"

namespace BMW{
namespace SLG{
namespace Action{

void CActionNormal::Serialize(ISerialize& s)
{// 書き出しだけ
	if(s.IsStoring())
	{
		int nID = Action::NORMAL;
		s << nID;
		nID = 0;
		s << nID;
	}
}

void CActionNormal::getActionParam(int& nActionID, list<int>& listParam)
{
	nActionID=Action::NORMAL;
}

void CActionNormal::action(CDataCharaSLG& chara, CSLGContext& p)
{// 一番基本的な思考ルーチンVer.2
 // この思考ルーチンは、攻撃 → 移動攻撃 → 移動、という優先順位で計算する
 // また、一度攻撃対象キャラを決めると、基本的にそれを攻撃し続ける
 // Ver.1と違って、武器範囲からキャラを選択し、そこに攻撃できる武器
 // という手順で計算する

	// 状況をリセット
	setUseWeapon(-1);	// 攻撃せず
	setMapIndex(-1);	// 移動もしない

	// 行動不能だったら、このまま終了
	if(chara.getBattle().IsCond(Chara::CValidCond::ACTION)) return;

	// その場で攻撃できるか？
	calcAttack(chara,p);
	if(!actionAttack(chara,p))
	{// 移動後攻撃できるか？
	 // もしくは、次のターン攻撃できそうな位置に移動
		// 移動範囲計算
		calcMove(chara,p);
		if(!actionMoveAttack(chara,p))
		{// それでもだめなら、攻撃できそうなやつに近づくだけ
			actionMove(chara,p);
		}
	}
	
	// 移動範囲が設定されてなかったら、移動範囲をクリア
	if(getMapIndex()<0) p.clearMove();
}

////////////////////////////////////////////////////////
// 思考ルーチン補助
////////////////////////////////////////////////////////
bool CActionNormal::IsWeaponSelect(Weapon::CDataWeaponBattle* pWeaponSelect, Weapon::CDataWeaponBattle* pWeapon, int nAttack)
{
	return pWeaponSelect==NULL													// NULLチェック
		||	(!pWeaponSelect->IsCore(nAttack) && pWeapon->IsCore(nAttack))		// 中心距離優先
		||	(pWeaponSelect->getAttack() < pWeapon->getAttack());				// 攻撃力チェック
}

int CActionNormal::selectWeaponAttack(CDataCharaSLG& chara, int nAttack, int nDist, int nHeight, bool bP, CSLGContext& p)
{
	//CDbg().Out("ATTACK %d %d %d", nAttack, nDist, nHeight);

	Weapon::CDataWeaponBattle *pWeapon, *pWeaponSelect=NULL;
	int nID=-1;
	if(nAttack>=0)
	{// 何かの武器の射程内のはず
		int nWeapon;
		Chara::CDataCharaBattle& battle = chara.getBattle();
		battle.beginWeapon();
		while(!battle.endWeapon())
		{
			nWeapon = *battle.nextWeapon();
			pWeapon = p.getWeaponData(nWeapon);
			// MAP兵器だったらスキップ
			if(pWeapon->IsF()) continue;

			//CDbg().Out("Weapon %d %d", pWeapon->getID(),pWeapon->enable(chara,bP,p,nAttack,nDist,nHeight));

			if(pWeapon->enable(chara,bP,p,nAttack,nDist,nHeight))
			{// 武器が使用可能
				if(IsWeaponSelect(pWeaponSelect, pWeapon, nAttack))
				{// より良い武器？
					pWeaponSelect=pWeapon;
					nID=nWeapon;
				}
			}
		}
	}

	return nID;
}

int CActionNormal::selectWeapon(CDataCharaSLG& chara, CDataCharaSLG* pTargetChara, bool bP, CSLGContext &p)
{// pCharaで渡ってきたキャラに攻撃できる武器があるか判定
	Map::CMapChip* pChip;
	int nID=-1;
	if(pTargetChara!=NULL
	&& pTargetChara->IsExist())
	{// そのキャラがマップ上にいるならば
		pChip = p.getMapChip(pTargetChara->getIndex());
		nID = selectWeaponAttack(chara,
								 pChip->getMapChipState()->getAttack(),
								 pChip->getMapChipState()->getRealDist(),
								 pChip->getMapChipState()->getAtkHeight(),
								 bP,p);
	}

	return nID;
}

////////////////////////////////////////////////////////
// 思考ルーチン本体
////////////////////////////////////////////////////////
bool CActionNormal::actionAttack(SLG::CDataCharaSLG& chara, CSLGContext& p)
{
	// とりあえず、設定されているキャラに攻撃できるか？
	SLG::CDataCharaSLG* pChara = p.getCharaData(getTargetChara());
	// 二回攻撃時は移動後扱いにするため、設定されているActを見る
	int nID = selectWeapon(chara, pChara, chara.getState().getAct()==Act::MOVE, p);
	if(nID>=0)
	{// 攻撃できる武器あったよー
		setUseWeapon(nID);
		return true;
	}

	// 新しく探すよ
	setTargetChara(-1);

	// できないなら、攻撃範囲内に攻撃できるキャラいるか？
	if(!IsAtk(&chara, &p, false)) // いねえ・・・
		return false;
	
	// いるっぽいぞ検索だー！
	// 攻撃可能範囲からキャラIDリストをゲットするぞー
	list<int> listChara;
	p.getRangeChara(chara.getPhase(), false, listChara);
	// その中から現在HPが一番少ないやつを探すで
	list<int>::iterator it;
	CDataCharaSLG* pCharaSelect=NULL;
	int nWeapon;
	for(it=listChara.begin(); it!=listChara.end(); ++it)
	{
		pChara = p.getCharaData(*it);
		if(pCharaSelect==NULL
		|| pCharaSelect->getBattle().getHP() > pChara->getBattle().getHP())
		{
			// そいつに攻撃するのに一番良い武器探すで
			// 二回攻撃時は移動後扱いにするため、設定されているActを見る
			nWeapon = selectWeapon(chara, pChara, chara.getState().getAct()==Act::MOVE, p);
			if(nWeapon>=0)
			{// 攻撃できる武器あったよー
				pCharaSelect=pChara;
				nID=nWeapon;
			}
		}
	}

	if(pCharaSelect!=NULL)
	{// 攻撃できる！
		setTargetChara(pCharaSelect->getID());
		setUseWeapon(nID);
		return true;
	}

	// ここまでやっても、ダメなのか！！
	// 次は移動後攻撃ですよ
	return false;
}

int CActionNormal::rantingAction(CDataCharaSLG& chara, int nMove, int nAttack, Weapon::CDataWeaponBattle* pWeapon)
{// 移動後攻撃評価関数
 // 評価値は、とりあえず
 // 0.中心距離である
 //	1.移動距離が短い
 //	2.攻撃距離が長い
 //	3.相手のHPが少ない
 // な感じで
	return pWeapon->IsCore(nAttack)*1000 + nMove*25 + nAttack*20 - chara.getBattle().getHP()/50;
}

bool CActionNormal::actionMoveAttack(SLG::CDataCharaSLG& chara, CSLGContext& p)
{// キャラリストを回し、そこから攻撃判定
 // 自分の移動範囲と積をとりつつ、武器を踏まえつつ評価

	// フェイズリスト取得
	list<int>& listPhase = getPhaseList(chara.getPhase(),p);
	list<int>::iterator it;
	CDataCharaSLG *pTargetChara, *pCharaSelect=NULL;
	int nWeaponID=-1;
	int nIndex=-1;
	Map::CMapChip* pChip;
	int nMaxPoint=INT_MIN;
	set<int>& setMove = p.getIndexSet();
	set<int>::iterator mit;
	// 計算のために一度自分自身をはずす
	Task::ITaskBase* pSelf = p.getMapChip(chara.getIndex())->removeTask(Map::CMapChip::CHARA);

	for(it=listPhase.begin(); it!=listPhase.end(); ++it)
	{
		// そいつを軸に攻撃範囲を計算
		pTargetChara = p.getCharaData(*it);
		// キャラがマップにいなかったら次へ
		if(pTargetChara==NULL || !pTargetChara->IsExist()) continue;
		// そのキャラを軸に攻撃範囲計算
		calcAttack(*pTargetChara,p,&chara);
		int nHeight = p.getMapChip(pTargetChara->getIndex())->getMapInfo().getHeight();
		for(mit=setMove.begin(); mit!=setMove.end(); ++mit)
		{// 移動範囲を見つつ判定をしていく
			pChip = p.getMapChip(*mit);
			// 攻撃範囲に入ってない、もしくはそこにキャラがいたら次へ
			if(pChip->getMapChipState()->getAttack()<0
			|| pChip->getTask(Map::CMapChip::CHARA)!=NULL) continue;
			// そこに攻撃できる武器があるか
			int nID = selectWeaponAttack(chara,
									     pChip->getMapChipState()->getAttack(),
										 pChip->getMapChipState()->getRealDist(),
										 abs(pChip->getMapInfo().getHeight()-nHeight),
										 true, p);
			if(nID>=0)
			{// あるなら、評価
				Weapon::CDataWeaponBattle* pWeapon = p.getWeaponData(nID);
				int nPoint = rantingAction(*pTargetChara,
										   pChip->getMapChipState()->getMove(),
										   pChip->getMapChipState()->getAttack(),
										   pWeapon);
				if(nMaxPoint<nPoint
				// 同じだった場合は、ランダムで選択
				|| (nMaxPoint==nPoint && CApp::rand_.Get(2)))
				{// 今の状況の方がポイント高
					nMaxPoint=nPoint;
					pCharaSelect = pTargetChara;
					nIndex = *mit;
					// 武器はP属性が無いと攻撃できない
					// こうしておくことで、攻撃できそうな場所に
					// 移動だけはするようになる
					if(pWeapon->IsP()) nWeaponID=nID;
				}
			}
		}
	}
	// 終わったので戻す
	p.getMapChip(chara.getIndex())->addTask(pSelf,Map::CMapChip::CHARA);

	if(pCharaSelect!=NULL)
	{// キャラが設定されてるなら状況確定
		setTargetChara(pCharaSelect->getID());
		setUseWeapon(nWeaponID);
		setMapIndex(nIndex);
		return true;
	}

	// 移動後攻撃もできねー
	return false;
}

void CActionNormal::actionMove(SLG::CDataCharaSLG& chara, CSLGContext& p)
{// ここに来たということは、とにかく一番近い相手に近づく
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
		for(it_m=setMove.begin(); it_m!=setMove.end(); ++it_m)
		{
			if(p.getMapChip(*it_m)->getMapChipState()->getMove()==nMove)
				moveOut.insert(*it_m);
		}
		++nMove;
	}

	// 移動先判定
	int nMinDist=INT_MAX;
	SLG::CDataCharaSLG* pTarget;
	list<int>& playerList = getPhaseList(chara.getPhase(),p);
	list<int>::iterator it;
	int nIndex=-1;
	for(it=playerList.begin(); it!=playerList.end(); ++it)
	{	// 対象取得
		pTarget = p.getCharaData(*it);
		if(pTarget==NULL) continue;
		dist.setEndIndex(pTarget->getIndex());
		for(it_m=moveOut.begin(); it_m!=moveOut.end(); ++it_m)
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
	}
	p.clearDist();

	setMapIndex(nIndex);

	// IndexSetを戻す
	p.getIndexSet().insert(setMove.begin(), setMove.end());
}

////////////////////////////////////////////////////////////
// いってみればヘルパ
////////////////////////////////////////////////////////////
void CActionNormal::calcAttack(CDataCharaSLG& chara, CSLGContext& p, CDataCharaSLG* pAbility)
{
	p.clearRange();
	// 攻撃範囲の取得
	Attack::CAttack_range2 range;
	// 計算状況設定
	range.setSLGContext(&p);
	Map::CMapChip* pMap = p.getMapChip(chara.getIndex());
	range.actionAbility(pAbility==NULL ? &chara : pAbility,true);
	range.setIndex(chara.getIndex());

	// 計算
	range.calcAttack(pMap,0,0,0,Way::NO);
}

void CActionNormal::calcMove(CDataCharaSLG& chara, CSLGContext& p)
{
	p.clearMove();
	// 移動範囲を計算
	Move::CMove_range move;
	move.setContext(&p);
	move.setPhase(chara.getPhase());
	p.getIndexSet().insert(chara.getIndex());
	// 計算
	move.calcMove(p.getMapChip(chara.getIndex()), move.actionAbility(&chara));
}

list<int>& CActionNormal::getPhaseList(int nPhase, CSLGContext& p)
{
	if(nPhase==Phase::PLAYER)	return p.getEnemyPhaseList();
	else						return p.getPlayerPhaseList();
}

} // namespace Action end
} // namespace SLG end
} // namespace BMW end