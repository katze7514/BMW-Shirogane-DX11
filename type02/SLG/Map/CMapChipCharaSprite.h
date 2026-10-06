/*
	katze 05/03/24
	キャラコマを表現するクラス
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // naemsapce Task end

namespace SLG{
class CCharaState;

namespace Map{

class CMapChipCharaSprite : public Task::CTaskBase
{/**
	キャラコマを表現するクラス
 */
public:
	// コンストラクタ・デストラクタ
	CMapChipCharaSprite();
	// タスク
	void Task(Task::CTaskContext*);
	void OnDraw(Task::CTaskContext*);

	// 設定・取得
	GUI::CGraphic*				getGraphic(int nAct, int nWay){ return &sprite_[nAct][nWay]; }
	void						setCharaState(const smart_ptr<CCharaState>& pState){ pState_=pState; }
	void						getSize(LONG& lWidth, LONG& lHeight);
	void						getDrawSize(LONG& lWidth, LONG& lHeight);

private:
	// 一次元目が、行動前:0 済み:1
	// 二次元目が、前：0  左：1  後：2  右：3
	GUI::CGraphic sprite_[2][4];

	// キャラ状態(CDataCharaSLGのを共有する)
	smart_ptr<CCharaState> pState_;

	int getAct();
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end