/*
	katze 06/01/20
	インターフェイスの動作実行クラス
*/
#pragma once

namespace BMW{

namespace Movie{
class CKeyFrame;
} // namespace Movie end

namespace GUI{
class CPanel;

class CInterfaceLayer : public Task::ITaskBase
{/**
	インターフェイス動作のためのレイヤー

	現在の動作しているアクションは、state。
	CLayerと違いは、一つのキーフレームが動作終わったら、
	終わったところで停止する。
 */
public:
	typedef vector<Movie::IKeyFrame*> frame_vec;
	// デストラクタ
	virtual ~CInterfaceLayer();

	// タスク
	void Task(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	
	// 設定・取得
	IPanel*				getPanel(){ return pPanel_; }
	void				setPanel(IPanel* pPanel){ pPanel_=pPanel; }
	Movie::IKeyFrame*	getKeyFrame(int nState){ return vecFrame_[nState]; }
	void				setKeyFrame(Movie::IKeyFrame* pFrame, int nState);

	// 操作
	void				resizeKeyFrameVec(int nSize){ vecFrame_.resize(nSize); }
	void				resetKeyFrame(int nState,CTaskContext* pContext);
	// 現在の動作が終了したか？
	bool				IsEnd();

private:
	IPanel*		pPanel_; // 動作対象インターフェイス
	frame_vec	vecFrame_;
};

} // namespace GUI end
} // namespace BMW end