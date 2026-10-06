/*
	katze 06/01/20
	キャラ名グラフィック
*/
#pragma once

namespace BMW{
namespace GUI{

class CGraphicName : public CGraphic
{/**
	キャラ名グラフィック
 */
public:
	// デストラクタ
	virtual ~CGraphicName(){}

	void setCharaName(int nCharaID, Task::CTaskContext* pContext, int nX=0, int nY=0);

	// 取得・設定
	int	 getToward()const{ return nToward_; }
	void setToward(int nToward){ nToward_=nToward; }

private:
	int nToward_;
};

} // namespace GUI end
} // namespace BMW end