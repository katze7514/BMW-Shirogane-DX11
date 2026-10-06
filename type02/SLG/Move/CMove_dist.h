/*
	katze 05/05/16
	距離計算を行う
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Move{

class CMove_dist : public Task::ITaskList
{/**
	距離（=必要移動力）計算を行う

	計算スタックに
	　スタートインデックス
	　エンドインデックス
	　ジャンプ
	　フェーズ
	　計算する最大移動力
	を積んでおく

	結果は、スタックトップに積まれる。
 */
public:
	// コンストラクタ
	CMove_dist():nMaxMove_(60),nDist_(-1){}

	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// 計算用
	void calcDist(Map::CMapChip* pMap, int nMove);
	// 指定したキャラが、指定したマップに移動できるか
	enum eReturn
	{
		NOTENABLE,	// 移動不可
		THROUGH,	// 通り抜けは可
		ENABLE,		// 移動可能
	};
	int IsDist(Map::CMapChip* pMap, int nCurrentHeight);
	void setResult(int nDist);

	// 設定・取得
	void	setSLGContext(CSLGContext* p_){ p=p_; }

	int		getEndIndex() const { return nEnd_; }
	void	setEndIndex(int nEnd){ nEnd_=nEnd; }
	int		getJump() const { return nJump_; }
	void	setJump(int nJump){ nJump_=nJump; }
	int		getPhase() const { return nPhase_; }
	void	setPhase(int nPhase){ nPhase_=nPhase; }
	int		getMaxMove() const { return nMaxMove_; }
	void	setMaxMove(int nMaxMove){ nMaxMove_=nMaxMove; }

	int		getDist() const { return nDist_; }
	void	setDist(int nDist){ nDist_=nDist; }

private:
	// コンテキスト
	CSLGContext* p;
	// 引数
	int nEnd_,nJump_,nPhase_,nMaxMove_;
	// 結果
	int nDist_;
};

} // namespace Move end
} // namespace SLG end
} // namespace BMW end