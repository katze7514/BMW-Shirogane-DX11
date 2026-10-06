/*
	katze 05/03/01
	インターミッション用キャラデータクラス Ver.2
	内部的にTrainデータを持って、直接作用する
*/
#pragma once

#include "ConstChara.h"
#include "CDataCharaBase.h"

namespace BMW{
namespace Chara{

class CDataCharaTrain;

struct CStatusItem
{// アイテムによる補正値構造体
	CStatusBattle battle_;
	int nSP_;
	int nMental_;

	CStatusItem():nSP_(0),nMental_(0){}
};

class CDataCharaInter : public CDataCharaBase
{/**
	インターミッション用キャラデータクラス

	構成方法は、基本的にBattleと同じだが、微妙に違う
	養成やアイテム装備を受け入れるための構造を持つ
 */
public:
	// コンストラクタ
//	CDataCharaInter():nWeaponTrain_(0){}

	// 設定・取得
	smart_ptr<CDataCharaTrain>& getTrainData(){ return pTrain_; }
	void						setTrainData(const smart_ptr<CDataCharaTrain>& pTrain){ pTrain_=pTrain; }

	const CStatusFund&		getFundTrain() const { return pTrain_->getFund(); }
	void					setFundTrain(const CStatusFund& fundTrain){ pTrain_->setFund(fundTrain); }

	const CStatusBattle&	getBattleTrain() const { return pTrain_->getBattle(); }
	void					setBattleTrain(const CStatusBattle& battleTrain){ pTrain_->setBattle(battleTrain); }

	smart_ptr<CStatusItem>&		getItemOffset(){ return pItem_; }
	void						setItemOffset(const smart_ptr<CStatusItem>& pItem){ pItem_=pItem; }

	const CStatusBattle&	getBattleItem() const { return pItem_->battle_; }
	void					setBattleItem(const CStatusBattle& battleItem){ pItem_->battle_=battleItem; }

	int						getWeaponTrain() const { return pTrain_->getWeapon(); }
	void					setWeaponTrain(int nWeaponTrain){ pTrain_->setWeapon(nWeaponTrain); }
	void					calcWeaponTrain(int nWeaponTrain){ pTrain_->setWeapon(getWeaponTrain()+nWeaponTrain); }

	// 操作
	// 基礎ステ
	int						getSP()const{ return fund_.getSP() + pItem_->nSP_; }
	int						getMaxSP()const{ return fund_.getSP(); }
	void					setItemSP(int nSP){ pItem_->nSP_=nSP; }
	void					calcItemSP(int nSP){ pItem_->nSP_+=nSP; }

	int						getMental()const{ return nMental_ + pItem_->nMental_; }
	void					setItemMental(int nMental){ pItem_->nMental_=nMental; }
	void					calcItemMental(int nMental){ pItem_->nMental_+=nMental; }

	int						getStrength() const { return fund_.getStrength() + getFundTrain().getStrength()<400
														 ? fund_.getStrength() + getFundTrain().getStrength()
														 : 400; }
	int						getSourceStrength()const{ return fund_.getStrength(); }
	void					calcStrength(int nStr){ pTrain_->setStrength(getFundTrain().getStrength()+nStr); }
	int						getMagic() const { return fund_.getMagic() + getFundTrain().getMagic()<400
													  ? fund_.getMagic() + getFundTrain().getMagic()
													  : 400; }
	int						getSourceMagic()const{ return fund_.getMagic(); }
	void					calcMagic(int nMag){ pTrain_->setMagic(getFundTrain().getMagic()+nMag); }
	int						getHit() const { return fund_.getHit() + getFundTrain().getHit()<400
													? fund_.getHit() + getFundTrain().getHit()
													: 400; }
	int						getSourceHit()const{ return fund_.getHit(); }
	void					calcHit(int nHit){ pTrain_->setHit(getFundTrain().getHit()+nHit); }
	int						getAvoid() const { return fund_.getAvoid() + getFundTrain().getAvoid()<400
													  ? fund_.getAvoid() + getFundTrain().getAvoid()
													  : 400; }
	int						getSourceAvoid()const{ return fund_.getAvoid(); }
	void					calcAvoid(int nAvoid){ pTrain_->setAvoid(getFundTrain().getAvoid()+nAvoid); }
	int						getDefence() const { return fund_.getDefence() + getFundTrain().getDefence()<400
														? fund_.getDefence() + getFundTrain().getDefence()
														: 400; }
	int						getSourceDefence()const{ return fund_.getDefence(); }
	void					calcDefence(int nDefence){ pTrain_->setDefence(getFundTrain().getDefence()+nDefence); }
	int						getSkill() const { return fund_.getSkill() + getFundTrain().getSkill()
													  ? fund_.getSkill() + getFundTrain().getSkill()
													  : 400; }
	int						getSourceSkill()const{ return fund_.getSkill(); }
	void					calcSkill(int nSkill){ pTrain_->setSkill(getFundTrain().getSkill()+nSkill); }

	// 戦闘ステ
	int						getHP() const { return battle_.getHP() + calcPer(battle_.getHP(), Const::HP_TRAINING_VALUE, getBattleTrain().getHP()) + pItem_->battle_.getHP(); }
	int						getSourceHP()const{ return battle_.getHP(); }
	void					setTrainHP(int nTrain){ pTrain_->setHP(nTrain); }
	int						getItemHP()const{ return pItem_->battle_.getHP(); }
	void					setItemHP(int nItem){ pItem_->battle_.setHP(nItem); }
	void					calcHP(int nHP){ pTrain_->setHP(getBattleTrain().getHP()+nHP); }

	int						getEN() const { return battle_.getEN() + calcPer(battle_.getEN(), Const::EN_TRAINING_VALUE, getBattleTrain().getEN()) + pItem_->battle_.getEN(); }
	int						getSourceEN()const{ return battle_.getEN(); }
	void					setTrainEN(int nTrain){ pTrain_->setEN(nTrain); }
	int						getItemEN()const{ return pItem_->battle_.getEN(); }
	void					setItemEN(int nItem){ pItem_->battle_.setEN(nItem); }
	void					calcEN(int nEN){ pTrain_->setEN(getBattleTrain().getEN()+nEN); }

	int						getTough() const { return battle_.getTough() + calcPer(battle_.getTough(), Const::TOUGH_TRAINING_VALUE, getBattleTrain().getTough()) + pItem_->battle_.getTough(); }
	int						getSourceTough()const{ return battle_.getTough(); }
	void					setTrainTough(int nTrain){ pTrain_->setTough(nTrain); }
	int						getItemTough()const{ return pItem_->battle_.getTough(); }
	void					setItemTough(int nItem){ pItem_->battle_.setTough(nItem); }
	void					calcTough(int nTough){ pTrain_->setTough(getBattleTrain().getTough()+nTough); }

	int						getQuick() const { return battle_.getQuick() + calcPer(battle_.getQuick(), Const::QUICK_TRAINING_VALUE, getBattleTrain().getQuick()) + pItem_->battle_.getQuick(); }
	int						getSourceQuick()const{ return battle_.getQuick(); }
	void					setTrainQuick(int nTrain){ pTrain_->setQuick(nTrain); }
	int						getItemQuick()const{ return pItem_->battle_.getQuick(); }
	void					setItemQuick(int nItem){ pItem_->battle_.setQuick(nItem); }
	void					calcQuick(int nQuick){ pTrain_->setQuick(getBattleTrain().getQuick()+nQuick); }

	int						getMove()const{ return battle_.getMove() + pItem_->battle_.getMove(); }
	int						getSourceMove()const{ return battle_.getMove(); }
	int						getItemMove()const{ return pItem_->battle_.getMove(); }
	void					setItemMove(int nItem){ pItem_->battle_.setMove(nItem); }
	int						getJump()const{ return battle_.getJump() + pItem_->battle_.getJump(); }
	int						getSourceJump()const{ return battle_.getJump(); }
	int						getItemJump()const{ return pItem_->battle_.getJump(); }
	void					setItemJump(int nItem){ pItem_->battle_.setJump(nItem); }

	// 技能リスト
	int						hasSkill(int nID)const
	{ // 先天と後天を足し合わせる
		int nAttr=-1;
		skill_list::iterator it;
		beginSkill();
		while(!endSkill())
		{
			it=nextSkill();
			if(it->getID()==nID)
			{
				nAttr=it->getAttr();
				break;
			}
		}

		beginSkillAcqu();
		while(!endSkillAcqu())
		{
			it=nextSkillAcqu();
			if(it->getID()==nID)
			{// 後天技能の分を足し合わせたりしなかったり
				nAttr<0 ? nAttr=it->getAttr() : nAttr+=it->getAttr();
				break;
			}
		}

		return nAttr;
	}
	// 後天技能
	skill_list::iterator	beginSkillAcqu() const 
							{ 
								return pTrain_->beginSkill(); 
							}
	bool					endSkillAcqu() const
							{ 
								return pTrain_->endSkill();
							}
	skill_list::iterator	nextSkillAcqu() const
							{
								return pTrain_->nextSkill(); 
							}
	void					addSkillAcqu(const CStatusAbility& skill){ addSkillAcqu(skill.getID(),skill.getAttr()); }
	void					addSkillAcqu(int nID,int nAttr=0)
							{	// 同じIDがあったら属性を上書き
								skill_list::iterator it;
								beginSkillAcqu();
								while(!endSkillAcqu())
								{
									it=nextSkillAcqu();
									if(it->getID()==nID) 
									{
										it->setAttr(nAttr);
										return;
									}
								}
								// なかったら追加
								pTrain_->addSkill(nID,nAttr);
							}
	void					addSkillAcqu(int nID1, int nID2, int nAttr)
							{// nID1の技能に、それ以降の内容を上書きする
								skill_list::iterator it;
								beginSkillAcqu();
								while(!endSkillAcqu())
								{
									it=nextSkillAcqu();
									if(it->getID()==nID1) 
									{
										it->setID(nID2);
										it->setAttr(nAttr);
										return;
									}
								}
							}
	bool					delSkillAcqu(int nID){ return pTrain_->delSkill(nID); }
	int						sizeSkillAcqu(){ return (int)pTrain_->getSkillList().size(); }
	int						hasSkillAcqu(int nID)const
	{ // 後天のレベルを取得
		skill_list::iterator it;
		beginSkillAcqu();
		while(!endSkillAcqu())
		{
			it=nextSkillAcqu();
			if(it->getID()==nID)
			{// 後天技能のAttrを返す
				return it->getAttr();
			}
		}

		return -1;
	}

	// アイテム
	item_list::iterator		beginItem() const
							{
								return pTrain_->beginItem();
							}
	bool					endItem() const
							{ 
								return pTrain_->endItem();
							}
	item_list::iterator		nextItem() const
							{ 
								return pTrain_->nextItem();
							}
	void					addItem(const CStatusAbility& item){ pTrain_->addItem(item); }
	void					addItem(int nID,int nAttr=0){ pTrain_->addItem(nID,nAttr); }
									
	bool					delItem(int nAttr){	return pTrain_->delItem(nAttr);	}

	int						sizeItem() const { return (int)pTrain_->getItemList().size(); }
	void					clearItem(){ pTrain_->getItemList().clear(); }
	// 指定したAttrのアイテムのIDを返す
	int						hasItemAttr(int nAttr)
							{
								item_list& listItem_ = pTrain_->getItemList();
								item_list::iterator it;
								for(it=listItem_.begin(); it!=listItem_.end(); ++it)
								{
									if(it->getAttr()==nAttr)
										return it->getID();
								}

								return -1;
							}
	// Attr順にソートする
	void					sortItem() const { pTrain_->getItemList().sort(sort_Item()); }

	void					addItem(int nID,int nAttr, Item::CItemDB& db);
	bool					delItem(int nID, Item::CItemDB& db);
	bool					delItemID(int nID, Item::CItemDB& db);
							
private:
	// こいつが変更する養成データ
	smart_ptr<CDataCharaTrain> pTrain_;
	// 基礎ステ
	//CStatusFund		fundTrain_;
	// 戦闘ステ
	//CStatusBattle	battleTrain_;
	// アイテムによる補正値
	smart_ptr<CStatusItem>	pItem_;
	// 技能
	//skill_list		listSkillAcqu_;		// 後天技能 
	//skill_list::iterator it_sa;
	// 武器
	//int				nWeaponTrain_;		// 武器養成段階
};

} // namespace Chara end
} // namespace BMW end