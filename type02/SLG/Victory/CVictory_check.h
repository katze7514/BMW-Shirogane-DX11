/*
	katze 05/05/21
	勝利条件判定
*/
#pragma once

#include "../various/CScript_base.h"

namespace BMW{
namespace SLG{
class CSLGContext;

namespace Victory{

class CVictory_check : public Script::CScript_base
{/**
	勝利条件判定

	但し、これはスケルトン
	というか、良く使う機能の詰め合わせ
 */
public:
	// デストラクタ
	virtual ~CVictory_check(){}
	
	// 機能
	// 指定したキャラ死んでる？
	static bool IsDeath(int nID, CSLGContext& context);
	static bool IsDeath(const string& sID, CSLGContext& context);
	// 指定したフェーズキャラが全滅？
	static bool IsDeathPhase(int nPhase, CSLGContext& context);
	// 指定したフェーズキャラが全滅？ 但し、Listのキャラは生きていてOK
	static bool IsDeathPhase(int nPhase, CSLGContext& context, set<int>& setChara);
	// 指定したフェーズのキャラが一人でも戦闘不能？
	static bool IsDeathPhaseOne(int nPhase, CSLGContext& context);
	// ↑らの下請け
	static bool IsDeathList(list<int>& List, CSLGContext& context, bool bAll=true);
	// 指定したキャラ生きてる？
	static bool IsAlive(int nID, CSLGContext& context);
	static bool IsAlive(const string& sID, CSLGContext& context);
	// 指定したフェーズキャラみんな生きてる？
	static bool IsAlivePhase(int nPhase, CSLGContext& context);
	// 指定したフェーズのキャラが一人でもいきてる？
	static bool IsAlivePhaseOne(int nPhase, CSLGContext& context);
	// ↑らの下請け
	static bool IsAliveList(list<int>& List, CSLGContext& context, bool bAll=true);
};

} // namespace Victory end
} // namespace SLG end
} // namespace BMW end