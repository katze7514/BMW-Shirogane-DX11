/*
	katze 05/03/31
	OKされたら、指定した値を親の状態に
	設定するだけのルール
*/
#pragma once

#include "IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // naemsapce Task end

namespace Rule{

class CRuleOK : public IRuleTask
{/**
	OKされたら、指定した値を親の状態に
	設定するだけのルールクラス
 */
public:
	// コンストラクタ・デストラクタ
	CRuleOK(int nValue=0):nValue_(nValue){}
	virtual ~CRuleOK(){}
	// タスク
	void OnAction(Task::CTaskContext*);

	// 設定・取得
	int		getValue() const { return nValue_; }
	void	setValue(int nValue){ nValue_=nValue; }

private:
	int nValue_;
};

} // namespace Rule end
} // namespace BMW end