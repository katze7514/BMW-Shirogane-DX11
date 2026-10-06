/*
	katze 05/03/13
	実行中のゲームデータ
*/
#pragma once

#include "CExecDataHead.h"

namespace BMW{

namespace Chara{
class CDataCharaTrain;
} // namespace Chara end

namespace Save{

class CExecData : public IArchive
{/**
	つまり、こいつが個別セーブデータ
 */
public:
	typedef map<int, Chara::CDataCharaTrain*>	train_map;
	typedef map<int, int>						item_map;
	typedef map<int, int>						flag_map;
	// セーブ対象なのでシリアライズを実装する
	void Serialize(ISerialize& s);

	// デストラクタ
	~CExecData();

	// 設定・取得
	void	setHeader(const CExecDataHead& head){ head_=head; }
	int		getNextScenario() const { return nNextScenario_; }
	void	setNextScenario(int nNextScenario){ nNextScenario_=nNextScenario; }
	bool	getFlag(const string& sFlag, int nValue);
	bool	getFlag(int nFlag, int& nValue);
	void	setFlag(int nFlag, int nValue);
	void	setFlag(const string& sFlag, int nValue);
	void	calcFlag(int nFlag, int nValue);
	void	calcFlag(const string& sFlag, int nValue);
	
	// 操作
	// データクリア系
	void					clear();
	// 引継ぎ時クリア
	void					clearHandover();
	// 有効なキャラリストへの操作
	// 有効というのは、インターミッションで養成ができたり
	// 出撃選択で選択が可能ということ
	set<int>&				getValidSet(){ return setValid_; }
	void					addValid(int nID){ /*if(IsHideChara(nID,mapFlag_))*/ setValid_.insert(nID); }
	void					delValid(int nID){ setValid_.erase(nID); }
	bool					IsValid(int nID)const{ return setValid_.find(nID)!=setValid_.end(); }
	set<int>::iterator		beginValid(){ it_s=setValid_.begin(); return it_s; }
	bool					endValid(){ return it_s==setValid_.end(); }
	set<int>::iterator		nextValid(){ return it_s++; }

	// 養成データを取得する
	// もし、mapに無い場合は、自動的に空の養成データが生成され、
	// mapに追加され、それが返る
	train_map&				getTrainMap(){ return mapTrain_; }
	Chara::CDataCharaTrain* getTrainData(int nID, bool bNew=true);
	void					delTrainData(int nID);
	void					clearTrainData();

	item_map&				getItemMap(){ return mapItem_; }
	void					incItem(int nID);
	void					decItem(int nID);

	int		getVer() const { return head_.getVer(); }
	void	setVer(int nVer) { head_.setVer(nVer); }
	int		getClear() const { return head_.getClear(); }
	void	setClear(int nClear){ head_.setClear(nClear); }
	int		getContinue() const { return head_.getContinue(); }
	void	setContinue(int nContinue){ head_.setContinue(nContinue); }
	int		getHero() const { return head_.getHero(); }
	void	setHero(int nHero){ head_.setHero(nHero); }
	int		getLv() const { return head_.getLv(); }
	void	setLv(int nLv){ head_.setLv(nLv); }
	int		getStory() const { return head_.getStory(); }
	void	setStory(int nStory){ head_.setStory(nStory); }
	int		getBP() const { return head_.getBP(); }
	void	setBP(int nBP){ head_.setBP(nBP); }
	int		getFP() const { return head_.getFP();}
	void	setFP(int nFP){ head_.setFP(nFP); }
	int		getTurn() const { return head_.getTurn(); }
	void	setTurn(int nTurn){ head_.setTurn(nTurn); }
	int		getExpert() const;
	void	setExpert(int nExpert){ head_.setExpert(nExpert); }
	int		getAce() const { return head_.getAce(); }
	void	setAce(int nAce){ head_.setAce(nAce); }

	void	incClear(){ head_.setClear(head_.getClear()+1); }
	void	incContinue(){ head_.setContinue(head_.getContinue()+1); }
	void	calcBP(int nBP){ head_.setBP(head_.getBP()+nBP); if(head_.getBP()<0) head_.setBP(0); ef(head_.getBP()>BP_MAX) head_.setBP(BP_MAX); }
	void	calcFP(int nFP){ head_.setFP(head_.getFP()+nFP); if(head_.getFP()<0) head_.setFP(0); ef(head_.getFP()>FP_MAX) head_.setFP(FP_MAX); }
	void	calcTurn(int nTurn){ head_.setTurn(head_.getTurn()+nTurn); }
	void	incExpert(){ head_.setExpert(head_.getExpert()+1); }

	// 養成マップ
	static void addTrainMap(int nID, int nTrainID){ trainMap_.insert(pair<int,int>(nID,nTrainID)); }
	static int getTrainID(int nID)
	{
		map<int,int>::iterator it = trainMap_.find(nID);
		if(it==trainMap_.end()) return nID;
		return it->second;
	}

private:
	// ヘッダ
	CExecDataHead	head_;
	// 次のシナリオID
	int				nNextScenario_;
	// 有効なキャラID集合
	set<int>			setValid_;
	set<int>::iterator	it_s;
	// 養成データマップ
	train_map		mapTrain_;
	// 保持アイテム
	// アイテムIDの保持個数のMap
	item_map		mapItem_;

	// フラグ
	// つうか、わりと汎用的なデータホルダーだと思いねー
	flag_map		mapFlag_;

	// 養成データIDマップ
	// 一部キャラでデータ共有があるのでそのため
	static map<int,int>	trainMap_;
};

} // namespace Save end
} // namespace BMW end