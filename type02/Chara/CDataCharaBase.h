/*
	katze 05/03/02
	バトル用とインターミッション用のインターセクション
*/
#pragma once

#include "CStatusFund.h"
#include "CStatusBattle.h"
#include "CStatusAbility.h"
#include "CValidSkill.h"

namespace BMW{
namespace Chara{

struct sort_Item : public binary_function<const CStatusAbility&, const CStatusAbility&, bool>
{
	bool operator()(const CStatusAbility& first,const CStatusAbility& second)
	{
		return first.getAttr() < second.getAttr();
	}
};

class CDataCharaBase
{/**
	バトル用とインターミッション用キャラデータ共通部分

	バトルとインターミッションはこいつの差分を持てば良い。
	こうすることで、Init適用とGrowth適用が一本化できる。
	Trainだけは、それぞれで扱いが違うで別々に適用
 */
public:
	// コンストラクタ・デストラクタ
	CDataCharaBase():nID_(-1),nLv_(1),nExp_(EXP_MAX),nKill_(0),nChara_(1),nPena_(0),nMental_(MENTAL_NORMAL){}
	virtual ~CDataCharaBase(){}

	// 設定・取得
	int						getID() const { return nID_; }
	void					setID(int nID){ nID_=nID; }
	int						getFaceID() const { return nFaceID_;}
	void					setFaceID(int nFaceID){ nFaceID_=nFaceID; }
	const string&			getMapSymbolID(){ return sMapSymbolID_; }
	void					setMapSymbolID(const string& sMapSymbol){ sMapSymbolID_=sMapSymbol; }
	const string&			getDemoID() const { return sDemoID_;}
	void					setDemoID(const string& sDemoID){ sDemoID_=sDemoID; }
	const string&			getSymbolID() const { return sSymbolID_;}
	void					setSymbolID(const string& sSymbolID){ sSymbolID_=sSymbolID; }
	int						getBgmID() const { return nBgmID_; }
	void					setBgmID(int nID){ nBgmID_=nID; }
	
	int						getLv() const { return nLv_; }
	void					setLv(int nLv){ nLv_=nLv; }
	int						getExp() const { return nExp_; }
	void					setExp(int nExp){ nExp_=nExp; }
	int						getKill() const { return nKill_; }
	void					setKill(int nKill){ nKill_= ((KILL_MAX>nKill) ? nKill : KILL_MAX); }
	void					incKill(){ if(KILL_MAX>nKill_) ++nKill_; }
	int						getChara() const { return nChara_; }
	void					setChara(int nChara){ nChara_=nChara; }
	int						getGrowth() const { return nGrowth_; }
	void					setGrowth(int nGrowth){ nGrowth_=nGrowth; }
	int						getPena() const { return nPena_; }
	void					setPena(int nPena){ nPena_=nPena; }
	int						getFP() const { return nFP_; }
	void					setFP(int nFP){ nFP_=nFP; }
	virtual int				getMental() const { return nMental_; }
	void					setMental(int nMental){ nMental_=nMental; }

	const CStatusFund&		getFund() const { return fund_; }
	void					setFund(const CStatusFund& fund){ fund_=fund; }
	void					setFundSub(const CStatusFund& fund){ fund_.copySub(fund); }
	
	const CStatusBattle&	getBattle() const { return battle_; }
	void					setBattle(const CStatusBattle& battle){ battle_=battle; }
	void					setBattleSub(const CStatusBattle& battle){ battle_.copySub(battle); }

	const CStatusAbility&	getSpirit(int nSlot) const { return spirit_[nSlot]; }
	void					setSpirit(const CStatusAbility& spirit, int nSlot){ spirit_[nSlot]=spirit; }


	const CValidSkill&		getSkillValid() const { return skillValid_; }
	void					setSkillValid(const CValidSkill& skill){ skillValid_=skill; }

	int						getWeaponCost() const { return nWeaponCost_; }
	void					setWeaponCost(int nWeaponCost){ nWeaponCost_=nWeaponCost; }

	int						getItemMax() const { return nItemMax_; }
	void					setItemMax(int nItemMax){ nItemMax_=nItemMax; }

	// 操作
	// 基礎ステ
	virtual int				getStrength() const { return fund_.getStrength(); }
	void					setStrength(int nStrength)
							{ fund_.setStrength(nStrength>=CStatusFund::FUND_MAX ? CStatusFund::FUND_MAX : nStrength); }
	virtual int				getMagic() const { return fund_.getMagic(); }
	void					setMagic(int nMagic)
							{ fund_.setMagic(nMagic>=CStatusFund::FUND_MAX ? CStatusFund::FUND_MAX : nMagic); }
	virtual int				getHit() const { return fund_.getHit(); }
	void					setHit(int nHit)
							{ fund_.setHit(nHit>=CStatusFund::FUND_MAX ? CStatusFund::FUND_MAX : nHit); }
	virtual int				getAvoid() const { return fund_.getAvoid(); }
	void					setAvoid(int nAvoid)
							{ fund_.setAvoid(nAvoid>=CStatusFund::FUND_MAX ? CStatusFund::FUND_MAX : nAvoid); }
	virtual int				getDefence() const { return fund_.getDefence(); }
	void					setDefence(int nDefence)
							{ fund_.setDefence(nDefence>=CStatusFund::FUND_MAX ? CStatusFund::FUND_MAX : nDefence); }
	virtual int				getSkill() const { return fund_.getSkill(); }
	void					setSkill(int nSkill)
							{ fund_.setSkill(nSkill>=CStatusFund::FUND_MAX ? CStatusFund::FUND_MAX : nSkill); }
	virtual int				getSP() const { return fund_.getSP(); }
	void					setSP(int nSP){ fund_.setSP(nSP); }

	// 戦闘ステ
	virtual int				getHP() const { return battle_.getHP(); }
	void					setHP(int nHP){ battle_.setHP(nHP); }
	virtual int				getEN() const { return battle_.getEN(); }
	void					setEN(int nEN){ battle_.setEN(nEN); }
	virtual int				getTough() const { return battle_.getTough(); }
	void					setTough(int nTough){ battle_.setTough(nTough); }
	virtual int				getQuick() const { return battle_.getQuick(); }
	void					setQuick(int nQuick){ battle_.setQuick(nQuick); }
	virtual int				getMove() const { return battle_.getMove(); }
	void					setMove(int nMove){ battle_.setMove(nMove); }
	virtual int				getJump() const { return battle_.getJump(); }
	void					setJump(int nJump){ battle_.setJump(nJump); }

	// 固有リスト
	talent_list::iterator	beginTalent() const
							{ 
								CDataCharaBase* pBase = const_cast<CDataCharaBase*>(this);
								pBase->it_t=pBase->listTalent_.begin(); 
								return pBase->it_t;
							}
	bool					endTalent() const
							{
								CDataCharaBase* pBase = const_cast<CDataCharaBase*>(this);
								return pBase->it_t==pBase->listTalent_.end();
							}
	talent_list::iterator	nextTalent() const
							{ 
								CDataCharaBase* pBase = const_cast<CDataCharaBase*>(this);
								return pBase->it_t++;
							}
	void					addTalent(const CStatusAbility& talent){ listTalent_.push_back(talent); }
	void					clearTalent(){ listTalent_.clear(); }
	bool					IsTalent(int nID)
							{
								talent_list::iterator it;
								beginTalent();
								while(!endTalent())
								{
									it=nextTalent();
									if(it->getID()==nID) return true;
								}
								return false;
							}


	// 技能リスト
	// 技能
	int						IsSkillValid(int nID) const { return skillValid_.IsValid(nID); }
	skill_list::iterator	beginSkill() const
							{
								CDataCharaBase* pBase = const_cast<CDataCharaBase*>(this);
								pBase->it_s=pBase->listSkill_.begin(); 
								return pBase->it_s; 
							}
	bool					endSkill() const
							{
								CDataCharaBase* pBase = const_cast<CDataCharaBase*>(this);
								return pBase->it_s==pBase->listSkill_.end();
							}
	skill_list::iterator	nextSkill() const
							{
								CDataCharaBase* pBase = const_cast<CDataCharaBase*>(this);
								return pBase->it_s++; 
							}
	void					addSkill(const CStatusAbility& skill, bool bOver=true){ addSkill(skill.getID(),skill.getAttr(),bOver); }
	void					addSkill(int nID,int nAttr=0, bool bOver=true)
							{
								if(bOver)
								{// 同じIDがあったら属性を上書き
									skill_list::iterator it;
									beginSkill();
									while(!endSkill())
									{
										it=nextSkill();
										if(it->getID()==nID) 
										{
											it->setAttr(nAttr);
											return;
										}
									}
								}
								// なかったら追加
								CStatusAbility skill;
								skill.setID(nID);
								skill.setAttr(nAttr);
								listSkill_.push_back(skill); 
							}
	int						sizeSkill() const { return (int)listSkill_.size(); }
	void					clearSkill(){ listSkill_.clear(); }
	virtual int				hasSkill(int nID)const{ return -1; }

	// アイテム操作インターフェイス
	virtual item_list::iterator	beginItem() const=0;
	virtual bool				endItem() const=0;
	virtual item_list::iterator	nextItem() const=0;
	virtual void				addItem(const CStatusAbility& item)=0;
	virtual void				addItem(int nID,int nAttr=0)=0;
	virtual bool				delItem(int nAttr)=0;
	virtual int					sizeItem() const=0;
	virtual void				clearItem()=0;
	// 指定したAttrのアイテムのIDを返す
	virtual int					hasItemAttr(int nAttr)=0;
	// Attr順にソートする
	virtual void				sortItem() const=0;

	// 武器リスト
	weapon_list::iterator	beginWeapon() const 
							{ 
								CDataCharaBase* pBase = const_cast<CDataCharaBase*>(this);
								pBase->it_w=pBase->listWeapon_.begin();
								return pBase->it_w;
							}
	bool					endWeapon() const
							{
								CDataCharaBase* pBase = const_cast<CDataCharaBase*>(this);
								return pBase->it_w==pBase->listWeapon_.end();
							}
	weapon_list::iterator	nextWeapon()const
							{
								CDataCharaBase* pBase = const_cast<CDataCharaBase*>(this);
								return pBase->it_w++; 
							}
	void					addWeapon(int nWeapon){ listWeapon_.push_back(nWeapon); }
	int						sizeWeapon() const { return (int)listWeapon_.size(); }
	void					clearWeapon(){ listWeapon_.clear(); }


protected:
	int		nID_;			// キャラID
	
	int		nFaceID_;		// 顔ID
	string	sMapSymbolID_;	// マップシンボルファイル名
	string	sDemoID_;		// デモ定義ファイル名
	string	sSymbolID_;		// シンボル定義ファイル名
	int		nBgmID_;		// BGM ID

	int		nLv_;		// 現在Lv
	int		nExp_;		// LvUPまでの残りExp
	int		nKill_;		// 撃墜数
	int		nChara_;	// 性格
	int		nGrowth_;	// 成長タイプ
	int		nPena_;		// ペナルティ
	int		nFP_;		// 保持FP（敵のみ有効）
	int		nMental_;	// 気力

	// 基礎ステ
	CStatusFund		fund_;
	// 戦闘ステ
	CStatusBattle	battle_;
	// 精神
	CStatusAbility	spirit_[6];
	// 固有
	talent_list		listTalent_;
	talent_list::iterator it_t;
	// 技能
	skill_list		listSkill_;
	skill_list::iterator it_s;
	CValidSkill		skillValid_;		// 覚えられる技能とられない技能の区別
	// アイテム
	//item_list		listItem_;
	//item_list::iterator it_i;
	int				nItemMax_;
	// 武器
	weapon_list		listWeapon_;
	weapon_list::iterator it_w;
	int				nWeaponCost_;
};

} // namespace Chara end
} // namespace BMW end