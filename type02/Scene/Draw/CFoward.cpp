#include "stdafx.h"
#include "../../mode.h"

#include "../../Movie/DB/CSymbolDB.h"

#include "../Unit/CHelpUnit.h"

#include "CFoward.h"

namespace BMW{
namespace Scene{

CFoward::CFoward()
{
	pPopUpCtrl_ = new Draw::CPopUpCtrl();
	pSymbolDB_ = new Movie::CSymbolDB();
}

CFoward::~CFoward()
{
	listTask_.clear(); // リスト内をdeleteされたら困るんで
	DELETE_SAFE(pPopUpCtrl_);
	DELETE_SAFE(pSymbolDB_);
}
void CFoward::OnInit(Task::CTaskContext* pContext)
{
	pSymbolDB_->setSymbol(BMW::Config::Const::configDB_.getConfigFileStr("FOWARD"));
	pSymbolDB_->setGraphicGui(&loading_,"BACK_LOAD_G");
	pSymbolDB_->setGraphicGui(&dict_caution_,"JITEN_CAUTION_G");
}

/////////////////////////////////////
// ポップアップ動作
/////////////////////////////////////
void CFoward::createPopUp(GUI::IButton* pButton, Task::CTaskContext* pContext)
{
	// ヘルプフラグによって出したり出さなかったり
	if(!pContext->getApp()->getGlobal().IsHelp()) return;
	// ポップアップをタスクに追加
	Draw::CPopUpBase* pBase = pPopUpCtrl_->createPopUp(pButton,pContext);
	if(pBase!=NULL){ pBase->OnReset(pContext); addTask(pBase,POPUP); }
}

void CFoward::clearPopUp()
{
	removeTask(POPUP);
}

void CFoward::clearPopUpAll()
{
	clearPopUp();
	pPopUpCtrl_->clearPopUp();
}

/////////////////////////////////////
// ロードグラフィック
/////////////////////////////////////
void CFoward::visibleLoad(bool bVisible)
{// 表示非表示で、リストから出し入れ
	if(bVisible && getTask(LOADING)==NULL)
		addTask(&loading_, LOADING);
	else	
		removeTask(LOADING);
}

/////////////////////////////////////
// コーショングラフィック
/////////////////////////////////////
void CFoward::visibleDictCaution(bool bVisible)
{// 表示非表示で、リストから出し入れ
	if(bVisible && getTask(DICT_CAUTION)==NULL)
		addTask(&dict_caution_, DICT_CAUTION);
	else	
		removeTask(DICT_CAUTION);
}

/////////////////////////////////////
// フェーダ動作
/////////////////////////////////////
void CFoward::setFadeColor(COLORREF rgb)
{ 
	fader_.setColor(rgb);
}

void CFoward::fadeIn(int nFrame)
{ 
	fader_.fadeIn(nFrame);
	if(getTask(FADE)==NULL) addTask(&fader_,FADE);
}

void CFoward::fadeOut(int nFrame)
{
	fader_.fadeOut(nFrame);
	if(getTask(FADE)==NULL) addTask(&fader_,FADE);
}

void CFoward::setFaderHandler(const FaderEvent& fun)
{
	fader_.setFaderHandler(fun);
}

bool CFoward::IsFadeEnd()const
{
	return fader_.getState()==Draw::CFader::NORMAL;
}

/////////////////////////////////////
// ヘルプ動作
/////////////////////////////////////
void CFoward::createHelp(int nID, Task::CTaskContext* pContext)
{
	// すでにヘルプが動いてたら何もしない
	if(getTask(HELP)!=NULL) return;

	// ヘルプユニット生成
	Unit::CHelpUnit* pUnit = new Unit::CHelpUnit();
	pContext->push(nID);
	pUnit->OnInit(pContext);
	addTask(pUnit, HELP);
}

} // namespace Scene end
} // namespace BMW end