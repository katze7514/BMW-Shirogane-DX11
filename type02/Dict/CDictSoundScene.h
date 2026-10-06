/**
	サウンド辞典
	katze 08/08/25
*/
#pragma once

#include "../Scene/CScene.h"
#include "CDictSoundContext.h"

namespace BMW{
namespace Dict{

class CDictSoundScene : public Scene::CScene<CDictSoundContext>
{/**
	サウンド辞典シーン
 */
public:
	enum eState{
		NORMAL,
		FADE,
		FADE_END,
	};
	enum eButton{
		VOLUME_0=-14,
		VOLUME_1,
		VOLUME_2,
		VOLUME_3,
		VOLUME_4,
		VOLUME_5,
		VOLUME_6,
		VOLUME_7,
		VOLUME_8,
		VOLUME_9,
		VOLUME_10,
		CHANGE,
		LEFT,
		RIGHT,
		// 0以上はサウンドボタンに対応
	};
	// コンストラクタ・デストラクタ
	CDictSoundScene():pCurVol_(NULL){}
	~CDictSoundScene();

	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// イベントハンドラ
	void eventFade(Task::CTaskContext*);
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);

private:
	int nPage_,nMaxPage_; // 現在のページ
	int nSoundNo_[24]; // 現在配置されてるサウンドNo
	list<string> pageComment_;
	list<string>::iterator page_it_;

	// GUI
	GUI::CPanel*	pPanel_;

	// 操作系
	GUI::CText*		pNow_;
	GUI::CPanel*	pChange_;
	GUI::INum*		pChangePage_;
	// 書き換え系
	GUI::CPanel*	pLeft_;
	GUI::CPanel*	pRight_;
	GUI::CText*		pTitleNum_;
	GUI::CText*		pTitle_;
	GUI::CText*		pComposer_;
	GUI::CText*		pUse_;
	GUI::CText*		pComment_;
	GUI::CPanel*	pVol_;
	GUI::CButton*	pCurVol_;

	// サウンド辞典xml読み込み
	void setDictSound();
	// サウンド項目設定
	void setSoundPage(int nPage);
	void createCommentPage();
	void setVolume(int nVol, Task::CTaskContext* pContext);
};

} // namespace Dict end
} // namespace BMW end