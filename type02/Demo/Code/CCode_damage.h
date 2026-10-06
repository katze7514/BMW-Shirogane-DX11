/*
	katze 05/05/24
	ダメージ
*/
#pragma once

namespace BMW{
namespace Demo{
namespace Code{

class CCode_damage : public BMW::Rule::IRuleTask
{/**
	ダメージ
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);

	// 設定・取得
	int		getSide()const{ return nSide_; }
	void	setSide(int nSide){ nSide_=nSide; }
	int		getValue()const{ return nValue_; }
	void	setValue(int nValue){ nValue_=nValue; }

private:
	int nSide_;
	int nValue_;
};

} // namespace Code end
} // namespace Demo end
} // namespace BMW end