/*
	katze 05/02/28
	update 06/01/18
	キャラの基本データを扱うクラス
*/
#pragma once

#include "CStatusFund.h"
#include "CStatusBattle.h"
#include "CStatusAbility.h"

namespace BMW{
namespace Chara{

class CDataCharaBase;

class CDataCharaInit
{/**
	キャラの基本データ（初期値）を表現する
	つまり、CharaStatusDBの要素となる
 */
public:
	// コンストラクタ
	CDataCharaInit():nID_(-1),nFaceID_(-1),nBgmID_(-1),nChara_(0),nPena_(0),nFP_(0),nGrowth_(-1),nWeaponCost_(-1),nItemMax_(0){}

	// 設定・取得
	int						getID() const { return nID_;}
	void					setID(int nID){ nID_=nID; }

	int						getFaceID() const { return nFaceID_;}
	void					setFaceID(int nFaceID){ nFaceID_=nFaceID; }
	const string&			getMapSymbolID()const{ return sMapSymbolID_; }
	void					setMapSymbolID(const string& sMapSymbol){ sMapSymbolID_=sMapSymbol; }
	const string&			getDemoID() const { return sDemoID_;}
	void					setDemoID(const string& sDemoID){ sDemoID_=sDemoID; }
	const string&			getSymbolID() const { return sSymbolID_;}
	void					setSymbolID(const string& sSymbolID){ sSymbolID_=sSymbolID; }
	int						getBgmID() const { return nBgmID_; }
	void					setBgmID(int nID){ nBgmID_=nID; }
	
	int						getChara() const { return nChara_;}
	void					setChara(int nChara){ nChara_=nChara; }
	int						getGrowth() const { return nGrowth_; }
	void					setGrowth(int nGrowth){ nGrowth_=nGrowth; }
	int						getPena() const { return nPena_; }
	void					setPena(int nPena){ nPena_=nPena; }
	int						getFP() const { return nFP_; }
	void					setFP(int nFP){ nFP_=nFP; }

	const CStatusFund&		getFund() const { return fund_; }
	void					setFund(const CStatusFund& fund) { fund_=fund; }

	const CStatusBattle&	getBattle() const { return battle_; }
	void					setBattle(const CStatusBattle& battle) { battle_=battle; }

	//const CValidSkill&		getSkillValid() const { return skillValid_; }
	//void						setSkillValid(const CValidSkill& skill){ skillValid_=skill; }

	int						getWeaponCost() const { return nWeaponCost_; }
	void					setWeaponCost(int nWeaponCost){ nWeaponCost_=nWeaponCost; }

	int						getItemMax() const { return nItemMax_; }
	void					setItemMax(int nItem){ nItemMax_=nItem; }

	// 操作
	// 基礎ステ
	int						getStrength() const { return fund_.getStrength(); }
	int						getMagic() const { return fund_.getMagic(); }
	int						getHit() const { return fund_.getHit(); }
	int						getAvoid() const { return fund_.getAvoid(); }
	int						getDefence() const { return fund_.getDefence(); }
	int						getSkill() const { return fund_.getSkill(); }
	int						getSP() const { return fund_.getSP(); }

	// 戦闘ステ
	int						getHP() const { return battle_.getHP(); }
	int						getEN() const { return battle_.getEN(); }
	int						getTough() const { return battle_.getTough(); }
	int						getQuick() const { return battle_.getQuick(); }
	int						getMove() const { return battle_.getMove(); }
	int						getJump() const { return battle_.getJump(); }

	// 武器リスト
	weapon_list::iterator	beginWeapon() const 
							{ 
								CDataCharaInit* pInit = const_cast<CDataCharaInit*>(this);
								pInit->it_w=pInit->listWeapon_.begin();
								return pInit->it_w;
							}
	bool					endWeapon() const
							{
								CDataCharaInit* pInit = const_cast<CDataCharaInit*>(this);
								return pInit->it_w==pInit->listWeapon_.end();
							}
	weapon_list::iterator	nextWeapon()
							{
								CDataCharaInit* pInit = const_cast<CDataCharaInit*>(this);
								return pInit->it_w++; 
							}
	void					addWeapon(int nWeapon){ listWeapon_.push_back(nWeapon); }

	// 固有リスト
	talent_list::iterator	beginTalent() const
							{
								CDataCharaInit* pInit = const_cast<CDataCharaInit*>(this);
								pInit->it_t=pInit->listTalent_.begin();
								return pInit->it_t; 
							}
	bool					endTalent() const
							{
								CDataCharaInit* pInit = const_cast<CDataCharaInit*>(this);
								return pInit->it_t==pInit->listTalent_.end();
							}
	talent_list::iterator	nextTalent() const
							{ 
								CDataCharaInit* pInit = const_cast<CDataCharaInit*>(this);
								return pInit->it_t++; 
							}
	void					addTalent(const CStatusAbility& talent){ listTalent_.push_back(talent); }

	// データ適用
	void					apply(CDataCharaBase* base,bool bCont=false);
	void					applySub(CDataCharaBase* base,bool bCont=false);

private:
	// キャラID
	int				nID_;
	// 顔ID
	int				nFaceID_;
	// マップシンボルID
	string			sMapSymbolID_;
	// 防御時デモID
	string			sDemoID_;
	// 防御時シンボルID
	string			sSymbolID_;
	// BGM ID
	int				nBgmID_;
	
	// 性格
	int				nChara_;
	// ペナルティ
	int				nPena_;
	// 保持FP（敵のみ値が有効）
	int				nFP_;

	// 成長タイプ
	int				nGrowth_;
	// 初期基礎ステータス
	CStatusFund		fund_;
	// 初期戦闘ステータス
	CStatusBattle	battle_;
	// 保持固有能力
	talent_list		listTalent_;
	talent_list::iterator it_t;
	// スキル獲得フラグ
	//CValidSkill		skillValid_;
	// 初期保持武器
	weapon_list		listWeapon_;
	weapon_list::iterator it_w;
	int				nWeaponCost_;	// 成長コスト
	// 最大アイテム保持数
	int				nItemMax_;
};

} // namespace Chara end
} // namespace BMW end