/*
	katze 06/04/06
	戦闘系判定
*/
#pragma once

#include "ISlgCond.h"

namespace BMW{
namespace SLG{

class CCondBattle : public ISlgCond
{/**
	戦闘系判定
	戦闘キャラ判定に援護系の判定をいれてない
 */
public:
	enum eKind
	{
		CHARA, // 参加したキャラ判定
		DAMAGE, // 被ダメージ量
	};
	// コンストラクタ
	CCondBattle():nChara_(-1),nKind_(CHARA),nValue_(0){}
	// 判定
	bool judg(CSLGContext* p);

	// アクセッサ
	int		getChara()const{ return nChara_; }
	void	setChara(int nChara){ nChara_=nChara; }
	int		getKind()const{ return nKind_; }
	void	setKind(int nKind){ nKind_=nKind; }
	int		getValue()const{ return nValue_; }
	void	setValue(int nValue){ nValue_=nValue; }

private:
	int nChara_;
	int nKind_;
	int nValue_;
};

} // namespace SLG end
} // namespace BMW end