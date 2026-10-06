/*
	katze 05/05/16
	update 06/06/09
	簡単な思考ルーチン Ver.2
	攻撃範囲計算Ver.2対応版
*/
#pragma once

#include "CActionSimple.h"

namespace BMW{
namespace SLG{

namespace Action{

class CActionNormal : public CActionSimple
{/**
	簡単な思考ルーチン
	ついでに、思考ルーチンの雛型でもある
 */
public:
	// コンストラクタ・デストラクタ
	CActionNormal():nCharaID_(-1),nWeaponID_(-1),nMapIndex_(-1){}
	virtual ~CActionNormal(){}

	// シリアライズ
	virtual void Serialize(ISerialize& s);
	virtual void getActionParam(int& nActionID, list<int>& listParam);

	virtual bool IsNonPlayer()const{ return true; }

	// アクション
	virtual void action(SLG::CDataCharaSLG& chara, CSLGContext& p);

	// 設定・取得
	int		getTargetChara() const { return nCharaID_; }
	void	setTargetChara(int nID){ nCharaID_=nID; }
	int		getUseWeapon() const { return nWeaponID_; }
	void	setUseWeapon(int nID){ nWeaponID_=nID; }
	int		getMapIndex() const { return nMapIndex_; }
	void	setMapIndex(int nIndex){ nMapIndex_=nIndex; }

protected:
	int nCharaID_;	// 攻撃対象
	int nWeaponID_;	// 攻撃に使う武器ID
	int nMapIndex_;	// 移動対象マップインデックス

	// 内部使用
	virtual bool actionAttack(SLG::CDataCharaSLG& chara, CSLGContext& p);
	virtual bool actionMoveAttack(SLG::CDataCharaSLG& chara, CSLGContext& p);
	virtual void actionMove(SLG::CDataCharaSLG& chara, CSLGContext& p);
	// 評価
	virtual int	 rantingAction(CDataCharaSLG& chara, int nMove, int nAttack, Weapon::CDataWeaponBattle* pWeapon=NULL);
	// 武器選択判定
	// pWeaponがpWeaponSelectより望ましい武器の時にtrueが返る
	virtual bool IsWeaponSelect(Weapon::CDataWeaponBattle* pWeaponSelect, Weapon::CDataWeaponBattle* pWeapon, int nAttack);
	int			 selectWeapon(CDataCharaSLG& chara, CDataCharaSLG* pChara, bool bP, CSLGContext& p);
	int			 selectWeaponAttack(CDataCharaSLG& chara, int nAttack, int nDist, int nHeight, bool bP, CSLGContext& p);
	// 範囲計算ヘルパ
	virtual void	calcAttack(CDataCharaSLG& chara, CSLGContext& p, CDataCharaSLG* pAbility=NULL);
	void			calcMove(CDataCharaSLG& chara, CSLGContext& p);
	// フェイズリスト取得
	list<int>& getPhaseList(int nPhase, CSLGContext& p);
};

} // namespace Action end
} // namespace SLG end
} // namespace BMW end