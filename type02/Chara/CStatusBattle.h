/*
	katze 05/02/28
	update 06/01/18
	キャラの戦闘能力を扱うクラス
*/
#pragma once

namespace BMW{
namespace Chara{

class CStatusBattle : public IArchive
{/**
	キャラの基礎能力を表現する
 */
public:
	// コンストラクタ
	CStatusBattle(){ reset(); }

	// シリアライズ
	void Serialize(ISerialize& s);

	// 設定・取得
	int	 getHP() const { return nHP_; }
	void setHP(int nHP){ nHP_=nHP; }
	int	 getEN() const { return nEN_; }
	void setEN(int nEN){ nEN_=nEN; }
	int	 getTough() const { return nTough_; }
	void setTough(int nTough){ nTough_=nTough; }
	int	 getQuick() const { return nQuick_; }
	void setQuick(int nQuick){ nQuick_=nQuick; }
	int	 getMove() const { return nMove_; }
	void setMove(int nMove){ nMove_=nMove; }
	int	 getJump() const { return nJump_; }
	void setJump(int nJump){ nJump_=nJump; }

	/// 与えられた数値で全部初期化
	void reset(int nNum=0)
	{
		nHP_=nEN_=nTough_=nQuick_=nMove_=nJump_=nNum;
	}

	// 差分適用
	void copySub(const CStatusBattle& battle)
	{
		if(battle.getHP()>0)	nHP_=battle.getHP();
		if(battle.getEN()>0)	nEN_=battle.getEN();
		if(battle.getTough()>0) nTough_=battle.getTough();
		if(battle.getQuick()>0) nQuick_=battle.getQuick();
		if(battle.getMove()>0)	nMove_=battle.getMove();
		if(battle.getJump()>0)	nJump_=battle.getJump();
	}

private:
	int nHP_;			// HP
	int nEN_;			// EN
	int nTough_;		// 耐久
	int nQuick_;		// 敏捷
	int nMove_;			// 移動力
	int nJump_;			// ジャンプ
};

// ここでするかは悩むとこだけど
typedef list<int> weapon_list;

} // namespace Chara end
} // namespace BMW end