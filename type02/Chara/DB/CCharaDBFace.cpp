#include "stdafx.h"

#include "CCharaDB.h"

namespace BMW{
namespace Chara{

CDataFace* CCharaDB::getFaceData(int nID, int nFace)
{
	return mapFace_[nID]->getFaceData(nFace);
}

CDataFace* CCharaDB::getFaceData(int nID, const string& sFace)
{
	return mapFace_[nID]->getFaceData(sFace);
}

//////////////////////////////////////////////////////////////
// グラフィックデータ変換
//////////////////////////////////////////////////////////////
void CCharaDB::setFaceGraphic(CGraphic* pGraphic, int nID, int nFace, int nToward, bool bBattle=false)
{
}

void CCharaDB::setFaceGraphic(CGraphic* pGraphic, int nID, const string& sFace, int nToward, bool bBattle=false)
{
}

} // namespace Chara end
} // namespace BMW end