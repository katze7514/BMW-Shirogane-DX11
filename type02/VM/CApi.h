/*
	katze 05/04/26
	APIを担うルール
*/
#pragma once

#include "../Rule/CRuleList.h"

namespace BMW{
namespace VM{

class CApi : public Rule::CRuleList
{/**
	APIを担うルール

	ようは、Initフラグを持ってるかどうかだけだが
 */
public:
	// コンストラクタ・デストラクタ
	CApi():bInit_(false){}
	virtual ~CApi(){}

	// 設定・取得
	bool IsInit() const { return bInit_; }
	void init(bool bInit){ bInit_=bInit; }

protected:
	// 初期化フラグ
	bool bInit_;
};

} // namespace VM end
} // namespace BMW end