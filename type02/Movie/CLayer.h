/*
	katze 05/05/10
	レイヤーを表現するクラス
*/
#pragma once

namespace BMW{
namespace Movie{
class IKeyFrame;

class CLayer : public Task::ITaskBase
{/**
	レイヤーを表現するクラス

	別名CKeyFrameCtrl
 */
public:
	typedef vector<IKeyFrame*> frame_vec;
	enum eState{
		END=-1,
	};
	// コンストラクタ・デストラクタ
	CLayer():nFrame_(1),nExec_(0){}
	virtual ~CLayer(){ clearKeyFrame(); }
	// タスク
	void Task(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);

	// 設定・取得
	int		getExecFrame() const { return nExec_; }
	void	setExecFrame(int nFrame){ nExec_=nState_=nFrame; }
	// 操作
	void		resizeFrame(int nSize){ vecKeyFrame_.resize(nSize); }
	IKeyFrame*	getKeyFrame(int nKey){ return vecKeyFrame_[nKey]; }
	IKeyFrame*	getExecKeyFrame(){ return getState()>=0 ? vecKeyFrame_[nExec_] : NULL; }
	void		setKeyFrame(IKeyFrame* pFrame, int nKey);
	void		clearKeyFrame();

	void getSize(LONG& lWidth, LONG& lHeight)const;
	void getDrawSize(LONG& lWidth, LONG& lHeight)const{ getSize(lWidth,lHeight); }

private:
	// キーフレームvector
	frame_vec vecKeyFrame_;
	// 現在のフレーム数
	int nFrame_;
	// 現在実行中キーフレーム
	int nExec_;
	// 実行中のキーフレームNoは、stateで代用
};

} // namespace Movie end
} // namespace BMW end