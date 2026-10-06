/*
	katze 05/06/30
	吸血種テーブル
*/
#pragma once

namespace BMW{
namespace Ability{

class CVampireTableBase
{/**
	吸血種テーブルベース
 */
public:
	// コンストラクタ
	CVampireTableBase():nHit_(0),nAvoid_(0),nDef_(0),nCT_(0){}

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
	int nHit_;		// 命中率
	int nAvoid_;	// 回避率
	int nDef_;		// 装甲
	int	nCT_;		// CT補正
};

class CVampireTable
{/**
	吸血種テーブル
 */
public:
	// コンストラクタ
	CVampireTable()
	{
		for(int i=1; i<9; i++)
		{
			table_[i].setHit(i*2);
			table_[i].setAvoid(i*2);
			table_[i].setDef(1+(i-1)*2);
			table_[i].setCT(1+(i-1)*2);
		}
	}

	// 取得
	const CVampireTableBase& getTable(int nLv){ return table_[nLv]; }


private:
	CVampireTableBase table_[9];
};

} // namespace Ability end
} // namespace BMW end