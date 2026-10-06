/*
	katze 05/03/06
	CharaDB解析時に使うClosure
*/
#pragma once

#include "../CStatusFund.h"
#include "../CStatusBattle.h"
#include "../CStatusAbility.h"
#include "../CStatusGrowthAbility.h"
#include "../CStatusGrowthStatus.h"
#include "../CValidSkill.h"
#include "../CDataCharaInit.h"
#include "../CDataCharaGrowthAbility.h"
#include "../CDataCharaGrowthStatus.h"
#include "../CDataCharaTrain.h"

namespace BMW{
namespace Chara{

// 基礎ステータスルールクロージャ
struct fund_closure : public boost::spirit::closure<fund_closure, CStatusFund>
{
	member1 val;
};

// 戦闘ステータスルールクロージャ
struct battle_closure : public boost::spirit::closure<battle_closure, CStatusBattle>
{
	member1 val;
};

// アビリティルールクロージャ
struct ability_closure : public boost::spirit::closure<ability_closure, CStatusAbility>
{
	member1 val;
};

// スキルValidクロージャ
struct skill_closure : public boost::spirit::closure<skill_closure, CValidSkill>
{
	member1 val;
};

// アビリティ成長ルールクロージャ
struct ability_g_closure : public boost::spirit::closure<ability_g_closure, CStatusGrowthAbility>
{
	member1 val;
};

// 基礎ステ成長ルールクロージャ
struct status_g_closure : public boost::spirit::closure<status_g_closure, CStatusGrowthStatus>
{
	member1 val;
};

// キャラ初期値ルールクロージャ
struct init_closure : public boost::spirit::closure<init_closure, CDataCharaInit,int,string>
{
	member1 val;
	member2 n;
	string  s;
};

// キャラアビリティ成長ルールクロージャ
struct c_ability_g_closure : public boost::spirit::closure<c_ability_g_closure, CDataCharaGrowthAbility, int>
{
	member1 val;
	member2 count;
};

// 養成データクロージャ
struct train_closure : public boost::spirit::closure<train_closure, CDataCharaTrain>
{
	member1	val;
};

}
}