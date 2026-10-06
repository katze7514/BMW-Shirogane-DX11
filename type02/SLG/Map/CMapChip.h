/*
	katze 05/03/24
	マップ一マスを表現するクラス
*/
#pragma once

#include "CMapChipInfo.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // naemsapce Task end

namespace SLG{
namespace Map{

class CMapChipState;
class CMapChip : public BMW::Rule::CRuleListDraw
{/**
	マップ一マスを表現する

	最大で、
	　オブジェクト・判定領域・キャラ・エフェクト
	の一連のタスクを持つ
 */
public:
	enum eState{
		NORMAL,
		OVER,
	};
	// タスクプライオリティ
	enum ePriority{
		OBJ,		// オブジェクト。つまり、MAPチップのスプライト
		STATE,		// グリッドなどのあたり判定領域
		CHARA,		// キャラ
		EFFECT,
#ifdef BMW_DEBUG
		INDEX,
#endif
	};

	// コンストラクタ・デストラクタ
	CMapChip():nIndex_(-1),pState_(NULL){}
	virtual ~CMapChip(){}

	// タスク
	void OnAction(Task::CTaskContext*);

	// 設定・取得
	int					getIndex() const { return nIndex_; }
	void				setIndex(int nIndex)
						{ 
							nIndex_=nIndex;
							#ifdef BMW_DEBUG
								text_ = new GUI::CText();
								text_->visible(false);
								text_->setText(CStringScanner::NumToString(nIndex_));
								text_->setSide(1);
								text_->setColor(RGB(255,0,0));
								text_->UpdateText();
								text_->setX(32);
								text_->setY(16);
								addTask(text_,INDEX);
							#endif
						}
	const CMapChipInfo& getMapInfo() const { return info_; }
	void				setMapInfo(const CMapChipInfo& info)
						{
							info_=info;
							//#ifdef BMW_DEBUG
							//	//text_->setText(CStringScanner::NumToString(info_.getHeight()));
							//	//text_->UpdateText();
							//#endif
						}

	// 操作
	void				createMapChipState();
	CMapChipState*		getMapChipState(){ return pState_; }

	void				setRange(const RECT& range);

	// アクション
	void actionOver(Task::CTaskContext*);

protected:
	// このチップのIndex
	int				nIndex_;
	// このチップの情報
	CMapChipInfo	info_;
	// このチップの状態
	CMapChipState*	pState_;

#ifdef BMW_DEBUG
	GUI::CText* text_;
#endif
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end