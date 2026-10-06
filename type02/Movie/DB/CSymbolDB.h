/*
	katze 05/05/10
	update 06/01/16
	シンボルデータベース

	現在は、GUIと統合された
*/
#pragma once

#include "../../GUI/DB/IDGuiDef.h"
#include "../../Draw/DB/CSpriteDB.h"
#include "IDataSymbol.h"

namespace BMW{
namespace Movie{
class CDataSymbolMovieClip;
class CDataKeyFrame;
class CDataSymbolButton;

class CSymbolDB
{/**
	シンボルテーブル

	シンボルとは、

	MovieClip
	CGraphic（Movie仕様）
	トゥイーン

	のいずれかである。
	で、あったのは過去のこと。
	現在では、あらゆるグラフィックオブジェクトを扱う
 */
public:
	typedef map<int, IDataSymbol*> symbol_map;

	// コンストラクタ・デストラクタ
	CSymbolDB():bRead_(false){}
	virtual ~CSymbolDB();

	// 操作
	void			setSymbol(const string& sFile);
	void			setSymbolPre(const string& sFile, const string& sPrefix="");
	void			setSymbolStream(const string& sData, const string& sPrefix="");
	IDataSymbol*	getSymbolData(int nID)
	{ 
		symbol_map::iterator it = mapSymbol_.find(nID);
		return it!=mapSymbol_.end() ? it->second : NULL;
	}
	IDataSymbol*	getSymbolDataStr(const string& sID){ return getSymbolData(symbolID_.getValue(sID));	}
	void			setSymbolData(IDataSymbol* pSymbol, int nID){ mapSymbol_.insert(pair<int, IDataSymbol*>(nID,pSymbol)); }
	void			setSymbolData(IDataSymbol* pSymbol, const string& sID);
	void			clearSymbol();
	void			writeID(const string& sID, int nID){ symbolID_.writeMap(sID,nID); }
	void			writeIDStr(const string& sID){ symbolID_.writeMap(sID,symbolID_.getMapSize()); }
	int				getID(const string& sID){ return symbolID_.getValue(sID); }

	// シンボル生成
	virtual Task::ITaskBase*	createSymbol(int nSymbol);
	virtual Task::ITaskBase*	createSymbolStr(const string& sID);
	virtual Task::ITaskBase*	createCode(int nID,CDataKeyFrame* pData);
	virtual Movie::CMovieClip*	createMovieClip(IDataSymbol* pData, int nType=-1);
	virtual GUI::CGraphic*		createGraphic(IDataSymbol* pData);
	virtual GUI::CButton*		createButton(IDataSymbol* pData,int nAct=GUI::Button::NORMAL);
	virtual GUI::CNum*			createNum(IDataSymbol* pData);

	// ムービークリップ設定
	void						setMovieClip(CMovieClip* pMovie, IDataSymbol* pData);
	void						setMovieClip(CMovieClip* pMovie, CDataSymbolMovieClip* pData);
	virtual	CKeyFrame*			createKeyFrame(bool b=false){ return b ? new CKeyFrameBorn() : new CKeyFrame(); }
	void						setKeyFrame(CKeyFrame* pFrame, CDataKeyFrame* pData);
	virtual	CTween*				createTween(bool b=false){ return b ? new CTweenBorn() : new CTween(); }
	void						setTween(CTween* pTween, CDataKeyFrame* pData);


	// ボタン
	void						setButton(GUI::CButtonGraphic* pButton, CDataSymbolButton* pButtonData);
	void						setButton(GUI::CButtonSymbol* pButton, CDataSymbolButton* pButtonData);
	GUI::CButton*				createButton_ID(int nID,int nAct=GUI::Button::NORMAL);
	GUI::CButton*				createButton_ID(const string& sID,int nAct=GUI::Button::NORMAL);

	// シンボル取得ヘルパ
	template<class T>
	T* createSymbolCast(int nSymbol){ return static_cast<T*>(createSymbol(nSymbol)); }
	template<class T>
	T* createSymbolStrCast(const string& sID){ return static_cast<T*>(createSymbolStr(sID)); }

	// スプライトを設定
	CSpriteDB&	getSpriteDB(){ return sprite_; }
	void		setSprite(CSpriteInfo& info, int nID){ sprite_.setSprite(info,nID); }
	void		setSprite(CSpriteInfo& info, const string& sID){ sprite_.setSprite(info,sID); }

	// グラフィックスプライト設定
	void		setGraphic(GUI::CGraphic* pGraphic, int nID);
	void		setGraphic(GUI::CGraphic* pGraphic, const string& sID);

	// 旧GUIとの互換のため
	// グラフィック設定
	void		setGraphicGui(GUI::CGraphic* pGraphic, int nID, int nX=0, int nY=0);
	void		setGraphicGui(GUI::CGraphic* pGraphic, const string& sID, int nX=0, int nY=0);
	// ボタン設定
	void		setButtonGui(GUI::CButtonGraphic* pButton, int nID, int nX=0, int nY=0);
	void		setButtonGui(GUI::CButtonGraphic* pButton, const string& sID, int nX=0, int nY=0);
	void		setButtonGui(GUI::CButtonSymbol* pButton, int nID, int nX=0, int nY=0);
	void		setButtonGui(GUI::CButtonSymbol* pButton, const string& sID, int nX=0, int nY=0);
	// 数字設定
	void		setNumGui(GUI::CNum* pNum, int nID, int nX=0, int nY=0);
	void		setNumGui(GUI::CNum* pNum, const string& sID, int nX=0, int nY=0);

	// 何か読み込んであるか？
	bool IsRead()const{ return bRead_; }

protected:
	symbol_map					mapSymbol_;
	katzeSDK::Misc::CStringMap	symbolID_;

	CSpriteDB	sprite_;

	// 読み込み済みフラグ
	bool bRead_;
};

} // namespace Movie end
} // namespace BMW end