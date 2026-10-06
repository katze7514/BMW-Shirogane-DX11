/*
	katze 05/06/30
	魔術師テーブル
*/
#pragma once

namespace BMW{
namespace Ability{

class CMagicianTableBase
{/**
	魔術師テーブルベース
 */
public:
	// コンストラクタ
	CMagicianTableBase():nHit_(0),nAvoid_(0),nRange_(0){}

	// 設定・取得
	int		getHit()const{ return nHit_;}
	void	setHit(int nHit){ nHit_=nHit; }
	int		getAvoid()const{ return nAvoid_;}
	void	setAvoid(int nAvoid){ nAvoid_=nAvoid; }
	int		getRange()const{ return nRange_;}
	void	setRange(int nRange){ nRange_=nRange; }

private:
	int nHit_;		// 命中率
	int nAvoid_;	// 回避率
	int nRange_;	// 射程
};

class CMagicianTable
{/**
	魔術師テーブル
 */
public:
	// コンストラクタ
	CMagicianTable()
	{
		table_[1].setHit(5);
		table_[1].setAvoid(5);

		table_[2].setHit(10);
		table_[2].setAvoid(10);

		table_[3].setHit(15);
		table_[3].setAvoid(15);

		table_[4].setHit(20);
		table_[4].setAvoid(20);

		table_[5].setHit(25);
		table_[5].setAvoid(25);

		table_[6].setHit(25);
		table_[6].setAvoid(25);
		table_[6].setRange(1);

		table_[7].setHit(30);
		table_[7].setAvoid(30);
		table_[7].setRange(1);

		table_[8].setHit(30);
		table_[8].setAvoid(30);
		table_[8].setRange(2);
	}

	// 取得
	const CMagicianTableBase& getTable(int nLv){ return table_[nLv]; }


private:
	CMagicianTableBase table_[9];
};

} // namespace Ability end
} // namespace BMW end