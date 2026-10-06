/*
	katze 05/06/16
	アイテム保持情報
*/
#pragma once

namespace BMW{
namespace Inter{

class CDataItemInter
{/**
 	アイテム保持情報
 */
public:
	// コンストラクタ
	CDataItemInter():nNum_(0){}

	// 設定・取得
	int		getID()const{ return nID_; }
	void	setID(int nID){ nID_=nID; }
	int		getNum()const{ return nNum_; }
	void	setNum(int nNum){ nNum_=nNum; }

	multiset<int>&	getCharaSet(){ return setChara_; }
	void			addChara(int nID){ setChara_.insert(nID); }
	void			delChara(int nID)
	{// 一個だけ消す
		int n = setChara_.erase(nID);
		for(int i=1; i<n; i++)
			setChara_.insert(nID);
	}
	int				sizeChara(){ return (int)setChara_.size(); }
	int				realSizeChara(int nTarget=-1)
	{
		int nPrev=-1;
		int nCount=0;
		for(it=setChara_.begin(); it!=setChara_.end(); ++it)
		{
			if(nTarget!=*it && nPrev!=*it){ ++nCount; nPrev=*it; }
		}

		return nCount;
	}
	int				countChara(int nID){ return setChara_.count(nID); }

	multiset<int>::iterator beginChara(){ it=setChara_.begin(); return it; }
	bool					endChara(){ return it==setChara_.end(); }
	multiset<int>::iterator nextChara(){ return it++; }

private:
	// アイテムID	
	int nID_;
	// アイテム保持数
	int nNum_;
	// このアイテムを保持してるキャラID
	// 同じアイテムを複数持ってる時はキャラIDが重複している
	multiset<int>			setChara_;
	multiset<int>::iterator it;
};

} // namespace Inter end
} // namespace BMW end