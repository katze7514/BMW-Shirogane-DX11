/*
	katze 05/05/07
	すげー簡単なアクション
*/
#pragma once

#include "IAction.h"

namespace BMW{
namespace SLG{
class CSLGContext;
class CDataCharasSLG;

namespace Action{

class CActionSimple : public IAction
{/**
 	すげー簡単なアクション
 */
public:
	// デストラクタ
	virtual ~CActionSimple(){}

	virtual bool IsNonPlayer()const{ return false; }

	// シリアライズ
	virtual void Serialize(ISerialize& s);
	virtual void getActionParam(int& nActionID, list<int>& listParam);

	// フェーズ毎
	virtual void	actionPhasePer(SLG::CDataCharaSLG& chara, CSLGContext& context);
	// フェーズ開始時
	virtual void	actionPhaseStart(SLG::CDataCharaSLG& chara, CSLGContext& context, bool bIntro=false, bool bReset=false);
	// 反撃時の行動選択
	virtual int		actionCounter(SLG::CDataCharaSLG& chara, int nDist, int nRealDist, int nHeight, CSLGContext& context, int nHP=0, bool bBackUp=false);
	// 気力の増減 nIDは(Mental::eMental準拠)
	virtual void	actionMental(SLG::CDataCharaSLG& chara, int nID);
	// 戦闘終了時
	virtual void	actionBattleEnd(SLG::CDataCharaSLG& chara, CSLGContext& context);

	// 技能レスポンス
	void	responseAbility(SLG::CDataCharaSLG& chara, CSLGContext& context){}
};

} // namespace Action end
} // namespace SLG end
} // namespace BMW end