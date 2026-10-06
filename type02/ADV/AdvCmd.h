/*
	katze 05/05/21
	コマンド生成用設定クラス
*/
#pragma once

namespace BMW{
namespace ADV{

class CCmdMsg
{/**
	メッセージ変更
 */
public:
	// コンストラクタ
	CCmdMsg():nSide_(0),bMask_(false){}

	// 設定・取得
	int				getSide() const { return nSide_; }
	void			setSide(int nSide){ nSide_=nSide; }
	const string&	getChara(){ return sChara_; }
	void			setChara(const string& sChara){ sChara_=sChara; }
	const string&	getFace(){ return sFace_; }
	void			setFace(const string& sFace){ sFace_=sFace; }
	const string&	getText(){ return sText_; }
	void			setText(const string& sText){ sText_=sText; }
	bool			IsMask()const{ return bMask_; }
	void			mask(bool bMask){ bMask_=bMask; }

private:
	int		nSide_;
	string	sChara_;
	string	sFace_;
	string	sText_;
	bool	bMask_;
};

class CCmdBack
{/**
	背景変更
 */
public:
	const string&	getBack(){ return sBack_; }
	void			setBack(const string& sBack){ sBack_=sBack; }

private:
	string sBack_;
};

class CCmdMsgState
{/**
	MSGボードの状態変更
 */
public:
	// コンストラクタ
	CCmdMsgState():nSide_(0),nCtrl_(0),bFlag_(true){}

	// 設定・取得
	int		getSide()const{ return nSide_; }
	void	setSide(int nSide){ nSide_=nSide; }
	int		getCtrl()const{ return nCtrl_; }
	void	setCtrl(int nCtrl){ nCtrl_=nCtrl; }
	bool	IsFlag()const{ return bFlag_; }
	void	flag(bool bFlag){ bFlag_=bFlag; }

private:
	int		nSide_;
	int		nCtrl_;
	bool	bFlag_;
};

class CCmdFade
{/**
	フェードコントロール
 */
public:
	// 設定・取得
	int		getCtrl()const{ return nCtrl_; }
	void	setCtrl(int nCtrl){ nCtrl_=nCtrl; }
	int		getFrame()const{ return nFrame_; }
	void	setFrame(int nFrame){ nFrame_=nFrame; }
	int		getColor()const{ return nColor_; }
	void	setColor(int nColor){ nColor_=nColor; }

private:
	int nCtrl_;
	int nFrame_;
	int nColor_;
};

class CCmdValid
{/**
	有効キャラ操作
 */
public:
	// 設定・取得
	int		getCtrl()const{ return nCtrl_; }
	void	setCtrl(int nCtrl){ nCtrl_=nCtrl; }

	list<string>&	getCharaList(){ return listChara_; }
	void			addCharaList(const string& sChara){ listChara_.push_back(sChara); }

private:
	int nCtrl_;
	list<string> listChara_;
};

struct CCmdChangeChara
{
	string sSource_;
	string sTarget_;
};

struct CCmdTrain
{
	string sTarget_;
	int nKind_;
	int nType_;
	int nValue_;
	string sValue_;
	int nMax_;

	CCmdTrain():nType_(-1),nMax_(-1){}
};

struct CCmdTrainAll
{
	list<string> listTarget_;
	int nCtrl_;
	int nKind_;
	int nType_;
	int nValue_;
	int nMax_;

	void addTarget(const string& sTarget){ listTarget_.push_back(sTarget); }

	CCmdTrainAll():nCtrl_(-1),nType_(-1),nMax_(-1){}
};

struct CCmdItemCtrl{
	int nCtrl_;
	list<pair<int,int> > listItem_;
	void setItem(const pair<int,int>& item){ listItem_.push_back(item); }

	CCmdItemCtrl():nCtrl_(0){}
};

} // namespace ADV end
} // namespace BMW end