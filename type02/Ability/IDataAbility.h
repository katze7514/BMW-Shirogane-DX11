/*
	katze 05/03/18
	update 06/02/08
	Ability系データ
*/
#pragma once

namespace BMW{

namespace Chara{
class CDataCharaBattle;
class CDataCharaInter;
} // namespace Chara end

namespace Weapon{
class CDataWeaponBattle;
} // namespace Weapon end

namespace SLG{
class CSLGContext;
class CDataCharaSLG;
class COffsetMove;
class COffsetRange;
class COffsetHit;
class COffsetBattle;
} // namespace SLG end

namespace Ability{

class IDataAbility
{/**
	Ability系データ基底クラス

	ようは適用クラス
	各Abilityはこいつを継承する
 */
public:
	// コンストラクタ・デストラクタ
	IDataAbility(const string& sGuiDefID=""):sGuiDefID_(sGuiDefID){}
	virtual ~IDataAbility(){}

	// 設定・取得
	const string&	getGuiDefID() const { return sGuiDefID_; }
	void			setGuiDefID(const string& sGuiDefID){ sGuiDefID_=sGuiDefID;}

	// 操作
	// この技能を獲得するのに必要なFP
	virtual int		getGetFP(int nAttr=0)const{ return 0; }
	// この技能を使用するのに必要なEN
	virtual int		getEN(int nAttr=0)const{ return 0; }

	// 適用
	// ステータス適用
	virtual void	applyStatus(SLG::CDataCharaSLG& slg, int nAttr){}
	virtual void	backStatus(SLG::CDataCharaSLG& slg, int nAttr){}
	virtual void	applyStatus(Chara::CDataCharaInter& inter, int nAttr){}
	virtual void	backStatus(Chara::CDataCharaInter& inter, int nAttr){}
	virtual void	applyWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr){}
	virtual void	backWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr){}
	// SLGデータとしての適用は、個別に行う：つまりダウンキャストつうことで
	// 使用可能かのチェック
	// bCtrlは、自分がCtrl側かどうかのチェック
	virtual bool	enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return false; }
	virtual bool	enable(const SLG::CDataCharaSLG& base, const SLG::CDataCharaSLG& target, int nAttr, SLG::CSLGContext& p){ return false; }
	virtual bool	enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& pContext){ return false; }
	// 使用
	virtual void	use(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){}

protected:
	string	sGuiDefID_;	// これに対応するGuiDefID
};

} // namespace Ability end
} // namespace BMW end