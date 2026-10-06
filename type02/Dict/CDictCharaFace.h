/**
	katze 08/12/25
	キャラ辞典のキャラ表示
*/
#pragma once

#ifdef BMW_DEBUG

namespace BMW{

namespace Dict{

class CDictCharaItem;

class CDictCharaFace : public Task::CTaskList
{
public:
	typedef list<GUI::CPanel*> list_face;
	enum eState{
		NORMAL,
	};
	// コンストラクタ・デストラクタ
	CDictCharaFace():pBack_(NULL),nToward_(0),bBattle_(false){}
	~CDictCharaFace();

	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	// 現在、表示中のキャラデータ
	CDictCharaItem* pCurrentItem_;

	// インターフェイス
	GUI::CGuiDefDB gui_;
	// 背景
	CFastPlane* pBack_;
	// 顔組合わせ
	list_face listFace_;

	// 各ページの先頭イテレータリスト
	// firstがbeing secondがend
	list<pair<list_face::iterator,list_face::iterator> > listPagePairIterator_;
	// 現在のページ
	list<pair<list_face::iterator,list_face::iterator> >::iterator it_page_pair_;
	// 現在の表示状態
	int  nToward_; // 方向
	bool bBattle_; // 戦闘用か

	// ヘルパ
	// it_face_begin_から1画面いっぱいに顔を埋める
	// このページのendが戻ってくる
	list_face::iterator createFacePage(list_face::iterator it_face_begin, Task::CTaskContext* pContext);
	// it_page_pair_に併せてTaskListに追加
	void setFacePage();
	// 現在の状態に併せて顔をリフレッシュ
	void refreshFaceList(Task::CTaskContext* pContext);

	// ウインドウサイズ
	int nWidth_,nHeight_;
	// キーボード
	CKeyInput* pKey_;
};

} // namespace Dict end
} // namespace BMW end

#endif // BMW_DEBUG
