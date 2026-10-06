#include "stdafx.h"

#include "CCircleMenuButton.h"
#include "CCircleMenu.h"

namespace BMW{
namespace GUI{

CCircleMenu::~CCircleMenu()
{
	getWidgetList().getTaskList().clear();
	// ボタン削除
	button_map::iterator it;
	for(it=mapButton_.begin(); it!=mapButton_.end(); it++)
		DELETE_SAFE(it->second);

	mapButton_.clear();
}

/////////////////////////////////////////////////
// タスク
/////////////////////////////////////////////////
void CCircleMenu::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid())
		{
			callTaskAction(pContext);
			OnAction(pContext);
		}
	}
	else
	{
		getWidgetList().Task(pContext);
	}
}

void CCircleMenu::OnReset(Task::CTaskContext* pContext)
{// 設定されている状態に合わせて、動き設定をする
	// 角度としては、
	// 始点は、-180＋360/メニュ個数。終点は始点＋180
	// なお、y軸が↓方向なので注意
	if(getState()==INTRO || getState()==EXIT)
	{// 登場もしくは退場設定
		// 分割数
		int nDiv = 360 / getWidgetList().getTaskList().size();
		// 基点は90度
		int nAngle=-90;
		CCircleMenuButton* pButton;
		CPanelList::tasklist::iterator it;
		for(it=getWidgetList().getTaskList().begin(); it!=getWidgetList().getTaskList().end(); it++)
		{
			pButton = static_cast<CCircleMenuButton*>(*it);
			if(getState()==INTRO)
			{
				pButton->setStartAngle(-180+nAngle);
				pButton->setEndAngle(nAngle);
				// 中心位置計算
				pButton->setCoreX(gSinTable.Cos((nAngle*511)/360,getR()));
				pButton->setCoreY(gSinTable.Sin((nAngle*511)/360,getR()));
				// 次の角度へ
				// 時計回り
				nAngle+=nDiv;
			}
			// フレーム数
			pButton->setStep(getState()==INTRO?nIntro_:nExit_);
			// どっちからどっちに行くかは↓で自動的に入れ替わる
			pButton->OnReset(pContext);
		}
		// カーソル
		int nX,nY;
		pInput_ = pContext->getInput();
		pInput_->getCursolPos(nX,nY);
		motion_.setStart(nX,nY);
		Draw::CDrawInfo info = getDrawInfo();
		motion_.setEnd(info.getX(),info.getY());
		motion_.setStep(getState()==INTRO?nIntro_:nExit_);
		motion_.reset();
		pInput_->guardCursol(true);
	}
}

void CCircleMenu::OnAction(Task::CTaskContext* pContext)
{
	if(getState()==INTRO || getState()==EXIT)
	{// 登場・退場動作中
		// カーソルも移動
		motion_.inc();
		pInput_->setCursolPos(motion_.getX(),motion_.getY());
		if(IsButtonEnd() && motion_.IsEnd())
		{// 全ボタンの動作終了
			if(getState()==EXIT)
				resetButton();

			// 終了ハンドラ呼び出し
			fun_(getState(),pContext);
			setState(NORMAL);
			pInput_->guardCursol(false);
		}
	}
}

void CCircleMenu::callTaskAction(CTaskContext* pContext)
{// 動作用	
	// 子タスクを呼ぶ前にTaskListを入れ替える
	ITaskList* prevList=pContext->getTaskList();
	pContext->setTaskList(&getWidgetList());

	CPanelList::tasklist::iterator it=getWidgetList().getTaskList().begin();
	while(it!=getWidgetList().getTaskList().end())
	{
		(*it)->Task(pContext);
		if(getWidgetList().IsKill())
		{// killフラグだったらdelete
			DELETE_SAFE(*it); 
			it=getWidgetList().getTaskList().erase(it);
			// フラグリセット
			getWidgetList().kill(false);
		}
		ef(getWidgetList().IsRemove())
		{// こっちの時は、状況に応じてちょち動作いろいろ
			if(getState()==INTRO)
			{// 搭乗時
				++nEndButton_; // とりあえず、終了
			}
			else
			{// 退場時
				--nEndButton_;
				it=getWidgetList().getTaskList().erase(it);
			}
			// フラグリセット
			getWidgetList().remove(false);
		}
		else
		{
			++it;
		}
	}

	// 子タスクを呼んだのでTaskListを元に戻す
	pContext->setTaskList(prevList);
}

/////////////////////////////////////////////////
// 操作
/////////////////////////////////////////////////
CCircleMenuButton* CCircleMenu::getButton(const string& sID)
{
	button_map::iterator it = mapButton_.find(sID);
	if(it==mapButton_.end()) return NULL;
	return it->second;
}

void CCircleMenu::addButton(CCircleMenuButton* pButton ,const string& sID)
{// とりあえず、マップへ
	priorityID_.writeMap(sID,-1);
	mapButton_.insert(pair<string,CCircleMenuButton*>(sID,pButton));
	pButton->setTaskPriority(-1);
	// 半径設定
	pButton->setR(getR());
}

void CCircleMenu::validButton(bool bEnable, const string& sID, Task::CTaskContext* pContext)
{
	CCircleMenuButton* pButton = getButton(sID);
	if(pButton==NULL) return;

	if(bEnable)
	{// 有効化
		// すでに有効
		if(IsValidButton(sID)) return;
		// タスクプライオリティ書き換え
		priorityID_.writeMap(sID,getWidgetList().getTaskList().size());
		// リストに追加
		getWidgetList().addTask(pButton, getWidgetList().getTaskList().size());
		// 一度、リセット
		pButton->OnReset(pContext);
	}
	else
	{// 無効化
		// リストからはずす
		getWidgetList().removeTask(pButton->getTaskPriority());
		// タスクプライオリティ書き換え
		priorityID_.writeMap(sID,-1);
		pButton->setTaskPriority(-1);
	}
}

void CCircleMenu::resetButton()
{// 全ボタンをリストを無効かする
	button_map::iterator it;
	for(it=mapButton_.begin(); it!=mapButton_.end(); it++)
	{
		priorityID_.writeMap(it->first,-1);
		(it->second)->setTaskPriority(-1);
	}
	getWidgetList().getTaskList().clear();
}

bool CCircleMenu::IsValidButton(const string& sID)
{
	return getID(sID)>=0;
}

bool CCircleMenu::IsButtonEnd()
{
	if(getState()==INTRO)
		return nEndButton_==(int)getWidgetList().getTaskList().size();
	else
		return getWidgetList().getTaskList().empty();
	
}

void CCircleMenu::setButtonEventHandler(const string& sID, const GUI::CButton::ButtonEvent& fun, int nValue)
{
	GUI::CButton* pButton = getButton(sID)->getTaskCast<GUI::CButton>();
	pButton->setEventHandler(fun);
	pButton->getEvent()->setValue(nValue);
}

} // namespace GUI end
} // namesapce BMW end