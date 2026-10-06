/*
	katze 05/05/18
	ADV背景DB
*/
#pragma once

#include "../../Draw/DB/CSpriteDB.h"

namespace BMW{
namespace ADV{
class CDataBack;

class CBackDB
{/**
	ADV背景DB
 */
public:
	typedef map<int, CDataBack*> back_map;
	// デストラクタ
	~CBackDB();

	// 設定・取得
	void setBackDB(const string& sFile);

	CDataBack*	getBackData(int nID);
	CDataBack*	getBackData(const string& sID);
	void		setBackData(CDataBack* back, int nID);

	// 設定子
	void setBack(GUI::CGraphic* pGraphic, GUI::CText* pText, int nID);
	void setBack(GUI::CGraphic* pGraphic, GUI::CText* pText, const string& sID);

private:
	// 背景データ
	back_map					mapBack_;
	// 背景スプライト
	Draw::CSpriteDB				sprite_;
};

} // namespace ADV end
} // namespace BMW end