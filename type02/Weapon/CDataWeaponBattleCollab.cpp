#include "stdafx.h"

#include "../SLG/IDSLG.h"
#include "../SLG/Context/CSLGContext.h"
#include "../SLG/Context/CDataCharaSLG.h"

#include "../SLG/Map/CMapChip.h"
#include "../SLG/Map/CMapChipInfo.h"

#include "CalcWeapon.h"
#include "CDataWeaponBattleCollab.h"

namespace BMW{
namespace Weapon{

void CDataWeaponBattleCollab::Serialize(ISerialize& s)
{// やっぱし差分データ
	CDataWeaponBattle::Serialize(s);

	int nSize;
	if(s.IsStoring())
	{// Save
		// この合体攻撃を使用するキャラIDセットを保存
		nSize = (int)setSlgID_.size();
		s << nSize;
		for(set<int>::iterator it=setSlgID_.begin(); it!=setSlgID_.end(); ++it)
		{
			nSize = *it;
			s << nSize;
		}
	}
	else
	{// Load
		// この合体攻撃を使用するキャラIDセットを復元
		setSlgID_.clear();
		s << nSize;
		int n;
		for(int i=0; i<nSize; ++i)
		{
			s << n;
			setSlgID_.insert(n);
		}
	}
}

void CDataWeaponBattleCollab::delWeapon(SLG::CDataCharaSLG* pChara, SLG::CSLGContext& p)
{// 削除される時は、マップからいなくなる時なので
 // 対応する相手から自分を削除する
	if(pChara==NULL) return;
	
	set<int>::iterator it;
	for(it=setSlgID_.begin(); it!=setSlgID_.end(); ++it)
	{
		// キャラ取得
		SLG::CDataCharaSLG* pChara2 = p.getCharaData(*it);
		// そもそもいねーし
		if(pChara2==NULL) continue;
		// 対応する武器ID取得
		map<int,int>& mapCharaID = status_.getCollabCharaMap();
		for(map<int,int>::iterator it_m = mapCharaID.begin(); it_m!=mapCharaID.end(); ++it_m)
		{
			// 対応するキャラを見付ける
			int nID = p.searchSlgChild(it_m->first,pChara->getPhase());

			if(nID>=0 // 対応するキャラみっけ
			&& nID!=pChara->getID() // 自分自身はどうせ消えるので対象にしない
			)
			{	// 武器取得
				CDataWeaponBattle* pWeapon = getCollabWeapon(*pChara2,it_m->second,p);
				if(pWeapon!=NULL)
				{// 武器もちゃんと持ってる
					// 合体武器化
					CDataWeaponBattleCollab* pCollab = static_cast<CDataWeaponBattleCollab*>(pWeapon);
					// 相手の合体武器から自分を削除
					pCollab->getSlgIDSet().erase(pChara->getID());
				}
			}
		}
	}
}

bool CDataWeaponBattleCollab::enableNeed(const SLG::CDataCharaSLG& chara, bool bP, SLG::CSLGContext& context)
{
	// 合体攻撃武器の武器選択時の表示・非表示のため
	// 現在フェーズと使用キャラの所属フェーズが同じじゃないと使えない
	if(context.getPhase()!=chara.getPhase())
	{
		context.push(1);
		return false;
	}
	// 必要なキャラが全員いるか？
	// データをSLGにロードする段階で必要なキャラが登場する可能性がなかったら
	// そもそも武器データを生成しないのでenableNeedは必要なし
	if(!IsCollabLoad())
	{
		context.push(1);
		return false;
	}
	// 必要なマップの範囲を求める
	set<int> setMap;
	setMap.insert(chara.getIndex());
	// 使用するキャラの周囲8マスに全員がいないといけない
	getEightMapIndex(chara.getIndex(), setMap, context);
	// -1を削除
	setMap.erase(-1);

	int nCount=0;
	SLG::CDataCharaSLG* pChara;

	set<int>::iterator it;
	for(it=setSlgID_.begin(); it!=setSlgID_.end(); ++it)
	{
		pChara = context.getCharaData(*it);
		if(pChara!=NULL
		&& setMap.find(pChara->getIndex())!=setMap.end())
		{// 範囲内にいるなら、各キャラの武器使用条件を判定
		 // Need条件だけOK
			// ↓とやってしまうので、全キャラ同じNeed条件にすること
			if(CDataWeaponBattle::enableNeed(*pChara,bP,context))
				++nCount;
		}
		else
		{// ひとりでもいないなら、使えない
			context.push(1);
			return false;
		}
	}

	if((int)setSlgID_.size()!=nCount)
	{// 使用できない、けど、近くにはいるから表示はする
		context.push(0);
		return false; 
	}
	
	// 全キャラOKなら使用可能
	context.push(0);
	return true;
}

void CDataWeaponBattleCollab::use(SLG::CDataCharaSLG& chara, SLG::CSLGContext& context)
{
	// 各キャラのこれと同じ武器を探し適用する
	// 自分自身
	CDataWeaponBattle::use(chara,context);

	// 他
	map<int,int>::iterator it_m;
	map<int,int>& mapChara = status_.getCollabCharaMap();
	set<int>::iterator it;
	for(it=setSlgID_.begin(); it!=setSlgID_.end(); ++it)
	{
		// 自分自身は適用済み
		if(*it==chara.getID()) continue;
		
		// キャラデータ取得
		SLG::CDataCharaSLG* pChara = context.getCharaData(*it);
		if(pChara==NULL) return;

		// 対応する武器ID取得
		int nWeaponID=-1;
		for(it_m=mapChara.begin(); it_m!=mapChara.end(); ++it_m)
		{// キャラIDは親かもしれないのでIsChildが必要
			if(context.getApp()->getChara().IsChild(pChara->getCharaID(),it_m->first))
				nWeaponID = it_m->second;
		}

		// 武器取得
		CDataWeaponBattle* pWeapon = getCollabWeapon(*pChara,nWeaponID,context);

		// ループが何度も回り重複適用することになるので、直呼び
		if(pWeapon!=NULL) 
			pWeapon->CDataWeaponBattle::use(*pChara,context);
	}
}

CDataWeaponBattle* CDataWeaponBattleCollab::getCollabWeapon(SLG::CDataCharaSLG& chara, int nWeaponID, SLG::CSLGContext& context)
{
	// まだ、武器をロードしていないかもしれない
	if(!chara.IsWeaponLoad())	return NULL;

	// してたら、対象の武器データを検索
	Chara::CDataCharaBattle& battle = chara.getBattle();
	battle.beginWeapon();
	while(!battle.endWeapon())
	{// 対応する武器を探す
		int nID = *battle.nextWeapon();
		CDataWeaponBattle* pWeapon = context.getWeaponData(nID);
		
		if(pWeapon!=NULL
		&& pWeapon->getID()==nWeaponID) // これだ！
			return pWeapon;
	}

	return NULL;
}

void CDataWeaponBattleCollab::chara2slgCollab(SLG::CDataCharaSLG& chara, CDataWeaponBattleCollab& collab, SLG::CSLGContext& context)
{
	// まずは、現在ロードされているキャラを取得する
	int nID;
	map<int,int>& mapCharaID = collab.getCollabCharaMap();
	map<int,int>::iterator it;
	CDataWeaponBattle* pWeapon;
	for(it=mapCharaID.begin(); it!=mapCharaID.end(); ++it)
	{
		// 合体攻撃は対象IDの子供だったらOK
		nID = context.searchSlgChild(it->first,chara.getPhase());
		if(nID>=0)
		{// 見つかったら、そいつをこいつのセットに入れつつ
			// 自分自身だったらスキップ
			if(nID==chara.getID()) continue;
			// 別キャラ
			collab.setSlgID(nID);
			// こいつのIDを見つけた相手の武器はロードされてる？
			pWeapon = getCollabWeapon(*context.getCharaData(nID),it->second,context);

			if(pWeapon!=NULL)
			{// 相手の武器もロードされてたら、セットに突っ込む
				CDataWeaponBattleCollab* pCollab = static_cast<CDataWeaponBattleCollab*>(pWeapon);
				pCollab->setSlgID(chara.getID());

				// 全キャラがロードされたら養成適用
				if(!pCollab->IsCollabSetup()
				&& pCollab->IsCollabLoad())
				{
					pCollab->collabSetup(true);
					setWeaponData(pCollab,chara,context);
				}
			}
		}
	}

	if(!collab.IsCollabSetup()
	&& collab.IsCollabLoad())
	{// 最後に自分自身だったら状況が揃っているかを確認
		collab.collabSetup(true);
		setWeaponData(&collab,chara,context);
	}
}

void CDataWeaponBattleCollab::getEightMapIndex(int nIndex, set<int>& setMap, SLG::CSLGContext& context)
{
	// 基準となるマップチップ取得
	SLG::Map::CMapChip* pBase = context.getMapChip(nIndex);
	// 上
	SLG::Map::CMapChip* pTemp = context.getMapChip(pBase->getMapInfo().getOnMap(SLG::Way::TOP));
	if(pTemp!=NULL)
	{	setMap.insert(pTemp->getIndex());
		// 上左
		setMap.insert(pTemp->getMapInfo().getOnMap(SLG::Way::LEFT));
		// 上右
		setMap.insert(pTemp->getMapInfo().getOnMap(SLG::Way::RIGHT));
	}
	// 下
	pTemp = context.getMapChip(pBase->getMapInfo().getOnMap(SLG::Way::BOTTOM));
	if(pTemp!=NULL){
		setMap.insert(pTemp->getIndex());
		// 下左
		setMap.insert(pTemp->getMapInfo().getOnMap(SLG::Way::LEFT));
		// 下右
		setMap.insert(pTemp->getMapInfo().getOnMap(SLG::Way::RIGHT));
	}
	// 左
	pTemp = context.getMapChip(pBase->getMapInfo().getOnMap(SLG::Way::LEFT));
	if(pTemp!=NULL){
		setMap.insert(pTemp->getIndex());
		// 左上
		setMap.insert(pTemp->getMapInfo().getOnMap(SLG::Way::TOP));
		// 左下
		setMap.insert(pTemp->getMapInfo().getOnMap(SLG::Way::BOTTOM));
	}
	// 右
	pTemp = context.getMapChip(pBase->getMapInfo().getOnMap(SLG::Way::RIGHT));
	if(pTemp!=NULL){
		setMap.insert(pTemp->getIndex());
		// 左上
		setMap.insert(pTemp->getMapInfo().getOnMap(SLG::Way::TOP));
		// 左下
		setMap.insert(pTemp->getMapInfo().getOnMap(SLG::Way::BOTTOM));
	}
}

} // namespace Weapon end
} // namespace BMW end