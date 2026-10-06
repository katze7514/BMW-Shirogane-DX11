/*
	katze 05/03/24
	update 06/03/25
	SLGシーンのマップを表現するクラス
*/
#pragma once

#include "CMapLoader.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // naemsapce Task end

namespace SLG{
namespace Map{

class CMapChip;
class CMap : public Task::CTaskBase
{/**
	マップ="ボード"を担うクラス
 */
public:
	typedef vector<CMapChip*> map_vector;
	// マップ状態
	enum eState{
		NORMAL,			// 通常
		SCROLL_READY,	// スクロール準備
		SCROLL,			// スクロールモード
	};

	// コンストラクタ・デストラクタ
	CMap(const string& sFile){ OnInit(sFile); }
	~CMap();

	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(const string& sFile);
	void OnAction(Task::CTaskContext*);
	//virtual void OnDraw(Task::CTaskContext*);

	// 操作
	Task::CTaskList*	getBack(){ return &back_; }

	map_vector& getMapChipVector(){ return vecMap_; }
	void		setMapChipSize(int nSize){ vecMap_.resize(nSize); nSize_=nSize; }
	CMapChip*	getMapChip(int nIndex){ if(nIndex<0 || nIndex>=nSize_){ return NULL; }else{ return vecMap_[nIndex]; } }
	void		setMapChip(CMapChip* pChip, int nIndex);

	// 端の設定
	const RECT& getRect()const{ return rect_; }
	void		setRect(const RECT& rect){ rect_=rect; }

	// 背景
	const string&	getDemoBack()const{ return sBack_; }
	void			setDemoBack(const string& sBack){ sBack_=sBack; }

	// スクロール
	void		scroll(Task::CTaskContext*);
	void		scrollIndex(int nIndex);
	void		scrollPos(int nX, int nY);

	// 位置取得
	void		getMapPos(int& nX, int& nY);
	bool		getMapChipPos(int nIndex, int& nX, int& nY, bool bP=true);

private:
	// 背景
	Task::CTaskList back_;
	// マップチップ
	// vectorの添え字がIndexでもある
	map_vector vecMap_;
	// マップチップの数
	int nSize_;
	// マップとしての有効範囲
	RECT rect_;
	// このマップの背景
	string sBack_;

	// スクロールモードの際に使用する
	// マウスカーソル位置
	int nX_,nY_;

	// マップチップローダ
	CMapLoader loader_;
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end
