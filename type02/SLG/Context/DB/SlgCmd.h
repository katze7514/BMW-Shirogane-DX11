/*
	katze 05/06/28
	コマンド生成のための一時データ
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Code{

class CCmdAction
{
public:
	int				getAction()const{ return nAction_; }
	void			setAction(int nAction){ nAction_=nAction; }
	list<int>&		getParam(){ return listParam_; }
	void			addParam(int nParam){ listParam_.push_back(nParam); }
	const string&	getTargetChara()const{ return sTarget_; }
	void			setTargetChara(const string& sTarget)
					{ 
						sTarget_=sTarget;
						// ターゲットSLG IDを入れる証
						listParam_.push_back(-1);
					}

protected:
	int			nAction_;	// Action ID
	list<int>	listParam_;	// アクションパラメタ
	string		sTarget_;	// ↑ようの対象キャラID
};

class CCmdAddChara
{/**
	AddCharaコマンド
 */
public:
	// コンストラクタ
	CCmdAddChara(){}
	// 設定・取得
	int				getID()const{ return nID_; }
	void			setID(int nID){ nID_=nID; }
	const string&	getSlg()const{ return sSlg_; }
	void			setSlg(const string& sSlg){ sSlg_=sSlg; nID_=-1; }
	int				getTrain()const{ return nTrain_; }
	void			setTrain(int nTrain){ nTrain_=nTrain; }
	const string&	getChara()const{ return sChara_; }
	void			setChara(const string& sChara){ sChara_=sChara; }
	int				getPhase()const{ return nPhase_; }
	void			setPhase(int nPhase){ nPhase_=nPhase; }

	CCmdAction&		getActionInfo(){ return action_; }
	void			setActionInfo(const CCmdAction& act){ action_=act; }
	
	bool			IsID()const{ return nID_>=0; }

private:
	int		nID_;		// SLG IDそのもの
	string	sSlg_;		// SLG ID
	int		nTrain_;	// Train ID
	string	sChara_;	// Chara ID
	int		nPhase_;	// 所属フェーズ
	CCmdAction action_; // action情報
};

class CCmdSetWeapon
{/**
	SetWeaponコマンド
 */
public:
	// 設定・取得
	int				getID()const{ return nID_; }
	void			setID(int nID){ nID_=nID; }
	const string&	getSlg()const{ return sSlg_; }
	void			setSlg(const string& sSlg){ sSlg_=sSlg; nID_=-1; }
	
	bool			IsID()const{ return nID_>=0; }

private:
	int		nID_;
	string	sSlg_; // SLG ID
};

class CCmdAddCharaMap
{/**
	AddCharaMapコマンド

	nTypeの情報によって動的な追加を行う
 */
public:
	enum eType{
		NORMAL,
		TARGET,
	};
	// コンストラクタ
	CCmdAddCharaMap():nOn_(-1),nEffect_(0){}
	// 設定・取得
	int				getID()const{ return nID_; }
	void			setID(int nID){ nID_=nID; }
	const string&	getSlg()const{ return sSlg_; }
	void			setSlg(const string& sSlg){ sSlg_=sSlg; nID_=-1; }
	int				getEffect()const{ return nEffect_; }
	void			setEffect(int nEffect){ nEffect_=nEffect; }

	int				getType()const{ return nType_; }
	void			setType(int nType){ nType_=nType; }
	const string&	getTarget()const{ return sTarget_; }
	void			setTarget(const string& sTarget){ sTarget_=sTarget; nType_=TARGET; }
	int				getOn()const{ return nOn_; }
	void			setOn(int nOn){ nOn_=nOn; }

	int				getIndex()const{ return nIndex_; }
	void			setIndex(int nIndex){ nIndex_=nIndex; nType_=NORMAL; }
	int				getWay()const{ return nWay_; }
	void			setWay(int nWay){ nWay_=nWay; }

	bool			IsID()const{ return nID_>=0; }

private:
	int		nID_;
	string	sSlg_;		// SLG ID
	int		nEffect_;	// 追加時に発生させるエフェクト

	int		nType_;		// 追加方法
	string	sTarget_;	// 追加対象ID
	int		nOn_;		// ↑のどこに入るのか
	int		nIndex_;	// 追加Index
	int		nWay_;		// 追加方向
};

/// DelCharaコマンド
typedef CCmdSetWeapon CCmdDelChara;

class CCmdDelCharaMap
{/**
	DelCharaMapコマンド
 */
public:
	// 設定・取得
	int				getID()const{ return nID_; }
	void			setID(int nID){ nID_=nID; }
	const string&	getSlg()const{ return sSlg_; }
	void			setSlg(const string& sSlg){ sSlg_=sSlg; nID_=-1; }
	int				getEffect()const{ return nEffect_; }
	void			setEffect(int nEffect){ nEffect_=nEffect; }

	bool			IsID()const{ return nID_>=0; }

private:
	int		nID_;
	string	sSlg_;
	int		nEffect_;
};

class CCmdMsg
{/**
	Msgコマンド
 */
public:
	// コンストラクタ
	CCmdMsg():bMask_(false){}
	// 設定・取得
	int				getSide()const{ return nSide_; }
	void			setSide(int nSide){ nSide_=nSide; }
	int				getID()const{ return nSlg_; }
	void			setID(int nID){ nSlg_=nID; sChara_.clear(); }
	int				getSlg()const{ return nSlg_; }
	void			setSlg(const string& sChara){ sChara_=sChara; nSlg_=1; }
	const string&	getChara()const{ return sChara_; }
	void			setChara(const string& sChara){ sChara_=sChara; nSlg_=-1; }
	const string&	getFace()const{ return sFace_; }
	void			setFace(const string& sFace){ sFace_=sFace; }
	const string&	getMsg()const{ return sMsg_; }
	void			setMsg(const string& sMsg){ sMsg_=sMsg; }
	bool			IsMask()const{ return bMask_; }
	void			mask(bool bMask){ bMask_=bMask; }

	bool			IsSlg()const{ return nSlg_>=0 && !sChara_.empty(); }
	bool			IsID()const{ return	nSlg_>=0 && sChara_.empty(); }

private:
	int		nSide_;
	int		nSlg_;
	string	sChara_; // FaceMapで定義しているID
	string	sFace_;
	string	sMsg_;
	bool	bMask_;
};

struct CCmdBattle
{/**
	イベント戦闘
 */
	int		nID_;
	bool	bDemo_;
	int		nSide_;
};

struct CCmdChara
{/**
	キャラデータ操作
 */
	string		sChara_;
	int			nKind_;
	int			nType_;
	CCmdAction	action_;

	void addParam(int nValue){ action_.addParam(nValue); }
	void setTarget(const string& sTarget){ action_.setTargetChara(sTarget); }
};

class CCmdSally
{/**
	出撃選択
 */
public:
	// アクセッサ
	int		getMaxNum()const{ return nMaxNum_; }
	void	setMaxNum(int nMaxNum){ nMaxNum_=nMaxNum; }

	int		getIndex()const{ return nIndex_; }
	void	setIndex(int nIndex){ nIndex_=nIndex; }

	void			addChara(const string& sChara){ listChara_.push_back(sChara); }
	list<string>&	getCharaList(){ return listChara_; }

	void			addOut(const string& sOut){ listOut_.push_back(sOut); }
	list<string>&	getOutList(){ return listOut_; }

	void			addIndex(int nIndex){ listIndex_.push_back(nIndex); }
	list<int>&		getIndexList(){ return listIndex_; }

private:
	int nMaxNum_;
	int nIndex_;
	list<string>	listChara_;
	list<string>	listOut_;
	list<int>		listIndex_;
};

struct CCmdVicChange
{
	int nVic_;
	int nLose_;
	int nExpert_;
	int nApper_;

	CCmdVicChange():nApper_(1){}
};

struct CCmdChangeChara
{
	string sID_;
	int nID_;
	string sCharaID_;
	int nTrain_;
	int nFlag_;
	int nOffset_;

	CCmdChangeChara():nID_(-1),nTrain_(-1),nFlag_(0),nOffset_(0){}
};

} // namespace Code end
} // namespace SLG end
} // namespace BMW end