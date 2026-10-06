/*
	katze 06/01/20
	インターフェイス定義DB
*/
#pragma once

#include "../../Movie/DB/CSymbolDB.h"

namespace BMW{
namespace GUI{

class IDataGuiDef;
class CDataGuiDefPanel;
class IDataGuiDefWidget;
class CDataGuiDefNum;
class CNumCtrl;
class CNumRemain;
class CGage;
class CCircleMenu;
class CGraphicFace;
class CGraphicName;
class CInterfaceLayer;

class CGuiDefDB
{/**
	インターフェイス定義DB

	シンボルDBを下位に持ち、その組み合わせ情報を扱う
	基本的には、SymbolDBと同様構築情報のみを持ち。
	必要に応じて、実体を生成する
 */
public:
	typedef map<string,IDataGuiDef*> guidef_map;

	// デストラクタ
	~CGuiDefDB();

	// 設定・取得
	void			setGuiDef(const string& sFile);
	void			setGuiDefStream(const string& sData);

	IDataGuiDef*	getGuiDefData(const string& sID)
	{
		guidef_map::iterator it = mapGuiDef_.find(sID);
		return it!=mapGuiDef_.end() ? it->second : NULL;
	}
	void			setGuiDefData(const string& sID, IDataGuiDef* pDef)
	{
		#ifdef BMW_DEBUG
			if(getGuiDefData(sID)!=NULL)
			{
				CDbg().Out("GuiDef %s はすでに存在しています", sID.c_str());
				return;
			}
		#endif
			mapGuiDef_.insert(pair<string,IDataGuiDef*>(sID,pDef));
	}

	Movie::CSymbolDB& getSymbolDB(){ return symbolDB_; }

	// 生成
	Task::ITaskBase*	createInterface(const string& sID);
	CNumRemain*			createRemain(IDataGuiDef*);
	CGage*				createGage(IDataGuiDef*);
	CCircleMenu*		createCircle(IDataGuiDef*);
	CButton*			createButton(IDataGuiDef*,int nAct=Button::NORMAL);
	CButton*			createButton_ID(const string& sID,int nAct=Button::NORMAL);
	CGraphic*			createGraphic(IDataGuiDef*);
	CText*				createTextGui(IDataGuiDef*);
	CInterfaceLayer*	createAction(IDataGuiDef*);
	IPanel*				createPanel(IDataGuiDef*);
	void				createPanelWidget(IPanel*,CDataGuiDefPanel*);
	template<class Panel>
	Panel*				createPanelType(CDataGuiDefPanel*);

	Task::ITaskBase*	createWidget(IDataGuiDefWidget*);
	CGraphicFace*		createFace(IDataGuiDefWidget*);
	CGraphicName*		createName(IDataGuiDefWidget*);
	CNumRemain*			createWidgetRemain(IDataGuiDefWidget*);
	CGage*				createWidgetGage(IDataGuiDefWidget*);
	INum*				createNum(IDataGuiDefWidget*);
	CNumCtrl*			createNumCtrl(CDataGuiDefNum*);
	IPanel*				createWidgetPanel(IDataGuiDefWidget*);
	Task::ITaskBase*	createSymbol(IDataGuiDefWidget*);
	CText*				createText(IDataGuiDefWidget*);
	CButton*			createWidgetButton(IDataGuiDefWidget*);
	CGraphic*			createWidgetGraphic(IDataGuiDefWidget*);
	Task::ITaskBase*	createObj(IDataGuiDefWidget*);
	
	// ホルダに対して使うヘルパ
	void setGraphicHolder(CGraphicPopUp* pGraphic, const string& s);
	void setTextHolder(CTextPopUp* pText, const string& s);
	void setTextPopUpHolder(CTextPopUp* pText, const string& s); // テキストの内容とポップアップ内容だけをコピーする
	void setButtonHolder(CButtonGraphic* pButton, const string& s);
	void setButtonHolder(CButtonSymbol* pButton, const string& s);
	void setButtonHolder(CButtonSymbol* pButton, const string& s, const CButton::ButtonEvent& fun, int nValue);


	// インターフェイス取得ヘルパ
	template<class T>
	T* createInterfaceCast(const string& sID){ return static_cast<T*>(createInterface(sID)); }

	size_t getMapSize(){ return mapGuiDef_.size(); }

private:
	guidef_map					mapGuiDef_;
	Movie::CSymbolDB			symbolDB_;
};

} // namespace GUI end
} // namespace BMW end