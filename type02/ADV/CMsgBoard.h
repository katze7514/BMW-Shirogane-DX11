/*
	katze 05/05/18
	update 06/02/12
	メッセージボード
*/
#pragma once

namespace BMW{

namespace GUI{
class CGraphicFace;
class CGraphicName;
} // namespace GUI end

namespace ADV{

class CMsgBoard : public Task::ITaskBase
{/**
	メッセージボード
 */
public:
	enum eSide{
		LEFT,
		RIGHT,
	};
	// コンストラクタ・デストラクタ
	CMsgBoard();
	virtual ~CMsgBoard();

	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);

	const Draw::CDrawInfo getDrawInfo(bool bRela=true);
	
	// 設定・取得
	int		getSide() const { return nSide_; }
	void	setSide(int nSide){ nSide_=nSide; }
	bool	IsMsgValid()const{ return bMsgValid_; }
	void	msgValid(bool bMsgValid);

	// 簡単なテキスト状況設定
	COLORREF	getTextColor()const{ return pMsg_->getFontConf().GetColor(); }
	void		setTextColor(COLORREF rgb){ pMsg_->setColor(rgb); }
	void		setTextFont(int nFont){ pMsg_->setFont(nFont); }

	GUI::CPanel* getPanel(){ return pPanel_; }

	// アクション
	void	changeMsg(int nChara,int nFace,const string& sMsg,bool bMask,Task::CTaskContext*);

protected:
	// サイド
	int nSide_;
	// 有効・無効フラグ
	// 無効だと、alphaを下げる
	bool bMsgValid_;

	// 現在表示中の情報
	int nChara_;
	int nFace_;

	// メッセージボード要素
	GUI::CPanel*		pPanel_;
	// ↑の中身よくアクセスするので
	GUI::CGraphicFace*	pFace_;
	GUI::CGraphicName*	pName_;
	GUI::CText*			pMsg_;
};

} // namespace ADV end
} // namesapce BMW end