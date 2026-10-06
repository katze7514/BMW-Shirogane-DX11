/*
	katze 07/02/05
	養成データ操作
	対象は与えられているキャラIDリスト
*/
#pragma once

#include "CCode_train.h"

namespace BMW{
namespace ADV{
namespace Code{

class CCode_train_all : public BMW::Rule::CRuleList
{/**
	養成データ操作
	対象は与えられているキャラIDリスト
 */
public:
	enum eCtrl{
		NORMAL,	// 通常
		SUB,	// キャラリストとvalidリストの差分に対して操作
	};
	// 以下はCCode_trainと共有の方向で
	/*enum eKind{
		EXP,	// 経験値増量
		KILL,	// 撃墜数増減
	};*/
	/*enum eType{
		ABS,
		ADD,
	};*/
	// コンストラクタ
	CCode_train_all():nCtrl_(NORMAL),nType_(CCode_train::ADD),nValue_(0),nMax_(-1){}

	// タスク
	void OnAction(Task::CTaskContext*);

	// アクセッサ
	list<int>&	getCharaIDList(){ return listCharaID_; }
	void		setCharaIDList(list<int>& listCharaID){ listCharaID_=listCharaID; }
	void		addCharaID(int nCharaID){ listCharaID_.push_back(nCharaID); }

	int		getCtrl()const{ return nCtrl_; }
	void	setCtrl(int nCtrl){ nCtrl_=nCtrl; }
	int		getKind()const{ return nKind_; }
	void	setKind(int nKind){ nKind_=nKind; }
	int		getType()const{ return nType_; }
	void	setType(int nType){ nType_=nType; }
	int		getValue()const{ return nValue_; }
	void	setValue(int nValue){ nValue_=nValue; }
	int		getMax()const{ return nMax_; }
	void	setMax(int nMax){ nMax_=nMax; }

private:
	list<int> listCharaID_;
	int nCtrl_;
	int nKind_;
	int nType_;
	int nValue_;
	int nMax_;
};

} // namespace Code end
} // namespace ADV end
} // namespace BMW end