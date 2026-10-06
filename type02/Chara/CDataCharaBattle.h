/*
	katze 05/02/28
	Init + Train + Growth = Battle
*/
#pragma once

#include "CDataCharaBase.h"
#include "CValidSpirit.h"
#include "CValidCond.h"

namespace BMW{
namespace Chara{

class CDataCharaBattle : public CDataCharaBase, public IArchive
{/**
	戦闘用キャラデータクラス

	InitとTrainとGrowthから生成される
 */
public:
	// コンストラクタ・デストラクタ
	CDataCharaBattle():nAttack_(0),nDefence_(0),nCure_(-1),nRefill_(-1){}

	// シリアライズ
	// ようは、差分を保存する感じ
	void Serialize(ISerialize& s);

	// 設定・取得
	const CStatusFund&		getFundOffset() const { return fundOffset_; }
	void					setFundOffset(const CStatusFund& fundOffset){ fundOffset_=fundOffset; }

	const CStatusBattle&	getBattleOffset() const { return battleOffset_; }
	void					setBattleOffset(const CStatusBattle& battleOffset){ battleOffset_=battleOffset; }

	int						IsCure() const { return nCure_; }
	void					cure(int nCure){ nCure_=nCure; }

	int						IsRefill() const { return nRefill_; }
	void					refill(int nRefill){ nRefill_=nRefill; }

	int						getBackUpAttack() const { return nAttack_; }
	void					setBackUpAttack(int nAttack){ nAttack_=nAttack; }

	int						getBackUpDefence() const { return nDefence_; }
	void					setBackUpDefence(int nDefence){ nDefence_=nDefence; }

	// 操作
	void					calcLv(int nLv);
	int						calcExp(int nExp);	// 上がった分の値が返る
	void					calcMental(int nMental);
	void					calcKill(int nKill){ if(KILL_MAX>nKill_) nKill_+=nKill; }

	// 基礎ステ
	int						getStrength() const { return fund_.getStrength() + fundOffset_.getStrength(); }
	int						getMagic() const { return fund_.getMagic() + fundOffset_.getMagic(); }
	int						getHit() const { return fund_.getHit() + fundOffset_.getHit(); }
	int						getAvoid() const { return fund_.getAvoid() + fundOffset_.getAvoid(); }
	int						getDefence() const { return fund_.getDefence() + fundOffset_.getDefence(); }
	int						getSkill() const { return fund_.getSkill() + fundOffset_.getSkill(); }
	int						getSP() const { return fund_.getSP() - fundOffset_.getSP(); }
	// Offset補正無し
	int						getMaxStrength() const { return fund_.getStrength(); }
	int						getMaxMagic() const { return fund_.getMagic(); }
	int						getMaxHit() const { return fund_.getHit(); }
	int						getMaxAvoid() const { return fund_.getAvoid(); }
	int						getMaxDefence() const { return fund_.getDefence(); }
	int						getMaxSkill() const { return fund_.getSkill(); }
	int						getMaxSP() const { return fund_.getSP(); }
	// Offset計算
	void					calcStrength(int nStrength);
	void					calcMagic(int nMagic);
	void					calcHit(int nHit);
	void					calcAvoid(int nAvoid);
	void					calcDefence(int nDefence);
	void					calcSkill(int nSkill);
	void					calcSP(int nSP);

	// 戦闘ステ
	int						getHP() const { return battle_.getHP() - battleOffset_.getHP(); }
	int						getEN() const { return battle_.getEN() - battleOffset_.getEN(); }
	int						getTough() const { return battle_.getTough() - battleOffset_.getTough(); }
	int						getQuick() const { return battle_.getQuick() - battleOffset_.getQuick(); }
	int						getMove() const { return battle_.getMove() - battleOffset_.getMove(); }
	int						getJump() const { return battle_.getJump() - battleOffset_.getJump(); }
	// Offset補正無し
	int						getMaxHP() const { return battle_.getHP(); }
	int						getMaxEN() const { return battle_.getEN(); }
	int						getMaxTough() const { return battle_.getTough(); }
	int						getMaxQuick() const { return battle_.getQuick(); }
	int						getMaxMove() const { return battle_.getMove(); }
	int						getMaxJump() const { return battle_.getJump(); }
	// Offset計算
	void					calcHP(int nHP);
	void					calcEN(int nEN);
	void					calcTough(int nTough);
	void					calcQuick(int nQuick);
	void					calcMove(int nMove);
	void					calcJump(int nJump);

	// 技能
	int						hasSkill(int nID)const;

	// 精神
	bool					IsHasSpirit(int nSpirit)const;
	int						hasSpirit(int nSpirit)const;

	// アイテム
	bool					IsHasItem(int nItem)const;
	int						countHasItem(int nItem)const;
	// アイテムリスト
	item_list::iterator		beginItem() const
							{
								CDataCharaBattle* pBase = const_cast<CDataCharaBattle*>(this);
								pBase->it_i=pBase->listItem_.begin();
								return pBase->it_i;
							}
	bool					endItem() const
							{ 
								CDataCharaBattle* pBase = const_cast<CDataCharaBattle*>(this);
								return pBase->it_i==pBase->listItem_.end();
							}
	item_list::iterator		nextItem() const
							{ 
								CDataCharaBattle* pBase = const_cast<CDataCharaBattle*>(this);
								return pBase->it_i++;
							}
	void					addItem(const CStatusAbility& item){ listItem_.push_back(item); }
	void					addItem(int nID,int nAttr=0)
							{	
								CStatusAbility item; 
								item.setID(nID);
								item.setAttr(nAttr);
								listItem_.push_back(item); 
							}
	bool					delItem(int nAttr)
							{// アイテムは重複がありえるので、装備位置をキーとする
								item_list::iterator it;
								for(it=listItem_.begin(); it!=listItem_.end(); it++)
								{
									if(it->getAttr()==nAttr)
									{
										it->setAttr(-1);
										listItem_.erase(it);
										return true;
									}
								}

								return false;
							}

	int						sizeItem() const { return (int)listItem_.size(); }
	void					clearItem(){ listItem_.clear(); }
	// 指定したAttrのアイテムのIDを返す
	int						hasItemAttr(int nAttr)
							{
								item_list::iterator it;
								for(it=listItem_.begin(); it!=listItem_.end(); it++)
								{
									if(it->getAttr()==nAttr)
										return it->getID();
								}

								return -1;
							}
	// Attr順にソートする
	void					sortItem() const { const_cast<CDataCharaBattle*>(this)->listItem_.sort(sort_Item()); }
	
	// 状態
	const CValidSpirit&		getValidSpirit()const{ return spiritValid_; }
	CValidSpirit&			getValidSpirit(){ return spiritValid_; }
	void					getValidSpirit(const CValidSpirit& spirit){ spiritValid_=spirit; }
	bool					IsSpirit(int nID) const { return spiritValid_.IsValid(nID); }
	void					spirit(bool bSpirit,int nID){ spiritValid_.valid(bSpirit,nID); }
	const CValidCond&		getValidCond()const{ return condValid_; }
	CValidCond&				getValidCond(){ return condValid_; }
	void					setValidCond(const CValidCond& valid){ condValid_=valid; }
	bool					IsCond(int nID) const { return condValid_.IsValid(nID); }
	void					cond(int nCond,int nID){ condValid_.valid(nCond,nID); }
	void					condReset(){ condValid_.reset(); }
	void					condDec(int nID=-1){ if(nID<0){ condValid_.dec(); }else{ condValid_.dec(nID); } }

	// 援護
	bool					IsAttack()const{ return nAttack_>0; }
	bool					decAttack(){ if(nAttack_<=0){ return false; }else{ --nAttack_; return true; } }
	bool					IsDefence()const{ return nDefence_>0; }
	bool					decDefence(){ if(nDefence_<=0){ return false; }else{ --nDefence_; return true; } }

private:
	// 基礎ステ
	CStatusFund		fundOffset_;
	// 戦闘ステ
	CStatusBattle	battleOffset_;
	
	// 戦闘中のみに意味のある各種フラグ
	// キャラ状態
	CValidSpirit	spiritValid_;	// 有効精神
	CValidCond		condValid_;		// 有効状態
	// 援護能力
	int nAttack_;		// 援護攻撃残り回数
	int nDefence_;		// 援護防御残り回数
	// 補助能力
	int nCure_;		// 治癒持ちなら正の数
	int nRefill_;	// 補給持ちなら正の数

	// アイテム
	item_list		listItem_;
	item_list::iterator it_i;
	int				nItemMax_;
};

__inline void CDataCharaBattle::calcLv(int nLv)
{
	nLv_+=nLv;
	if(nLv_>=LV_MAX) nLv_=LV_MAX;
	ef(nLv_<1)		 nLv_=1;
}

__inline int CDataCharaBattle::calcExp(int nExp)
{// つまり、LvUP判定
 // 上がった分の値が返る
	nExp_-=nExp;
	int nLv=getLv();
	// nExp_が負だったらLvUP
	if(nExp_<=0)
	{// EXP_MAXを足して入って0以上になるまでLvを上げる
		do{ calcLv(1); nExp_+=EXP_MAX; }while(nExp_<=0);
	}
	return getLv()-nLv;
}

__inline void CDataCharaBattle::calcMental(int nMental)
{
	nMental_+=nMental;
	if(nMental_>MENTAL_MAX) nMental_=MENTAL_MAX;
	ef(nMental_<MENTAL_MIN) nMental_=MENTAL_MIN;
}

__inline void CDataCharaBattle::calcStrength(int nStrength)
{ 
	fundOffset_.setStrength(fundOffset_.getStrength() + nStrength); 
	if(fundOffset_.getStrength() > CStatusFund::FUND_MAX - fund_.getStrength()) 
		fundOffset_.setStrength(CStatusFund::FUND_MAX - fund_.getStrength());
	ef(fundOffset_.getStrength() < 0)
		fundOffset_.setStrength(0);
}

__inline void CDataCharaBattle::calcMagic(int nMagic)
{ 
	fundOffset_.setMagic(fundOffset_.getMagic() + nMagic); 
	if(fundOffset_.getMagic() > CStatusFund::FUND_MAX - fund_.getMagic()) 
		fundOffset_.setMagic(CStatusFund::FUND_MAX - fund_.getMagic());
	ef(fundOffset_.getMagic() < 0)
		fundOffset_.setMagic(0);
}

__inline void CDataCharaBattle::calcHit(int nHit)
{ 
	fundOffset_.setHit(fundOffset_.getHit() + nHit);
	if(fundOffset_.getHit() > CStatusFund::FUND_MAX - fund_.getHit()) 
		fundOffset_.setHit(CStatusFund::FUND_MAX - fund_.getHit());
	ef(fundOffset_.getHit() < 0)
		fundOffset_.setHit(0);
}

__inline void CDataCharaBattle::calcAvoid(int nAvoid)
{ 
	fundOffset_.setAvoid(fundOffset_.getAvoid() + nAvoid);
	if(fundOffset_.getAvoid() > CStatusFund::FUND_MAX - fund_.getAvoid()) 
		fundOffset_.setAvoid(CStatusFund::FUND_MAX - fund_.getAvoid());
	ef(fundOffset_.getAvoid() < 0)
		fundOffset_.setAvoid(0);
}

__inline void CDataCharaBattle::calcDefence(int nDefence)
{ 
	fundOffset_.setDefence(fundOffset_.getDefence() + nDefence);
	if(fundOffset_.getDefence() > CStatusFund::FUND_MAX - fund_.getDefence()) 
		fundOffset_.setDefence(CStatusFund::FUND_MAX - fund_.getDefence());
	ef(fundOffset_.getDefence() < 0)
		fundOffset_.setDefence(0);
}

__inline void CDataCharaBattle::calcSkill(int nSkill)
{
	fundOffset_.setSkill(fundOffset_.getSkill() + nSkill);
	if(fundOffset_.getSkill() > CStatusFund::FUND_MAX - fund_.getSkill()) 
		fundOffset_.setSkill(CStatusFund::FUND_MAX - fund_.getSkill());
	ef(fundOffset_.getSkill() < 0)
		fundOffset_.setSkill(0);
}

__inline void CDataCharaBattle::calcSP(int nSP)
{
	fundOffset_.setSP(fundOffset_.getSP() + nSP);
	if(fundOffset_.getSP() > fund_.getSP()) fundOffset_.setSP(fund_.getSP());
	ef(fundOffset_.getSP() < 0)			    fundOffset_.setSP(0);
}

__inline void CDataCharaBattle::calcHP(int nHP)
{
	battleOffset_.setHP(battleOffset_.getHP() + nHP);
	if(battleOffset_.getHP() > battle_.getHP()) battleOffset_.setHP(battle_.getHP());
	if(battleOffset_.getHP() < 0)				battleOffset_.setHP(0);
}

__inline void CDataCharaBattle::calcEN(int nEN)
{
	battleOffset_.setEN(battleOffset_.getEN() + nEN);
	if(battleOffset_.getEN()>battle_.getEN()) battleOffset_.setEN(battle_.getEN());
	if(battleOffset_.getEN()<0)				  battleOffset_.setEN(0);
}

__inline void CDataCharaBattle::calcTough(int nTough)
{
	battleOffset_.setTough(battleOffset_.getTough() + nTough);
	if(getTough()<0) battleOffset_.setTough(battle_.getTough());
}

__inline void CDataCharaBattle::calcQuick(int nQuick)
{
	battleOffset_.setQuick(battleOffset_.getQuick() + nQuick);
	if(getQuick()<0) battleOffset_.setQuick(battle_.getQuick());
}

__inline void CDataCharaBattle::calcMove(int nMove)
{
	battleOffset_.setMove(battleOffset_.getMove() + nMove);
	if(getMove()<0) battleOffset_.setMove(battle_.getMove());	
}

__inline void CDataCharaBattle::calcJump(int nJump)
{
	battleOffset_.setJump(battleOffset_.getJump() + nJump);
	if(getJump()<0) battleOffset_.setJump(battle_.getJump());
}

} // namespace Chara end
} // namespace BMW end