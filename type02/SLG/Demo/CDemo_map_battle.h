/*
	katze 05/06/05
	update 06/03/17
	戦闘用デモOFF
*/
#pragma once

#include "CDemo_map_base.h"

namespace BMW{
namespace SLG{
class CDataBattle;

namespace Effect{
class CEffectMovieClip;
} // namespace Effect end

namespace Demo{
class CDemo_map;

class CDemo_map_battle : public CDemo_map_base
{/**
	戦闘用デモOFF
 */
public:
	enum eState{
		NORMAL,
		EF, // エフェクト実行中
		EF_CHARA,
		EF_NUM,
		WAIT,
	};
	enum eEffect{
		CHANGE=-2,	// キャラ変更
		WAIT_E,
		HIT,
		AVOID_R,
		AVOID_L,
		DEF,
		CT,
		BACKUP_ATK,
		BACKUP_DEF,
		BUNSHIN,
		COUNTER_E,
		BARRIER_L,
		BARRIER_R,
		STATUS_UP_L,
		STATUS_UP_R,
		DOKURO_L,
		DOKURO_R,
		EFFECT_END,
	};
	enum eSide{
		LEFT,
		RIGHT,
	};
	// デストラクタ
	~CDemo_map_battle();

	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	// エフェクト
	Effect::CEffectMovieClip* pEffect_[EFFECT_END];

	/////////////////////////////////
	// 通常武器用
	// 戦闘データ
	smart_ptr<CDataBattle> battle;
	// キャラ
	enum eCharaChip{
		DEFO,
		ATK,
		DEF_C,
		AVOID,
		DMG,
		CHARA_END,
	};
	enum eBattle{
		ATTACK,
		COUNTER,
		ATTACK_B,
		COUNTER_B,
	};
	Task::ITaskBase* pChara_[4][CHARA_END];
	void resetChara()
	{
		for(int i=0; i<CHARA_END; i++)
		{
			pChara_[0][i]=NULL;
			pChara_[1][i]=NULL;
			pChara_[2][i]=NULL;
			pChara_[3][i]=NULL;
		}
	}
	void clearChara()
	{
		for(int i=0; i<CHARA_END; i++)
		{
			DELETE_SAFE(pChara_[0][i]);
			DELETE_SAFE(pChara_[1][i]);
			DELETE_SAFE(pChara_[2][i]);
			DELETE_SAFE(pChara_[3][i]);
		}
	}

	// アクション
	void actionNext(Task::CTaskContext*);
	void updateChara(GUI::CPanel* pPanel, Task::CTaskContext* pContext);
	void updateNum(GUI::CPanel* pPanel);

	///////////////////////////////////////
	// ムービー再生のため
	enum eChara{
		CHARA_DEFAULT,
		CHARA_ATK,
		CHARA_DEF,
		CHARA_AVOID,
		CHARA_DAMAGE,
	};
	typedef struct demo_movie
	{// デモムービー構成構造体
		bool	bLeft_;		// サイド
		int		nBattle_;	// キャラホルダ位置
		int		nChara_;	// キャラ実行ID
		int		nEffect_;	// 実行エフェクトID
		int		nFrame_;	// ウェイトフレーム
		int		nDamage_;	// ダメージ

		// コンストラクタ
		demo_movie(bool bLeft=true,
				   int nBattle=ATTACK,
				   int nChara=CHARA_DEFAULT,
				   int nEffect=EFFECT_END,
				   int nFrame=-1,
				   int nDamage=-1)
				   : bLeft_(bLeft),nBattle_(nBattle),nChara_(nChara),nEffect_(nEffect),
					 nFrame_(nFrame),nDamage_(nDamage){}
	} DEMO_MOVIE;
	// デモムービーリスト
	list<DEMO_MOVIE> listMovie_;
	list<DEMO_MOVIE>::iterator it; // 現在実行中

	// ムービーデータ設定
	void initMap(const smart_ptr<Weapon::CDataWeaponBattle>& pWeapon, CSLGContext* p);
	void initNormal(Task::CTaskContext* pContext);
	void setCounter(bool bLeft, CDataBattleBase& attack, CDataBattleBase& counter);
	void setDefBack(bool bBackDef,bool bLeft, 
				    CDataBattleBase& attack, CDataBattleBase& counter, CDataBattleBase& counterBack);
	enum eWeapon{
		NORMAL_W,
		STATUS,
		COND,
	};
	int	 setAtkDefMovie(bool bLeft,int From, int nTo,
						int nDamgae, bool bCT,
						CDataBattleDefence& def,
						int nWeapon);
	void loadChara(int nBattle, int nChara, smart_ptr<CDataCharaSLG>& pChara, bool bLeft);
};

} // namespace Demo end
} // namespace SLG end
} // namespace BMW end