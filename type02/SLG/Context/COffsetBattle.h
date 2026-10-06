/*
	katze 06/03/14
	戦闘時補正値
*/
#pragma once

namespace BMW{
namespace SLG{

class COffsetBattle
{/**
	戦闘時補正値
 */
public:
	// コンストラクタ
	COffsetBattle(){ reset(); }
	// 設定・取得
	int		getAttack()const{ return nAttack_; }
	void	setAttack(int nAttack){ nAttack_=nAttack; }
	void	calcAttack(int nAttack){ nAttack_+=nAttack; }
	int		getTough()const{ return nTough_; }
	void	setTough(int nTough){ nTough_=nTough; }
	void	calcTough(int nTough){ nTough_+=nTough; }
	int		getCT()const{ return nCT_; }
	void	setCT(int nCT){ nCT_=nCT; }
	void	calcCT(int nCT){ nCT+=nCT; }
	int		getDamage()const{ return nDamage_; }
	void	setDamage(int nDamage){ nDamage_=nDamage; }
	void	calcDamage(int nDamage){ nDamage_+=nDamage; }
	
	// リセット
	void reset()
	{
		nAttack_=nTough_=nCT_=nDamage_=0;
	}

private:
	int nAttack_;	// 攻撃力
	int nTough_;	// Tough
	int nCT_;		// CT率
	int nDamage_;	// ダメージ
};


} // namespace SLG end
} // namespace BMW end