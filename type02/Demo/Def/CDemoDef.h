/*
	katze 06/03/24
	デモムービー定義
*/
#pragma once

namespace BMW{

namespace SLG{
class CDataCharaSLG;
} // namespace SLG end

namespace Demo{
class CDemoContext;
class CDemoMovieClip;
class CDemoSymbolCond;
class CDemoSymbolDB;
class CDemoMsgCond;
class CDemoMsgDB;

class CDemoDef
{/**
	デモムービー定義

	ムービーとメッセージの組み合わせを処理する
	防御時用もこれから取得することで、コード上に書かないといけない部分を
	スクリプトレベルに落とす。
	コードで書かないといけないのは、ability表示ぐらい。
 */
public:
	typedef map<string,CDemoMsgCond*> msg_cond_map;
	enum eSymbolCond{
		ATTACK,
		HIT,
		DEFENCE,
		AVOID,
		SYMBOL_END,
	};
	// コンストラクタ・デストラクタ
	CDemoDef();
	~CDemoDef();

	// 設定
	void setDemoDef(const string& sFile,CDemoContext* pContext);
	void addSymbolCond(CDemoSymbolCond* pCond, int nCond){ listSymbolCond_[nCond].push_back(pCond); }
	void addMsgCond(CDemoMsgCond* pCond, const string& sCond){ mapMsgCond_.insert(pair<string,CDemoMsgCond*>(sCond,pCond)); }

	// 取得
	CDemoSymbolDB&	getSymbolDB(){ return *pSymbolDB_; }
	CDemoSymbolDB*	getSymbolDBPtr(){ return pSymbolDB_; }
	CDemoMsgDB&		getMsgDB(){ return *pMsgDB_; }

	// 操作
	void			setMsgList(const string& sID,
							   int nDamage,
							   const SLG::CDataCharaSLG& chara,
							   const SLG::CDataCharaSLG& target,
							   Task::CTaskContext* pContext,
							   bool bEvent=false);
	// Movie取得
	CDemoMovieClip* createMovieClip(const string& sID, int nRatio=100);

private:
	int getSymbolID(int nID, int nRatio=100);
	// シンボル
	list<CDemoSymbolCond*>	listSymbolCond_[SYMBOL_END];
	CDemoSymbolDB*			pSymbolDB_;

	// メッセージ
	msg_cond_map	mapMsgCond_;
	CDemoMsgDB*		pMsgDB_;
};

} // namespace Demo end
} // namespace BMW end