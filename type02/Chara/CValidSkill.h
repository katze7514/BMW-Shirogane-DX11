/*
	katze 05/03/01
	update 06/06/12
	覚えられる技能フラグクラス
*/
#pragma once

namespace BMW{
namespace Chara{

class CValidSkill
{/**
	有効な状態フラグや覚えられる最大Lvを管理するクラス
 */
public:
	enum eValidSkill
	{
		FUNDPOWER,		// 底力
		COUNTER,		// カウンター
		BACKUPATTACK,	// 援護攻撃
		BACKUPDEFENCE,	// 援護防御
		SPUP,			// SPUP
		MOVE_UP,		// 移動力UP
	};

	// コンストラクタ
	CValidSkill(){ reset(); }

	// 設定・取得
	int  IsValid(int nID) const { return naValid_[nID]; }
	void valid(int nValid,int nID){ naValid_[nID]=nValid; }

	// すべてのフラグを倒す
	void reset()
	{
		naValid_[FUNDPOWER]=9;
		naValid_[COUNTER]=9;
		naValid_[BACKUPATTACK]=4;
		naValid_[BACKUPDEFENCE]=4;
		naValid_[SPUP]=9;
		naValid_[MOVE_UP]=2;
	}

	// 操作
	void decValid(int nID);

private:
	// 有効フラグ配列
	int naValid_[MOVE_UP+1];
};

} // namespace Chara end
} // namepsace BMW end
