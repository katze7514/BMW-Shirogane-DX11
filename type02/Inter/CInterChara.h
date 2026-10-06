/*
	katze 05/06/17
	インターミッションで取り回されるキャラデータ
*/
#pragma once

#include "../Chara/CDataCharaInter.h"
#include "../Movie/DB/CSymbolDB.h"

namespace BMW{
namespace Inter{

class CInterContext;

class CInterChara
{/**
	インターミッションで取り回されるキャラデータ
 */
public:
	// コンストラクタ・デストラクタ
	CInterChara():pChipSort_(NULL){}
	virtual ~CInterChara();
	// 設定・取得
	int								getID()const{ return nID_; }
	void							setID(int nID){ nID_=nID; }
	BMW::Chara::CDataCharaInter*		getData(){ return &data_; }
	const BMW::Chara::CDataCharaInter&	getData()const { return data_; }
	Movie::CSymbolDB&				getSymbol(){ return symbol_; }
	GUI::CPanel*					getChipSort(){ return pChipSort_; }

	// 初期化
	void OnInit(Task::CTaskContext*);
	void OnReset(CInterContext*,bool bSecond=true);

private:
	int	nID_; // 見た目上のID
	BMW::Chara::CDataCharaInter	data_;
	Movie::CSymbolDB			symbol_;
	GUI::CPanel*				pChipSort_;
};

} // namespace Inter end
} // namespace BMW end