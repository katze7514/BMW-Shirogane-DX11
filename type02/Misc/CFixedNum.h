/*
	katze 05/05/15
	固定小数点
*/
#pragma once

namespace katzeSDK{
namespace Misc{

class CFixedNum
{/**
	固定小数点(16bit)
 */
public:
	// コンストラクタ
	CFixedNum():nNum_(0){}
	CFixedNum(int n):nNum_(n<<16){}

	// 取得
	int getNum()const{ return nNum_; }
	void setNum(int n){ nNum_=n; }

	// 四則演算
	const CFixedNum operator+(const CFixedNum& rhs)
	{
		CFixedNum tmp;
		tmp.nNum_ = nNum_ + rhs.nNum_;
		return tmp;
	}

	const CFixedNum operator-(const CFixedNum& rhs)
	{
		CFixedNum tmp;
		tmp.nNum_ = nNum_ - rhs.nNum_;
		return tmp;
	}

	const CFixedNum operator*(const CFixedNum& rhs)
	{
		CFixedNum tmp;
		tmp.nNum_ = nNum_ * rhs.nNum_;
		return tmp;
	}

	const CFixedNum operator/(const CFixedNum& rhs)
	{
		CFixedNum tmp;
		tmp.nNum_ = nNum_ / rhs.nNum_;
		return tmp;
	}

	// int型と見せかけるためのoperator
	operator int () const { return roundRShift(nNum_,16); }
	int operator=(int n){ nNum_ = (n<<16); return n; }

private:
	// ↓こいつの下位16bitが小数部
	int nNum_;
};

} // namespace Misc end
} // namespace katzeSDK end