/*
	katze 05/03/05
	update 06/01/18
	キャラの初期情報や成長情報を扱うDB
*/
#pragma once

#include "../IDChara.h"

#include "../CDataCharaGrowthStatus.h"

namespace BMW{
namespace Chara{

class CDataCharaData;
class CDataCharaBattle;
class CDataCharaInter;
class CDataCharaTrain;

class CCharaDB
{/**
	キャラの初期情報や成長情報を扱うDB
*/
public:
	typedef map<int, CDataCharaData*>	chara_map;
	// デストラクタ
	~CCharaDB();
	
	// キャラ基本能力データ
	void				setCharaDB(const string& sFile);
	CDataCharaData*		getCharaData(int nID)
	{ 
		chara_map::iterator it=mapChara_.find(nID);
		return it!=mapChara_.end() ? it->second : NULL;
		
	}
	const CDataCharaData*		getCharaData(int nID)const
	{ 
		chara_map::const_iterator it=mapChara_.find(nID);
		return it!=mapChara_.end() ? it->second : NULL;
		
	}
	void				addCharaData(CDataCharaData* pData, int nID)
						{ 
							mapChara_.insert(pair<int,CDataCharaData*>(nID,pData));
						}

	// 成長データ
	void						setStatusDB(const string& sFile);
	CDataCharaGrowthStatus&		getStatusData(int nID){ return apStatus_[nID]; }
	void						addStatusData(const CStatusGrowthStatus& data, int nID){ apStatus_[nID].addStatus(data); }

	// 各CharaDataに対する設定
	void					setBattle(CDataCharaBattle* battle, int nID, CDataCharaTrain& train, int nLv=0) const;
	void					setInter(CDataCharaInter* inter, int nID, CDataCharaTrain& train) const;

	// 操作
	// nChildが、nParentの子かどうかを判定する
	bool					IsChild(int nChild, int nParent)const;

private:
	chara_map				mapChara_;
	CDataCharaGrowthStatus	apStatus_[Growth::VERSATILITY_SLOW+1];
};

} // namespace Chara end
} // namespace BMW end