#include "stdafx.h"

#include "../../Scene/GUI/CNumCtrl.h"
#include "../../Scene/GUI/CNumRemain.h"
#include "../../Scene/GUI/CGage.h"
#include "../../Scene/GUI/CCircleMenu.h"
#include "../../Scene/GUI/CCircleMenuButton.h"
#include "../../Scene/GUI/CInterfaceLayer.h"

#include "IDGuiDef.h"
#include "CGuiDefParser.h"
#include "CGuiDefDB.h"

namespace BMW{
namespace GUI{

CGuiDefDB::~CGuiDefDB()
{
	guidef_map::iterator it;
	for(it=mapGuiDef_.begin(); it!=mapGuiDef_.end(); it++)
		DELETE_SAFE(it->second);

	mapGuiDef_.clear();
}

void CGuiDefDB::setGuiDef(const string& sFile)
{
#ifdef BMW_DEBUG_INTER
	CDbg().Out("GUIDEF %s",sFile.c_str());
#endif

	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(sFile);
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	setGuiDefStream(p);
}

void CGuiDefDB::setGuiDefStream(const string& sData)
{
	using namespace boost::spirit;
	using namespace phoenix;

	// 構文解析
	CGuiDefParser ps(*this,symbolDB_);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(sData.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("%s 読み込み失敗！！", r.stop);
#endif
}

//////////////////////////////////////////////////////
// 生成
//////////////////////////////////////////////////////
Task::ITaskBase* CGuiDefDB::createInterface(const string& sID)
{
	IDataGuiDef* pData = getGuiDefData(sID);
#ifdef BMW_DEBUG_INTER
	if(pData==NULL)
	{
		CDbg().Out("GUI DEF %s の定義データが存在しません",sID.c_str());
		return NULL;
	}
	else
	{
		CDbg().Out("GUI DEF %s",sID.c_str());
	}
#endif
	switch(pData->getKind())
	{// 種別による生成関数のディスパッチ
	case Def::REMAIN:	return createRemain(pData);
	case Def::GAGE:		return createGage(pData);
	case Def::PANEL:	return createPanel(pData);
	case Def::ACTION:	return createAction(pData);
	case Def::CIRCLE:	return createCircle(pData);
	case Def::BUTTON:	return createButton(pData);
	case Def::TEXT:		return createTextGui(pData);
	case Def::GRAPHIC:	return createGraphic(pData);
	default:			return NULL;
	}
}

//////////////////////////////////////////////////////
// リメイン
//////////////////////////////////////////////////////
CNumRemain*	CGuiDefDB::createRemain(IDataGuiDef* pData)
{
	CDataGuiDefRemain* pRemainDef = static_cast<CDataGuiDefRemain*>(pData);
	CNumRemain* pRemain = new CNumRemain();

	// カレント
	// 通常時
	CNum* pNum = new GUI::CNum();
	symbolDB_.setNumGui(pNum,
						pRemainDef->getCurrentNumN());
	pRemain->getCurrentNumGui()->addNumGui(pNum, CNumRemain::WHITE);
	// 危険時
	pNum = new GUI::CNum();
	symbolDB_.setNumGui(pNum,
						pRemainDef->getCurrentNumE());
	pRemain->getCurrentNumGui()->addNumGui(pNum, CNumRemain::RED);

	// MAX
	if(pRemainDef->getMaxNum(CDataGuiDefRemain::SYMBOL)>0)
	{
		symbolDB_.setNumGui(pRemain->getMaxNumGui(),
							pRemainDef->getMaxNum(CDataGuiDefRemain::SYMBOL),
							pRemainDef->getMaxNum(CDataGuiDefRemain::X),
							pRemainDef->getMaxNum(CDataGuiDefRemain::Y));
	}
	else
	{// シンボルが指定されてないなら非動作
		pRemain->getMaxNumGui()->valid(false);
		pRemain->getMaxNumGui()->visible(false);
	}

	// スラッシュ
	if(pRemainDef->getSlash(CDataGuiDefRemain::SYMBOL)>0)
	{
		symbolDB_.setGraphicGui(pRemain->getSlashGui(),
								pRemainDef->getSlash(CDataGuiDefRemain::SYMBOL),
								pRemainDef->getSlash(CDataGuiDefRemain::X),
								pRemainDef->getSlash(CDataGuiDefRemain::Y));
	}
	else
	{// シンボルが指定されてないなら非動作
		pRemain->getSlashGui()->valid(false);
		pRemain->getSlashGui()->visible(false);
	}

	// Turn
	pRemain->setTurn(pRemainDef->getTurn());

	return pRemain;
}

////////////////////////////////////////////////////////
// ゲージ
////////////////////////////////////////////////////////
CGage* CGuiDefDB::createGage(IDataGuiDef* pData)
{
	CDataGuiDefGage* pGageDef = static_cast<CDataGuiDefGage*>(pData);
	CGage* pGage = new CGage();

	// ゲージグラフィック
	symbolDB_.setGraphicGui(pGage->getGageGui(),
							pGageDef->getCurrentGage());

	// 数字リメイン
	pGage->setRemain(createInterfaceCast<CNumRemain>(pGageDef->getRemain()));
	pGage->getRemain()->setX(pGageDef->getRemainX());
	pGage->getRemain()->setY(pGageDef->getRemainY());

	// 減る方向
	pGage->left(pGageDef->IsLeft());

	return pGage;
}

////////////////////////////////////////////////
// サークルメニュー
////////////////////////////////////////////////
CCircleMenu* CGuiDefDB::createCircle(IDataGuiDef* pData)
{
	CDataGuiDefCircle* pCircleDef = static_cast<CDataGuiDefCircle*>(pData);
	CCircleMenu* pCircle = new CCircleMenu();

	// POS設定
	pCircle->setX(pCircleDef->getX());
	pCircle->setY(pCircleDef->getY());

	// 半径
	pCircle->setR(pCircleDef->getR());
	// 動作フレーム数	
	pCircle->setIntro(pCircleDef->getIntro());
	pCircle->setExit(pCircleDef->getExit());

	// widget設定
	IDataGuiDefWidget *pWidget;
	CCircleMenuButton* pButton;
	pCircleDef->beginWidget();
	while(!pCircleDef->endWidget())
	{
		pWidget = *pCircleDef->nextWidget();
		#ifdef BMW_DEBUG_INTER
			CDbg().Out("C_WID %s",pWidget->getID().c_str());
		#endif
		pButton = new GUI::CCircleMenuButton();
		pButton->setTask(createWidget(pWidget));
		pCircle->addButton(pButton,pWidget->getID());
	}

	return pCircle;
}
////////////////////////////////////////////////
// ボタン
////////////////////////////////////////////////
CButton* CGuiDefDB::createButton(IDataGuiDef* pData,int nAct)
{
	CDataGuiDefButton* pButtonData = static_cast<CDataGuiDefButton*>(pData);

	// ボタン生成
	CButton* pButton = symbolDB_.createButton_ID(pButtonData->getSymbolID(),nAct);

	// ポップアップ設定
	pButton->setPopUp(pButtonData->getPopUp());

	return pButton;
}

CButton* CGuiDefDB::createButton_ID(const string& sID,int nAct)
{
	CDataGuiDefButton* pButtonData = static_cast<CDataGuiDefButton*>(getGuiDefData(sID));

	// ボタン生成
	CButton* pButton = symbolDB_.createButton_ID(pButtonData->getSymbolID(),nAct);

	// ポップアップ設定
	pButton->setPopUp(pButtonData->getPopUp());

	return pButton;
}

////////////////////////////////////////////////
// グラフィック
////////////////////////////////////////////////
CGraphic* CGuiDefDB::createGraphic(IDataGuiDef* pData)
{
	CDataGuiDefGraphic* pGraphicData = static_cast<CDataGuiDefGraphic*>(pData);

	// ボタン生成
	CGraphicPopUp* pGraphic = new CGraphicPopUp();

	// グラフィック設定
	symbolDB_.setGraphicGui(pGraphic,pGraphicData->getSymbolID());

	// ポップアップ設定
	pGraphic->setPopUp(pGraphicData->getPopUp());

	// Range設定
	const Draw::CSpriteInfo& info = pGraphic->getSpriteInfo();
	RECT rect;
	::SetRect(&rect, -info.getX(), -info.getY(), info.getWidth()-info.getX(), info.getHeight()-info.getY());
	pGraphic->setRange(rect);

	return pGraphic;
}

////////////////////////////////////////////////
// テキスト
////////////////////////////////////////////////
CText* CGuiDefDB::createTextGui(IDataGuiDef* pData)
{
	CDataGuiDefTextGui* pTextData = static_cast<CDataGuiDefTextGui*>(pData);
	CText* pText;

	switch(pTextData->getType())
	{
	case Text::SIZE:
		pText = new CTextSize();
	break;

	default:
		CTextPopUp* pTextPop = new CTextPopUp();
		pTextPop->setPopUp(pTextData->getPopUp());
		pText = pTextPop;
	break;
	}
	LPSTR pStr = const_cast<LPSTR>(pTextData->getText().c_str());
	CLineParser::ConvertCR(pStr);
	pText->setText(string(pStr));
	pText->setFont(pTextData->getFont());
	pText->setSize(pTextData->getSize());
	pText->setSide(pTextData->getSide());
	pText->setColor(pTextData->getColor());
	pText->UpdateTextAA();

	return pText;
}

////////////////////////////////////////////////
// アクション
////////////////////////////////////////////////
CInterfaceLayer* CGuiDefDB::createAction(IDataGuiDef* pData)
{
	CDataGuiDefAction *pAction = static_cast<CDataGuiDefAction*>(pData);
	CInterfaceLayer* pLayer = new CInterfaceLayer();

	// パネルの設定
	pLayer->setPanel(createPanel(getGuiDefData(pAction->getTargetID())));
	// 動作設定
	pLayer->resizeKeyFrameVec(pAction->getActionSize());
	int n=0;
	CDataGuiDefActionState* pState;
	pAction->beginAction();
	while(!pAction->endAction())
	{
		pState = *pAction->nextAction();
		if(pState->getAction()==CDataGuiDefActionState::STATE)
		{
			Movie::CKeyFrame* pFrame = new Movie::CKeyFrame();
			pFrame->setDrawInfo(pState->getDrawInfo());
			pLayer->setKeyFrame(pFrame,n++);
		}
		else
		{
			CDataGuiDefActionTween* pTweenData = static_cast<CDataGuiDefActionTween*>(pState);
			Movie::CTween* pTween = new Movie::CTween();
			Movie::CMotion motion;

			motion.setStart(pTweenData->getDrawInfo());
			motion.setEnd(pTweenData->getEnd());
			motion.setStep(pTweenData->getFrame());
			motion.setEdging(pTweenData->getEdging());
			motion.reset();

			pTween->setMotion(motion);
			pLayer->setKeyFrame(pTween,n++);
		}
	}

	return pLayer;
}

/////////////////////////////////////////////////
// パネル生成
/////////////////////////////////////////////////
IPanel* CGuiDefDB::createPanel(IDataGuiDef* pData)
{
	CDataGuiDefPanel* pPanelDef = static_cast<CDataGuiDefPanel*>(pData);
	// 種別によって生成されるパネルが代わるB
	switch(pPanelDef->getPanelType())
	{
	case Panel::CTRL:	return createPanelType<CPanelCtrl>(pPanelDef);
	default:			return createPanelType<CPanel>(pPanelDef);
	}
}

template<class Panel>
Panel*	CGuiDefDB::createPanelType(CDataGuiDefPanel* pData)
{
	Panel* pPanel = new Panel();

	// widget設定
	IDataGuiDefWidget *pWidget;
	pData->beginWidget();
	while(!pData->endWidget())
	{
		pWidget = *pData->nextWidget();
		#ifdef BMW_DEBUG_INTER
			CDbg().Out("C_WID %s",pWidget->getID().c_str());
		#endif
		pPanel->addWidget(createWidget(pWidget),pWidget->getID());
	}
	// POS設定
	pPanel->setX(pData->getX());
	pPanel->setY(pData->getY());

	return pPanel;
}

///////////////////////////////////////////////////////////////////////////////
void CGuiDefDB::setGraphicHolder(CGraphicPopUp* pGraphic, const string& s)
{
	CDataGuiDefGraphic* pGraphicData = static_cast<CDataGuiDefGraphic*>(getGuiDefData(s));
	//CDbg().Out("%s %d",s.c_str(),pGraphicData->getSymbolID());

	// グラフィック設定
	symbolDB_.setGraphicGui(pGraphic,pGraphicData->getSymbolID());

	// ポップアップ設定
	pGraphic->setPopUp(pGraphicData->getPopUp());
	// レンジは画像の大きさ
	const CSpriteInfo& info = pGraphic->getSpriteInfo();
	RECT rect;
	rect.left=-info.getX(); rect.top=-info.getY();
	rect.right=info.getWidth()-info.getX(); rect.bottom=info.getHeight()-info.getY();
	pGraphic->setRange(rect);
}

void CGuiDefDB::setTextHolder(CTextPopUp* pText, const string& s)
{
	CDataGuiDefTextGui* pTextData = static_cast<CDataGuiDefTextGui*>(getGuiDefData(s));

	pText->setText(pTextData->getText());
	pText->setFont(pTextData->getFont());
	pText->setSize(pTextData->getSize());
	pText->setSide(pTextData->getSide());
	pText->setColor(pTextData->getColor());
	pText->setPopUp(pTextData->getPopUp());
}

void CGuiDefDB::setTextPopUpHolder(CTextPopUp* pText, const string& s)
{// テキストの内容とポップアップ内容だけをコピーする
	CDataGuiDefTextGui* pTextData = static_cast<CDataGuiDefTextGui*>(getGuiDefData(s));
	pText->setText(pTextData->getText());
	pText->setPopUp(pTextData->getPopUp());
}

void CGuiDefDB::setButtonHolder(CButtonGraphic* pButton, const string& s)
{
	CDataGuiDefButton* pButtonData = static_cast<CDataGuiDefButton*>(getGuiDefData(s));

	// ボタン生成
	symbolDB_.setButtonGui(pButton,pButtonData->getSymbolID());

	// ポップアップ設定
	pButton->setPopUp(pButtonData->getPopUp());
}

void CGuiDefDB::setButtonHolder(CButtonSymbol* pButton, const string& s)
{
	CDataGuiDefButton* pButtonData = static_cast<CDataGuiDefButton*>(getGuiDefData(s));

	// ボタン生成
	symbolDB_.setButtonGui(pButton,pButtonData->getSymbolID());

	// ポップアップ設定
	pButton->setPopUp(pButtonData->getPopUp());
}

void CGuiDefDB::setButtonHolder(CButtonSymbol* pButton, const string& s, const CButton::ButtonEvent& fun, int nValue)
{
	CDataGuiDefButton* pButtonData = static_cast<CDataGuiDefButton*>(getGuiDefData(s));

	// ボタン生成
	symbolDB_.setButtonGui(pButton,pButtonData->getSymbolID());

	// ポップアップ設定
	pButton->setPopUp(pButtonData->getPopUp());

	// イベントハンドラ設定
	pButton->setEventHandler(fun);

	// 値設定
	pButton->getEvent()->setValue(nValue);
}

} // namespace GUI end
} // namespace BMW end