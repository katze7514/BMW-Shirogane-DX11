#include "stdafx.h"

#include "../../Chara/CDataCharaBattle.h"
#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/Weapon.h"
#include "../../Weapon/CalcWeapon.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "CSally_setup_weapon.h"

namespace BMW{
namespace SLG{
namespace Sally{

void CSally_setup_weapon::setupWeapon(Task::CTaskContext* pContext)
{
	using namespace BMW::Weapon;

	// SLGコンテキスト化
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	// キャラ戦闘データの取得
	CDataCharaSLG* pChara = p->getTargetCharaData();

	// 多重ロード禁止
	if(pChara->IsWeaponLoad()) return;

	Chara::CDataCharaBattle* pBattle = pChara->getBattlePtr();
	
	// 武器ID
	// 現在の設定IDを取得
	// どうせ++ずつなので、追加後のnIDとnStartを使えば、
	// すぐにWeponSLG IDを設定できる
	int nStart=p->getNextWeapon();
	// ランク計算用マップ
	map<int, Weapon::CDataWeaponBattle*> strMap; // 格闘用
	map<int, Weapon::CDataWeaponBattle*> mgcMap; // 魔術用
	// 武器データ生成ループ
	pBattle->beginWeapon();
	while(!pBattle->endWeapon())
	{
		list<int>::iterator it=pBattle->nextWeapon();

		#ifdef BMW_DEBUG
			CDbg().Out("WeaponInit %d",*it);
		#endif

		// 武器初期データの取得
		CDataWeaponInit* pInit = const_cast<Weapon::CWeaponDB&>(p->getApp()->getWeapon()).getData(*it);
	
		switch(pInit->getKind())
		{// 種別別生成
		// 合体
		case Kind::FIGHT_COLLAB:
		case Kind::MAGIC_COLLAB:
		{
			bool b = createBattleCollab(pInit, *it, pChara, p);
			if(!b) continue;
			// ↑データ設定を違えたら、NextWeaponIDを進めないためにcontinue
		}
		break;

		// 能力値アップ
		case Kind::STATUS:
			createBattle<Weapon::CDataWeaponBattleStatus>(pInit, *it, pChara, p);
		break;
		// 状態変化
		case Kind::MAGIC_COND:
		case Kind::FIGHT_COND:
			createBattle<Weapon::CDataWeaponBattleCond>(pInit, *it, pChara, p);
		break;
		
		// 治療
		case Kind::CURE: 
			// 治癒可能フラグを立てる
			pBattle->cure(p->getNextWeapon());
			createBattle<Weapon::CDataWeaponBattleCure>(pInit, *it, pChara, p);
		break;
		// 補給
		case Kind::REFILL:
			// 補給可能フラグを立てる
			pBattle->refill(p->getNextWeapon());
			createBattle<Weapon::CDataWeaponBattleRefill>(pInit, *it, pChara, p);	
		break;
		// 格闘
		case Kind::FIGHT:
		{
			Weapon::CDataWeaponBattle* pWeapon = createBattle<Weapon::CDataWeaponBattle>(pInit, *it, pChara, p);
			strMap.insert(pair<int,Weapon::CDataWeaponBattle*>(pWeapon->getAttack(),pWeapon));
		}
		break;
		// 魔術
		case Kind::MAGIC:
		{
			Weapon::CDataWeaponBattle* pWeapon = createBattle<Weapon::CDataWeaponBattle>(pInit, *it, pChara, p);
			mgcMap.insert(pair<int,Weapon::CDataWeaponBattle*>(pWeapon->getAttack(),pWeapon));
		}
		break;

		default: break;
		}
		p->setNextWeapon(p->getNextWeapon()+1);
	}

	// 武器IDのSLG ID化
	int i;
	pBattle->clearWeapon();
	for(i=nStart; i<p->getNextWeapon(); ++i) pBattle->addWeapon(i);

	// 攻撃力に合わせてRank計算
	map<int,Weapon::CDataWeaponBattle*>::iterator it;
	i=0;
	// 格闘
	for(it=strMap.begin(); it!=strMap.end(); ++it,++i)
		it->second->setRank(i);
	// 魔術
	i=0;
	for(it=mgcMap.begin(); it!=mgcMap.end(); ++it,++i)
		it->second->setRank(i);

	// ロード済みフラグを立てる
	pChara->weaponLoad(true);
}

void CSally_setup_weapon::OnAction(Task::CTaskContext* pContext)
{
	// セットアップ
	setupWeapon(pContext);
	// そして、戻る
	getTaskListCtrl()->returnTaskList();
}
////////////////////////////////////////////
// 武器種別別データ生成
////////////////////////////////////////////
template<class WeaponBattle>
WeaponBattle* CSally_setup_weapon::createBattle(Weapon::CDataWeaponInit* pInit, int nID, CDataCharaSLG* pChara, CSLGContext* pContext)
{
	WeaponBattle* pBattle = new WeaponBattle();
	// IDの設定
	pBattle->setID(nID);
	// ステータスの設定
	pBattle->setStatus(*pInit);
	// データ設定
	Weapon::setWeaponData(pBattle,*pChara,*pContext);
	// コンテキストに追加
	pContext->setWeaponData(pContext->getNextWeapon(), pBattle);

	return pBattle;
}

bool CSally_setup_weapon::createBattleCollab(Weapon::CDataWeaponInit* pInit, int nID, CDataCharaSLG* pChara, CSLGContext* pContext)
{// 合体武器
	Weapon::CDataWeaponBattleCollab* pBattle = new Weapon::CDataWeaponBattleCollab();
	// IDの設定
	pBattle->setID(nID);
	// ステータスの設定
	pBattle->setStatus(*pInit);
	// 自分自身をとりあえず設定
	pBattle->setSlgID(pChara->getID());
	// データ設定
	// これはcahra2slgCollabにて設定される
	//Weapon::setWeaponData(pBattle,*pChara,*pContext);

	// コンテキストに追加
	pContext->setWeaponData(pContext->getNextWeapon(), pBattle);

	// 必要なキャラはロードされている？
	Weapon::CDataWeaponBattleCollab::chara2slgCollab(*pChara,*pBattle,*pContext);

	return true;
}

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end