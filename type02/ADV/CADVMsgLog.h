/**
	katze 07/02/17
	MSGバックログのための構造体
*/
#pragma once

namespace BMW{
namespace ADV{

class CADVMsgLog
{/**
	MSGバックログのための構造体
 */
public:
	// コンストラクタ
	CADVMsgLog(int nSide=0, int nChara=-1, int nFace=0, int nString=-1, int nMask=0)
		:nSide_(nSide),nChara_(nChara),nFace_(nFace),nString_(nString),nMask_(nMask){}

	// アクセッサ
	int		getMask()const{ return nMask_; }
	int		getString()const{ return nString_; }
	int		getFace()const{ return nFace_; }
	int		getChara()const{ return nChara_; }
	int		getSide()const{ return nSide_; }

	// 一括設定
	void	setLog(int nSide, int nChara, int nFace, int nString, int nMask)
	{
		nSide_=nSide;
		nChara_=nChara;
		nFace_=nFace;
		nString_=nString;
		nMask_=nMask;
	}

private:
	int nMask_;		// マスクフラグ
	int nString_;	// 文字列ID
	int nFace_;		// 顔ID
	int nChara_;	// キャラID
	int nSide_;		// サイド
};

} // namespace ADV end
} // namespace BMW end