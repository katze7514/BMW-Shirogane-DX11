/*
	katze 06/02/04
	数字の基底クラス
*/
#pragma once

namespace BMW{
namespace GUI{

class INum : public Task::CTaskBase
{/**
	数字のインターフェイスを基底するクラス
	数字のNullDeviceとしても使える
*/
public:
	// デストラクタ
	virtual ~INum(){}

	// 設定・取得
	virtual LONG getNum() const { return 0; }
	virtual void setNum(LONG lNum){}
	virtual bool IsPlus() const { return false; }
	virtual void plus(bool bPlus){}

};

} // namespace GUI end
} // namespace BMW end