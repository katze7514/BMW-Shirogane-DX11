/*
	katze 06/06/06
	出撃選択
*/
#pragma once

#include "../../Scene/Event/IListenerCircleMenu.h"

namespace BMW{
namespace SLG{
class CDataCharaSLG;
class CSLGContext;

namespace Sally{

class CSally_select : public Task::CTaskList, public BMW::Event::IListenerCircleMenu
{/**
	出撃選択
 */
public:
	enum ePriority
	{
		STATUS,
		WAIT,
		MENU,
		INTRO_EFFECT,
	};
	enum eState
	{
		INTRO,
		CLICK_WAIT,
		EXIT,

		START,
		NORMAL,
		CHARA,
		END,
		CIRCLE,
		UPDATE,
		YES_CANCEL,
	};
	enum eChara
	{// サークルメニュー
		PUT_OUT,	// 外す
		CHANGE,		// キャラ交換
		SPEC,		// ステータス
		ARC,		// アルクへ戻る
		PHANTAS,	// ファンタズムーンへ変身
		ECLIPS,		// ファンタズムーン・エクリプスへ変身
		RIN,		// 凛へ戻る
		KALEIDO,	// カレイドルビーへ転身
		KOHAKU,		// 琥珀へ戻る
		AMBER,		// アンバーへ変身
		SABER,		// セイバーになる
		LILY,		// リリィになる
	};
	enum eButton
	{
		SALLY=-2,	// 出撃
		WAIT_PAGE,	// 待機パネルのページ
	};
	enum eYes
	{
		YES,NO,
	};
	// コンストラクタ・デストラクタ
	CSally_select():pEv_(NULL),pMapChip_(NULL){}
	~CSally_select();

	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext* pContext);

	// アクション
	void actionMenu(int nState, Task::CTaskContext*);

	// イベントリスナー
	// サークルメニュー用
	// 動き終了時に呼ばれる
	void eventCircle(int nState, Task::CTaskContext* pContext);
	// ボタンイベントリスナ
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	// 待機パネル用
	void eventChara(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	// 確認ダイアログ用
	void eventYes(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	void eventCancel(Task::CTaskContext* pCotext);
	
private:
	Movie::CMovieClip*  pMovie_;
	//GUI::CCircleMenu* pMenu_;
	GUI::CPanel*		pPanel_;
	GUI::INum*			pRemain_;		// 残り何人
	GUI::CPanel*		pStatus_;		// ステータスパネル
	Movie::CMovieClip*	pIntroEffect_;	// 出撃選択
	GUI::CGraphic*		pEv_;			// Evグラフィック

	CSLGContext* p;
	// 最大出撃人数
	int nMaxChara_;

	// イベントキャラIDセット
	// つまり、これに入ってるキャラは外せない
	set<int> setEvent_;
	// 除外キャラ
	set<int> setOut_;
	// 操作可能なキャラセット
	// これに入っていないと操作不可
	set<int> setCtrlChara_;
	// 現在マップ上にいるキャラセット
	set<int> setMapChara_;
	// すでにロード済みのキャラIDセット
	//set<int> setLoadChara_;
	// これが呼ばれた時のPlayerPhaseListコピー
	list<int> listPlayerPhaseList_;

	// 待機パネルにいるキャラ（SLG ID）
	// PlayerPhaseListで代用
	int nMapSelect_;	// 現在、選択中のキャラ（MAP上）
	int nToward_;		// 追加方向
	int nTargetMap_;	// 対象のマップインデクッス
	Map::CMapChip* pMapChip_; // EV_Gが突っ込んであるマップチップ

	// 変身データストア
	int nArc_; // 現在、選ばれてるアルクモード
	int nRin_; // 現在、選ばれてる凛モード
	int nKohaku_; // 現在、選ばれてる琥珀モード
	int nSaber_; // 現在、選ばれてるセイバーモード
	map<int, CDataCharaSLG*> mapMetaor_;
	CDataCharaSLG*	getMetaorData(int nCharaID);
	void			setMetaorData(int nCharaID, CDataCharaSLG* pChara);
	void			clearMetaorData();

	// アクション
	// サークルメニューだし
	void setMenuIntro();
	// キャラだし
	void actionInstall(int nID, Task::CTaskContext*);
	// 外す
	void actionPutOut(Task::CTaskContext*);
	// 位置交換
	void actionChange(Task::CTaskContext*);
	// 変身
	void actionMetamor(int nMeta);
	// ステータス動作
	void actionStatus(int nTarget, Task::CTaskContext*);
};

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end