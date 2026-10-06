/*
	katze 05/03/22
	全体を通したコンフィグデータ
*/
#pragma once

#include "DB/CConfigDB.h"

namespace BMW{
namespace Config{

class Const
{
public:
	// コンフィグIDのマップ
	static CConfigDB configDB_;
};

} // namespace Config end
} // namesoace BMW end