#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"

#include "IDAction.h"
#include "CActionNormalEval.h"

namespace BMW{
namespace SLG{
namespace Action{

void CActionNormalEval::Serialize(ISerialize& s)
{// 書き出しだけ
	if(s.IsStoring())
	{
		int nID = Action::NORMAL_EVAL;
		s << nID;
		// 書き出すサイズ
		nID = 6;
		s << nID;
		// データ
		nID = getWait();
		s << nID;
		nID = getMove();
		s << nID;
		nID = getSnipeChara();
		s << nID;
		nID = getEvalMove()>0 ? Action::SHORT : Action::LONG;
		s << nID;
		nID = getEvalAtk()>0 ? Action::LONG : Action::SHORT;
		s << nID;
		nID = getEvalHP()>0 ? Action::LOW : Action::HIGH;
		s << nID;
	}
}

void CActionNormalEval::getActionParam(int& nActionID, list<int>& listParam)
{
	nActionID=Action::NORMAL_EVAL;
	listParam.push_back(getWait());
	listParam.push_back(getMove());
	listParam.push_back(getSnipeChara());
	listParam.push_back(getEvalMove());
	listParam.push_back(getEvalAtk());
	listParam.push_back(getEvalHP());
}

////////////////////////////////////////////////////////
// 思考ルーチン本体
////////////////////////////////////////////////////////
bool CActionNormalEval::actionAttack(SLG::CDataCharaSLG& chara, CSLGContext& p)
{
	// 狙うキャラが設定されてる？
	if(getSnipeChara()>=0) setTargetChara(getSnipeChara());

	// とりあえず、設定されているキャラに攻撃できるか？
	CDataCharaSLG* pChara = p.getCharaData(getTargetChara());
	// 二回攻撃時は移動後扱いになるため、設定されてるActを見ておく
	int nID = selectWeapon(chara, pChara, chara.getState().getAct()==Act::MOVE, p);
	if(nID>=0)
	{// 攻撃できる武器あったよー
		setUseWeapon(nID);
		return true;
	}

	// 狙うキャラがいないなら、新しく探すよ
	if(getSnipeChara()<0)	setTargetChara(-1);
	else					return false;

	// できないなら、攻撃範囲内に攻撃できるキャラいるか？
	if(!IsAtk(&chara, &p, false)) // いねえ・・・
		return false;
	
	// いるっぽいぞ検索だー！
	// 攻撃可能範囲からキャラIDリストをゲットするぞー
	list<int> listChara;
	p.getRangeChara(chara.getPhase(), false, listChara);
	// その中から攻撃対象を探すで
	int nMaxPoint = INT_MIN;
	CDataCharaSLG* pCharaSelect=NULL;
	Map::CMapChip* pChip;
	int nWeapon,nPoint;

	list<int>::iterator it;
	for(it=listChara.begin(); it!=listChara.end(); ++it)
	{	// キャラデータ取得
		pChara = p.getCharaData(*it);
		pChip = p.getMapChip(pChara->getIndex());
		if(pChip!=NULL)
		{	// そいつに攻撃するのに一番良い武器探すで
			// 二回攻撃時は移動後扱いになるため、設定されてるActを見ておく
			nWeapon = selectWeapon(chara, pChara, chara.getState().getAct()==Act::MOVE, p);
			if(nWeapon>=0)
			{// 武器があったら評価
				nPoint = rantingAction2(*pChara,pChip->getMapChipState()->getAttack(),p.getWeaponData(nWeapon));
				if(nMaxPoint < nPoint)
				{// こっちの方がいいらしい
					pCharaSelect=pChara;
					nID=nWeapon;
					nMaxPoint = nPoint;
				}
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

int CActionNormalEval::rantingAction(CDataCharaSLG& chara, int nMove, int nAttack, Weapon::CDataWeaponBattle* pWeapon)
{// 移動後攻撃評価関数
 // フラグによって変化する
	return pWeapon->IsCore(nAttack)*1000 + nMove*getEvalMove() + nAttack*getEvalAtk() - chara.getBattle().getHP()*getEvalHP()/50;
}

int CActionNormalEval::rantingAction2(CDataCharaSLG& chara, int nAttack, Weapon::CDataWeaponBattle* pWeapon)
{// 移動しない攻撃評価関数
 // フラグによって変化する
 // 攻撃評価がマイナスなのは、移動後と違ってこっちは自分基準で攻撃範囲が計算されているから
	return pWeapon->IsCore(nAttack)*1000 - nAttack*getEvalAtk() - chara.getBattle().getHP()*getEvalHP()/50;
}

} // namespace Action end
} // namespace SLG end
} // namespace BMW end