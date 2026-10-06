/*
	katze 05/05/10
	update 06/03/24
	戦闘デモシーン
*/
#pragma once

#include "../Scene/CScene.h"

#include "Back/CDemoBackLoader.h"

#include "Def/CDemoDef.h"
#include "CDemoContext.h"

namespace BMW{

namespace SLG{
class CDataBattleBase;
} // namespace SLG end

namespace Demo{
class CDemoMovieClip;
class CLayerChara;
class IDemoBack;
class CDemoEasyStatus;

class CDemoScene : public Scene::CScene<CDemoContext>
{/**
	戦闘デモシーン
 */
public:
	enum eSymbol{
		ATTACK,
		COUNTER,
		ATTACK_BACK,
		COUNTER_BACK,
	};
	enum eState{
		NORMAL,
		FADE_IN,
		FADE_OUT,
		RETURN,
	};
	// HP差分
	enum eSub{
		HP,
		EN,
	};
	// 技能登場パネルID
	enum eIntro{
		BACK_DEF,
		BACK_ATK,
		COUNTER_INTRO,
	};
	// コンストラクタ・デストラクタ
	CDemoScene();
	~CDemoScene();
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// イベントハンドラ
	void eventFade(Task::CTaskContext*);
	// 設定
	void setStatus(int nSymbol);
	void setChara();

	int	 getSub(int nBattle, int nSub){ return nSub_[nBattle][nSub]; }
	void calcSub(int nValue, int nBattle, int nSub){ nSub_[nBattle][nSub]+=nValue; }
	void resetSub()
	{
		nSub_[0][0]=nSub_[0][1]=nSub_[1][0]=nSub_[1][1]=0;
	}

	// アクセッサ
	int				getCurtain()const{ return nCurtain_; }
	void			setCurtain(int nCurtain){ nCurtain_=nCurtain; }
	CDemoMovieClip* getCurtainMovie(int nSide){ return pCurtain_[nSide]; }

private:
	// 差分データ
	int nSub_[2][2];
	
	enum eDef{
		ATK,
		DEF,
	};
	// シンボル
	int nRatio_[4]; // ↓の取得で使う
	CDemoDef def_[4][2];
	void loadData(int nBattle, int nFlag, SLG::CDataBattleBase& data, CDemoSymbolDB* pDB, CDemoContext* p);
	void setAttack(bool bBackDef, SLG::CDataBattleBase& attack,SLG::CDataBattleBase& counter, SLG::CDataBattleBase& counterBack);
	void setCounterDef(bool bBackDef, SLG::CDataBattleBase& counter, SLG::CDataBattleBase& counterBack, SLG::CDataBattleBase& attack);
	void setCounterAttack(SLG::CDataBattleBase& counter, SLG::CDataBattleBase& attack);
	// 技能などのためのシンボル
	// デモシーンのCGuiDefDBの中にある
	katzeSDK::Misc::CIntMap abilityID_[2]; // AbilityIDとGUI IDの対応表

	void setAbilityIntro(int nSide, CDemoMovieClip* pClip, int nID);
	void setAbilityDamage(int nSide, CDemoMovieClip* pClip, SLG::CDataBattleAbility& data, bool bCT);
	void setAbilityAtk(int nSide, CDemoMovieClip* pClip, SLG::CDataBattleAbility& data);
	void setAbilityDef(int nSide, CDemoMovieClip* pClip, SLG::CDataBattleAbility& data);
	// ↑のヘルパ
	void setAbility(int nSide, CDemoMovieClip* pClip, SLG::CDataBattleAbility& data, set<int>& validSet, int nState=0);
	// カーテン取得
	Task::ITaskBase*	createCurtain(int nSide);
	// 状態変化Symbol設定
	void setCondSymbol(int nBattle, SLG::CDataBattleBase& attack);

	// 背景ローダ
	CDemoBackLoader		backLoader_;

	// デモムービー
	CLayerChara*		pChara_;
	//IDemoBack*			pBack_;
	// 実際には↓に切り替える
	IDemoBack*			pBack_;
	CDemoEasyStatus*	pStatus_[2];
	CDemoMovieClip*		pCurtain_[2];
	CDemoMsgBoard*		pMsg_;
	CDemoDamage*		pDamage_;

	// イベントモード？
	bool bEvent_;
	// 現在表示中のカーテン方向
	// 負だったらカーテン動いてない
	int nCurtain_;

	void callTaskAction(Task::CTaskContext*);
	void callTaskDraw(Task::CTaskContext*);
};

} // namespace Demo end
} // namespace BMW end
