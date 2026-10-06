/*
	katze 06/01/20
	顔グラフィック
*/
#pragma once

namespace BMW{
namespace GUI{

class CGraphicFace : public CGraphic
{/**
	顔グラフィック
	顔データを入れると自動的にデータを取ってきてくれる
 */
public:
	// コンストラクタ・デストラクタ
	CGraphicFace():nToward_(0),bBattle_(false){}
	virtual ~CGraphicFace(){}

	// 設定・取得
	int		getToward()const{ return nToward_; }
	void	setToward(int nToward){ nToward_=nToward; }
	bool	IsBattle()const{ return bBattle_; }
	void	battle(bool bBattle){ bBattle_=bBattle; }

	// 設定子
	void	setFace(int nCharaID, int nFaceID, Task::CTaskContext* pContext, int nX=0, int nY=0);
	void	setFace(int nCharaID, const string& sID, Task::CTaskContext* pContext, int nX=0, int nY=0);

private:
	int  nToward_; // 向き
	bool bBattle_; // 戦闘用？
};

} // namespace GUI end
} // namespace BMW end