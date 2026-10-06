#include "stdafx.h"

#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"

#include "../Attack/CAttack_range2.h"

#include "IDAction.h"
#include "CActionFieldEval.h"

namespace BMW{
namespace SLG{
namespace Action{

void CActionFieldEval::Serialize(ISerialize& s)
{// 書き出しだけ
	if(s.IsStoring())
	{
		int nID = Action::FIELD_EVAL;
		s << nID;
		nID = 8;
		s << nID;
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
		nID = getCharaNum();
		s << nID; 
		nID = getFieldWeapon();
		s << nID;
	}
}

void CActionFieldEval::getActionParam(int& nActionID, list<int>& listParam)
{
	nActionID=Action::FIELD_EVAL;
	listParam.push_back(getWait());
	listParam.push_back(getMove());
	listParam.push_back(getSnipeChara());
	listParam.push_back(getEvalMove());
	listParam.push_back(getEvalAtk());
	listParam.push_back(getEvalHP());
	listParam.push_back(getCharaNum());
	listParam.push_back(getFieldWeapon());
}

void CActionFieldEval::action(CDataCharaSLG& chara, CSLGContext& p)
{// とりあえず、マップ兵器が撃てるかを判定して、OKならうつ
	if(getWait()<=0)
	{// WAIT優先
		Weapon::CDataWeaponBattle* pWeapon;
		if(getFieldWeapon()<0)
		{// マップ武器が設定されてなかったら、一度探す
			Chara::CDataCharaBattle& battle = chara.getBattle();
			int nWeapon;
			battle.beginWeapon();
			while(!battle.endWeapon())
			{
				nWeapon = *battle.nextWeapon();
				pWeapon = p.getWeaponData(nWeapon);
				if(pWeapon->IsF())
				{// みっけ
					setFieldWeapon(nWeapon);
					break;
				}
			}
		}

		// 行動不能だったら、このまま終了
		if(chara.getBattle().IsCond(Chara::CValidCond::ACTION))
		{
			setUseWeapon(-1);	// 攻撃せず
			setMapIndex(-1);	// 移動もしない
			return;
		}

		if(getFieldWeapon()<0)
		{// ん？ 持ってないのおかしくね？ でも、しゃあねー
			CActionNormalEval::action(chara,p);
		}
		else
		{
			// とりあえず、初期化
			setUseWeapon(-1);	// 攻撃せず
			setMapIndex(-1);	// 移動もしない

			// 挑発されてたらそれを設定
			swapProvo(chara);

			// 攻撃範囲計算
			calcAttack(chara,p);
			pWeapon = p.getWeaponData(getFieldWeapon());
			// 使える？
			// 二回攻撃時は移動後扱いになるため、設定されてるActを見ておく
			if(pWeapon->enable(chara, chara.getState().getAct()==Act::MOVE, p))
			{// 使える
				int anToward[4]={0,0,0,0};
				bool abSnipe[4]={false,false,false,false};
				// マップ兵器の範囲内に何人いるかをカウント
				p.getRangeFieldToward(pWeapon->getField(),
									  chara.getPhase(),
									  pWeapon->getMin(),
									  pWeapon->getMax(),
									  anToward,
									  getSnipeChara(),
									  abSnipe);
				int nMaxCounter=0;
				if(pWeapon->getField()==Weapon::Field::LINE)
				{// LINEタイプの場合、キャラ最大数を検索。同じ方向がある場合は、ランダムにしちゃうかｗ
					for(int nToward=0; nToward<=3; ++nToward)
					{
						// 人数同じ
						if(anToward[nMaxCounter] == anToward[nToward])
						{
							// 狙うキャラがどっちにもいる/いないなら、ランダムで選択
							if(abSnipe[nMaxCounter]  == abSnipe[nToward])
								nMaxCounter = CApp::rand_.Get(2) ? nMaxCounter : nToward;
							// snipeがいる方を優先
							ef(!abSnipe[nMaxCounter] && abSnipe[nToward])
								nMaxCounter = nToward;
						}
						// どらにもsnipeがいる/いないなら、人数が多い方優先
						ef(anToward[nMaxCounter] < anToward[nToward]
						&& abSnipe[nMaxCounter] == abSnipe[nToward])
							nMaxCounter = nToward;
					}
				}

				// 挑発されてたら元に戻す
				swapProvo(chara);
				endProvo(chara);

				#ifdef BMW_DEBUG
					CDbg().Out("FIELD_EVAL %d %d %d %d",(int)getPhaseList(chara.getPhase(),p).size(),anToward[nMaxCounter],getCharaNum(),abSnipe[nMaxCounter]);
				#endif

				if(anToward[nMaxCounter]>=0
				|| (int)getPhaseList(chara.getPhase(),p).size()<=anToward[nMaxCounter]
				|| getCharaNum()<=anToward[nMaxCounter]
				|| abSnipe[nMaxCounter])
				{// マップ兵器をうつ最低人数に達していれば
				 // 狙うキャラがいたら問答無用でぶっ放し
					Map::CMapChipState::setField(nMaxCounter);
					setUseWeapon(getFieldWeapon());
					setMapIndex(-1);
					setTargetChara(-1);
				}
				else
				{// 達してないなら、通常動作
					CActionNormalEval::action(chara,p);
				}
			}
			else
			{// 使えない
				// 通常動作
				CActionNormalEval::action(chara,p);
			}
		}
	}
	else
	{ --nWait_; }
}

////////////////////////////////////////////////////////////
// いってみればヘルパ
////////////////////////////////////////////////////////////
void CActionFieldEval::calcAttack(CDataCharaSLG& chara, CSLGContext& p, CDataCharaSLG* pAbility)
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
	if(getFieldWeapon()>=0) range.calcField(pMap);

	nMin_ = range.getMin();
	nReach_ = range.getReach();
}

} // namespace Action end
} // namespace SLG end
} // namespace BMW end