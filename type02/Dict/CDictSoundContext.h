/**
	サウンド辞典コンテキスト
	katze 08/08/25
*/
#pragma once

namespace BMW{
namespace Dict{

class CDictSoundItem
{/**
	サウンド情報一つ
 */
public:
	CDictSoundItem():nNo_(-1){}

	// アクセッサ
	int				getNo()const{ return nNo_; }
	void			setNo(int nNo){ nNo_=nNo; }
	const string&	getID()const{ return sID_; }
	void			setID(const string& sID){ sID_=sID; }
	const string&	getTitle()const{ return sTitle_; }
	void			setTitle(const string& sTitle){ sTitle_=sTitle; }
	const string&	getComposer()const{ return sComposer_; }
	void			setComposer(const string& sComposer){ sComposer_=sComposer; }
	const string&	getUse()const{ return sUse_; }
	void			setUse(const string& sUse){ sUse_=sUse; }
	const string&	getComment()const{ return sComment_; }
	void			setComment(const string& sComment){ sComment_=sComment; }

private:
	int		nNo_;		// ソートID
	string	sID_;		// BGM ID
	string	sTitle_;	// タイトル
	string	sComposer_;	// 作曲者
	string	sUse_;		// 使用場面
	string	sComment_;	// コメント
};

class CDictSoundContext : public Task::CTaskContext
{/**
	サウンド辞典コンテキスト
 */
public:
	typedef map<int, CDictSoundItem*> sound_item_map;
	
	// コンストラクタ・デストラクタ
	CDictSoundContext():pCurrentItem_(NULL){}
	~CDictSoundContext();

	// アクセッサ
	sound_item_map& getSoundItemMap(){ return mapSoundItem_; }
	CDictSoundItem*	getCurrentSoundItem(){ return pCurrentItem_; }
	void			setCurrentSoundItem(CDictSoundItem* pItem){ pCurrentItem_=pItem; }

	// 操作
	void			addSoundItem(int nID, CDictSoundItem* pItem)
	{
		mapSoundItem_.insert(pair<int,CDictSoundItem*>(nID,pItem));
	}
	void			setCurrentSoundItemID(int nID)
	{
		sound_item_map::iterator it = mapSoundItem_.find(nID);
		if(it!=mapSoundItem_.end()) pCurrentItem_ = it->second;
		else						pCurrentItem_ = NULL;
	}

private:
	// サウンドアイテムマップ
	sound_item_map mapSoundItem_;

	// 現在、鳴っているサウンドアイテム
	CDictSoundItem* pCurrentItem_;
};

} // namespace Dict end
} // namespace BMW end
