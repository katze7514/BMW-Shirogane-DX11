/*
	katze 05/03/28
	Rule内で良く使うであろうメニューボタン
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // naemsapce Task end

namespace Rule{

class CRuleMenuButton : public GUI::CPanel
{/**
	Rule内で良く使うであろうボタンが押されると
	設定されてる値をTaskContextに設定するボタン
	として使えるスケルトン

	BUTTONタスクには、CRuleButtonを使う
 */
public:
	enum eState{
		NORMAL,
		RELEASE,
	};
	enum ePriority{
		BACK,
		BUTTON,
		FORWARD,
	};
	// デストラクタ
	virtual ~CRuleMenuButton(){}

	// タスク
	virtual void OnAction(Task::CTaskContext*);

	// 設定・取得
	int		getValue() const { return nValue_; }
	void	setValue(int nValue){ nValue_=nValue; }

protected:
	// こいつがReleaseされるとTaskContextに設定される
	int nValue_;
};

} // namespace Rule end
} // namespace BMW end