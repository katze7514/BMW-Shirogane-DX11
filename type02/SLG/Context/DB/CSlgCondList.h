/*
	katze 06/04/06
	ISlgCondを持ち管理するクラス
*/
#pragma once

#include "ISlgCond.h"

namespace BMW{
namespace SLG{
class CSLGContext;

class CSlgCondList : public ISlgCond
{/**
	ISlgCondを持ち管理するクラス
 */
public:
	typedef list<ISlgCond*> cond_list;
	// デストラクタ
	virtual ~CSlgCondList();
	// 判定
	virtual bool judg(CSLGContext* p);

	// 設定
	void		addCond(ISlgCond* pCond){ listCond_.push_back(pCond); }
	ISlgCond*	popBackCond(){ ISlgCond* p = listCond_.back(); listCond_.pop_back(); return p; }

protected:
	cond_list listCond_;
};

class CSlgCondListOr : public CSlgCondList
{/*
	or動作するCondList
 */
public:
	// デストラクタ
	virtual ~CSlgCondListOr(){}
	// 判定
	virtual bool judg(CSLGContext* p);
};

class CSlgCondNot : public ISlgCond
{/*
	Not動作するCond
 */
public:
	// デストラクタ
	CSlgCondNot(ISlgCond* pCond):pCond_(pCond){}
	virtual ~CSlgCondNot();
	// 判定
	virtual bool judg(CSLGContext* p);

	// アクセッサ
	ISlgCond*	getCond(){ return pCond_; }
	void		setCond(ISlgCond* pCond){ pCond_=pCond; }

protected:
	ISlgCond* pCond_;
};

} // namespace SLG end
} // namespace BMW end