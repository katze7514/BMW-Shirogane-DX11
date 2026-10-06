/*
	katze 05/06/15
	シナリオデータ
*/
#pragma once

namespace BMW{
namespace Scenario{

class CDataScenarioBase
{/**
	シナリオデータのベース
 */
public:
	enum eScene{
		ADV,
		SLG,
		ED,
		ED_RETURN,
		END,
	};
	// コンストラクタ
	CDataScenarioBase():nScene_(-1),nID_(-1){}

	// 設定・取得
	int		getScene() const { return nScene_; }
	void	setScene(int nScene){ nScene_=nScene; }
	int		getID() const{ return nID_; }
	void	setID(int nID){ nID_=nID; }

private:
	int		nScene_; // 飛ぶシーン
	int		nID_;	 // 設定ID
};

class CDataScenario
{/**
	シナリオデータ
 */
public:
	typedef list<CDataScenarioBase> base_list;

	// コンストラクタ
	CDataScenario():nID_(-1),nNo_(0),nNormal_(INT_MAX),nHard_(INT_MAX){}
	// 設定・取得
	int				getID()const{ return nID_; }
	void			setID(int nID){ nID_=nID; }
	int				getNo()const{ return nNo_; }
	void			setNo(int nNo){ nNo_=nNo; }
	const string&	getTitle() const { return sTitle_; }
	void			setTitle(const string& sTitle){ sTitle_=sTitle; }
	int				getNormal()const{ return nNormal_; }
	void			setNormal(int nExpert){ nNormal_=nExpert; }
	int				getHard()const{ return nHard_; }
	void			setHard(int nExpert){ nHard_=nExpert; }

	void			addBase(const CDataScenarioBase& base){ listBase_.push_back(base); }

	base_list::iterator	beginBase(){ it=listBase_.begin(); return it; }
	bool				endBase(){ return it==listBase_.end(); }
	base_list::iterator	prevBase(){ return --it; }
	base_list::iterator	nextBase(){ return it++; }
	// 指定されてるIDまでイテレータを進める
	base_list::iterator progBase(int nID)
	{
		it=listBase_.begin();
		while(it!=listBase_.end())
		{
			if(it->getID()==nID) break;
			else				 ++it;
		}
		return ++it;
	}

	set<int>&	getEventCharaSet(){ return eventCharaSet_; }
	void		setEventCharaID(int nID){ eventCharaSet_.insert(nID); }

	// 操作
	int	 getExpertRank(int nExpert);

	const string&	getScenarioFile(int nID){ return scenarioFile_.getValue(nID); }
	void			setScenarioFile(const string& sID,int nID){ scenarioFile_.writeMap(nID,sID); }

private:
	// シナリオID
	int		nID_;
	// 話数
	int		nNo_;
	// タイトル
	string	sTitle_;
	// 熟練度敷居値
	int nNormal_;
	int nHard_;

	// 進行データ
	base_list			listBase_;
	base_list::iterator	it;
	// ↑に対応するファイル名
	CIntMap				scenarioFile_;

	// EVキャラIDリスト
	set<int> eventCharaSet_;
};

} // namespace Scenario end
} // namespace BMW end