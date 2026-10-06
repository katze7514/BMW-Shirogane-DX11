/*
	katze 05/03/01
	状態変化フラグ管理クラス
*/
#pragma once

namespace BMW{
namespace Chara{

class CValidCond : public IArchive
{/**
	状態変化フラグを管理するクラス
	状態変化武器によって変化させられる
 */
public:
	enum eValidCond
	{
		ACTION,		// 行動
		MOVE,		// 移動
		DEFENCE,	// 防御
		HIT,		// 命中
		AVOID,		// 回避
		EN,			// EN
		MENTAL,		// 気力
		END,		// 番兵
	};

	// コンストラクタ
	CValidCond(){ reset(); }

	void Serialize(ISerialize& s)
	{
		for(int i=ACTION; i<=AVOID; ++i)
			s << naValid_[i];
	}

	// 設定・取得
	bool IsValid(int nID) const { return naValid_[nID]>0; }
	void valid(int nValid,int nID){ naValid_[nID]=nValid; }

	// すべてのフラグを倒す
	void reset()
	{
		for(int i=ACTION; i<=AVOID; ++i)
			naValid_[i]=0;
	}

	// デクリメント
	void dec()
	{
		for(int i=ACTION; i<=AVOID; ++i)
			dec(i);
	}
	void dec(int nID){ --naValid_[nID]; if(naValid_[nID]<0) naValid_[nID]=0; }


private:
	// 有効フラグ配列
	int naValid_[AVOID+1];
};

} // namespace Chara end
} // namespace BMW end