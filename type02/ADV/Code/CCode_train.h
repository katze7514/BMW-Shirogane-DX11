/*
	katze 06/07/04
	養成データ操作
*/
#pragma once

namespace BMW{
namespace ADV{
namespace Code{

class CCode_train : public BMW::Rule::CRuleList
{/**
	養成データ操作
 */
public:
	enum eKind{
		EXP,	// 経験値増量
		KILL,	// 撃墜数増減
		CHANGE,	// キャラチェンジ
		COPY,	// 養成データコピー
		BACK,	// 養成状況をBP・FPに還元
		CLEAR,	// 養成状況・アイテムをクリア
		DEL,	// 養成データを削除
	};
	enum eType{
		ABS,
		ADD,
	};
	// コンストラクタ
	CCode_train():nCharaID_(-1),nType_(ADD),nValue_(0),nMax_(-1){}

	// タスク
	void OnAction(Task::CTaskContext*);

	// アクセッサ
	int		getCharaID()const{ return nCharaID_; }
	void	setCharaID(int nCharaID){ nCharaID_=nCharaID; }
	int		getKind()const{ return nKind_; }
	void	setKind(int nKind){ nKind_=nKind; }
	int		getType()const{ return nType_; }
	void	setType(int nType){ nType_=nType; }
	int		getValue()const{ return nValue_; }
	void	setValue(int nValue){ nValue_=nValue; }
	int		getMax()const{ return nMax_; }
	void	setMax(int nMax){ nMax_=nMax; }

	static void ctrlTrain(int nCharaID, int nKind, int nType, int nValue, int nMax, Task::CTaskContext* pContext);

private:
	int nCharaID_;
	int nKind_;
	int nType_;
	int nValue_;
	int nMax_;
};

} // namespace Code end
} // namespace ADV end
} // namespace BMW end