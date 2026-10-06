#pragma once

#include "Input/CTaskInput.h"
#include "Face/CFaceMap.h"
#include "Chara/DB/CCharaDB.h"
#include "Weapon/DB/CWeaponDB.h"
#include "Spirit/CSpiritDB.h"
#include "Ability/DB/CAbilityDB.h"
#include "Item/CItemDB.h"
#include "Scenario/CScenarioDB.h"

class CApp : public CAppFrame 
{
public:
	// こいつではじまる
	virtual void MainThread();

	///	描画
	CFastDraw* GetDraw() { return GetDrawFactory()->GetDraw(); }
	CFastPlaneFactory* GetDrawFactory() { return &planeFactory_; }

	// サウンド
	CSoundFactory* GetSoundFactory() { return &soundFactory_; }

	// ゲームの終了
	void end();

	// 入力タスクの取得
	BMW::Input::CTaskInput*		getInputTask(){ return pInput_; }
	// BGMの取得
	BMW::Sound::CBgm*			getBgm(){ return pBgm_; }
	// 前景の取得
	BMW::Scene::CFoward*		getFoward(){ return pFoward_; }
	// フレームスキップ
	bool						IsSkip()const{ return bSkip_; }
	void						skip(bool bSkip){ bSkip_=bSkip; }
	void						animeSkip();

	// タスクコントローラへの設定
	bool	IsStopScene(){ return !pCtrl_->IsValid(); }
	void	stopScene(bool bStop){ pCtrl_->valid(!bStop); }

	// 各DBのデータ取得
	BMW::Sound::CSoundDB&				getBgmDB(){ return bgmDB_;}
	BMW::Sound::CSeDB&					getSeDB(){ return seDB_;}
	BMW::Face::CFaceMap&				getFaceMap(){ return faceMap_; }
	const BMW::Chara::CCharaDB&			getChara() const { return charaDB_; }
	const BMW::Weapon::CWeaponDB&		getWeapon() const { return weaponDB_; }
	BMW::Spirit::CSpiritDB&				getSpirit(){ return spiritDB_; }
	BMW::Ability::CAbilityDB&			getAbility(){ return abilityDB_; }
	BMW::Item::CItemDB&					getItem(){ return itemDB_; }
	BMW::Scenario::CScenarioDB&			getScenario(){ return scenarioDB_; }
	BMW::Save::CGlobalData&				getGlobal(){ return globalData_; }
	BMW::Save::CExecData&				getExec(){ return execData_; }

//#ifdef BMW_DEBUG
	smart_ptr<CFPSTimer>&				getTimer(){ return timer_; }
	void								setTimer(const smart_ptr<CFPSTimer>& timer){ timer_=timer; }
//#endif

	// 操作
	void clearSeCache();

	// グローバルコンフィグ設定
	void setupGlobal();
	void saveGlobal(const string& sSave="");
	void loadGlobal(const string& sSave="");

	// ランダム
	static CRand rand_;

#ifdef BMW_DEBUG
	static bool bAction_;

	// ウインドウサイズ取得（描画領域）
	void getWindowSize(int& nWidth, int& nHeight)
	{
		RECT rect;
		::GetClientRect(CAppManager::GetMyWindow()->GetHWnd(),&rect);
		nWidth = rect.right;
		nHeight = rect.bottom;
	}

	// ウインドウタイトル設定
	void setWindowTitle(const string& sTitle)
	{
		::SetWindowText(CAppManager::GetMyWindow()->GetHWnd(),sTitle.c_str());
	}
#endif

protected:
	CFastPlaneFactory	planeFactory_;
	//	↑こいつがCFastDrawを内包しているので、こいつ経由で描画すれば良い
	CSoundFactory		soundFactory_;
	//	↑Soundについても同様

	// 入力
	BMW::Input::CTaskInput* pInput_;
	// BGM
	BMW::Sound::CBgm*		pBgm_;
	// 前景
	BMW::Scene::CFoward*	pFoward_;
	// フレームスキップフラグ
	bool					bSkip_;

	// シーンコントローラ
	BMW::Task::CTaskListCtrl*	pCtrl_;
	
	// ここに統合的なデータマネージャーが配置
	// 種別では無く用途別

	// BGM用のサウンドDB
	BMW::Sound::CSoundDB		bgmDB_;
	// SE DB
	BMW::Sound::CSeDB			seDB_;
	// 顔マップ
	BMW::Face::CFaceMap			faceMap_;
	// キャラデータ
	BMW::Chara::CCharaDB		charaDB_;
	// 武器データ
	BMW::Weapon::CWeaponDB		weaponDB_;
	// 精神データ
	BMW::Spirit::CSpiritDB		spiritDB_;
	// アビリティデータ
	BMW::Ability::CAbilityDB	abilityDB_;
	// アイテムデータ
	BMW::Item::CItemDB			itemDB_;
	// シナリオデータ
	BMW::Scenario::CScenarioDB	scenarioDB_;
	// ゲームグローバルデータ
	BMW::Save::CGlobalData		globalData_;
	// 現在実行中のゲームデータ
	BMW::Save::CExecData		execData_;
	// FPSタイマ
	smart_ptr<CFPSTimer>		timer_;
};