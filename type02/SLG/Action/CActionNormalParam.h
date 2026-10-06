/*
	katze 06/06/12
	思考ルーチン基本 Ver.2
	パラメタ化
*/
#pragma once

#include "CActionNormal.h"

namespace BMW{
namespace SLG{
namespace Action{

class CActionNormalParam : public CActionNormal
{/**
	思考ルーチン基本 Ver.2
	パラメタ化
*/
public:
	// コンストラクタ・デストラクタ
	CActionNormalParam():nWait_(0),nMove_(MOVE),nSnipeChara_(-1),bInner_(false),bProvo_(false){}
	virtual ~CActionNormalParam(){}

	// シリアライズ
	virtual void Serialize(ISerialize& s);
	virtual void getActionParam(int& nActionID, list<int>& listParam);

	// 思考ルーチン
	virtual void action(CDataCharaSLG& chara, CSLGContext &p);
	// 技能レスポンス
	void	responseAbility(SLG::CDataCharaSLG& chara, CSLGContext& context);

	// アクセッサ
	int		getWait()const{ return nWait_; }
	void	setWait(int nWait){ nWait_=nWait; }
	int		getMove()const{ return nMove_; }
	void	setMove(int nMove){ nMove_=nMove; }
	int		getSnipeChara()const{ return nSnipeChara_; }
	void	setSnipeChara(int nSnipeChara){ nSnipeChara_=nSnipeChara; }

protected:
	int nWait_;			// ウェイトターン
	int nMove_;			// 移動フラグ
	int nSnipeChara_;	// 狙うキャラ
	bool bInner_;		// 最小射程内にキャラがいる
	int nMin_;			// 最小射程
	int nReach_;

	virtual bool actionAttack(SLG::CDataCharaSLG& chara, CSLGContext& p);
	virtual bool actionMoveAttack(SLG::CDataCharaSLG& chara, CSLGContext& p);
	virtual void actionMove(SLG::CDataCharaSLG& chara, CSLGContext& p);

	// 狙うキャラがいる用
	bool actionAttackSnipe(SLG::CDataCharaSLG& chara, CSLGContext& p);
	bool actionMoveAttackSnipe(SLG::CDataCharaSLG& chara, CSLGContext& p);
	void actionMoveSnipe(SLG::CDataCharaSLG& chara, CSLGContext& p);

	virtual void	calcAttack(CDataCharaSLG& chara, CSLGContext& p, CDataCharaSLG* pAbility=NULL);

	// 挑発用SnipeとProvoID入れ替え
	void	swapProvo(CDataCharaSLG& chara);
	// 挑発終了判定
	void	endProvo(CDataCharaSLG& chara);
	// 前回挑発だった？
	// これがtrueだったら、次のTargetCharaはリセットする
	// 保存しないといけないかなー、と思ったけど
	// どうせTargetChara保存してないから、コンテニュー
	// すれば同じだった
	bool	bProvo_;
};

} // namespace Action end
} // namespace SLG end
} // namespace BMW end