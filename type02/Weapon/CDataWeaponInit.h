/*
	katze 05/03/08
	武器初期値
*/
#pragma once

#include "CStatusWeaponStatus.h"
#include "CStatusWeapon.h"

namespace BMW{

namespace Chara{
class CStatusFund;
} // namespace Chara end

namespace Weapon{

class CStatusWeapon;
class CStatusWeaponStatus;

class CDataWeaponInit
{/**
	武器の初期値を表現するクラス

	また、WeaponDBの要素でもある
 */
public:
	// コンストラクタ・デストラクタ
	CDataWeaponInit():nBgmID_(-1),nSpecial_(0){}
	virtual ~CDataWeaponInit(){}

	// 設定・取得
	const string&			getName() const { return sName_; }
	void					setName(const string& sName){ sName_=sName; }
	const string&			getDemoID() const { return sDemoID_; }
	void					setDemoID(const string& sID){ sDemoID_=sID; }
	int						getBgmID() const { return nBgmID_; }
	void					setBgmID(int nID){ nBgmID_=nID; }
	const CStatusWeapon&	getStatus() const { return status_; }
	void					setStatus(const CStatusWeapon& status){ status_=status; }

	// 操作
	int						getKind() const { return status_.getKind(); }
	void					setKind(int nKind){ status_.setKind(nKind); }
	bool					IsP() const { return status_.IsP(); }
	void					p(bool bP){ status_.p(bP); }
	bool					IsM() const { return status_.IsM(); }
	void					m(bool bM){ status_.m(bM); }
	bool					IsT() const { return status_.IsT(); }
	void					t(bool bT){ status_.t(bT); }
	bool					IsF() const { return status_.IsF(); }
	void					f(bool bF){ status_.f(bF); }
	int						getAttack() const { return status_.getAttack();}
	void					setAttack(int nAttack){ status_.setAttack(nAttack);}
	int						getMin()const{ return status_.getMin(); }
	void					setMin(int nMin){ status_.setMin(nMin); }
	int						getMax()const{ return status_.getMax(); }
	void					setMax(int nMax){ status_.setMax(nMax); }
	int						getCoreMin() const { return status_.getCoreMin(); }
	void					setCoreMin(int nCoreMin){ status_.setCoreMin(nCoreMin); }
	int						getCoreMax() const { return status_.getCoreMax(); }
	void					setCoreMax(int nCoreMax){ status_.setCoreMax(nCoreMax); }
	int						getHeight() const { return status_.getHeight(); }
	void					setHeight(int nHeight){ status_.setHeight(nHeight); }
	int						getHit() const { return status_.getHit(); }
	void					setHit(int nHit){ status_.setHit(nHit); }
	int						getCT() const { return status_.getCT(); }
	void					setCT(int nCT){ status_.setCT(nCT); }
	int						getEN() const { return status_.getEN(); }
	void					setEN(int nEn){ return status_.setEN(nEn); }
	int						getBallet() const { return status_.getBallet(); }
	void					setBallet(int nBallet){ status_.setBallet(nBallet); }
	int						getMental() const { return status_.getMental(); }
	void					setMental(int nMental){ status_.setMental(nMental); }
	int						getTrainingType() const { return status_.getTrainingType(); }
	void					setTrainingType(int nType){ status_.setTrainingType(nType); }

	// 追加情報
	// 合体武器
	map<int,int>&				getCollabCharaMap(){ return mapChara_; }
	void						setCollabChara(int nID, int nWeapon){ mapChara_.insert(pair<int,int>(nID,nWeapon)); }

	// ステータス
	const CStatusWeaponStatus&	getStatusAid() const { return statusAid_; }
	void						setStatusAid(const CStatusWeaponStatus& statusAid){ statusAid_=statusAid; }
	const Chara::CStatusFund&	getFundAid() const { return statusAid_.getFund(); }
	void						setFundAid(const Chara::CStatusFund& fund){ statusAid_.setFund(fund); }
	int							getStrengthAid() const { return statusAid_.getStrength(); }
	void						setStrengthAid(int nStrength){ statusAid_.setStrength(nStrength); }
	int							getMagicAid() const { return statusAid_.getMagic(); }
	void						setMagicAid(int nMagic){ statusAid_.setMagic(nMagic); }
	int							getHitAid() const { return statusAid_.getHit(); }
	void						setHitAid(int nHit){ statusAid_.setHit(nHit); }
	int							getAvoidAid() const { return statusAid_.getAvoid(); }
	void						setAvoidAid(int nAvoid){ statusAid_.setAvoid(nAvoid); }
	int							getDefenceAid() const { return statusAid_.getDefence(); }
	int							setDefenceAid(int nDefence){ statusAid_.setDefence(nDefence); }
	int							getSkillAid() const { return statusAid_.getSkill(); }
	void						setSkillAid(int nSkill){ statusAid_.setSkill(nSkill); }
	int							getSPAid() const { return statusAid_.getSP(); }
	void						setSPAid(int nSP){ statusAid_.setSP(nSP); }
	int							getMentalAid() const { return statusAid_.getMental(); }
	void						setMentalAid(int nMental){ statusAid_.setMental(nMental); }

	// 状態変化
	int							getCond() const { return nCond_; }
	void						setCond(int nCond){ nCond_=nCond; }
	int							getCondValue()const{ return nCondValue_; }
	void						setCondValue(int nCondValue){ nCondValue_=nCondValue; }

	// フィールド武器
	int							getField()const{ return nField_;}
	void						setField(int nField){ nField_=nField; }
	bool						IsFieldFriend()const{ return bFriend_; }
	void						fieldFriend(bool bFriend){ bFriend_=bFriend; }
	int							getFieldSize()const{ return nFieldSize_; }
	void						setFieldSize(int nFieldSize){ nFieldSize_=nFieldSize; }
	const string&				getFieldFile()const{ return sFieldCutIn_; }
	void						setFieldFile(const string& sFieldCutIn){ sFieldCutIn_=sFieldCutIn; }

	// 特殊効果
	int							getSpecial()const{ return nSpecial_; }
	void						setSpecial(int nSpecial){ nSpecial_=nSpecial; }

protected:
	string			sName_;		// 武器名
	string			sDemoID_;	// 対応するデモ定義ファイル
	int				nBgmID_;	// 対応するBGMID
	CStatusWeapon	status_;	// 全武器共通ステータス

	// 追加情報
	// memo:↓なんでunionとかにしてないんだっけ？
	//		今だったら、boost::variant が使えるかな

	// 合体攻撃武器用
	// キャラIDと対応する武器ID
	map<int,int> mapChara_;

	// ステータス変化武器用
	CStatusWeaponStatus statusAid_;

	// 状態変化武器用
	int nCond_;		// タイプ
	int	nCondValue_;// 持続ターン数

	// フィールド武器用
	// 直線タイプの場合は、最小射程が横幅、最大射程が縦幅になる
	int nField_;	// タイプ
	bool bFriend_;	// 味方認識するかしないか
	int	nFieldSize_;	// 投げ込みタイプの範囲
	string sFieldCutIn_;// フィール武器絵シンボルファイル名

	// 特別な効果が何かあったり？
	int nSpecial_;
};

} // namespace Weapon end
} // namespace BMW end