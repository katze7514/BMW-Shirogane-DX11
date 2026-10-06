/*
	katze 05/04/26
	update 06/02/12
	超簡易ステータス
*/
#pragma once

namespace BMW{
namespace SLG{
class CDataCharaSLG;

class CStatusCharaVeryEasy : public Task::CTaskBase
{/**
	超簡易ステータスを表現するクラス
 */
public:
	enum eSide{
		LEFT,
		RIGHT,
	};
	enum eInfo{
		HIT,	// 命中
		MENTAL, // 気力
		ACTION, // 戦闘行動
	};
	// コンストラクタ・デストラクタ
	CStatusCharaVeryEasy():nSide_(LEFT){}
	virtual ~CStatusCharaVeryEasy();

	// タスク
	virtual void Task(Task::CTaskContext*);
	virtual void OnInit(Task::CTaskContext*);

	// アクション
	virtual void actionReset(const CDataCharaSLG& chara, int nInfo, int nValue, int nHP=0, int nEN=0);

	// 方向(OnInitを呼び出す前に設定する)
	int		getSide() const { return nSide_; }
	void	setSide(int nSide){ nSide_=nSide; } 

protected:
	// 方向
	int nSide_;

	// GUI
	GUI::CPanel*	 pPanel_;
};

} // namespace SLG end
} // namespace BMW end