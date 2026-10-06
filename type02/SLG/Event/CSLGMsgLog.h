/**
	katze 07/02/17
	MSGバックログのための構造体
*/
#pragma once

namespace BMW{
namespace SLG{

class CSLGMsgLog
{/**
	MSGバックログのための構造体
 */
public:
	// コンストラクタ
	CSLGMsgLog(int nSide=0, int nSlg=-1, int nChara=-1, int nFace=0, int nString=-1, int nMask=0)
		:nSide_(nSide),nSlg_(nSlg),nChara_(nChara),nFace_(nFace),nString_(nString),nMask_(nMask){}

	// アクセッサ
	int		getMask()const{ return nMask_; }
	int		getString()const{ return nString_; }
	int		getFace()const{ return nFace_; }
	int		getChara()const{ return nChara_; }
	int		getSlg()const{ return nSlg_; }
	int		getSide()const{ return nSide_; }

	// 一括設定
	void	setLog(int nSide, int nSlg, int nChara, int nFace, int nString, int nMask)
	{
		nSide_=nSide;
		nSlg_=nSlg;
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
	int nSlg_;		// SLG ID
	int nSide_;		// サイド
};

} // namespace SLG end
} // namespace BMW end