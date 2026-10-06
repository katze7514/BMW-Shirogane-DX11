/*
	katze 07/01/27
	思考ルーチン基本 Ver.3
	パラメタ化
*/
#pragma once

#include "CActionNormalParam.h"

namespace BMW{
namespace SLG{
namespace Action{

class CActionNormalEval : public CActionNormalParam
{/**
	思考ルーチン基本 Ver.3
	評価関数もパラメタ化
*/
public:
	// コンストラクタ・デストラクタ
	CActionNormalEval():nEvalMove_(50),nEvalAtk_(20),nEvalHP_(1){}
	virtual ~CActionNormalEval(){}

	// シリアライズ
	virtual void Serialize(ISerialize& s);
	virtual void getActionParam(int& nActionID, list<int>& listParam);

	int		getEvalMove()const{ return nEvalMove_; }
	void	setEvalMove(int nEvalMove){ nEvalMove_=nEvalMove; }
	int		getEvalAtk()const{ return nEvalAtk_; }
	void	setEvalAtk(int nEvalAtk){ nEvalAtk_=nEvalAtk; }
	int		getEvalHP()const{ return nEvalHP_; }
	void	setEvalHP(int nEvalHP){ nEvalHP_=nEvalHP; }

protected:
	// 評価関数系
	int nEvalMove_;
	int nEvalAtk_;
	int nEvalHP_;

	// アクションアタック
	virtual bool actionAttack(SLG::CDataCharaSLG& chara, CSLGContext& p);
	// 評価
	virtual int	 rantingAction(CDataCharaSLG& chara, int nMove, int nAttack, Weapon::CDataWeaponBattle* pWeapon=NULL);
	virtual int	 rantingAction2(CDataCharaSLG& chara, int nAttack, Weapon::CDataWeaponBattle* pWeapon=NULL);
};

} // namespace Action end
} // namespace SLG end
} // namespace BMW end