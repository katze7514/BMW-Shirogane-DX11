/*
	katze 05/05/22
	背景速度を変更するコード
*/
#pragma once

namespace BMW{
namespace Demo{
namespace Code{

class CCode_back_visible : public BMW::Rule::IRuleTask
{/**
	背景描画を変更するコード

	描画のON/OFFはStateで代用
 */
public:
	enum eType{
		ALL,
		BACK,
		FORWARD,
	};
	// タスク
	void OnAction(Task::CTaskContext*);

	// アクセッサ
	int		getType()const{ return nType_; }
	void	setType(int nType){ nType_=nType; }

private:
	int nType_;
};

} // namespace Code end
} // namespace Demo end
} // namespace BMW end