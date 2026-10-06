#include "stdafx.h"

#include "../SLG/Context/CSLGContext.h"
#include "../SLG/Context/CDataCharaSLG.h"
#include "../SLG/Context/COffsetWeapon.h"

#include "IDWeapon.h"
#include "ConstWeapon.h"
#include "CDataWeaponBattle.h"

namespace BMW{
namespace Weapon{

CDataWeaponBattle::CDataWeaponBattle():nBallet_(0),nRank_(0)
{
	pRange_ = new SLG::COffsetRange();
}
CDataWeaponBattle::~CDataWeaponBattle()
{
	DELETE_SAFE(pRange_);
}

// シリアライズ
void CDataWeaponBattle::Serialize(ISerialize& s)
{// やっぱし差分データ
	// 06/05/09
	// ランクも保存しないとあかん
	// つうことで、コンテニューファイルの互換性が無くなりましたとさ
	s << nID_ << nBallet_ << nRank_;
}

bool CDataWeaponBattle::IsWeaponID(const string& sWeaponID)const
{// 与えられた文字列IDとの比較
	return getID()==Const::weaponID_.getValue(sWeaponID);
}

void CDataWeaponBattle::refill()
{
	// 弾数を満タンにする
	setBalletRest(getBallet());
}

bool CDataWeaponBattle::enable(const SLG::CDataCharaSLG& chara, bool bP, SLG::CSLGContext& context, int nDist, int nRealDist, int nHeight)
{
	return enableNeed(chara,bP,context) && enableRange(chara,context,nDist,nRealDist,nHeight);
}

bool CDataWeaponBattle::enableNeed(const SLG::CDataCharaSLG& chara, bool bP, SLG::CSLGContext& context)
{// 必要用件を満たしているかをチェック
	const Chara::CDataCharaBattle& battle = chara.getBattle();

	// カウンター専用武器なら、自フェイズ時には使えない
	if(getSpecial()==Weapon::Special::COUNTER_SPECIAL
	&& chara.getPhase()==context.getPhase()) return false;

	// 移動後攻撃で、P属性が無いなら使えない
	// 突撃が合ったら、いつでもP兵器。でも、MAP兵器にはつかない
	if(bP && !(IsP() || (chara.getBattle().IsSpirit(Chara::CValidSpirit::CHARGE) && !IsF()))) return false;

	if(getBallet()>0)
	{// 弾数武器の場合は、弾が残ってるか？
		if(!IsBalletRest()) return false;
	}

	// 必要ENだけENが残っているか
	if(getEN() > battle.getEN()) return false;
	// 必要気力だけ気力があるか
	if(getMental() > battle.getMental()) return false;

	// 全部クリアしたら使える
	return true;
}

bool CDataWeaponBattle::enableRange(const SLG::CDataCharaSLG& chara, SLG::CSLGContext& context, int nDist, int nRealDist, int nHeight)
{
	// 相手との高さが設定されてたら、チェック
	// 到達度以下じゃないといかん
	if(nHeight>=0 && getHeight()<nHeight) return false;
	
	// 相手との距離が設定されてたら、それをチェック
	if(nDist>=0)
	{// 射程内でなければ、使えない
		if(getMin()>nRealDist || getMax()<nRealDist || getMin()>nDist || getMax()<nDist) return false;
	}
	ef(nDist<0)
	{// こいつが攻撃できる距離にキャラがいるか？
		bool b=false;
		int nMax = getMax();
		int nMin = getMin();
		if(IsF())
		{
			switch(getField())
			{
				case Field::THROW: nMax+=getFieldSize(); break;
				case Field::LINE:  nMin=1; break;
			}
		}
		for(int i=nMin; i<=nMax; ++i)
			b = b || context.IsRangeChara(i-1,chara.getPhase(), getKind()==Weapon::Kind::STATUS 
																|| getKind()==Weapon::Kind::CURE
																|| getKind()==Weapon::Kind::REFILL,
																getHeight(),
																IsF(),
																IsFieldFriend());
	#ifdef BMW_DEBUG
		CDbg().Out("E_RANGE %d",b);
	#endif
		// 誰もいなかったら、false
		if(!b) return false;
	}
	
	// 全部クリアしたら使える
	return true;
}

bool CDataWeaponBattle::enableTarget(const SLG::CDataCharaSLG& chara, const SLG::CDataCharaSLG& target, SLG::CSLGContext& context)
{
	return target.IsLive() && chara.getPhase()!=target.getPhase();
}

bool CDataWeaponBattle::IsRange(int nRange)const
{ 
	return getMin()<=nRange && getMax()>=nRange;
}

bool CDataWeaponBattle::IsCore(int nRange)const
{ 
	// フィールド武器ラインだとちょっと変わる
	if(IsF() && getField()==Weapon::Field::LINE)
		return getCoreMax()>=nRange;
	else
		return getCoreMin()<=nRange && getCoreMax()>=nRange;
}

int	CDataWeaponBattle::getMax()const
{ 
	return status_.getMax() + pRange_->getMax();
}

int	CDataWeaponBattle::getCoreMax()const
{ 
	return status_.getCoreMax() + pRange_->getMax();
}

int CDataWeaponBattle::getHeight(bool bFlyOffset)const
{
	return status_.getHeight() + pRange_->getReach(bFlyOffset);
}

void CDataWeaponBattle::use(SLG::CDataCharaSLG& chara, SLG::CSLGContext& context)
{
	Chara::CDataCharaBattle& battle = chara.getBattle();
	if(getStatus().getBallet()>0)
	{// 弾数武器だったら、弾数を減らす
		decBallet();
	}

	// エネルギーを減らす
	battle.calcEN(getEN());
}

} // namespace Weapon end
} // namespace BMW end