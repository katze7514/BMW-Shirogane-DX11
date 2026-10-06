#include "stdafx.h"

#include "../Chara/CDataCharaBattle.h"
#include "CFundTable.h"

namespace BMW{
namespace Ability{

CFundTable::CFundTable()
{
	for(int i=0; i<9; i++)
	{
		for(int j=0; j<10; j++)
		{
			int nValue = ((i+1)-j)*5;
			if(nValue<0) nValue=0;
				
			table_[i][j].setHit(nValue);
			table_[i][j].setAvoid(nValue);
			table_[i][j].setDef(nValue);
			table_[i][j].setCT((nValue*8)/5);
		}
	}
}

int	CFundTable::getRank(const Chara::CDataCharaBattle& battle)const
{
	// Žc‚èHP‚É‚æ‚Á‚ÄRank‚ªŒˆ‚Ü‚é
	int nRank = battle.getHP()*100/battle.getMaxHP();

	if(nRank<=10) return 0;
	ef(nRank<=20) return 1;
	ef(nRank<=30) return 2;
	ef(nRank<=40) return 3;
	ef(nRank<=50) return 4;
	ef(nRank<=60) return 5;
	ef(nRank<=70) return 6;
	ef(nRank<=80) return 7;
	ef(nRank<=90) return 8;
	else		  return 9;
}

} // namesapce Ability end
} // namespace BMW end