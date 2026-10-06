/*
	katze 05/05/15
	デモ背景の1ライン
*/
#pragma once

namespace BMW{
namespace Demo{

class IDemoBackLine : public Task::CTaskBase
{/**
	デモ背景の1ラインを担うクラスの基底
 */
public:
	// 状態という名の速度
	enum eState{
		RIGHT_10,
		RIGHT_9,
		RIGHT_8,
		RIGHT_7,
		RIGHT_6,
		RIGHT_5,
		RIGHT_4,
		RIGHT_3,
		RIGHT_2,
		RIGHT_1,
		STOP,
		LEFT_1,
		LEFT_2,
		LEFT_3,
		LEFT_4,
		LEFT_5,
		LEFT_6,
		LEFT_7,
		LEFT_8,
		LEFT_9,
		LEFT_10,
	};
	// コンストラクタ・デストラクタ
	IDemoBackLine(){ nVel_[STOP]=0; }
	virtual ~IDemoBackLine(){}

	// タスク
	virtual void Task(Task::CTaskContext*);

	// 設定・取得
	int		getVel(int nIndex) const { return nVel_[nIndex]; }
	void	setVel(int nVel, int nIndex){ nVel_[nIndex].setNum(nVel<<16); }
	void	setVelFloat(float fVel, int nIndex);
	void	setVel(const katzeSDK::Misc::CFixedNum& nVel, int nIndex){ nVel_[nIndex]=nVel; }

	static int	getBackState(){ return nBackState_;}
	static void setBackState(int nState){ nBackState_=nState; }
	static bool IsBackVisible(){ return bBackVisible_; }
	static void backVisible(bool bBack){ bBackVisible_=bBack; }
	static bool IsForwardVisible(){ return bForwardVisible_; }
	static void forwardVisible(bool bForward){ bForwardVisible_=bForward; }

	// 操作
	int		getCurrentVel() const { return nVel_[getBackState()]; }

protected:
	// 速度テーブル
	katzeSDK::Misc::CFixedNum nVel_[LEFT_10+1];

	// 背景状態
	static int	nBackState_;		// 速度
	static bool bBackVisible_;		// 描画
	static bool bForwardVisible_;	// 描画
};

} // namespace Demo end
} // namespace BMW end