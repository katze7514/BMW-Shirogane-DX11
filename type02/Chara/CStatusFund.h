/*
	katze 05/02/28
	update 06/01/18
	キャラの基礎能力を扱うクラス
*/
#pragma once

namespace BMW{
namespace Chara{

class CStatusFund : public IArchive
{/**
	キャラの基礎能力を表現する
 */
public:
	// 基礎能力値MAX
	const static int FUND_MAX=400;
	// コンストラクタ
	CStatusFund(){ reset(); }

	// シリアライズ
	void Serialize(ISerialize& s);

	// 設定・取得
	int	 getStrength() const { return nStrength_; }
	void setStrength(int nStrength){ nStrength_=nStrength; }
	int	 getMagic() const { return nMagic_; }
	void setMagic(int nMagic){ nMagic_=nMagic; }
	int	 getHit() const { return nHit_; }
	void setHit(int nHit){ nHit_=nHit; }
	int	 getAvoid() const { return nAvoid_; }
	void setAvoid(int nAvoid){ nAvoid_=nAvoid; }
	int	 getDefence() const { return nDefence_; }
	void setDefence(int nDefence){ nDefence_=nDefence; }
	int	 getSkill() const { return nSkill_; }
	void setSkill(int nSkill){ nSkill_=nSkill; }
	int	 getSP() const { return nSP_; }
	void setSP(int nSP){ nSP_=nSP; }

	// 操作
	// 値をリセットする
	void reset(int nNum=0)
	{
		nStrength_=nMagic_=nHit_=nAvoid_=nDefence_=nSkill_=nSP_=nNum;
	}

	// 差分適用
	void copySub(const CStatusFund& fund)
	{
		if(fund.getStrength()>0)	nStrength_=fund.getStrength();
		if(fund.getMagic()>0)		nMagic_=fund.getMagic();
		if(fund.getHit()>0)			nHit_=fund.getHit();
		if(fund.getAvoid()>0)		nAvoid_=fund.getAvoid();
		if(fund.getDefence()>0)		nDefence_=fund.getDefence();
		if(fund.getSkill()>0)		nSkill_=fund.getSkill();
		if(fund.getSP()>0)			nSP_=fund.getSP();
	}

private:
	int nStrength_;	// 腕力
	int nMagic_;	// 魔力
	int nHit_;		// 命中
	int nAvoid_;	// 回避
	int nDefence_;	// 防御
	int nSkill_;	// 技量
	int nSP_;		// SP
};

// static宣言
const int CStatusFund::FUND_MAX;

} // namespace Chara end
} // namespace BMW end