/*
	katze 05/06/16
	インターミッションコンテキスト
*/
#pragma once

#include "IDInter.h"

namespace BMW{

namespace Chara{
class CDataCharaInter;
} // namespace Chara end

namespace Inter{
class CInterChara;
class CDataItemInter;

class CInterContext : public Task::CTaskContext
{/**
	インターミッションコンテキスト
 */
public:
	typedef map<int, CInterChara*>		chara_map;
	typedef map<int, CDataItemInter*>	item_map;
	// コンストラクタ・デストラクタ
	CInterContext();
	~CInterContext();

	// フラグ
	void					clearFlag();
	bool					IsInit(){ return mapValue_[Flag::INITIALIZE]; }
	void					init(bool bInit){ mapValue_[Flag::INITIALIZE] = (bInit ? 1 : 0); }
	bool					IsLoad(){ return mapValue_[Flag::LOAD]; }
	void					load(bool bLoad){ mapValue_[Flag::LOAD] = (bLoad ? 1 : 0); }

	// キャラリスト
	list<int>&				getCharaList(){ return listChara_; }
	list<int>::iterator		beginChara(){ it=listChara_.begin(); return it; }
	bool					endChara(){ return it==listChara_.end(); }
	void					backLoopChara(){ if(it==listChara_.begin()){ it=listChara_.end(); } --it;}
	void					nextLoopChara(){ if(++it==listChara_.end()){ it=listChara_.begin(); } }
	list<int>::iterator		nextChara(){ return it++; }
	list<int>::iterator		getCurrentChara(){ return it; }
	void					setCurrentChara(list<int>::iterator it_){ it=it_; }
	// イテレータをターゲットキャラのにする
	void					setChara();
	// キャラをソートする
	void					sortChara(int nKey, int nOrder);

	// キャラデータ
	int						getTargetChara(){ return mapValue_[Flag::TARGET_CHARA]; }
	void					setTargetChara(int nID){ mapValue_[Flag::TARGET_CHARA]=nID; }
	CInterChara*			getTargetCharaData(){ return mapChara_[mapValue_[Flag::TARGET_CHARA]]; }
	int						getExchangeChara(){ return mapValue_[Flag::EXCHANGE_CHARA]; }
	void					setExchangeChara(int nID){ mapValue_[Flag::EXCHANGE_CHARA]=nID; }
	CInterChara*			getExchangeCharaData(){ return mapChara_[mapValue_[Flag::EXCHANGE_CHARA]]; }

	CInterChara*			getCharaData(int nID){ return mapChara_[nID]; }
	void					setCharaData(int nID, CInterChara* pData);
	void					clearCharaData();

	// アイテムデータ
	int						getCtrlItem(){ return mapValue_[Flag::CTRL_ITEM]; }
	void					setCtrlItem(int nID){ mapValue_[Flag::CTRL_ITEM]=nID; }
	int						getTargetItem(){ return mapValue_[Flag::TARGET_ITEM]; }
	void					setTargetItem(int nID){ mapValue_[Flag::TARGET_ITEM]=nID; }
	int						getTargetItemID(){ return mapValue_[Flag::TARGET_ITEM_ID]; }
	void					setTargetItemID(int nID){ mapValue_[Flag::TARGET_ITEM_ID]=nID; }
	CDataItemInter*			getTargetItemData(){ return getItemData(getTargetItemID()); }
	int						getExchangeItemID()const{ return item_.getID(); }
	void					setExchangeItemID(int nID){ item_.setID(nID); }
	CDataItemInter*			getExchangeItemData(){ return getItemData(getExchangeItemID()); }
	int						getExchangeItemAttr()const{ return item_.getAttr(); }
	void					setExchangeItemAttr(int nAttr){ item_.setAttr(nAttr); }

	item_map&				getItemMap(){ return mapItem_; }
	CDataItemInter*			getItemData(int nID);
	void					setItemData(int nID, CDataItemInter* pData);
	void					clearItemData();

	// 技能
	int						getTargetAbility(){ return mapValue_[Flag::TARGET_ABILITY]; }
	void					setTargetAbility(int nAbility){ mapValue_[Flag::TARGET_ABILITY]=nAbility; }
	int						getCtrlAbility(){ return mapValue_[Flag::CTRL_ABILITY]; }
	void					setCtrlAbility(int nAbility){ mapValue_[Flag::CTRL_ABILITY]=nAbility; }

private:
	// キャラリスト
	list<int> listChara_;
	list<int>::iterator it;

	// キャラデータ
	chara_map	mapChara_;
	// アイテムデータ
	item_map	mapItem_;
	// 交換対象のアイテムデータ
	// IDが対象のアイテム
	// Attrが交換される時の装備側のAttr
	BMW::Chara::CStatusAbility	item_;
};

} // namespace Inter end
} // namespace BMW end