/*
	katze 05/03/08
	戦闘用武器
*/
#pragma once

namespace BMW{

namespace Chara{
class CDataCharaBattle;
} // namespace Chara end

namespace SLG{
class CSLGContext;
class CDataCharaSLG;
class COffsetRange;
}// namespace SLG end
namespace Weapon{

class CDataWeaponBattle : public IArchive
{/**
	戦闘用武器クラス

	CDataWeaponInitとCDataCharaTrainの武器養成段階
	から生成される。
 */
public:
	// コンストラクタ・デストラクタ
	CDataWeaponBattle();
	virtual ~CDataWeaponBattle();

	// シリアライズ
	virtual void Serialize(ISerialize& s);
	
	// 削除に呼ばれる
	// 主に合体武器用
	virtual void delWeapon(SLG::CDataCharaSLG* pChara, SLG::CSLGContext& p){}

	// 設定・取得
	int						getID() const { return nID_; }
	void					setID(int nID){ nID_=nID; }
	const CDataWeaponInit&	getStatus() const { return status_; }
	void					setStatus(const CDataWeaponInit& status){ status_=status; }
	int						getBalletRest() const { return nBallet_; }
	void					setBalletRest(int nBallet){ nBallet_=nBallet; }
	SLG::COffsetRange&		getRange(){ return *pRange_; }
	int						getRank() const { return nRank_; }
	void					setRank(int nRank){ nRank_=nRank; }

	// 判定
	bool					IsWeaponID(const string& sWeaponID)const;

	// 操作
	bool					IsBalletRest() const { return nBallet_>0; }
	void					decBallet(){ nBallet_--; }

	// 補給
	void					refill();
	// この武器が使用可能かどうかの判定
	virtual bool			enable(const SLG::CDataCharaSLG& chara, bool bP, SLG::CSLGContext& context, int nDist=-1, int nRealDist=-1,int nHeight=-1);
	// ↑の分割
	virtual bool			enableNeed(const SLG::CDataCharaSLG& chara, bool bP, SLG::CSLGContext& context);
	virtual bool			enableRange(const SLG::CDataCharaSLG& chara, SLG::CSLGContext& context, int nDist=-1, int nRealDist=-1,int nHeight=-1);
	// 攻撃できる相手？
	virtual bool			enableTarget(const SLG::CDataCharaSLG& chara, const SLG::CDataCharaSLG& target, SLG::CSLGContext& context);
	// 全距離にはいってるか否か
	bool					IsRange(int nRange)const;
	// 中心距離にはいってるか否か
	bool					IsCore(int nRange)const;
	// 武器を使用する
	virtual void			use(SLG::CDataCharaSLG& chara, SLG::CSLGContext& context);
	// この武器の特殊効果を適用する
	virtual void			apply(SLG::CDataCharaSLG& chara, SLG::CSLGContext& context){}

	// 武器パラメタへのアクセッサ
	const string&			getName() const { return status_.getName(); }
	void					setName(const string& sName){ status_.setName(sName); }
	const string&			getDemoID() const { return status_.getDemoID(); }
	void					setDemoID(const string& sID){ status_.setDemoID(sID); }
	int						getBgmID() const { return status_.getBgmID(); }
	void					setBgmID(int nID){ status_.setBgmID(nID); }
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
	int						getMax()const;
	void					setMax(int nMax){ status_.setMax(nMax); }
	int						getCoreMin() const { return status_.getCoreMin(); }
	void					setCoreMin(int nCoreMin){ status_.setCoreMin(nCoreMin); }
	int						getCoreMax() const;
	void					setCoreMax(int nCoreMax){ status_.setCoreMax(nCoreMax); }
	int						getHeight(bool bFlyOffset=true)const;
	void					setHeight(int nHeight){ status_.setHeight(nHeight); }
	int						getHit() const { return status_.getHit(); }
	void					setHit(int nHit){ status_.setHit(nHit); }
	int						getCT() const { return status_.getCT(); }
	void					setCT(int nCT){ status_.setCT(nCT); }
	int						getEN() const { return status_.getEN(); }
	void					setEN(int nEn){ status_.setEN(nEn); }
	int						getBallet() const { return status_.getBallet(); }
	void					setBallet(int nBallet){ status_.setBallet(nBallet); }
	int						getMental() const { return status_.getMental(); }
	void					setMental(int nMental){ status_.setMental(nMental); }
	int						getTrainingType() const { return status_.getTrainingType(); }
	void					setTrainingType(int nType){ status_.setTrainingType(nType); }

	// 追加情報
	map<int,int>&				getCollabCharaMap(){ return status_.getCollabCharaMap(); }
	int							getCond() const { return status_.getCond(); }
	int							getCondValue() const { return status_.getCondValue(); }

	const CStatusWeaponStatus&	getStatusAid() const { return status_.getStatusAid(); }
	int							getStrengthAid() const { return status_.getStrengthAid(); }
	int							getMagicAid() const { return status_.getMagicAid(); }
	int							getHitAid() const { return status_.getHitAid(); }
	int							getAvoidAid() const { return status_.getAvoidAid(); }
	int							getDefenceAid() const { return status_.getDefenceAid(); }
	int							getSkillAid() const { return status_.getSkillAid(); }
	int							getSPAid() const { return status_.getSPAid(); }
	int							getMentalAid() const { return status_.getMentalAid(); }

	int							getField()const{ return status_.getField(); }
	bool						IsFieldFriend()const{ return status_.IsFieldFriend(); }
	int							getFieldSize()const{ return status_.getFieldSize(); }
	const string&				getFieldFile()const{ return status_.getFieldFile(); }

	int							getSpecial()const{ return status_.getSpecial(); }
	void						setSpecial(int nSpecial){ status_.setSpecial(nSpecial); }


protected:
	int				nID_;		// こいつの武器ID(not SLG ID)
	CDataWeaponInit status_;	// 養成済みのデータ
	int				nBallet_;	// 残り弾数
	SLG::COffsetRange*	pRange_;		// 補正値

	// 武器ランク
	// ↓に合わせて、格闘と魔術武器の数が選ばれる
	int				nRank_;
};

} // namespace Weapon end
} // namespace BMW end