/*
	katze 05/12/22
	キャラをマップ上に追加する Ver.2
*/
#pragma once

#include "CSally_add_chara_map.h"

namespace BMW{
namespace SLG{
namespace Sally{

class CSally_add_chara_map2 : public CSally_add_chara_map
{/**
 　キャラをマップ上に追加する
 　
	ようは、キャラチップVer.2に対応する
 */
protected:
	virtual void setupCharaChip(CDataCharaSLG* pChara, int nIndex, CSLGContext* p);
};

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end