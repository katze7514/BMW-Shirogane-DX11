#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"
#include "../../Item/IDItem.h"

#include "../IDSLG.h"
#include "../Action/IAction.h"
#include "../Action/CActionFactory.h"

#include "CSLGContext.h"
#include "CMapSymbolDB.h"

#include "CDataCharaSLG.h"

namespace BMW{
namespace SLG{

CDataCharaSLG::CDataCharaSLG():nID_(-1),nPhase_(-1),pAction_(NULL),bWeapon_(false)
{}

CDataCharaSLG::~CDataCharaSLG()
{
	DELETE_SAFE(pAction_);
}

void CDataCharaSLG::createMapSymbol(const string& sFile)
{
	pMapSymbol_.Add(new CMapSymbolDB());
	pMapSymbol_->setSymbol(sFile);
}

bool CDataCharaSLG::IsExist() const
{
	return state_.getAct()>=0
		&& state_.getAct()!=Act::DEATH
		&& state_.getAct()!=Act::REMOVE 
		&& state_.getIndex()>=0;
}

bool CDataCharaSLG::IsLive() const
{
	return state_.getAct()>=0
		&& state_.getAct()!=Act::DEATH
		&& state_.getAct()!=Act::DEATH_EVENT
		&& state_.getAct()!=Act::REMOVE
		&& state_.getIndex()>=0
		;
}

int CDataCharaSLG::getWeaponSlgID(int nID, CSLGContext& context)
{
	if(!IsWeaponLoad()) return -1;

	getBattle().beginWeapon();
	int nWeapon;
	while(!getBattle().endWeapon())
	{
		nWeapon = *getBattle().nextWeapon();
		if(nID==context.getWeaponData(nWeapon)->getID()) return nWeapon;
	}

	return -1;
}

int CDataCharaSLG::getWeaponSlgID(const string& sID, CSLGContext& context)
{
	return getWeaponSlgID(Weapon::Const::weaponID_.getValue(sID),context);
}

bool CDataCharaSLG::IsItem(CSLGContext* pContext)
{
	Item::CItemDB& db = pContext->getApp()->getItem();
	getBattle().beginItem();
	while(!getBattle().endItem())
	{
		Chara::item_list::iterator it = getBattle().nextItem();
		if(it->getAttr()>=0
		&& db.IsUse(it->getID())
		&& db.enable(*this,it->getAttr(),*pContext,it->getID()))
			return true;
	}
	
	return false;
}

void CDataCharaSLG::refill(CSLGContext* p,bool bRefill)
{
	// EN全開
	getBattle().calcEN(-(getBattle().getMaxEN()-getBattle().getEN()));

	// 武器弾数全開
	getBattle().beginWeapon();
	while(!getBattle().endWeapon())
		p->getWeaponData(*getBattle().nextWeapon())->refill();

	if(bRefill) // 補給コマンド時は気力-10
		getBattle().calcMental(-10);
}

void CDataCharaSLG::calcHP(int nHP)
{
	Chara::CDataCharaBattle& battle = getBattle();
	battle.calcHP(nHP);
	getState().pinch((double)battle.getHP()/(double)battle.getMaxHP() <= 0.3 );
}

int CDataCharaSLG::actionCounter(int nDist, int nRealDist, int nHeight, CSLGContext& p, int nHP, bool bBackUp)
{ 
	return getAction()->actionCounter(*this,nDist,nRealDist,nHeight,p,nHP,bBackUp); 
}

void CDataCharaSLG::actionPhasePer(CSLGContext& p)
{
	getAction()->actionPhasePer(*this,p);
}

void CDataCharaSLG::actionPhaseStart(CSLGContext& p,bool bIntro,bool bReset)
{
	getAction()->actionPhaseStart(*this,p,bIntro,bReset);
}

void CDataCharaSLG::actionMental(int nID)
{
	getAction()->actionMental(*this,nID);
}


///////////////////////////////////////////////////////////
// シリアライズ	
///////////////////////////////////////////////////////////
void CDataCharaSLG::Serialize(ISerialize& s)
{// これを行うとSLGでの状況だけがシリアライズされる
 // ので復元の時は、こいつに、大本のデータを足し合わせたりすること
	
	s << nID_ << nPhase_ << battle_ << nTrain_ << state_ << bWeapon_;

	int nSize;
	if(s.IsStoring())
	{// save
		// 説得
		nSize = listPers_.size();
		s << nSize;
		list<int>::iterator it;
		for(it=listPers_.begin(); it!=listPers_.end(); it++)
			s << *it;

		// アクション
		// アクションは、基本的にIDとParamから生成できる(ようにする)
		if(pAction_==NULL)
		{
			nSize = -1;
			s << nSize;
			nSize = 0;
			s << nSize;
		}
		else
		{// もしかしたら、追加パラメタが必要かもしれんので、
		 // とりあず、actionの実体にシリアライズを委譲
			s << *pAction_;
		}
	}
	else
	{// load
		// 説得の復元
		listPers_.clear();
		s << nSize;
		int nID;
		for(int i=0; i<nSize; i++)
		{
			s << nID;
			listPers_.push_back(nID);
		}
		
		// アクションの復元
		Action::CActionFactory fct;
		// 一端、ファクトリの中にアクション情報を取得
		// こうすることで、IDとParam以外の情報が必要な
		// アクション生成をカバーする
		s << fct;
		// その後、解放！
		DELETE_SAFE(pAction_);
		pAction_ = fct.createAction();

		// シンボルのロード
		// ↑は、このインスタンスが完全にならないとできないので、
		// 別フェーズで行う
	}
}

} // namespace SLG end
} // namespace BMW end