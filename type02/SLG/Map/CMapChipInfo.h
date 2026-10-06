/*
	katze 05/03/22
	マップ一つが持つ基本情報
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Map{

class CMapChipInfo
{/**
	一マスのマップが持つ基本情報
	こいつを元に移動判定などが行われる
 */
public:
	// コンストラクタ・デストラクタ
	CMapChipInfo():nMove_(1),nHeight_(0),nHP_(0),nEN_(0),nHit_(0),nDefence_(0)//,nBack_(0)
	{
		naOnMapIndex_[0]=-1;
		naOnMapIndex_[1]=-1;
		naOnMapIndex_[2]=-1;
		naOnMapIndex_[3]=-1;
	}
	virtual ~CMapChipInfo(){}

	// 設定・取得
	int		getMove() const { return nMove_; }
	void	setMove(int nMove){ nMove_=nMove; }
	int		getHeight() const { return nHeight_; }
	void	setHeight(int nHeight){ nHeight_=nHeight; }
	int		getHP() const { return nHP_; }
	void	setHP(int nHP){ nHP_=nHP; }
	int		getEN() const { return nEN_; }
	void	setEN(int nEN){ nEN_=nEN; }
	int		getHit() const { return nHit_; }
	void	setHit(int nHit){ nHit_=nHit; }
	int		getDefence() const { return nDefence_; }
	void	setDefence(int nDefence){ nDefence_=nDefence; }

	int		getOnMap(int nToward) const { return naOnMapIndex_[nToward]; }
	void	setOnMap(int nIndex, int nToward){ naOnMapIndex_[nToward]=nIndex; }

	//int		getBack() const { return nBack_; }
	//void	setBack(int nBack){ nBack_=nBack; }

protected:
	int nMove_;		// 必要移動力
	int nHeight_;	// 高さ
	int nHP_;		// HP回復率
	int nEN_;		// EN回復率
	int nHit_;		// 命中補正
	int nDefence_;	// 防御補正

	int naOnMapIndex_[4]; // 隣接するマップインデックス

	//int nBack_;		// ここで戦闘が起きた時の背景ID
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end