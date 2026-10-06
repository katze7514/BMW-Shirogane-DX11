/*
	katze 06/01/20
	パネルの動きだけを規定するクラス
*/
#pragma once

#include "IDGuiDef.h"
#include "IDataGuiDef.h"

namespace BMW{
namespace GUI{

class CDataGuiDefActionState
{/**
	アクション定義
 */
public:
	enum eAction{
		STATE,
		TWEEN,
	};
	// コンストラクタ・デストラクタ
	CDataGuiDefActionState():nAction_(STATE){}
	virtual ~CDataGuiDefActionState(){}

	// 設定・取得
	int		getAction()const{ return nAction_; }
	void	setAction(int nAction){ nAction_=nAction; }

	const Draw::CDrawInfo&	getDrawInfo()const{ return info_; }
	void					setDrawInfo(const Draw::CDrawInfo& info){ info_=info; }

protected:
	int				nAction_;
	Draw::CDrawInfo info_;
};

class CDataGuiDefActionTween : public CDataGuiDefActionState
{/**
	トゥイーン
 */
public:
	// コンストラクタ
	CDataGuiDefActionTween(){ setAction(TWEEN); }

	// 設定・取得
	const Draw::CDrawInfo&	getEnd()const{ return end_; }
	void					setEnd(const Draw::CDrawInfo& end){ end_=end; }
	int						getFrame(){ return nFrame_; }
	void					setFrame(int nFrame){ nFrame_=nFrame; }
	int						getEdging()const{ return nEdging_; }
	void					setEdging(int nEdging){ nEdging_=nEdging; }

protected:
	Draw::CDrawInfo end_;
	int				nFrame_;
	int				nEdging_;
};

class CDataGuiDefAction : public IDataGuiDef
{/**
	パネルの動きだけを規定するクラス
 */
public:
	typedef list<CDataGuiDefActionState*> action_list;
	// コンストラクタ・デストラクタ
	CDataGuiDefAction(){ setKind(Def::ACTION); }
	virtual ~CDataGuiDefAction()
	{
		action_list::iterator it;
		for(it=listAction_.begin(); it!=listAction_.end(); it++)
			DELETE_SAFE(*it);

		listAction_.clear();
	}


	// 設定・取得
	void	setAction(CDataGuiDefActionState* pAction){ listAction_.push_back(pAction); }

	const string&	getTargetID()const{ return sTargetID_; }
	void			setTargetID(const string& sID){ sTargetID_=sID; }

	// 操作
	action_list::iterator	beginAction(){ it=listAction_.begin(); return it; }
	bool					endAction(){ return it==listAction_.end(); }
	action_list::iterator	nextAction(){ return it++; }
	int						getActionSize()const{ return listAction_.size(); }

protected:
	action_list listAction_;
	action_list::iterator it;

	// このアクションの適用対象
	string sTargetID_;
};

} // namespace GUI end
} // namespace BMW end