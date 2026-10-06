/*
	katze 05/02/20
	数字クラス
*/
#pragma once

#include "../Draw/CSpriteInfo.h"
#include "INum.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace GUI{
using namespace Task;
using Draw::CSpriteInfo;

class CNum : public INum
{/**
	数字を表示するクラス
 */
public:
	// 数字記号
	enum eNumMark{
		MINUS=10,
		PLUS,
	};
	// コンストラクタ・デストラクタ
	CNum():lNum_(0),bPlus_(false){}
	virtual ~CNum(){}

	// タスク
	virtual void Task(CTaskContext*);
	virtual void OnDraw(CTaskContext*);

	// サイズ取得
	void getSize(LONG& nWidth, LONG& nHeight) const;
	void getDrawSize(LONG& nWidth, LONG& nHeight) const;

	// 設定・取得
	LONG getNum() const { return lNum_; }
	void setNum(LONG lNum){ lNum_=lNum; }
	bool IsPlus() const { return bPlus_; }
	void plus(bool bPlus){ bPlus_=bPlus; }

	const CSpriteInfo&	getSpriteInfo(int nNum) const { return sprite_[nNum]; }
	void				setSpriteInfo(const CSpriteInfo& info,int nNum){ sprite_[nNum]=info; }

protected:
	// 数値
	LONG lNum_;
	// +を表示するかフラグ
	bool bPlus_;
	// 言ってみればフォント
	CSpriteInfo sprite_[12];
};

} // namespace GUI end
} // namespace BMW end