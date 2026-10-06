/*
	katze 05/03/02
	update 06/02/03
	基本的なモーションを担うクラス
*/
#pragma once

#include "IMotion.h"

namespace BMW{
namespace Movie{

class CMotion : public IMotion
{/**
	線形動作をするモーションクラス
 */
public:
	// 設定・取得
	const CDrawInfo&	getStart() const { return start_; }
	void				setStart(const CDrawInfo& start){ start_=start; }
	void				setStart(LONG lX=0, LONG lY=0, LONG lAlpha=255, LONG lWidth=256, LONG lHeight=256, LONG lAngle=0)
						{	
							start_.setX(lX); start_.setY(lY); 
							start_.setAlpha(lAlpha); 
							start_.setWidth(lWidth); start_.setHeight(lHeight); 
							start_.setAngle(lAngle); 
						}
	const CDrawInfo&	getEnd() const { return end_; }
	void				setEnd(const CDrawInfo& end){ end_=end; }
	void				setEnd(LONG lX=0, LONG lY=0, LONG lAlpha=255, LONG lWidth=256, LONG lHeight=256, LONG lAngle=0)
						{	
							end_.setX(lX); end_.setY(lY); 
							end_.setAlpha(lAlpha); 
							end_.setWidth(lWidth); end_.setHeight(lHeight); 
							end_.setAngle(lAngle); 
						}

	// 操作
	bool inc();	// 開始値から終了値へ進む。終了値に到達するとtrueを返す
	bool dec(); // 終了値から開始値へ進む。開始値に到達するとtrueを返す
	bool IsStart(){ return start_==current_; }
	bool IsEnd(){ return end_==current_; }
	void reset(bool bEnd=false)
	{
		IMotion::reset(bEnd);
		bEnd ? current_=end_ :current_=start_;
	}

private:
	CDrawInfo	start_;
	CDrawInfo	end_;

	void edging();
};

///////////////////////////////////////////////////////////////
// ヘッダファイルでの実装ｗ
///////////////////////////////////////////////////////////////
__inline bool CMotion::inc()
{
	if(lCurrentStep_>=lStep_)
	{
		current_ = end_;
		return true;
	}
	++lCurrentStep_;
	edging();
	return false;
}

__inline bool CMotion::dec()
{
	if(lCurrentStep_<=0)
	{
		current_ = start_;
		return true;
	}
	--lCurrentStep_;
	edging();
	return false;
}

} // namespace Movie end
} // namespace BMW end