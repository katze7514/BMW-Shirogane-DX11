/*
	katze 05/03/26
	キャラの行動を決めるクラスのインターフェイス
*/
#pragma once

namespace BMW{
namespace SLG{
class CSLGContext;
class CDataCharaSLG;
namespace Action{

class IAction : public IArchive
{/*
	キャラ行動を決めるインターフェイス
 */
public:
	// デストラクタ
	virtual ~IAction(){}

	// NPCかどうか
	virtual bool IsNonPlayer()const{ return true; }

	// アクション
	// フェーズ毎
	virtual void	actionPhasePer(SLG::CDataCharaSLG& chara, CSLGContext& context)=0;
	// フェーズ開始時
	virtual void	actionPhaseStart(SLG::CDataCharaSLG& chara, CSLGContext& context,bool bIntro=false, bool bReset=false)=0;
	// 反撃時の行動選択
	virtual int		actionCounter(SLG::CDataCharaSLG& chara, int nDist, int nRealDist, int nHeight, CSLGContext& context, int nHP=0, bool bBackUp=false)=0;
	// 気力の増減 nIDは(Mental::eMental準拠)
	virtual void	actionMental(SLG::CDataCharaSLG& chara, int nID)=0;
	// 戦闘終了時
	virtual void	actionBattleEnd(SLG::CDataCharaSLG& chara, CSLGContext& context)=0;

	// パラメタ取得
	virtual void	getActionParam(int& nActionID, list<int>& listParam)=0;

	// 二回行動など思考ルーチンに関わるものを持っていた時の対応
	virtual void	responseAbility(SLG::CDataCharaSLG& chara, CSLGContext& context)=0;
};

} // namespace Action end
} // namespace SLG end
} // namespace BMW end