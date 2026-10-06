/*
	katze 05/03/01
	キャラステータス成長データ
*/
#pragma once

#include "CStatusGrowthStatus.h"

namespace BMW{
namespace Chara{

class CDataCharaBattle;
class CDataCharaInter;

class CDataCharaGrowthStatus
{/**
	キャラステータスの成長データ
 */
public:
	// 操作
	status_g_list::iterator	beginStatus() const 
							{ 
								CDataCharaGrowthStatus* pStatus = const_cast<CDataCharaGrowthStatus*>(this);
								pStatus->it_st=pStatus->listStatus_.begin();
								return pStatus->it_st;
							}
	bool					endStatus() const
							{ 
								CDataCharaGrowthStatus* pStatus = const_cast<CDataCharaGrowthStatus*>(this);
								return pStatus->it_st==pStatus->listStatus_.end();
							}
	status_g_list::iterator	nextStatus() const 
							{
								CDataCharaGrowthStatus* pStatus = const_cast<CDataCharaGrowthStatus*>(this);
								return pStatus->it_st++;
							}
	void					addStatus(const CStatusGrowthStatus& status)
							{// Lv順に並べておく
								for(it_st=listStatus_.begin(); it_st!=listStatus_.end(); ++it_st)
									if(it_st->getLv() > status.getLv()) break;

								listStatus_.insert(it_st,status);
							}
	// データ適用
	void					apply(CDataCharaBattle* battle, int nLv=0);
	void					apply(CDataCharaInter* inter, int nLv=0);

private:
	// 成長データリスト
	status_g_list	listStatus_;
	status_g_list::iterator it_st;
};

} // namespace Chara end
} // namespace BMW end