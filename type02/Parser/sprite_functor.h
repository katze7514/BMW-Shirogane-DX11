/*
	katze 05/02/27
	解析用ファンクタ
*/
#pragma once
// Spiritの警告Lv4で発生する警告、Spiritでは問題ないらしいので抑制してしまう
#pragma warning(disable:4512)
#pragma warning(disable:4511)
#pragma warning(disable:4709)

#include <boost/spirit/phoenix/primitives.hpp>
#include <boost/spirit/phoenix/operators.hpp>
#include <boost/spirit/phoenix/functions.hpp>

namespace BMW{
namespace Func{
// usign宣言
using namespace phoenix;

// RECTへの設定
struct set_rect_impl
{
	template<typename RectT, typename ItemT, typename FlagT>
	struct result
	{
		typedef void type;
	};

	template<typename RectT, typename ItemT, typename FlagT>
	void operator()(RectT& r, ItemT nVal, FlagT nFlag) const
	{
		switch(nFlag)
		{
		case 0: r.left=nVal; break;
		case 1: r.top=nVal; break;
		case 2: r.right=nVal; break;
		case 3: r.bottom=nVal; break;
		default:break;
		}
	}
};
// lambda
const function<set_rect_impl> set_rect = set_rect_impl();

// Offset設定
struct set_offset_impl
{
	template<typename PointT, typename ItemT, typename FlagT>
	struct result
	{
		typedef void type;
	};

	template<typename PointT, typename ItemT, typename FlagT>
	void operator()(PointT& p, ItemT nVal, FlagT nFlag) const
	{
		switch(nFlag)
		{
		case 0: p.x=nVal; break;
		case 1: p.y=nVal; break;
		default:break;
		}
	}
};
// lambda
const function<set_offset_impl> set_offset = set_offset_impl();

} // namespace Func end
} // namespace BMW end