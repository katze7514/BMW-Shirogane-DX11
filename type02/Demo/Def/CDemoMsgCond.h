/**
	katze 06/03/24
	メッセージ条件
*/
#pragma once

namespace BMW{
namespace Demo{

class CDemoMsgCondBase
{/**
	メッセージ条件基本
 */
public:
	enum eType{
		HP,
		CHARA,
		RANDOM,
	};
	// コンストラクタ
	CDemoMsgCondBase():nType_(RANDOM),nCond_(100),nRand_(100){}

	// アクセッサ
	int	 getType()const{ return nType_; }
	void setType(int nType){ nType_=nType; }
	int	 getCond()const{ return nCond_; }
	void setCond(int nCond){ nCond_=nCond; }
	int	 getRand()const{ return nRand_; }
	void setRand(int nRand){ nRand_=nRand; }
	int	 getMsgID()const{ return nMsgID_; }
	void setMsgID(int nMsgID){ nMsgID_=nMsgID; }

private:
	// タイプ
	int nType_;
	// 条件パラメタ
	int nCond_;
	// 確率
	int nRand_;
	// MSG ID
	int nMsgID_;
};

class CDemoMsgCond
{/**
	メッセージ条件
 */
public:
	typedef list<CDemoMsgCondBase*> cond_list;
	// デストラクタ
	~CDemoMsgCond()
	{
		for_each(listCond_.begin(), listCond_.end(), DeleteObj());
	}

	// 設定
	void addMsgCond(CDemoMsgCondBase* pCond){ listCond_.push_back(pCond); }

	// MSG ID取得
	int getMsgID(int nHP, int nChara, Task::CTaskContext* pContext);

private:
	cond_list listCond_;
};

} // namespace Demo end
} // namespace BMW end