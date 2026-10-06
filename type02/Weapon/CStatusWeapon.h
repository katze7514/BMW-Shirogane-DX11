/*
	katze 05/03/08
	武器ステータス
*/
#pragma once

#include "IDWeapon.h"

namespace BMW{
namespace Weapon{

class CStatusWeapon
{/**
	武器ステータス
 */
public:
	// コンストラクタ
	CStatusWeapon():
	  nKind_(0),bP_(false),bM_(false),bT_(false),bF_(false),
	  nAttack_(0),nMin_(1),nMax_(1),nCoreMin_(1),nCoreMax_(1),nHeight_(1),
	  nHit_(0),nCT_(0),nEn_(0),nBallet_(0),nMental_(0),nTraining_(Type::F){};

	// 設定・取得
	int		getKind() const { return nKind_; }
	void	setKind(int nKind){ nKind_=nKind; }

	bool	IsP() const { return bP_; }
	void	p(bool bP){ bP_=bP; }
	bool	IsM() const { return bM_; }
	void	m(bool bM){ bM_=bM; }
	bool	IsT() const { return bT_; }
	void	t(bool bT){ bT_=bT; }
	bool	IsF() const { return bF_; }
	void	f(bool bF){ bF_=bF; }

	int		getAttack() const { return nAttack_;}
	void	setAttack(int nAttack){ nAttack_=nAttack; }
	int		getMin()const{ return nMin_; }
	void	setMin(int nMin){ nMin_=nMin; }
	int		getMax()const{ return nMax_; }
	void	setMax(int nMax){ nMax_=nMax; }
	int		getCoreMin() const { return nCoreMin_; }
	void	setCoreMin(int nCoreMin){ nCoreMin_=nCoreMin; }
	int		getCoreMax() const { return nCoreMax_; }
	void	setCoreMax(int nCoreMax){ nCoreMax_=nCoreMax; }
	int		getHeight() const { return nHeight_; }
	void	setHeight(int nHeight){ nHeight_=nHeight; }

	int		getHit() const { return nHit_; }
	void	setHit(int nHit){ nHit_=nHit; }
	int		getCT() const { return nCT_; }
	void	setCT(int nCT){ nCT_=nCT; }

	int		getEN() const { return nEn_; }
	void	setEN(int nEn){ nEn_=nEn; }
	int		getBallet() const { return nBallet_; }
	void	setBallet(int nBallet){ nBallet_=nBallet; }
	int		getMental() const { return nMental_; }
	void	setMental(int nMental){ nMental_=nMental; }

	int		getTrainingType() const { return nTraining_; }
	void	setTrainingType(int nType){ nTraining_=nType; }

private:
	int		nKind_;		// 種別

	bool	bP_;		// P武器属性
	bool	bM_;		// 魔術武器属性
	bool	bT_;		// 飛び武器属性
	bool	bF_;		// フィールド属性（ようはマップ兵器）

	int		nAttack_;	// 攻撃力
	int		nCoreMin_;	// 中心最小射程
	int		nCoreMax_;	// 中心射程
	int		nMin_;		// 全体最小射程
	int		nMax_;		// 全体最大射程
	int		nHeight_;	// 到達度

	int		nHit_;		// 命中補正
	int		nCT_;		// CT補正

	int		nEn_;		// 必要EN
	int		nBallet_;	// 弾数
	int		nMental_;	// 必要気力

	int		nTraining_;	// 養成タイプ
};


} // namespace Weapon end
} // namespace BMW end