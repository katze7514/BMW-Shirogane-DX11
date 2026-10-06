/*
	katze 05/04/10
	マップローダ
*/
#pragma once

#include "../../Draw/DB/CSpriteDB.h"

namespace BMW{
namespace SLG{
namespace Map{

class CMap;
class CMapLoader
{/**
	マップローダ

	他のParserと違って、ファイルを読みながら
	実データを構築していく
 */
public:
	// 操作
	void setMap(CMap* pMap, const string& sFile);

private:
	// このマップが持つスプライトデータ
	Draw::CSpriteDB sprite_;
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end