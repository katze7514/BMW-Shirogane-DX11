/*
	katze 06/01/20
	パネル定義データ
*/
#pragma once

#include "IDGuiDef.h"
#include "IDataGuiDefWidget.h"

namespace BMW{
namespace GUI{

class CDataGuiDefPanel : public IDataGuiDefWidget
{/**
	パネル定義データ
 */
public:
	typedef list<IDataGuiDefWidget*> widget_list;
	// コンストラクタ・デストラクタ
	CDataGuiDefPanel():nPanelType_(Panel::NORMAL){ setKind(Def::PANEL); }
	virtual ~CDataGuiDefPanel();

	// 取得・設定
	widget_list&	getWidgetList(){ return listWidget_; }
	void			setWidget(IDataGuiDefWidget* pWidget){ listWidget_.push_back(pWidget); }

	int		getPanelType()const{ return nPanelType_; }
	void	setPanelType(int nType){ nPanelType_=nType; }

	// 操作
	widget_list::iterator	beginWidget(){ it=listWidget_.begin(); return it; }
	bool					endWidget(){ return it==listWidget_.end(); }
	widget_list::iterator	nextWidget(){ return it++; }

protected:
	// ↓の先頭から実体化される
	widget_list listWidget_;
	widget_list::iterator it;

	int nPanelType_;
};

} // namespace GUI end
} // namespace BMW end