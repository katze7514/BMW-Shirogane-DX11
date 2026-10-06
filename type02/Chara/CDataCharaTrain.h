/*
	katze 05/02/28
	キャラの養成データ
*/
#pragma once

#include "CStatusBattle.h"
#include "CStatusBattle.h"
#include "CStatusAbility.h"

namespace BMW{

namespace Weapon{
class CDataWeaponBattle;
} // namespace Weapon end

namespace Chara{

class CDataCharaBase;
class CDataCharaBattle;
class CDataCharaInter;

class CDataCharaTrain : public IArchive
{/**
	キャラの養成(変化)データを扱う
	つまり、セーブ対象
 */
public:
	// コンストラクタ
	CDataCharaTrain():nID_(-1),nLv_(1),nExp_(EXP_MAX),nKill_(0),nPenalty_(-1),nFP_(-1),nWeapon_(0){}

	// セーブ対象なのでシリアライズを実装する
	void Serialize(ISerialize& s);

	// 設定・取得
	int						getID() const { return nID_; }
	void					setID(int nID){ nID_=nID; }
	int						getLv() const { return nLv_; }
	void					setLv(int nLv){ nLv_=nLv; }
	int						getExp() const { return nExp_; }
	void					setExp(int nExp){ nExp_=nExp; }
	int						getKill() const { return nKill_; }
	void					setKill(int nKill){ nKill_=nKill; }
	int						getPenalty() const { return nPenalty_; }
	void					setPenalty(int nPenalty){ nPenalty_=nPenalty; }
	int						getFP() const { return nFP_; }
	void					setFP(int nFP){ nFP_=nFP; }

	const CStatusFund&		getFund() const { return fund_; }
	void					setFund(const CStatusFund& fund){ fund_=fund; }
	void					clearFund(){ return fund_.reset(); }

	const CStatusBattle&	getBattle() const { return battle_; }
	void					setBattle(const CStatusBattle& battle){ battle_=battle; }
	void					clearBattle(){ return battle_.reset(); }

	int						getWeapon() const { return nWeapon_; }
	void					setWeapon(int nWeapon){ nWeapon_=nWeapon; }
	void					clearWeapon(){ nWeapon_=0; }

	// 操作
	// 基礎ステ
	int						getStrength() const { return fund_.getStrength(); }
	void					setStrength(int nStrength){ fund_.setStrength(nStrength); }
	int						getMagic() const { return fund_.getMagic(); }
	void					setMagic(int nMagic){ fund_.setMagic(nMagic); }
	int						getHit() const { return fund_.getHit(); }
	void					setHit(int nHit){ fund_.setHit(nHit); }
	int						getAvoid() const { return fund_.getAvoid(); }
	void					setAvoid(int nAvoid){ fund_.setAvoid(nAvoid); }
	int						getDefence() const { return fund_.getDefence(); }
	void					setDefence(int nDefence){ fund_.setDefence(nDefence); }
	int						getSkill() const { return fund_.getSkill(); }
	void					setSkill(int nSkill){ fund_.setSkill(nSkill); }
	int						getSP() const { return fund_.getSP(); }
	void					setSP(int nSP){ fund_.setSP(nSP); }

	// 戦闘ステ
	int						getHP() const { return battle_.getHP(); }
	void					setHP(int nHP){ battle_.setHP(nHP); }
	int						getEN() const { return battle_.getEN(); }
	void					setEN(int nEN){ battle_.setEN(nEN); }
	int						getTough() const { return battle_.getTough(); }
	void					setTough(int nTough){ battle_.setTough(nTough); }
	int						getQuick() const { return battle_.getQuick(); }
	void					setQuick(int nQuick){ battle_.setQuick(nQuick); }
	int						getMove() const { return battle_.getMove(); }
	void					setMove(int nMove){ battle_.setMove(nMove); }
	int						getJump() const { return battle_.getJump(); }
	void					setJump(int nJump){ battle_.setJump(nJump); }

	// 技能リスト
	skill_list&				getSkillList(){ return listSkill_; }
	const skill_list&		getSkillList()const{ return listSkill_; }
	skill_list::iterator	beginSkill() const 
							{
								CDataCharaTrain* pTrain = const_cast<CDataCharaTrain*>(this);
								pTrain->it_s=pTrain->listSkill_.begin();
								return pTrain->it_s; 
							}
	bool					endSkill() const
							{
								CDataCharaTrain* pTrain = const_cast<CDataCharaTrain*>(this);
								return pTrain->it_s==pTrain->listSkill_.end();
							}
	skill_list::iterator	nextSkill() const 
							{
								CDataCharaTrain* pTrain = const_cast<CDataCharaTrain*>(this);
								return pTrain->it_s++;
							}
	void					addSkillAbi(const CStatusAbility& skill){ listSkill_.push_back(skill); }
	void					addSkill(const CStatusAbility& skill){ listSkill_.push_back(skill); }
	void					addSkill(int nID,int nAttr=0)
							{	
								CStatusAbility skill; 
								skill.setID(nID);
								skill.setAttr(nAttr);
								listSkill_.push_back(skill); 
							}
	bool					delSkill(int nID)
							{// IDをキーにしてスキルを削除する
								skill_list::iterator it;
								for(it=listSkill_.begin(); it!=listSkill_.end(); ++it)
									if(it->getID()==nID)
									{
										listSkill_.erase(it);
										break;
									}
							}
	void					clearSkill(){ listSkill_.clear(); }

	// アイテムリスト
	item_list&				getItemList(){ return listItem_; }
	const item_list&		getItemList()const{ return listItem_; }
	item_list::iterator		beginItem() const 
							{ 
								CDataCharaTrain* pTrain = const_cast<CDataCharaTrain*>(this);
								pTrain->it_i=pTrain->listItem_.begin();
								return pTrain->it_i; 
							}
	bool					endItem() const
							{ 
								CDataCharaTrain* pTrain = const_cast<CDataCharaTrain*>(this);
								return pTrain->it_i==pTrain->listItem_.end();
							}
	item_list::iterator		nextItem() const
							{
								CDataCharaTrain* pTrain = const_cast<CDataCharaTrain*>(this);
								return pTrain->it_i++; 
							}
	void					addItemAbi(const CStatusAbility& item){ listItem_.push_back(item); }
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
								for(it_i=listItem_.begin(); it_i!=listItem_.end(); ++it_i)
									if(it_i->getAttr()==nAttr)
									{
										it_i->setAttr(-1);
										listItem_.erase(it_i);
										return true;
									}

								return false;
							}
	void					clearItem(){ listItem_.clear(); }

	// 固有技能
	skill_list&				getTalentList(){ return listTalent_; }
	void					addTalent(const CStatusAbility& talent){ listTalent_.push_back(talent); }
	void					clearTalent(){ listTalent_.clear(); }

	// データ適用
	void					apply(CDataCharaBattle* battle);
	void					apply(CDataCharaInter* Inter);

	void					applySkill(CDataCharaBattle* battle);
	
	// データ戻し
	// Lv・Exp・撃墜数などをフィードバックする
	void					back(const CDataCharaBattle* battle);
	void					back(const CDataCharaInter* inter);

	// コンテニュー時のデータ上書き
	void					over(CDataCharaBattle& battle);

	// クリア
	void					clear();

	// 養成段階一括いじり
	void					calcTrainAll(int nTrain)
	{
		// 武器
		nWeapon_ += nTrain;
		if(nWeapon_>10) nWeapon_=10;

		// 戦闘養成
		battle_.setHP(battle_.getHP()+nTrain);
		if(battle_.getHP()>10) battle_.setHP(10);

		battle_.setEN(battle_.getEN()+nTrain);
		if(battle_.getEN()>10) battle_.setEN(10);

		battle_.setQuick(battle_.getQuick()+nTrain);
		if(battle_.getQuick()>10) battle_.setQuick(10);

		battle_.setTough(battle_.getTough()+nTrain);
		if(battle_.getTough()>10) battle_.setTough(10);
	}

private:
	// キャラID
	int nID_;
	// LV
	int nLv_;
	// 残りEXP
	int nExp_;
	// 撃墜数
	int nKill_;
	// Penalty（敵でのみ使用）
	int nPenalty_;
	// FP（敵でのみ使用）
	int nFP_;

	// 基礎能力養成
	CStatusFund		fund_;
	// 戦闘能力養成段階
	CStatusBattle	battle_;
	// 武器養成段階
	int				nWeapon_;

	// 取得技能リスト
	skill_list		listSkill_;
	skill_list::iterator it_s;
	// 保持アイテムリスト
	item_list		listItem_;
	item_list::iterator it_i;
	// SLGパートで敵のみ使用
	// 取得固有技能リスト
	skill_list		listTalent_;

	// 内部利用
	void applyBase(CDataCharaBase* base);
};

} // namespace Chara end
} // namespace BMW end