/*
	katze 05/05/01
	マップ上のキャラを消去する
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{

namespace Map{
class CMapChip;
} // namespace Map end

namespace Effect{
class CEffectMovieClip;
} // namespace Effect end

namespace Sally{

class CSally_del_chara_map : public Task::ITaskList
{/**
	マップ上のキャラを消去する

	削除対象キャラID（SLG）を対象キャラにし、
	削除時に使用するエフェクトIDを積んでおく
 */
public:
	enum eState{
		EFFECT,
		END,
	};
	enum eEffect{
		NO=-1,
		DEATH,
		REMOVE,
	};
	// デスクトラクタ
	~CSally_del_chara_map();

	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// 削除
	void delChara(Task::CTaskContext*);

private:
	// エフェクト
	Task::CTaskCtrl<Effect::CEffectMovieClip>* pEffectCtrl_;
	int naVisibleFrame[2];
	// 対象マップ
	Map::CMapChip* pMap_;
	// スプライト
	Task::ITaskBase* pSprite_;

	void callTaskAction(Task::CTaskContext* pContext);
	void callTaskDraw(Task::CTaskContext* pContext);
};

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end