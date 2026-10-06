/*
	katze 06/03/02
	アイテム装備
*/
#pragma once

#include "ITrainBase.h"

namespace BMW{
namespace Inter{
namespace Chara{

class CItemEqup : public ITrainBase
{/**
	アイテム装備
 */
public:
	enum eGet{
		REMOVE=INT_MAX,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);

	// イベントハンドラ
	void eventHave(const smart_ptr<GUI::CEventButton>& pButton ,Task::CTaskContext*);
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton ,Task::CTaskContext*);
	// アクション
	void actionOK(Task::CTaskContext*);
	void actionStatus(Task::CTaskContext*);
	void updateStatus(BMW::Chara::CDataCharaInter* pChara);
	void actionReset(CInterChara* pChara, CInterContext* p);

	// アイテム設定
	void initItem(CInterContext*);

private:
	// アイテム一覧での位置と対応するアイテムIDのテーブル
	int anHaveID_[4];
	int anGetID_[28];
	// 現在、ステータスに適用されてるアイテム位置
	int nHave_;
	// インターフェイス
	GUI::CButtonSymbol* getItemButton(int nPos);
	void				setItemIconList(int nNum, Task::CTaskContext* p);
	void				changeStatus(int nFrom, int nTo, CInterContext& p);
	// GUI::CPanel* pPanel_;
	GUI::CPanel*		pGet_;
	GUI::CNumRemain*	pItemNum_;
	GUI::CPanelCtrl*	pHave_;
	GUI::CPanel*		pNowStatus_;
	GUI::CPanel*		pUpStatus_;
};

} // namespace Chara end
} // namespace Inter end
} // namespace BMW end