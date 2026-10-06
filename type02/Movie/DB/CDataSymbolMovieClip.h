/*
	katze 05/05/10
	ムービークリップシンボル
*/
#pragma once

#include "../IDMovieCode.h"
#include "IDataSymbol.h"

namespace BMW{
namespace Movie{

class CDataKeyFrame
{/**
	キーフレーム
 */
public:
	enum eKind{
		KEY,
		TWEEN,
	};
	enum eSymbolKind{
		SYMBOL,
		CODE,
	};
	// コンストラクタ・デストラクタ
	CDataKeyFrame():nKind_(KEY),nSymbolKind_(CODE),nID_(Code::NO),nFrame_(1){}
	virtual ~CDataKeyFrame(){}

	// 設定・取得
	int		getKind() const { return nKind_; }
	void	setKind(int nKind){ nKind_=nKind; }
	int		getSymbolKind() const { return nSymbolKind_; }
	void	setSymbolKind(int nKind){ nSymbolKind_=nKind; }
	int		getID() const { return nID_; }
	void	setID(int nID){ nID_=nID; }
	int		getFrame() const { return nFrame_; }
	void	setFrame(int nFrame){ nFrame_=nFrame; }
	const Draw::CDrawInfo&	getDrawInfo() const { return info_; }
	void					setDrawInfo(const Draw::CDrawInfo& info){ info_=info; }

	// Codeのパラメタ設定
	int						getParam()const{ return info_.getX(); }
	void					setParam(int nID){ info_.setX(nID); }
	int						getParam2()const{ return info_.getY(); }
	void					setParam2(int nID){ info_.setY(nID); }

private:
	int				nKind_;
	int				nSymbolKind_;
	int				nID_;
	int				nFrame_;
	Draw::CDrawInfo info_;	// Code実行のパラメタにも使われる
};

class CDataTween : public CDataKeyFrame
{/**
	トゥイーン

	KeyFarmeの	nFrameがステップ数
				info_がStart
				nSymbolが対象に相当する
 */
public:
	// コンストラクタ・デストラクタ
	CDataTween():nEdging_(0){ setKind(TWEEN); }
	virtual ~CDataTween(){}
	// 設定・取得
	const Draw::CDrawInfo&	getEnd() const { return end_; }
	void					setEnd(const Draw::CDrawInfo& end){ end_=end; }
	int						getEdging() const { return nEdging_; }
	void					setEdging(int nEdging){ nEdging_=nEdging; }

private:
	Draw::CDrawInfo end_;
	int				nEdging_;
};

class CDataLayer
{/**
	レイヤー
 */
public:
	typedef list<CDataKeyFrame*> frame_list;
		
	// 操作
	void					addKeyFrame(CDataKeyFrame* frame){ listFrame_.push_back(frame); }
	frame_list::size_type	getSize() const { return listFrame_.size(); }

	frame_list::iterator	beginKeyFrame(){ it=listFrame_.begin(); return it; }
	bool					endKeyFrame(){ return it==listFrame_.end(); }
	frame_list::iterator	nextKeyFrame(){ return it++; }

private:
	frame_list listFrame_;
	frame_list::iterator it;
};

class CDataSymbolMovieClip : public IDataSymbol
{/**
	ムービークリップシンボル
 */
public:
	typedef list<CDataLayer*> layer_list;
	// コンストラクタ・デストラクタ
	CDataSymbolMovieClip():nType_(Clip::NORMAL),IDataSymbol(MOVIE_CLIP){}
	virtual ~CDataSymbolMovieClip();

	int getType()const{ return nType_; }
	void setType(int nType){ nType_=nType;}

	// 操作
	void addLayer(CDataLayer* pLayer){ listLayer_.push_back(pLayer); }
	int getSize() const { return listLayer_.size(); }

	layer_list::iterator	beginLayer(){ it=listLayer_.begin(); return it; }
	bool					endLayer(){ return it==listLayer_.end(); }
	layer_list::iterator	nextLayer(){ return it++; }

	bool IsBorn()const{ return nType_==Clip::BORN; }

private:
	layer_list listLayer_;
	layer_list::iterator it;

	int nType_;
};

} // namespace Movie end
} // namespace BMW end