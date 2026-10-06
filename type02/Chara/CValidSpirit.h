/*
	katze 05/03/01
	有効な精神コマンドフラグ
*/
#pragma once

namespace BMW{
namespace Chara{

class CValidSpirit : public IArchive
{/**
	有効な精神コマンドフラグ
 */
public:
	enum eValidSpirit
	{
		FIREBALL,	// 熱血
		SPIRIT,		// 魂
		AVOID,		// ひらめき
		TOUGH,		// 不屈
		DEFENCE,	// 鉄壁
		CONCENT,	// 集中
		HIT,		// 必中
		ACC,		// 加速
		JUMP,		// 跳躍
		AWAKE,		// 覚醒
		EASYON,		// てかげん
		SNIPE,		// 狙撃
		DIRECT,		// 直撃
		CHARGE,		// 突撃
		FORTUNE,	// 幸運
		EFFORT,		// 努力
		FAITH,		// 信念
		PROVO,		// 挑発
		END,
	};
	// コンストラクタ
	CValidSpirit(){ reset(); }

	// シリアライズ
	void Serialize(ISerialize& s)
	{
		for(int i=0; i<END; i++) s << baValid_[i];
		// 08/05/27
		// 挑発してるキャラを保存
		// またまたコンテニューデータに互換性なしっと
		s << nProvoID_;
	}

	// 設定・取得
	bool	IsValid(int nID) const { return baValid_[nID]; }
	void	valid(bool bValid,int nID){ baValid_[nID]=bValid; }
	int		getProvoID()const{ return nProvoID_; }
	void	setProvoID(int nProvoID){ nProvoID_=nProvoID; }

	// 操作
	//全フラグをfalseにする
	void reset()
	{
		for(int i=0; i<END; i++) baValid_[i]=false;
		nProvoID_ = -1;
	}

private:
	// フラグ配列
	bool baValid_[END];
	// 挑発用相手のSLG ID
	int nProvoID_;

};

} // namespace Chara end
} // namespace BMW end