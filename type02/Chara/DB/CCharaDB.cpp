#include "stdafx.h"

#include "../../mode.h"

#include "../CDataCharaBattle.h"
#include "../CDataCharaInter.h"
#include "../CDataCharaGrowthStatus.h"
#include "../CDataCharaTrain.h"

#include "CDataCharaData.h"

#include "CCharaDB.h"

namespace BMW{
namespace Chara{
CCharaDB::~CCharaDB()
{
	chara_map::iterator it;
	for(it=mapChara_.begin(); it!=mapChara_.end(); it++)
		DELETE_SAFE(it->second);

	mapChara_.clear();
}

void CCharaDB::setCharaDB(const string& sFile)
{
	using namespace boost::spirit;
	using namespace phoenix;

	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(sFile);
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	// 構文解析
	CCharaParser ps(*this);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full)	CDbg().Out("%s 読み込み失敗！！", r.stop);
#endif
}

void CCharaDB::setStatusDB(const string& sFile)
{
	using namespace boost::spirit;
	using namespace phoenix;

	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(sFile);
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	// 構文解析
	CStatusParser ps(*this);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r =
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full)	CDbg().Out("%s 読み込み失敗！！", r.stop);
#endif
}

//////////////////////////////////////////////////////
// 各キャラクラスに対する設定
//////////////////////////////////////////////////////
void CCharaDB::setBattle(CDataCharaBattle* battle, int nID, CDataCharaTrain& train, int nLv) const
{
	CCharaDB* pDB = const_cast<CCharaDB*>(this);
	int nSpLv=0;
	int nMoveLv;
	CDataCharaData* pData = pDB->getCharaData(nID);
	if(nLv<=0)
	{// 追記フラグがoffなら、データ初期化なので
	 // 初期値と養成を適用する
		// とりあえず、キャラIDの設定
		//battle->setID(nID);
		// 初期値設定
		// 負の時はコンテニュー時のデータ生成
		pData->applyInit(battle,nLv<0);

		#ifdef HP_DIV // HP_DIVモードだったら敵のHP100分の1
		if(battle->getFP()>0) // FPが設定されてるのは敵だけ
			battle->setHP(battle->getBattle().getHP()/100);
		#endif

		// 養成
		train.apply(battle);
	}
	else
	{// 追記ONなら、一端SPUP/MOVEUPの効果削除
		nSpLv = battle->hasSkill(Ability::SPUP);
		if(nSpLv>0) battle->setSP(battle->getMaxSP() - 6*nSpLv);

		nMoveLv = battle->hasSkill(Ability::MOVE_UP);
		if(nMoveLv>0)
		{
			battle->setMove(battle->getMove()-1*nMoveLv);
			battle->setJump(battle->getJump()-2*nMoveLv);
		}
	}
	// そして、成長
	// まずは、技能
	pData->applyAbility(battle, nLv);
	// んで、ステータス
	pDB->getStatusData(battle->getGrowth()).apply(battle, nLv);

	// 養成技能
	// Lvアップの時は必要ない
	if(nLv<=0) train.applySkill(battle);

	// 技能による能力付与
	// SPUPの効果
	nMoveLv=nSpLv;
	nSpLv = battle->hasSkill(Ability::SPUP);
	if(nSpLv>0)
	{ 
		battle->setSP(battle->getMaxSP() + 6*nSpLv);
		if(nMoveLv<0) nMoveLv=0;
		// LVUPの時だけ回復する
		if(nLv>0) battle->calcSP(-6*(nSpLv-nMoveLv));
	}

	// 移動力の効果
	nMoveLv = battle->hasSkill(Ability::MOVE_UP);
	if(nMoveLv>0)
	{
		battle->setMove(battle->getMove()+1*nMoveLv);
		battle->setJump(battle->getJump()+2*nMoveLv);
	}

	// 真祖の効果　初期化時のみでOK
	if(nLv<=0 && battle->IsTalent(Ability::ORIGIN))
		battle->setDefence(battle->getMaxDefence()+30);

	// アイテム効果は別フェーズ
}

void CCharaDB::setInter(CDataCharaInter* inter, int nID, CDataCharaTrain& train) const
{
	CCharaDB* pDB = const_cast<CCharaDB*>(this);

	CDataCharaData* pData = pDB->getCharaData(nID);
	// とりあえず、キャラIDの設定
	//inter->setID(nID);
	// 初期値設定
	pData->applyInit(inter);
	// 養成
	train.apply(inter);
	inter->setTrainData(smart_ptr<CDataCharaTrain>(&train,false));
	// そして、成長
	// まずは、技能
	pData->applyAbility(inter);
	// んで、ステータス
	pDB->getStatusData(inter->getGrowth()).apply(inter);

	// 技能による能力付与
	// SPUPの効果
	int nLv = inter->hasSkill(Ability::SPUP);
	if(nLv>0) inter->setSP(inter->getSP() + 6*nLv);

	// 移動力の効果
	nLv = inter->hasSkill(Ability::MOVE_UP);
	if(nLv>0)
	{
		inter->setMove(inter->getMove()+1*nLv);
		inter->setJump(inter->getJump()+2*nLv);
	}

	// 真祖の効果　初期化時のみでOK
	if(nLv<=0 && inter->IsTalent(Ability::ORIGIN))
		inter->setDefence(inter->getSourceDefence()+20);

	// アイテムによる能力付与は別フェーズ
}


bool CCharaDB::IsChild(int nChild, int nParent) const
{// nChildが、nParentの子かどうかを判定する
	if(nChild==nParent) return true;

	// 親をさかのぼっていく
	CCharaDB* pDB = const_cast<CCharaDB*>(this);
	CDataCharaData* pData = pDB->getCharaData(nChild);
	if(pData==NULL) return false;
	while(!pData->getParentData().isNull())
	{
		if(pData->getParentData()->getInit().getID()==nParent)
			return true;
		
		pData = pDB->getCharaData(pData->getParentData()->getInit().getID());
	}
	// ここに来たら、継承関係はなかったことに
	return false;
}

} // namespace Chara end
} // namespace BMW end