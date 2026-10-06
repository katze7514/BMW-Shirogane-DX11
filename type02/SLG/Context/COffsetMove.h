/*
	katze 06/03/14
	移動時補正値
*/
#pragma once

namespace BMW{
namespace SLG{

class COffsetMove
{/**
	移動時補正値
 */
public:
	// コンストラクタ
	COffsetMove(){ reset(); }
	// アクセッサ
	int		getMove()const{ return nMove_; }
	void	setMove(int nMove){ nMove_=nMove; }
	void	calcMove(int nMove){ nMove_+=nMove; }
	int		getJump()const{ return nJump_; }
	void	setJump(int nJump){ nJump_=nJump; }
	void	calcJump(int nJump){ nJump_+=nJump; }

	// リセット
	void reset()
	{
		nMove_=nJump_=0;
	}

private:
	int nMove_;		// 移動力
	int nJump_;		// ジャンプ

};

} // namespace SLG end
} // namespace BMW end