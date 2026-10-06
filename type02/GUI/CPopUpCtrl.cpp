#include "stdafx.h"

#include "CPopUpCtrl.h"

namespace BMW{
namespace Draw{

// コンストラクタ・デストラクタ
CPopUpCtrl::CPopUpCtrl()
{}

CPopUpCtrl::~CPopUpCtrl()
{
	clearPopUp();
}

/*void CPopUpCtrl::Task(Task::CTaskContext* pContext)
{
	if((pContext->IsAction() && IsValid())
	|| IsVisible())
		pPopUp_->Task(pContext);
}*/

// ポップアップの生成とキャッシュ動作
CPopUpBase* CPopUpCtrl::createPopUp(GUI::IButton* pButton, Task::CTaskContext* pContext)
{
	const string& sText = pButton->getPopUp();
	if(sText.empty()) return NULL;
	CPopUpBase* pBase;
	popup_list::iterator it;
	for(it=listPopUp_.begin(); it!=listPopUp_.end(); it++)
	{// IDは、表示文字列そのもの
		if((*it)->getText().getText()==sText)
		{// キャッシュ上にみーっけ
			pBase = *it;
			// リストの先頭へ
			listPopUp_.erase(it);
			listPopUp_.push_front(pBase);
			pBase->setButtonTask(pButton);
			return pBase;
		}
	}

	// みつかんね。じゃ、新しくつくろ
	pBase = new CPopUpBase(sText);
	// キャッシュ動作
	if(listPopUp_.size()>CACHE)
	{// キャッシュ数越えてたら、末尾のを削除
		DELETE_SAFE(*listPopUp_.rbegin());
		listPopUp_.pop_back();
	}
	// キャッシュ先頭へ
	listPopUp_.push_front(pBase);
	// ポップアップ実体生成
	pBase->createPopUp();
	pBase->setButtonTask(pButton);

	return pBase;
}

void CPopUpCtrl::delPopUp(const string& sID)
{
	if(sID.empty()) return;
	popup_list::iterator it;
	for(it=listPopUp_.begin(); it!=listPopUp_.end(); ++it)
	{// IDは、表示文字列そのもの
		if((*it)->getText().getText()==sID)
		{// キャッシュ上にみーっけ
			// 削除
			DELETE_SAFE(*it);
			// リストから削除
			it=listPopUp_.erase(it);
		}
	}
}

void CPopUpCtrl::clearPopUp()
{
	popup_list::iterator it;
	for(it=listPopUp_.begin(); it!=listPopUp_.end(); it++)
		DELETE_SAFE(*it);

	listPopUp_.clear();
}

} // namespace Draw end
} // namespace BMW end