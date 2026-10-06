/*
	katze 05/02/28
	保持アビリティステータス
*/
#pragma once

namespace BMW{
namespace Chara{

class CStatusAbility : public IArchive
{/**
	精神・技能・固有・アイテムで使う共通のデータ構造

	今回、効果適用は、別のクラスが担うので、
	どういう属性を持っているかだけでOK
 */
public:
	// コンストラクタ
	CStatusAbility():nID_(-1),nAttr_(0){}

	// シリアライズ
	void Serialize(ISerialize& s);

	// 設定・取得
	int  getID() const { return nID_; }
	void setID(int nID){ nID_=nID; }
	int  getAttr() const { return nAttr_; }
	void setAttr(int nAttr){ nAttr_=nAttr; }

private:
	int nID_;	// 精神・技能・固有・アイテムID
	int nAttr_;	// 消費SPや技能・固有Lv・アイテム装備位置などに使う
};

// 良く使うパターンのtypedef
typedef list<CStatusAbility> skill_list;
typedef list<CStatusAbility> talent_list;
typedef list<CStatusAbility> item_list;

} // namespace Chara end
} // namespace BMW end