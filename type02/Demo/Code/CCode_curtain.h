/**
	katze 07/03/27
	カーテン動作
*/
#pragma once

namespace BMW{
namespace Demo{
class CDemoScene;
class CDemoMovieClip;

namespace Code{

class CCode_curtain : public Rule::IRuleTask
{/**
	カーテン動作
	カーテンをリセットして動作が終了するまで待つ
 */
public:
	enum eState{
		INIT,
		WAIT,
		END,
	};
	// コンストラクタ
	CCode_curtain(int nSide, CDemoScene* pDemo):nSide_(nSide),pScene_(pDemo){}
	// タスク
	void OnAction(Task::CTaskContext*);

private:
	int nSide_; // 操作サイド
	CDemoMovieClip* pCurtain_;
	CDemoScene* pScene_;
};

} // namespace Code end
} // namespace Demo end
} // namespace BMW end