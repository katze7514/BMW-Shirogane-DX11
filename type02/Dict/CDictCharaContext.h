/*
	katze 08/08/18
	キャラ辞典シーンコンテキスト
*/
#pragma once

#include "../Movie/DB/CSymbolDB.h"
#include "../SLG/Context/CDataCharaSLG.h"

namespace BMW{
namespace Dict{

class CDictCharaItem
{/*
	CharaViewで表示する一つのアイテムデータ
 */
public:
	// コンストラクタ
	CDictCharaItem():nNo_(-1){}

	// アクセッサ
	int				getNo()const{ return nNo_; }
	void			setNo(int nNo){ nNo_=nNo; }
	const string&	getCharaID()const{ return sCharaID_; }
	void			setCharaID(const string& sCharaID);
	const string&	getName()const{ return sName_; }
	void			setName(const string& sName){ sName_=sName; }
	const string&	getRuby()const{ return sRuby_; }
	void			setRuby(const string& sRuby){ sRuby_=sRuby; }
	const string&	getOrigin()const{ return sOrigin_; }
	void			setOrigin(const string& sOrigin){ sOrigin_=sOrigin; }
	const string&	getNcv()const{ return sNcv_; }
	void			setNcv(const string& sNcv){ sNcv_=sNcv; }

	const string&	getProfile()const{ return sProfile_; }
	void			setProfile(const string& sProfile){ sProfile_=sProfile; }
	const string&	getIntro()const{ return sIntro_; }
	void			setIntro(const string& sIntro){ sIntro_=sIntro; }
	const string&	getComment()const{ return sComment_; }
	void			setComment(const string& sComment){ sComment_=sComment; }

	int				getID()const{ return nID_; }
	void			setID(int nID){ nID_=nID; }

	SLG::CDataCharaSLG&		getCharaData(){ return data_; }
	Movie::CSymbolDB&		getChipDB(){ return chipDB_; }
	Movie::CSymbolDB&		getSymbolDB(){ return symbolDB_; }

private:
	int nNo_;			// ソートID
	string sCharaID_;	// キャラID
	string sName_;		// 名前
	string sRuby_;		// ふりがな
	string sOrigin_;	// 原作
	string sNcv_;		// 脳内声優
	
	string sProfile_;	// 原作設定
	string sIntro_;	// BMW的設定
	string sComment_;	// のうがき

	int nID_; // 表示ID

	SLG::CDataCharaSLG	data_; // キャラデータ
	Movie::CSymbolDB	chipDB_; // キャラチップを生成するDB
	Movie::CSymbolDB	symbolDB_; // キャラシンボルを生成するDB
};


class CDictCharaContext : public Task::CTaskContext
{/**
	キャラ辞典シーンコンテキスト
 */
public:
	typedef map<int, CDictCharaItem*> chara_item_map;

	// デストラクタ
	~CDictCharaContext();

	// アクセッサ
	chara_item_map&	getCharaItemMap(){ return mapCharaItem_; }
	CDictCharaItem*	getCurrentCharaItem(){ return current_it_->second; }

	// 操作
	void			addDictCharaItem(int nNo, CDictCharaItem* pItem){ mapCharaItem_.insert(pair<int, CDictCharaItem*>(nNo,pItem)); }
	CDictCharaItem* getDictCharaItem(int nNo)
	{
		chara_item_map::iterator it = mapCharaItem_.find(nNo);
		if(it!=mapCharaItem_.end()) return it->second;
		return NULL;
	}
	void setCharaIterator(int nNo){	current_it_ = mapCharaItem_.find(nNo); }
	void incCharaIterator()
	{ 
		if(++current_it_==mapCharaItem_.end()) current_it_ = mapCharaItem_.begin();
	}
	void decCharaIterator()
	{ 
		if(current_it_==mapCharaItem_.begin()) current_it_ = mapCharaItem_.end();
		--current_it_;
	}
	// nPhaseじゃないキャラをランダムに選択する
	CDictCharaItem* getAgainstCharaData(int nPhase);

private:
	// キャラアイテムマップ
	chara_item_map mapCharaItem_;

	// 現在、表示中のアイテムを指すイテレータ
	chara_item_map::iterator current_it_;
};

} // namespace Dict end
} // namespace BMW end
