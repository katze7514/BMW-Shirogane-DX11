/*
	katze 06/05/24
	エフェクトスクリプト生成構造体
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Effect{

struct CCmdLoad
{
	int nNo_;
	string sSymbol_;
	int nEffect_;

	CCmdLoad():nEffect_(0){}
};

struct CCmdAddSymbol
{
	int nNo_;
	int nTarget_;
	string sChara_;
	int nIndex_;
	int nX_,nY_;

	CCmdAddSymbol():nTarget_(0),nX_(0),nY_(0){}
};

struct CCmdCtrl
{
	int nNo_;
	int nType_;
	int nValue_;
};

struct CCmdMapScroll
{
	int nType_;
	int nIndex_;
	string sChara_;
	int nX_,nY_,nFrame_,nEdging_;

	CCmdMapScroll():nX_(INT_MAX),nY_(INT_MAX){}
};

struct CCmdMapChip
{
	int nType_;

	// 共通
	string sSlg_; // マップチップだったら、読み込むsymbolID
	bool bVisible_;


	// マップチップ
	int nCtrl_; // 操作フラグ
	int nIndex_;	// 操作対象マップインデックス
	int nPriority_; // マップタスクプライオリティ

	CCmdMapChip():nType_(0){}
};

} // namespace Effect end
} // namespace SLG end
} // namespace BMW end