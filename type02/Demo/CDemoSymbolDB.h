/*
	katze 05/05/11
	update 06/03/24
	デモ用シンボルDB
*/
#pragma once

#include "../Movie/DB/CSymbolDB.h"

namespace BMW{

namespace Movie{
class CDataKeyFrame;
} // namespace Movie end
namespace Demo{
class CDemoMovieClip;
class CDemoMsgList;
class CDemoMsgBoard;

class CDemoSymbolDB : public Movie::CSymbolDB
{/**
	デモ用シンボルDB

	ENEMYなどの特殊指定を吸収するためのシンボルDB
 */
public:
	enum eSymbol{
		DEF_DEFAULT_MORPH=-16,
		DEF_DEFENCE_MORPH,
		DEF_AVOID_MORPH,
		DEF_HIT_MORPH,
		DEF_DEFAULT_ROTATE,
		DEF_DEFENCE_ROTATE,
		DEF_AVOID_ROTATE,
		DEF_HIT_ROTATE,
		DEF_DEFAULT_SIZE,
		DEF_DEFENCE_SIZE,
		DEF_AVOID_SIZE,
		DEF_HIT_SIZE,
		DEF_DEFAULT,
		DEF_DEFENCE,
		DEF_AVOID,
		DEF_HIT,
	};

	// コンストラクタ・デストラクタ
	CDemoSymbolDB();
	virtual ~CDemoSymbolDB(){}
	// 設定・取得
	void setDefSymbolDB(const smart_ptr<CDemoSymbolDB>& db){ pDef_=db; }
	void setMsgList(CDemoMsgList* pList){ pMsgList_=pList; }
	static void setMsgBoard(const smart_ptr<CDemoMsgBoard>& pBoard){ pBoard_= pBoard; }

	// 設定子
	CDemoMovieClip*		createDemoSymbol(int nID);
	CDemoMovieClip*		createDemoSymbolStr(const string& sID);

	Task::ITaskBase*	createSymbol(int nSymbol);
	Task::ITaskBase*	createCode(int nID, Movie::CDataKeyFrame* pData);

private:
	// 防御側
	smart_ptr<CDemoSymbolDB> pDef_;
	// 戦闘Msg関係
	CDemoMsgList* pMsgList_;
	static smart_ptr<CDemoMsgBoard>	pBoard_;
};

} // namespace Demo end
} // namespace BMW end