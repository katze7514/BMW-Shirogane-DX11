/*
	katze 06/02/02
	サークルメニューシステム
*/
#pragma once

namespace BMW{
namespace GUI{
class CCircleMenuButton;

class CCircleMenu : public CPanel
{/**
	サークルメニューシステム

	こいつにボタンを登録しておき
	使う使わないの設定をしておくと、
	自動的に適切なデータ配置になる
 */
public:
	typedef map<string,CCircleMenuButton*>	button_map;
	typedef delegate<void,int,Task::CTaskContext*>	CircleEvent;
	enum eState{
		INTRO,	// 登場
		NORMAL,	// 通常
		EXIT,	// 退場
	};
	// コンストラクタ・デストラクタ
	CCircleMenu():nR_(1),nIntro_(10),nExit_(5),nEndButton_(0){}
	virtual ~CCircleMenu();

	// タスク
	// 設定されている状態に合わせて、動き設定をする
	void Task(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// 設定・取得
	int		getR()const{ return nR_; }
	void	setR(int nR){ nR_=nR; }
	int		getIntro()const{ return nIntro_; }
	void	setIntro(int nIntro){ nIntro_=nIntro; }
	int		getExit()const{ return nExit_; }
	void	setExit(int nExit){ nExit_=nExit; }
	void	setEventHandler(const CircleEvent& fun){ fun_=fun; }

	// ボタン設定
	// ボタンの追加
	CCircleMenuButton*	getButton(const string& sID);
	void				addButton(CCircleMenuButton* pButton ,const string& sID);
	// ボタンの有効・非有効化
	void	validButton(bool bEnable, const string& sID, Task::CTaskContext* pContext);
	void	resetButton();
	bool	IsValidButton(const string& sID);

	// ちょっとしたヘルパ
	void	setButtonEventHandler(const string& sID, const GUI::CButton::ButtonEvent& fun, int nValue);

protected:
	// 半径
	int nR_;
	// 動作フレーム数
	int nIntro_,nExit_;
	// いわば、ボタンキャッシュ
	button_map mapButton_;
	// カーソル移動用
	Movie::CMotion motion_;
	Input::IInput* pInput_;

	// 動作終了ボタン数
	int nEndButton_;
	// 動作終了時に呼ばれるイベントハンドラ
	CircleEvent	fun_;

	// これで、動作終了ボタンをゲットする
	void callTaskAction(CTaskContext* pContext);
	// 終了判定
	bool IsButtonEnd();
};

} // namespace GUI end
} // namespace BMW end