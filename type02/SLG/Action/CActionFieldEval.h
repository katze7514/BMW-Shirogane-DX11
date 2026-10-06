/*
	katze 06/11/24
	フィールド武器を持ってる敵の思考ルーチン
*/
#pragma once

#include "CActionNormalEval.h"

namespace BMW{
namespace SLG{
namespace Action{

class CActionFieldEval : public CActionNormalEval
{/**
	フィールド武器を持ってる敵の思考ルーチン
	ただし、敵方のTHROWフィールド武器には対応してないので、
	THROWタイプのフィールド兵器は持たせないこと

	通常時を評価をいじれる
 */
public:
	CActionFieldEval():nFieldWeapon_(-1){}
	virtual ~CActionFieldEval(){}

	// シリアライズ
	virtual void Serialize(ISerialize& s);
	virtual void getActionParam(int& nActionID, list<int>& listParam);

	// アクション
	virtual void action(CDataCharaSLG& chara, CSLGContext& p);

	// アクセッサ
	int	 getFieldWeapon()const{ return nFieldWeapon_; }
	void setFieldWeapon(int nWeapon){ nFieldWeapon_=nWeapon; }
	int	 getCharaNum()const{ return nCharaNum_; }
	void setCharaNum(int nCharaNum){ nCharaNum_=nCharaNum; }

protected:
	int nFieldWeapon_; // こいつが持ってるマップ兵器ID
	int nCharaNum_;	 // 範囲内にnCharaNum人以上敵がいたら撃つ

	virtual void	calcAttack(CDataCharaSLG& chara, CSLGContext& p, CDataCharaSLG* pAbility=NULL);
};

} // namespace Action end
} // namespace SLG end
} // namespace BMW end