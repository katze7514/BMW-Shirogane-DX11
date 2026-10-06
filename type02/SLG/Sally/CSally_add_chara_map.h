/*
	katze 05/04/30
	キャラをマップ上に追加する
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{
class CSLGContext;
class CDataCharaSLG;

namespace Map{
class CMapChip;
} // namespace Map end

namespace Effect{
class CEffectMovieClip;
} // namespace Effect end

namespace Sally{

class CSally_add_chara_map : public Task::ITaskList
{/**
 　キャラをマップ上に追加する
 　つまり、CCharaStateの設定と、CMapChipCharaの生成

  追加対象が、TargetChara
  追加位置が、TargetMap
  向きは、スタックに積んでおく
 */
public:
	enum eState{
		EFFECT,
		END,
	};
	enum eEffect{
		NO=-1,
		NORMAL,
		BOSS,
		WARAKIA,
		END_EFFECT,
	};
	// コンストラクタ・デスクトラクタ
	CSally_add_chara_map();
	virtual ~CSally_add_chara_map();

	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	void addChara(Task::CTaskContext* pContext);
	int	 getEnableAddMap(Map::CMapChip* pMap, CDataCharaSLG* pChara, CSLGContext* pContext);

protected:
	// エフェクト
	Effect::CEffectMovieClip* pEffect_[END_EFFECT];
	int nEffect_; // 現在再生中のエフェクトID
	int naVisibleFrame[END_EFFECT];
	// スプライト
	Task::ITaskBase* pSprite_;

	void callTaskAction(Task::CTaskContext* pContext);
	void callTaskDraw(Task::CTaskContext* pContext);

	// マップチップ設定
	virtual void setupCharaChip(CDataCharaSLG* pChara, int nIndex, CSLGContext* p);
};

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end