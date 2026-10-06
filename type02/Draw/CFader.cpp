#include "stdafx.h"

#include "CFader.h"

namespace BMW{
namespace Draw{

CFader::CFader()
{
	plane_.CreateSurface(640,480,false);
	visible(false);
	nFadeType_=FADE_OUT;
}

void CFader::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case FADE:
		setAlpha(++counter_);
		if(counter_.IsEnd())
		{ 
			setState(NORMAL);
			if(counter_==0)
			{
				visible(false);
				// FadeOut後ならタスクリストからはずれる
				pContext->getTaskList()->removeMe();
			}
			// 終了したのイベントハンドラを呼び出す
			faderFun(pContext);
		}
	break;

	default: break;
	}
}

void CFader::OnDraw(Task::CTaskContext* pContext)
{
	(*pContext->getDrawPlane())->BlendBltFast(&plane_,0,0,drawInfo_.getAlpha());
}

void CFader::fadeIn(int nFrame)
{
	counter_.Set(drawInfo_.getAlpha(),255,nFrame);
	setState(FADE);
	visible(true);
	setAlpha(counter_);
	nFadeType_=FADE_IN;
}

void CFader::fadeOut(int nFrame)
{ 
	counter_.Set(drawInfo_.getAlpha(),0,nFrame); 
	setState(FADE); 
	visible(true);
	setAlpha(counter_);
	nFadeType_=FADE_OUT;
}

} // namespace Draw end
} // namespace BMW end