/*
	katze 06/02/02
	deleteファンクタ
*/
#pragma once

namespace katzeSDK{
namespace Misc{

struct DeleteObj{
	template<typename T>
	void operator()(const T* ptr)const
	{
		DELETE_SAFE(ptr);
	}
};

struct DeleteMapObj{
	template<typename T>
	void operator()(T& Map)const
	{
		typename T::iterator it;
		for(it=Map.begin(); it!=Map.end(); it++)
			DELETE_SAFE(it->second);

		Map.clear();
	}
};

} // namespace Misc end
} // namespace katzeSDK end

using katzeSDK::Misc::DeleteObj;
using katzeSDK::Misc::DeleteMapObj;