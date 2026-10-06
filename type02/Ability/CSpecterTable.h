/*
	katze 05/06/30
	妖怪テーブル
*/
#pragma once

namespace BMW{
namespace Ability{

class CSpecterTableBase
{/**
	妖怪テーブルベース
 */
public:
	// コンストラクタ
	CSpecterTableBase():nHit_(0),nAvoid_(0),nAttack_(0){}

	// 設定・取得
	int		getHit()const{ return nHit_;}
	void	setHit(int nHit){ nHit_=nHit; }
	int		getAvoid()const{ return nAvoid_;}
	void	setAvoid(int nAvoid){ nAvoid_=nAvoid; }
	int		getAttack()const{ return nAttack_;}
	void	setAttack(int nAttack){ nAttack_=nAttack; }

private:
	int nHit_;		// 命中率
	int nAvoid_;	// 回避率
	int nAttack_;	// 攻撃力
};

class CSpecterTable
{/**
	妖怪テーブル
 */
public:
	// コンストラクタ
	CSpecterTable()
	{
		for(int i=1; i<9; i++)
		{
			table_[i].setHit(i*3);
			table_[i].setAvoid(i*3);
		}
		table_[1].setAttack(50);
		table_[2].setAttack(50);
		table_[3].setAttack(100);
		table_[4].setAttack(100);
		table_[5].setAttack(150);
		table_[6].setAttack(150);
		table_[7].setAttack(200);
		table_[8].setAttack(200);
	}

	// 取得
	const CSpecterTableBase& getTable(int nLv){ return table_[nLv]; }


private:
	CSpecterTableBase table_[9];
};

} // namespace Ability end
} // namespace BMW end