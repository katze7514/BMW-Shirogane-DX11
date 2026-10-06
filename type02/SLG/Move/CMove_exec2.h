/*
	katze 05/12/21
	
	move_execのVer.2
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
class CMapChipChara2;
class CMapChipCharaSprite2;
} // namespace Map end

namespace Move{

class CMove_exec2 : public Task::ITaskList
{/**
	move_execのVer.2

	移動時にアニメーションがつく
 */
public:
	enum eState{
		EXEC,
		EXEC_WALK,
		EXEC_JUMP_READY_UP,
		EXEC_JUMP_UP,
		EXEC_JUMP_DOWN,
		EXEC_JUMP_READY_DOWN,
		END_WAIT,
		END,
		CANCEL,
		OK,
	};
	// デストラクタ
	~CMove_exec2();
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	CSLGContext*				p;
	CDataCharaSLG*				pChara_;			// 移動対象キャラデータ
	Map::CMapChipChara2*		pCharaChip_;		// 移動対象キャラ
	Map::CMapChipCharaSprite2*	pCharaChipSprite_;	// 移動対象キャラスプライト
	Map::CMapChip*				pStartMap_;			// 現在いる(はずの)MAPチップ
	Map::CMapChip*				pEndMap_;			// 移動先MAPチップ
	bool						bStart_;			// 現在キャラスプライトが存在してるマップフラグ
	int							nHeight_;			// 高さの差
	int							nIndex_;			// 優先順位差
	/* 
		そのマスにキャラがいた場合
		優先度をずらさないといけないので、そのoffset
	*/
	int	 nPri_;

	Movie::CMotion				motion_;			// ムービークリップの移動
	int							nFrame_;			// 待ちフレーム数

	// OK・キャンセルタスク
	BMW::Rule::CRuleOK*			pOK_;
	BMW::Rule::CRuleCancel*		pCancel_;

//	void callTaskAction(Task::CTaskContext* pContext);
//	void callTaskDraw(Task::CTaskContext* pContext);
};

} // namespace Move end
} // namespace SLG end
} // namespace BMW end
