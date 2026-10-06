/*
	katze 06/01/20
	パネルに配置されるウィジットデータ
*/
#pragma once

#include "IDGuiDef.h"
#include "IDataGuiDef.h"

namespace BMW{
namespace GUI{

class IDataGuiDefWidget : public IDataGuiDef
{/*
	パネルに配置されるウィジットデータ
 */
public:
	// コンストラクタ・デストラクタ
	IDataGuiDefWidget():nX_(0),nY_(0){ setKind(Widget::OBJ); }
	virtual ~IDataGuiDefWidget(){}
	// 設定・取得
	int				getX()const{ return nX_; }
	void			setX(int nX){ nX_=nX; }
	int				getY()const{ return nY_; }
	void			setY(int nY){ nY_=nY; }

	void			setPos(const POINT& pos){ nX_=pos.x; nY_=pos.y; }

protected:
	// 描画位置
	int nX_,nY_;
};

} // namespace GUI end
} // namespace BMW end