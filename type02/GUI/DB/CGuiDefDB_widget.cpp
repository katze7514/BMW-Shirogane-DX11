#include "stdafx.h"

#include "../../Scene/GUI/CNumCtrl.h"
#include "../../Scene/GUI/CNumRemain.h"
#include "../../Scene/GUI/CGage.h"
#include "../../Scene/GUI/CGraphicFace.h"
#include "../../Scene/GUI/CGraphicName.h"

#include "IDGuiDef.h"
#include "CDataGuiDefWidget.h"
#include "CGuiDefDB.h"

namespace BMW{
namespace GUI{
/////////////////////////////////////////////////
// Widget生成
/////////////////////////////////////////////////
Task::ITaskBase* CGuiDefDB::createWidget(IDataGuiDefWidget* pData)
{
	// 種別でディスパッチ
	switch(pData->getKind())
	{
	case Widget::FACE:		return createFace(pData);
	case Widget::REMAIN:	return createWidgetRemain(pData);
	case Widget::GAGE:		return createWidgetGage(pData);
	case Widget::NAME:		return createName(pData);
	case Widget::NUM:		return createNum(pData);
	case Widget::PANEL:		return createWidgetPanel(pData);
	case Widget::SYMBOL:	return createSymbol(pData);
	case Widget::TEXT:		return createText(pData);
	case Widget::BUTTON:	return createWidgetButton(pData);
	case Widget::GRAPHIC:	return createWidgetGraphic(pData);
	default:				return createObj(pData);
	}
}

/////////////////////////////////////////////////
// 顔グラフィック生成
/////////////////////////////////////////////////
CGraphicFace* CGuiDefDB::createFace(IDataGuiDefWidget* pData)
{
	CDataGuiDefFace*	pFaceData = static_cast<CDataGuiDefFace*>(pData);
	CGraphicFace*		pFace = new CGraphicFace();

	pFace->setX(pFaceData->getX());
	pFace->setY(pFaceData->getY());
	pFace->setToward(pFaceData->getToward());
	pFace->battle(pFaceData->IsBattle());

	return pFace;
}

/////////////////////////////////////////////////
// RemainWidget生成
/////////////////////////////////////////////////
CNumRemain* CGuiDefDB::createWidgetRemain(IDataGuiDefWidget* pData)
{
	CDataGuiDefWidgetRemain*	pRemainData = static_cast<CDataGuiDefWidgetRemain*>(pData);
	CNumRemain*					pRemain = createInterfaceCast<CNumRemain>(pRemainData->getRemainID());
	
	pRemain->setX(pRemainData->getX());
	pRemain->setY(pRemainData->getY());

	if(pRemainData->getTurn()!=0)
		pRemain->setTurn(pRemainData->getTurn());

	return pRemain;
}

/////////////////////////////////////////////////
// GageWidget生成
/////////////////////////////////////////////////
CGage* CGuiDefDB::createWidgetGage(IDataGuiDefWidget* pData)
{
	CDataGuiDefWidgetGage*	pGageData = static_cast<CDataGuiDefWidgetGage*>(pData);
	CGage*					pGage = static_cast<CGage*>(createInterface(pGageData->getGageID()));
	
	pGage->setX(pGageData->getX());
	pGage->setY(pGageData->getY());
	if(pGageData->IsLeft()>=0)
		pGage->left(pGageData->IsLeft());

	return pGage;
}

/////////////////////////////////////////////////
// 名前生成
/////////////////////////////////////////////////
CGraphicName* CGuiDefDB::createName(IDataGuiDefWidget* pData)
{
	CDataGuiDefName*	pNameData = static_cast<CDataGuiDefName*>(pData);
	CGraphicName*		pName = new CGraphicName();

	pName->setX(pNameData->getX());
	pName->setY(pNameData->getY());
	pName->setToward(pNameData->getToward());

	return pName;
}

//////////////////////////////////////////////////////////////
// 数字生成
//////////////////////////////////////////////////////////////
INum* CGuiDefDB::createNum(IDataGuiDefWidget* pData)
{
	CDataGuiDefNum*	pNumData = static_cast<CDataGuiDefNum*>(pData);
	INum* pNum;

	if(pNumData->getNumList().size()!=1)
	{// 複数のフォントを持つ
		pNum = createNumCtrl(pNumData);
	}
	else
	{// 単一の数字
		CNum* pNumBase = new CNum();
		symbolDB_.setNumGui(pNumBase,
							*pNumData->getNumList().begin());

		pNum = pNumBase;
	}

	// 位置設定
	pNum->setX(pNumData->getX());
	pNum->setY(pNumData->getY());

	return pNum;
}

CNumCtrl* CGuiDefDB::createNumCtrl(CDataGuiDefNum* pData)
{// 複数のNumを持っていたので、Ctrlとして生成
	CNumCtrl* pCtrl = new CNumCtrl();
	CNum* pNum;
	int nCount=0;
	list<int>::iterator it;
	for(it=pData->getNumList().begin(); it!=pData->getNumList().end(); it++)
	{
		pNum = new CNum();
		symbolDB_.setNumGui(pNum,*it);
		pCtrl->addNumGui(pNum,nCount++);
	}

	return pCtrl;
}

/////////////////////////////////////////////////
// PanelWidget生成
/////////////////////////////////////////////////
IPanel* CGuiDefDB::createWidgetPanel(IDataGuiDefWidget* pData)
{
	CDataGuiDefWidgetPanel*	pPanelData = static_cast<CDataGuiDefWidgetPanel*>(pData);
	IPanel*					pPanel = createInterfaceCast<IPanel>(pPanelData->getPanelID());
	
	pPanel->setX(pPanelData->getX());
	pPanel->setY(pPanelData->getY());

	return pPanel;
}

/////////////////////////////////////////////////
// シンボル生成
/////////////////////////////////////////////////
Task::ITaskBase* CGuiDefDB::createSymbol(IDataGuiDefWidget* pData)
{
	CDataGuiDefSymbol*	pSymbolData = static_cast<CDataGuiDefSymbol*>(pData);

	#ifdef BMW_DEBUG_INTER
			CDbg().Out("C_SYM %d",pSymbolData->getSymbolID());
	#endif
		
	Task::ITaskBase*	pSymbol = symbolDB_.createSymbol(pSymbolData->getSymbolID());

	pSymbol->setX(pSymbolData->getX());
	pSymbol->setY(pSymbolData->getY());

	return pSymbol;
}

/////////////////////////////////////////////////
// Text生成
/////////////////////////////////////////////////
CText* CGuiDefDB::createText(IDataGuiDefWidget* pData)
{
	CDataGuiDefText*	pTextData = static_cast<CDataGuiDefText*>(pData);

	// Typeの値によって生成するテキスト分岐
	if(pTextData->getType()>=0)
	{
		CText*	pText;
		
		switch(pTextData->getType())
		{
		case Text::POPUP:	pText = new CTextPopUp();	break;
		case Text::SIZE:	pText = new CTextSize();	break;
		default:			pText = new CText();		break;
		}

		pText->setFont(pTextData->getFont());
		pText->setSize(pTextData->getSize());
		pText->setSide(pTextData->getSide());
		pText->setColor(pTextData->getColor());
		if(!pTextData->IsDynamic())
		{// 静的だったらここで生成してしまう
			LPSTR pStr = const_cast<LPSTR>(pTextData->getText().c_str());
			CLineParser::ConvertCR(pStr);
			pText->setText(string(pStr));
			pText->UpdateTextAA();
		}

		pText->setX(pTextData->getX());
		pText->setY(pTextData->getY());

		return pText;
	}
	else
	{
		return createInterfaceCast<GUI::CText>(pTextData->getText());
	}
}

/////////////////////////////////////////////////
// ボタン生成
/////////////////////////////////////////////////
CButton* CGuiDefDB::createWidgetButton(IDataGuiDefWidget* pData)
{
	CDataGuiDefWidgetButton*	pButtonData = static_cast<CDataGuiDefWidgetButton*>(pData);
	CButton* pButton;
	// ボタンIDの設定状態にあわせて、取得する場所を分岐
	if(!pButtonData->getButtonID().empty()) pButton = createButton_ID(pButtonData->getButtonID(),pButtonData->getAct());
	else									pButton = symbolDB_.createButton_ID(pButtonData->getSymbolID(),pButtonData->getAct());

	pButton->setX(pButtonData->getX());
	pButton->setY(pButtonData->getY());

	return pButton;
}

/////////////////////////////////////////////////
// グラフィック生成
/////////////////////////////////////////////////
CGraphic* CGuiDefDB::createWidgetGraphic(IDataGuiDefWidget* pData)
{
	CDataGuiDefWidgetGraphic*	pGraphicData = static_cast<CDataGuiDefWidgetGraphic*>(pData);
	CGraphic* pGraphic = createInterfaceCast<GUI::CGraphic>(pGraphicData->getGraphicID());

	pGraphic->setX(pGraphicData->getX());
	pGraphic->setY(pGraphicData->getY());

	return pGraphic;
}

/////////////////////////////////////////////////
// Obj生成
/////////////////////////////////////////////////
Task::ITaskBase* CGuiDefDB::createObj(IDataGuiDefWidget* pData)
{
	CDataGuiDefObj* pObjData = static_cast<CDataGuiDefObj*>(pData);
	Task::CTaskBase* pObj;
	switch(pObjData->getType())
	{
	case Obj::BUTTON: // ボタンホルダー
		pObj = new GUI::CButtonSymbol();
	break;

	case Obj::KEEP: // ボタンホルダー
		pObj = new GUI::CButtonKeepSymbol();
	break;

	case Obj::GRAPHIC: // グラフィックホルダー
		pObj = new GUI::CGraphicPopUp();
	break;

	default: // プレースホルダ
		pObj = new Task::CTaskBase();
	break;
	}

	pObj->setX(pData->getX());
	pObj->setY(pData->getY());
	return pObj;
}

} // namespace GUI end
} // namespace BMW end