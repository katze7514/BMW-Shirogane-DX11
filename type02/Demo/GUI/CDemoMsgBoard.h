/*
	katze 05/05/15
	update 06/03/24
	デモ用メッセージボード
*/
#pragma once

namespace BMW{

namespace GUI{
class CGraphicFace;
class CGraphicName;
} // namespace GUI end

namespace Demo{
class CDemoMsg;

class CDemoMsgBoard : public Task::ITaskBase
{/**
	デモ用メッセージボード
 */
public:
	// デストラクタ
	virtual ~CDemoMsgBoard();
	// タスク
	void OnInit(Task::CTaskContext*);
	void Task(Task::CTaskContext*);

	// 操作
	// メッセージの登録
	int		addMsg(CDemoMsg* msg){ vecMsg_.push_back(msg); return (int)vecMsg_.size()-1; }
	// メッセージの変更
	void	changeMsg(int nID, Task::CTaskContext*);
	void	changeMsg(int nSide,int nChara,int nFace,const string& sMsg,bool bMask,Task::CTaskContext*);

private:
	// インターフェイス
	GUI::CPanel*		pPanel_;
	GUI::CGraphicFace*	pFace_;
	GUI::CGraphicName*	pName_;
	GUI::CText*			pMsg_;
	// デモMSG登録
	// こいつに解体責任は無い
	vector<CDemoMsg*> vecMsg_;
};

} // namespace Demo end
} // namespace BMW end