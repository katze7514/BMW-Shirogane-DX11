#include "stdafx.h"

#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/Weapon.h"
#include "../../Weapon/CalcWeapon.h"
#include "../Action/IAction.h"

#include "CDataCharaSLG.h"
#include "CMapSymbolDB.h"
#include "CSLGDef.h"
#include "CSLGContext.h"

namespace BMW{
namespace SLG{

void CSLGContext::Serialize(ISerialize& s)
{
	if(s.IsStoring())	SerializeSave(s);
	else				SerializeLoad(s);
}

void CSLGContext::SerializeSave(ISerialize& s)
{// コンテニューデータとしてデータを格納する
	// 現在鳴っているBGM ID
	nBgm_ = getApp()->getBgm()->getBgmID();
	s << nBgm_;
	// フラグ
	int nFirst;
	int nSize = (int)mapValue_.size();
	s << nSize;
	map<int, int>::iterator it;
	for(it=mapValue_.begin(); it!=mapValue_.end(); ++it)
	{
		nFirst = it->first;
		s << nFirst << it->second;
	}

	// フェーズリスト
	SerializeList(getPlayerPhaseList(),s);
	SerializeList(getEnemyPhaseList(),s);
	SerializeList(getNeutralPhaseList(),s);
	// 死亡セット
	SerializeSet(getDeathSet(),s);

	// キャラデータ
	nSize = (int)mapChara_.size();
	s << nSize;
	chara_map::iterator it_c;
	for(it_c=mapChara_.begin(); it_c!=mapChara_.end(); ++it_c)
	{
		if(it_c->second!=NULL)
			s << *(it_c->second);
	}

	// 武器データ
	// SLG ID, 武器種別, 武器差分データ
	nSize = (int)mapWeapon_.size();
	s << nSize;
	weapon_map::iterator it_w;
	for(it_w=mapWeapon_.begin(); it_w!=mapWeapon_.end(); ++it_w)
	{
		nFirst = it_w->first;
		s <<  nFirst;
		nFirst = (it_w->second)->getKind();
		s << nFirst;
		s << *(it_w->second);
	}

	// 現在、手に入れているアイテムリスト
	SerializeList(listGetItem_,s);
}

void CSLGContext::SerializeLoad(ISerialize& s)
{// データを復元する
 // これを元にコンテキスト完全にするのは、別のフェーズで
	// 鳴っていたBGM ID
	s << nBgm_;
	// フラグ復元
	clearFlag();
	int nSize;
	s << nSize;
	int nFirst,nSecond;
	for(int i=0; i<nSize; ++i)
	{
		s << nFirst << nSecond;
		mapValue_[nFirst]=nSecond;
	}

	// フェーズリスト
	SerializeList(getPlayerPhaseList(),s);
	SerializeList(getEnemyPhaseList(),s);
	SerializeList(getNeutralPhaseList(),s);
	// 死亡セット
	SerializeSet(getDeathSet(),s);

	// キャラデータ
	int n=0;
	clearCharaData();
	s << nSize;
	CDataCharaSLG* pChara;
	for(int i=0; i<nSize; ++i)
	{
		pChara = new CDataCharaSLG();
		// このシリアライズは、差分情報のみやで
		s << *pChara;
		mapChara_[pChara->getID()]=pChara;

		// 味方でIDが確保されてなかったら、出撃キャラとして文字列ID追加
		// IDが確保されてれば次に追加するSLG IDSLG IDが大きいはず
		if(pChara->getPhase()==Phase::PLAYER && pChara->getID()>getNextSally()) 
			getSLGDef().setSlgID(Misc::linkStrAndNum("_CHARA",++n),pChara->getID());
	}

	// 武器データ
	clearWeaponData();
	Weapon::CDataWeaponBattle* pWeapon;
	s << nSize;
	for(int i=0; i<nSize; ++i)
	{
		s << nFirst << nSecond;
		// 種別によって生成されるインスタンスが変わる
		pWeapon = Weapon::createWeapon(nSecond);
		s << *pWeapon;
		pWeapon->setKind(nSecond);
		mapWeapon_[nFirst]=pWeapon;
	}

	// 現在、手に入れているアイテムリスト
	SerializeList(listGetItem_,s);
}

void CSLGContext::SerializeList(list<int>& List, ISerialize& s)
{
	int nSize,nFirst;
	if(s.IsStoring())
	{// Save
		nSize = (int)List.size();
		s << nSize;
		list<int>::iterator it_d;
		for(it_d=List.begin(); it_d!=List.end(); ++it_d)
			s << *it_d;
	}
	else
	{// Load
		List.clear();
		s << nSize;
		for(int i=0; i<nSize; ++i)
		{
			s << nFirst;
			List.push_back(nFirst);
		}
	}

}

void CSLGContext::SerializeSet(set<int>& Set, ISerialize& s)
{
	int nSize,nFirst;
	if(s.IsStoring())
	{// Save
		nSize = (int)Set.size();
		s << nSize;
		set<int>::iterator it_d;
		for(it_d=Set.begin(); it_d!=Set.end(); ++it_d)
			s << (int)*it_d;
	}
	else
	{// Load
		Set.clear();
		s << nSize;
		for(int i=0; i<nSize; ++i)
		{
			s << nFirst;
			Set.insert(nFirst);
		}
	}

}

void CSLGContext::completeData()
{// データの完全化
	// SLG DEF復元
	slgDef_->setSLGDef(getScenarioData()->getScenarioFile(getID()),this);
	// ↑をしておかないと、Trainデータが正しく取得できない
	map<string, int> mapID;
	CDataCharaSLG* pChara;
	chara_map::iterator it=mapChara_.begin();
	while(it!=mapChara_.end())
	{	// キャラデータ
		pChara = it->second;
		if(pChara==NULL){ ++it; continue; }

		if(pChara->getAction()->IsNonPlayer()
		&&(pChara->getState().getAct()==Act::DEATH
		|| pChara->getState().getAct()==Act::DEATH_EVENT
		|| pChara->getState().getAct()==Act::REMOVE))
		{// 最早存在してないなら、ここでも削除

			// 武器データの削除
			if(pChara->IsWeaponLoad())
			{// 武器がロードされていたら
				delCharaWeaponData(pChara);
			}

			// キャラデータは最後に削除
			DELETE_SAFE(pChara);
			// マップから削除
			mapChara_.erase(it++);
		}
		else
		{// データの正常化
			completeChara(*pChara);

			// 武器データ
			if(pChara->IsWeaponLoad())
			{// 武器がロードされていたら
				pChara->getBattle().beginWeapon();
				while(!pChara->getBattle().endWeapon())
					completeWeapon(getWeaponData(*(pChara->getBattle().nextWeapon())),*pChara);
			}

			++it;
		}
	}
}

void CSLGContext::completeChara(CDataCharaSLG& chara)
{// キャラデータを完全化
	// まずは、養成データの取得
	Chara::CDataCharaTrain train;
	readyTrain(train,chara);

	// 引継ぎプレイの場合、敵だったら設定されている養成段階を＋する
	// コピーを使ってるので養成段階を戻すのはいらない
	Save::CExecData& exec = getApp()->getExec();
	if((exec.getFlag("HANDOVER", 1) || exec.getFlag("HANDOVER", 2)) && chara.getPhase()==Phase::ENEMY)
	{
		int nEnemyTrain=0;
		exec.getFlag(Scene::Const::flagID_.getValue("ENEMY_TRAIN"),nEnemyTrain);
		train.calcTrainAll(nEnemyTrain);
	}

	// 完全化
	getApp()->getChara().setBattle(chara.getBattlePtr(),
								   chara.getBattle().getID(),
								   train,
								   -1);

	// アイテムの効果適用1
	Item::CItemDB& db = getApp()->getItem();
	Chara::CDataCharaBattle& battle = chara.getBattle();
	battle.beginItem();
	while(!battle.endItem())
	{
		Chara::CStatusAbility& item = *battle.nextItem();
		// また、アイテムによる能力UP等があればここで行う
		if(db.IsStatus(item.getID()))
			db.applyStatus(chara, item.getAttr(), item.getID());
	}

	// チップシンボル展開
	// それまでに、同じシンボルを持ってるやつがいたら共有
	int nID = searchMapSymbol(chara.getBattle().getMapSymbolID(), chara.getPhase());
	if(nID>=0)
		chara.setMapSymbol(getCharaData(nID)->getMapSymbol());
	else
		chara.createMapSymbol(chara.getBattle().getMapSymbolID());
}

void CSLGContext::readyTrain(Chara::CDataCharaTrain& train, CDataCharaSLG& chara)
{
#ifdef BMW_DEBUG
//	CDbg().Out("CHARA COMP %d %d %d",chara.getTrain(),chara.getCharaID(), chara.getID());
#endif
	if(chara.getTrain()>=0)
	{// 0以上だとSLGDEFより取得
		train = getTrain(chara.getTrain());
	}
	else
	{// そうじゃなければ、セーブデータより
		train = *getApp()->getExec().getTrainData(chara.getCharaID(),false);
	}
	train.over(chara.getBattle());
}

void CSLGContext::completeWeapon(Weapon::CDataWeaponBattle* pWeapon, CDataCharaSLG& chara)
{
	// ステータスの設定
	pWeapon->setStatus(*(const_cast<Weapon::CWeaponDB&>(getApp()->getWeapon()).getData(pWeapon->getID())));
	// 養成の設定
	if(pWeapon->getKind()!=Weapon::Kind::FIGHT_COLLAB
	&& pWeapon->getKind()!=Weapon::Kind::MAGIC_COLLAB)
		Weapon::setWeaponData(pWeapon,chara,*this,true);
	else
	{// 合体武器の場合はこっち
		Weapon::CDataWeaponBattleCollab* pCollab = static_cast<Weapon::CDataWeaponBattleCollab*>(pWeapon);
		if(pCollab->IsCollabLoad())
			Weapon::setWeaponData(pCollab,chara,*this,true);
	}
}

} // namespace SLG end
} // namespace BMW end