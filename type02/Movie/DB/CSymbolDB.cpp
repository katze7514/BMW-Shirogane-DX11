#include "stdafx.h"

#include "../../Sound/Code/CSoundCmdFactory.h"

#include "../../Demo/CDemoMovieClip.h"

#include "../IDMovieCode.h"
#include "../MovieCode.h"

#include "../CScriptExecSymbol.h"

#include "../Code/CSymbolCmdFactory.h"

#include "CSymbolParser.h"
#include "CSymbolDB.h"

namespace BMW{
namespace Movie{

#ifdef BMW_DEBUG
//#define BMW_DEBUG_SYMBOL
#endif

CSymbolDB::~CSymbolDB()
{
	clearSymbol();
}

void CSymbolDB::setSymbol(const string& sFile)
{
	setSymbolPre(sFile);
}

void CSymbolDB::setSymbolPre(const string& sFile,const string& sPrefix)
{
	using namespace boost::spirit;
	using namespace phoenix;

#ifdef BMW_DEBUG_SYMBOL
	CDbg().Out("SYMBOL %s",sFile.c_str());
#endif
	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(sFile);
	while(file.ReadLine(s)==0) p.append(s); 
	file.Close();

	setSymbolStream(p,sPrefix);

	bRead_=true;
}

void CSymbolDB::setSymbolStream(const string& sData,const string& sPrefix)
{
	// 構文解析
	CSymbolParser ps(*this,sprite_,sPrefix);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG_SYMBOL
	parse_info<> r = 
#endif
	parse(sData.c_str(), ps, skip);
#ifdef BMW_DEBUG_SYMBOL
	if(!r.full){ CDbg().Out("SYMBOL 読み込み失敗！！"); CDbg().Out(r.stop);}
#endif
}

void CSymbolDB::clearSymbol()
{
	symbol_map::iterator it;
	for(it=mapSymbol_.begin(); it!=mapSymbol_.end(); it++)
		DELETE_SAFE(it->second);

	mapSymbol_.clear();
	sprite_.clearSpriteDB();
	symbolID_.clearMap();
	bRead_=false;
}

void CSymbolDB::setSymbolData(IDataSymbol* pSymbol, const string& sID)
{
	#ifdef BMW_DEBUG_SYMBOL
	if(getID(sID)>0)
		CDbg().Out("SYMBOL %s はすでに存在します",sID.c_str());
	#endif

	writeIDStr(sID);

	/*#ifdef BMW_DEBUG
		CDbg().Out("SYMBOL %d %s",getID(sID),sID.c_str());
	#endif*/

	mapSymbol_.insert(pair<int, IDataSymbol*>(getID(sID),pSymbol)); 
}
//////////////////////////////////////////////////
// 各シンボルに対する生成子
//////////////////////////////////////////////////
Task::ITaskBase* CSymbolDB::createCode(int nID, CDataKeyFrame* pData)
{
	switch(nID)
	{
	case Code::SE:
	{// SE再生コード生成
	 // DrawInfoのXが再生するSE ID
		CScriptExecSymbol* pExec = new CScriptExecSymbol();
		VM::CScript*	 pScript = new VM::CScript();
		pExec->setScript(smart_ptr<VM::CScript>(pScript));
		Sound::Code::CSoundCmdFactory().createSe(pData->getParam(), pData->getParam2(),pScript);
		return pExec;
	}

	case Code::SE_WAIT:
	{// SE再生終了待ちコード生成
	 // DrawInfoのXが再生するSE ID
		CScriptExecSymbol* pExec = new CScriptExecSymbol();
		VM::CScript*	 pScript = new VM::CScript();
		pExec->setScript(smart_ptr<VM::CScript>(pScript));
		CSymbolCmdFactory::createSeWait(pData->getParam(),pScript);
		return pExec;
	}

	case Code::BGM:
	{// BGM再生コード生成
	 // DrawInfoのXが再生するBGM ID
		CScriptExecSymbol* pExec = new CScriptExecSymbol();
		VM::CScript*	 pScript = new VM::CScript();
		pExec->setScript(smart_ptr<VM::CScript>(pScript));
		// とりあえず、フェードは固定30フレーム（1秒）
		// 変える必要があったら、変更できるようにするっていう程度で
		Sound::Code::CSoundCmdFactory().createBgm(pData->getParam(), pData->getParam2(), 30,pScript);
		return pExec;
	}

	case Code::STOP_MOVIE:
		return new Code::CCode_movie_stop();

	case Code::END:
		return new Code::CCode_movie_end();

	default: return new Task::ITaskBase();
	}
}
//////////////////////////////////////////////////
// シンボル
//////////////////////////////////////////////////
Task::ITaskBase* CSymbolDB::createSymbol(int nSymbol)
{
	IDataSymbol* pData = getSymbolData(nSymbol);
	if(pData==NULL)
	{
		#ifdef BMW_DEBUG_SYMBOL
		CDbg().Out("NO_SYMBOL %d",nSymbol);
		#endif

		return NULL;
	}
	switch(pData->getKind())
	{
	case IDataSymbol::GRAPHIC:
		//CDbg().Out("GRAPHIC %d",nSymbol);
		return createGraphic(pData);

	case IDataSymbol::BUTTON:
		//CDbg().Out("BUTTON %d",nSymbol);
		return createButton(pData);

	case IDataSymbol::NUM:
		return createNum(pData);

	case IDataSymbol::MOVIE_CLIP:
		//CDbg().Out("MOVIE %d",nSymbol);
		return createMovieClip(pData);

	default: return NULL;
	}
}

Task::ITaskBase* CSymbolDB::createSymbolStr(const string& sID)
{
#ifdef BMW_DEBUG_SYMBOL
	Task::ITaskBase* pData = createSymbol(symbolID_.getValue(sID));
	if(pData==NULL) CDbg().Out("NO_SYMBOL %s",sID.c_str());
	return pData;
#else
	return createSymbol(symbolID_.getValue(sID));
#endif
}

Movie::CMovieClip* CSymbolDB::createMovieClip(IDataSymbol* pData, int nType)
{
	CDataSymbolMovieClip* pMovieData = static_cast<CDataSymbolMovieClip*>(pData);
	CMovieClip* pClip;

#ifdef BMW_DEBUG
	// nTypeが設定されてたら強制的にそちらを使う
	if(nType<0) 
#endif
	nType = pMovieData->getType();
	switch(nType)
	{
	case Clip::DEMO:	pClip = new Demo::CDemoMovieClip(); break;
	case Clip::BORN:	pClip = new CMovieClipBorn(); break;
	default:			pClip = new CMovieClip(); break;
	}

	setMovieClip(pClip,pMovieData);
	return pClip;
}

////////////////////////////////////////////////////////////////////////////
// グラフィック
////////////////////////////////////////////////////////////////////////////
GUI::CGraphic* CSymbolDB::createGraphic(IDataSymbol* pData)
{// グラフィック設定
	CDataSymbolGraphic* pGraphicData = static_cast<CDataSymbolGraphic*>(pData);
	GUI::CGraphic* pGraphic;
	// グラフィック種別によって生成するグラフィックを変える
	switch(pGraphicData->getGuiKind())
	{
	case CDataSymbolGraphic::SIZE:
		pGraphic = new GUI::CGraphicSize();
	break;

	case CDataSymbolGraphic::ROTATE:
		pGraphic = new GUI::CGraphicRotate();
	break;

	case CDataSymbolGraphic::ROTATE2:
	case CDataSymbolGraphic::ROTATE3:
		pGraphic = new GUI::CGraphicRotate3();
	break;

	case CDataSymbolGraphic::MORPH:
		pGraphic = new GUI::CGraphicMorph();
	break;

	case CDataSymbolGraphic::AFFINE:
		pGraphic = new GUI::CGraphicAffine();
	break;

	default:
		pGraphic = new GUI::CGraphic();
	break;
	}

	setGraphic(pGraphic,pGraphicData->getID(0));
	return pGraphic;
}

void CSymbolDB::setGraphic(GUI::CGraphic* pGraphic, int nID)
{
	sprite_.setSprite(const_cast<Draw::CSpriteInfo&>(pGraphic->getSpriteInfo()),nID);
}

void CSymbolDB::setGraphic(GUI::CGraphic* pGraphic, const string& sID)
{
	sprite_.setSprite(const_cast<Draw::CSpriteInfo&>(pGraphic->getSpriteInfo()),sID);
}

////////////////////////////////////////////////////////////////////////////
// ボタン
////////////////////////////////////////////////////////////////////////////
GUI::CButton* CSymbolDB::createButton(IDataSymbol* pData,int nAct)
{
	CDataSymbolButton* pButtonData = static_cast<CDataSymbolButton*>(pData);

	if(pButtonData->getGuiKind()==CDataSymbolButton::SYMBOL)
	{// 中がシンボル
		GUI::CButtonSymbol* pButton = nAct==GUI::Button::NORMAL ? new GUI::CButtonSymbol() : new GUI::CButtonKeepSymbol();
		setButton(pButton,pButtonData);	
		return pButton;
	}
	else
	{// 中がスプライト
		GUI::CButtonGraphic* pButton = nAct==GUI::Button::NORMAL ? new GUI::CButtonGraphic() : new GUI::CButtonKeep();
		setButton(pButton,pButtonData);		
		return pButton;
	}
}

GUI::CButton* CSymbolDB::createButton_ID(int nID,int nAct)
{
	return createButton(getSymbolData(nID),nAct);
}

GUI::CButton* CSymbolDB::createButton_ID(const string& sID,int nAct)
{
	return createButton(getSymbolData(symbolID_.getValue(sID)),nAct);
}

void CSymbolDB::setButton(GUI::CButtonGraphic* pButton, CDataSymbolButton* pButtonData)
{
	// ボタン設定
	pButton->setRange(pButtonData->getRect());
		
	// スプライト設定
	for(int i=0; i<3; i++)
		sprite_.setSprite(const_cast<Draw::CSpriteInfo&>(pButton->getSpriteInfo(i)),pButtonData->getID(i));
}

void CSymbolDB::setButton(GUI::CButtonSymbol* pButton, CDataSymbolButton* pButtonData)
{
	// ボタン設定
	pButton->setRange(pButtonData->getRect());
	
	Task::ITaskBase* pBase;
	// NORMAL
	pBase = createSymbol(pButtonData->getID(GUI::CButtonSymbol::NORMAL));
	pButton->setSymbol(smart_ptr<Task::ITaskBase>(pBase==NULL ? new Task::ITaskBase() : pBase),GUI::CButtonSymbol::NORMAL);

	// OVER
	pBase = createSymbol(pButtonData->getID(GUI::CButtonSymbol::OVER));
	pButton->setSymbol(smart_ptr<Task::ITaskBase>(pBase==NULL ? new Task::ITaskBase() : pBase),GUI::CButtonSymbol::OVER);

	// PUSH
	pBase = createSymbol(pButtonData->getID(GUI::CButtonSymbol::PRESS));
	pButton->setSymbol(smart_ptr<Task::ITaskBase>(pBase==NULL ? new Task::ITaskBase() : pBase),GUI::CButtonSymbol::PRESS);
}

////////////////////////////////////////////////////////////////////////////
// 数字
////////////////////////////////////////////////////////////////////////////
GUI::CNum* CSymbolDB::createNum(IDataSymbol* pData)
{
	CDataSymbolNum* pNumData = static_cast<CDataSymbolNum*>(pData);
	GUI::CNum* pNum = new GUI::CNum();
		
	for(int i=0; i<12; i++)
		sprite_.setSprite(const_cast<Draw::CSpriteInfo&>(pNum->getSpriteInfo(i)),pNumData->getID(i));
		
	return pNum;
}
////////////////////////////////////////////////////////////////////////////
// ムービークリップ
////////////////////////////////////////////////////////////////////////////
void CSymbolDB::setMovieClip(CMovieClip* pMovie, IDataSymbol* pData)
{
	setMovieClip(pMovie, static_cast<CDataSymbolMovieClip*>(pData));
}

void CSymbolDB::setMovieClip(CMovieClip* pMovie, CDataSymbolMovieClip* pMovieData)
{// MovieClip設定
	CDataSymbolMovieClip::layer_list::iterator it_l;
	CDataLayer::frame_list::iterator it_f;

	// Layer
	CLayer* pLayer;
	int		nLayer=0;

	// Frame
	CKeyFrame*		pFrame;
	CTween*			pTween;
	int				nFrame;

	pMovieData->beginLayer();
	while(!pMovieData->endLayer())
	{// Layer設定
		it_l = pMovieData->nextLayer();
		pLayer = new CLayer();
		pMovie->addTask(pLayer,nLayer++);

		// Frame
		nFrame=0;
		pLayer->resizeFrame((*it_l)->getSize());
		(*it_l)->beginKeyFrame();
		while(!(*it_l)->endKeyFrame())
		{
			it_f=(*it_l)->nextKeyFrame();
			switch((*it_f)->getKind())
			{
			case CDataKeyFrame::TWEEN:
			// Tween生成
				pTween = createTween(pMovieData->IsBorn());
				setTween(pTween, *it_f);
				pLayer->setKeyFrame(pTween,nFrame++);
			break;

			default:
				// KeyFrame生成
				pFrame = createKeyFrame(pMovieData->IsBorn());
				setKeyFrame(pFrame, *it_f);
				pLayer->setKeyFrame(pFrame,nFrame++);
			break;
			}
		}
	}
}

void CSymbolDB::setKeyFrame(CKeyFrame* pFrame, CDataKeyFrame* pData)
{
	int					nSymbol = pData->getID();
	Task::ITaskBase*	pSymbol = NULL;
		
	if(pData->getSymbolKind()==CDataKeyFrame::SYMBOL)
	{// シンボル
		if(nSymbol!=INT_MAX)
		{
			pSymbol = createSymbol(nSymbol);
		}
		pFrame->setDrawInfo(pData->getDrawInfo());
	}
	else
	{// コード
		pSymbol = createCode(pData->getID(),pData);
	}

	pFrame->setTask(pSymbol);
	pFrame->share(pSymbol==NULL);
	pFrame->setState(pData->getFrame());
}

void CSymbolDB::setTween(CTween* pTween, CDataKeyFrame* pData)
{	
	CDataTween*	pTweenData = static_cast<CDataTween*>(pData);
	pTween->setState(pTweenData->getFrame());

	int					nSymbol = pTweenData->getID();
	Task::ITaskBase*	pSymbol = NULL;

	if(nSymbol!=INT_MAX)
	{
		pSymbol = createSymbol(nSymbol);
	}
	
	pTween->setTask(pSymbol);
	pTween->share(pSymbol==NULL);
	pTween->setDrawInfo(pTweenData->getDrawInfo());

	CMotion	motion;
	motion.setStart(pTweenData->getDrawInfo());
	motion.setCurrent(pTweenData->getDrawInfo());
	motion.setEnd(pTweenData->getEnd());
	motion.setStep(pTweenData->getFrame());
	motion.setEdging(pTweenData->getEdging());

	pTween->setMotion(motion);
}

////////////////////////////////////////////////////////////////////////////////
// GUIとしての設定子
////////////////////////////////////////////////////////////////////////////////
// グラフィック
void CSymbolDB::setGraphicGui(GUI::CGraphic* pGraphic, int nID, int nX, int nY)
{
	IDataSymbol* pData = getSymbolData(nID);
#ifdef BMW_DEBUG_SYMBOL
	if(pData==NULL)
	{
		CDbg().Out("NO_SYMBOL %d",nID);
		//return;
	}
#endif
	CDataSymbolGraphic* pGraphicData = static_cast<CDataSymbolGraphic*>(pData);
	setGraphic(pGraphic,pGraphicData->getID(0));
	if(nX!=0) pGraphic->setX(nX);
	if(nY!=0) pGraphic->setY(nY);
}

void CSymbolDB::setGraphicGui(GUI::CGraphic* pGraphic, const string& sID, int nX, int nY)
{
#ifdef BMW_DEBUG
	IDataSymbol* pData = getSymbolData(symbolID_.getValue(sID));
	if(pData==NULL)
	{
		CDbg().Out("NO_SYMBOL %s",sID.c_str());
		//return;
	}
#endif
	setGraphicGui(pGraphic,symbolID_.getValue(sID),nX,nY);
}

// ボタン設定
void CSymbolDB::setButtonGui(GUI::CButtonGraphic* pButton, int nID, int nX, int nY)
{
	IDataSymbol* pData = getSymbolData(nID);
#ifdef BMW_DEBUG
	if(pData==NULL)
	{
		CDbg().Out("NO_SYMBOL %d",nID);
		//return;
	}
#endif
	CDataSymbolButton* pButtonData = static_cast<CDataSymbolButton*>(pData);

	// 設定
	setButton(pButton,pButtonData);

	if(nX!=0) pButton->setX(nX);
	if(nY!=0) pButton->setY(nY);
}

void CSymbolDB::setButtonGui(GUI::CButtonGraphic* pButton, const string& sID, int nX, int nY)
{
#ifdef BMW_DEBUG_SYMBOL
	IDataSymbol* pData = getSymbolData(symbolID_.getValue(sID));
	if(pData==NULL)
	{
		CDbg().Out("NO_SYMBOL %s",sID.c_str());
		//return;
	}
#endif
	setButtonGui(pButton,symbolID_.getValue(sID),nX,nY);
}

void CSymbolDB::setButtonGui(GUI::CButtonSymbol* pButton, int nID, int nX, int nY)
{
	IDataSymbol* pData = getSymbolData(nID);
#ifdef BMW_DEBUG_SYMBOL
	if(pData==NULL)
	{
		CDbg().Out("NO_SYMBOL %d",nID);
		//return;
	}
#endif
	CDataSymbolButton* pButtonData = static_cast<CDataSymbolButton*>(pData);
	// 設定
	setButton(pButton,pButtonData);

	if(nX!=0) pButton->setX(nX);
	if(nY!=0) pButton->setY(nY);
}

void CSymbolDB::setButtonGui(GUI::CButtonSymbol* pButton, const string& sID, int nX, int nY)
{
#ifdef BMW_DEBUG_SYMBOL
	IDataSymbol* pData = getSymbolData(symbolID_.getValue(sID));
	if(pData==NULL)
	{
		CDbg().Out("NO_SYMBOL %s",sID.c_str());
		//return;
	}
#endif
	setButtonGui(pButton,symbolID_.getValue(sID),nX,nY);
}


// 数字設定
void CSymbolDB::setNumGui(GUI::CNum* pNum, int nID, int nX, int nY)
{
	
	IDataSymbol* pData = getSymbolData(nID);
#ifdef BMW_DEBUG_SYMBOL
	if(pData==NULL)
	{
		CDbg().Out("NO_SYMBOL %d",nID);
		//return;
	}
#endif
	CDataSymbolNum* pNumData = static_cast<CDataSymbolNum*>(pData);

	for(int i=0; i<12; i++)
		sprite_.setSprite(const_cast<Draw::CSpriteInfo&>(pNum->getSpriteInfo(i)),pNumData->getID(i));

	if(nX!=0) pNum->setX(nX);
	if(nY!=0) pNum->setY(nY);
}

void CSymbolDB::setNumGui(GUI::CNum* pNum, const string& sID, int nX, int nY)
{
#ifdef BMW_DEBUG_SYMBOL
	IDataSymbol* pData = getSymbolData(symbolID_.getValue(sID));
	if(pData==NULL)
	{
		CDbg().Out("NO_SYMBOL %s",sID.c_str());
		//return;
	}
#endif
	setNumGui(pNum,symbolID_.getValue(sID),nX,nY);
}

} // namespace Movie end
} // namespace BMW end