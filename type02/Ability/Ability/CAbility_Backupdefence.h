/*
	katze 06/03/14
	BACKUPDEFENCE
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Backupdefence : public IDataAbility
{/**
	援護防御
*/
public:
	// コンストラクタ
	CAbility_Backupdefence(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	// この技能を獲得するのに必要なFP
	int  getGetFP(int nAttr=0)const;
	
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

