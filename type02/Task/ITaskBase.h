/*
	katze 06/02/01
	タスククラス
	DrawInfoを分離
*/
#pragma once

namespace BMW{

namespace Draw{
class CDrawInfo;
} // namespace Draw end

namespace Task{

class CTaskContext;

class ITaskBase
{/**
	BMWでのタスクベース
	描画情報はインターフェイスだけを持つ
 */
public:
	// コンストラクタ・デストラクタ
	ITaskBase():nTaskPri_(0),bValid_(true),nState_(0),/*nDrawPri_(0),*/bVisible_(true){}
	ITaskBase(smart_ptr<ITaskBase>& pP):nTaskPri_(0),bValid_(true),nState_(0),/*nDrawPri_(0),*/bVisible_(true),pParent_(pP){}
	virtual ~ITaskBase(){}

	// タスク処理
	virtual void Task(CTaskContext*);
	virtual void OnAction(CTaskContext*){}
	virtual void OnDraw(CTaskContext*){}
	virtual void OnReset(CTaskContext*){}

	// 設定・取得
	virtual int  getTaskPriority() const { return nTaskPri_; }
	virtual void setTaskPriority(int nPri){ nTaskPri_=nPri; }
	virtual bool IsValid() const { return bValid_; }
	virtual void valid(bool bV){ bValid_=bV; }
	virtual int  getState() const { return nState_; }
	virtual void setState(int nState){ nState_=nState; }

	virtual int  getDrawPriority() const { return /*nDrawPri_*/0; }
	virtual void setDrawPriority(int nPri){ /*nDrawPri_=nPri;*/ }
	virtual bool IsVisible() const { return bVisible_; }
	virtual void visible(bool bV){ bVisible_=bV; }

	// 操作
	// デフォルトで親に対する相対値を返す
	// 必要無い時はfalseにすればいい
	virtual const Draw::CDrawInfo getDrawInfo(bool bRela=true);
	virtual void setDrawInfo(const Draw::CDrawInfo& drawInfo){}

	virtual void setX(int nX){}
	virtual void setY(int nY){}
	virtual void setAlpha(int nAlpha){}
	virtual void setWidth(LONG lWidth){}
	virtual void setHeight(LONG lHeight){}
	virtual void setAngle(int nAngle){}

	// サイズの取得
	virtual void getSize(LONG& lWidth,LONG& lHeight) const { lWidth=lHeight=0;}
	virtual void getDrawSize(LONG& lWidth,LONG& lHeight) const { lWidth=lHeight=0; }

	const smart_ptr<ITaskBase>& getParent(){ return pParent_; }
	void						setParent(const smart_ptr<ITaskBase>& pP){ pParent_=pP; }

protected:
	int  nTaskPri_; // タスクプライオリティ
	bool bValid_;	// 動作・非動作
	int  nState_;	// 処理状態

	// ↓とりあえず、使っている場面が無いので、コメントアウトしておく
	//int	 nDrawPri_;	// 描画プライオリティ
	bool bVisible_;	// 表示・非表示

	// 親タスク
	smart_ptr<ITaskBase> pParent_;
};

} // namespace Task
} // namespace BMW
