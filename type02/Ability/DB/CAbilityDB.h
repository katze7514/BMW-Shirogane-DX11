/*
	katze 05/03/18
	update 06/02/19 GUI定義DB搭載
	Ability系DB
*/
#pragma once

namespace BMW{

namespace GUI{
class CGuiDefDB;
} // namespace GUI end

namespace Chara{
class CDataCharaInter;
} // namespace Chara end

namespace Weapon{
class CDataWeaponBattle;
} // namespace Weapon end

namespace SLG{
class CSLGContext;
class CDataCharaSLG;
class COffsetMove;
class COffsetWeapon;
class COffsetBattle;
} // namespace SLG end

namespace Ability{

class IDataAbility;

class CAbilityDB
{/**
	Ability系DBの基底クラス
 */
public:
	typedef map<int, IDataAbility*> ability_map;

	// コンストラクタ・デストラクタ
	CAbilityDB();
	virtual ~CAbilityDB();

	// 操作
	virtual	void	setAbilityDB(const string& sFile);

	IDataAbility*	getData(int nID){ return mapAbility_[nID]; }
	template<class T>
	T*				getDataCast(int nID){ return static_cast<T*>(mapAbility_[nID]); }
	void			addData(IDataAbility* pData, int nID)
					{
						//if(mapAbility_[nID]!=NULL) delete mapAbility_[nID];
						mapAbility_[nID]=pData;
					}

	// 必要FP
	int				getGetFP(int nID,int nAttr=0)const;
	// 消費EN
	int				getEN(int nID,int nAttr=0)const;
	// データ適用・戻し
	void			applyStatus(SLG::CDataCharaSLG& slg, int nAttr, int nID);
	void			backStatus(SLG::CDataCharaSLG& slg, int nAttr, int nID);
	void			applyStatus(Chara::CDataCharaInter& inter, int nAttr, int nID);
	void			backStatus(Chara::CDataCharaInter& inter,  int nAttr, int nID);
	void			applyWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr, int nID);
	// SLGデータ適用は各Abilityオブジェクトを取得してダウンキャストつうことで
	// 使用可能かのチェック
	bool			enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p, int nID);
	bool			enable(const SLG::CDataCharaSLG& base, const SLG::CDataCharaSLG& target, int nAttr, SLG::CSLGContext& p, int nID);
	bool			enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p, int nID);
	// 使用
	void			use(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p, int nID);

	// GuiDef関係
	GUI::CGuiDefDB&		getGuiDefDB(){ return *pGuiDef_; }
	const string&		getGuiDefID(int nID);

	// ヘルパ
	void setGraphicHolder(GUI::CGraphicPopUp* pGraphic, int nID);
	void setTextPopUpHolder(GUI::CTextPopUp* pText, int nID); // テキストの内容とポップアップ内容だけをコピーする
	void setButtonHolder(GUI::CButtonSymbol* pButton, int nID, const GUI::CButton::ButtonEvent& fun, int nValue);
	void setButtonHolder(GUI::CButtonSymbol* pButton, int nID);


protected:
	// アビリティのデータマップ
	ability_map mapAbility_;

	// アビリティのボタンとか持ってるGuiDefDB
	// 依存関係の都合上ポインタ
	GUI::CGuiDefDB* pGuiDef_;
};

} // namespace Ability end
} // namespace BMW end