/*
	katze 05/06/30
	底力テーブル
*/
#pragma once

namespace BMW{

namespace Chara{
class CDataCharaBattle;
} // namespace Chara end

namespace Ability{

class CFundTableBase
{/**
	底力テーブルの要素
 */
public:
	// コンストラクタ
	CFundTableBase():nHit_(0),nAvoid_(0),nDef_(0),nCT_(0){}

	// 設定・取得
	int		getHit()const{ return nHit_;}
	void	setHit(int nHit){ nHit_=nHit; }
	int		getAvoid()const{ return nAvoid_;}
	void	setAvoid(int nAvoid){ nAvoid_=nAvoid; }
	int		getDef()const{ return nDef_;}
	void	setDef(int nDef){ nDef_=nDef; }
	int		getCT()const{ return nCT_;}
	void	setCT(int nCT){ nCT_=nCT; }

private:
	int nHit_;	// 命中補正
	int nAvoid_;// 回避補正
	int nDef_;	// 装甲補正
	int nCT_;	// CT補正
};
class CFundTable
{/**
	底力テーブル
 */
public:
	// コンストラクタ
	CFundTable();

	// 操作
	int						getRank(const Chara::CDataCharaBattle& battle)const;
	const CFundTableBase&	getTable(int nAttr, int nRank)const{ return table_[nAttr][nRank]; }
	const CFundTableBase&	getTable(int nAttr, const Chara::CDataCharaBattle& battle)const{ return table_[nAttr][getRank(battle)]; }

private:
	CFundTableBase table_[9][10];
};

} // namespace Ability end
} // namespace BMW end