#include "stdafx.h"

#include "mode.h"

#include "Task/CTaskContext.h"
#include "Task/CTaskListCtrl.h"
#include "Movie/DB/CSymbolDB.h"
#include "Input/CTaskInput.h"
#include "Scene/IDScene.h"
#include "Scene/IDGui.h"
#include "Scene/CSceneFactory.h"
#include "SLG/Context/CDataBattle.h"
#include "SLG/Map/CMapChipState.h"

#include "CApp.h"

CRand CApp::rand_;

#ifdef BMW_DEBUG
bool CApp::bAction_=true;
#define BMW_CAPTURE
#endif

// ウインドウモード
#ifdef STAGE_CREATE
// SPECIAL_WINDOW倍ウインドウになる
#define SPECIAL_WINDOW 1
#define SPECIAL_WINDOW_WIDTH 640
#define SPECIAL_WINDOW_HEIGHT 480
#else
#define SPECIAL_WINDOW 0
#endif

void CApp::MainThread()
{
	// ogg読み込み設定
	GetSoundFactory()->GetSoundParameter()->GetStreamFactory()->GetPlugInMap()->Write("ogg", "CVorbisStream");

	/** 
		アプリケーション設定
	*/
	// グローバルコンフィグ設定
	setupGlobal();

	// こいつに渡すフラグは、コンフィグで設定できる
	// 色深度の設定（16 or 32）
	GetDraw()->SetDisplay(globalData_.IsFull(),0,0,globalData_.getColor());
	// システムカーソルは描画しない
	CAppManager::GetMyWindow()->ShowCursor(false);

	// FPSの設定
	CFPSTimer t;
	t.SetFPS(30); // 30fpsですよん

	//  これをメインプリにする（終了するときに、他のウィンドゥをすべて閉じる）
	SetMainApp(true);
	// Timer
	setTimer(smart_ptr<CFPSTimer>(&t,false));

	/**
		BMWの実体
	*/
	// 乱数初期化
	::srand(::GetTickCount());
	rand_.Randomize();
	
	// タスクの設定
	// タスクコンテキスト
	BMW::Task::CTaskContext context_;
	// App
	context_.setApp(smart_ptr<CApp>(this,false));
	// 描画対象
	CPlane plane(smart_ptr<ISurface>(GetDraw()->GetSecondary(),false));
	context_.setDrawPlane(&plane);
	// 入力タスク生成
	pInput_ = new BMW::Input::CTaskInput();
	context_.setInput(pInput_);
	// サウンド
	// BGM
	bgmDB_.setSoundFactory(smart_ptr<ISoundFactory>(GetSoundFactory(),false));
	bgmDB_.getSoundLoader()->SetStreamPlay(true);
	pBgm_ = new BMW::Sound::CBgm(smart_ptr<BMW::Sound::CSoundDB>(&bgmDB_,false));
	context_.setBgmSound(pBgm_);
	BMW::Sound::CBgm::setBgmVolume(DSBVOLUME_MAX);
	// SE
	seDB_.getSeLoader()->SetSoundFactory(smart_ptr<ISoundFactory>(GetSoundFactory(),false));
	seDB_.getSeLoader()->SetLockInterval(1);
	BMW::Sound::CSeDB::setSeVolume(DSBVOLUME_MAX);
	// フォワード
	pFoward_ = new BMW::Scene::CFoward();

	// グローバルデータ反映
	globalData_.setData(&context_);

	// 戦闘データ
	context_.setBattleData(smart_ptr<BMW::SLG::CDataBattle>(new BMW::SLG::CDataBattle()));
	// シーンファクトリ
	//BMW::Scene::CSceneFactory factory;
	// タスクコントローラ
	BMW::Task::CTaskListCtrl ctrl_(smart_ptr<BMW::Task::ITaskListFactory>(new BMW::Scene::CSceneFactory()));
	pCtrl_ = &ctrl_;
	// 初めは初期Load画面から
	ctrl_.jumpTaskList(BMW::Scene::ID::INIT_LOAD);
	//ctrl_.jumpTaskList(BMW::Scene::ID::TITLE);

	/**
		各種Constデータ初期化
		
		全体共通のは、初期ロード画面内で初期化される
	*/
	// コンフィグファイルテーブル
	// ほとんど唯一のハードコーディング
	BMW::Config::Const::configDB_.setConfigDB("data1/config.xml");

	// BGM DB
	bgmDB_.setSoundDB(BMW::Config::Const::configDB_.getConfigFileStr("BGM"),
					  BMW::Config::Const::configDB_.getConfigFileStr("BGM_ID"));
	BMW::Sound::Const::bgmID_.readMapFile(BMW::Config::Const::configDB_.getConfigFileStr("BGM_ID"));
	// SE Loader
	seDB_.setSeDB(BMW::Config::Const::configDB_.getConfigFileStr("SE"),
				  BMW::Config::Const::configDB_.getConfigFileStr("SE_ID"));
	(*seDB_.getSeLoader()->GetCancelFlag())=false;
	//CDbg().Out(seDB_.getSeLoader()->GetFileName(0));
	// 前景構築
	pFoward_->OnInit(&context_);
	// InputTaskのグラフィックの設定
	pFoward_->getSymbolDB().setGraphicGui(pInput_->getGraphic(),"CURSOL_G");
	pInput_->cursolVisible(false);
	pInput_->guard(true);

#ifdef BMW_CAPTURE
	// キャプチャーカウンタ
	int nCapCounter=0;
	string sCapFolder="cap\\";
	bool bCap_=false;
#endif

	//VGAに倍サイズ書き込んでみるテスト
//#if SPECIAL_WINDOW==2
//	CFastPlane triBuffer;
//	triBuffer.CreateSurface(640,480,true);
//	CPlane tri(smart_ptr<ISurface>(&triBuffer,false));
//	context_.setDrawPlane(&tri);
//	SIZE size;
//	size.cx=1280;
//	size.cy=960;
//#endif

	//char fps[52];
	//fps[0]='\0';

	/**
		ゲームループ
	*/
	// ちょっとしたループ最適化
	BMW::Input::CTaskInput* pInput	= pInput_;
	BMW::Sound::CSeDB&		se		= seDB_;
	BMW::Sound::CBgm*		pBgm	= pBgm_;
	BMW::Scene::CFoward*	pFoward	= pFoward_;
	// つまり、こいつが実体ってわけさ
	while (IsThreadValid())
	{
		// 移動フェーズ
		context_.action(true);
		pInput->Task(&context_);
	#ifdef BMW_DEBUG
		context_.action(bAction_);
	#endif
		ctrl_.Task(&context_);
		pFoward->Task(&context_);
		
		// prototypeでは、スキップ無しだったが、
		// type01以降では、基本的にスキップする
		// もちろん、オプション設定可能
		if(!(IsSkip() && t.ToBeSkip()))
		{// 描画フェーズ
			//triBuffer->Clear();
		#if SPECIAL_WINDOW>=1
			// SPECIAL_WINDOWモードの場合は、VGA外領域は何も
			// 描かれないのでクリアする
			GetDraw()->GetSecondary()->SetFillColor(RGB(0,0,0));
			GetDraw()->GetSecondary()->Clear();
		#endif

			context_.action(false);
			ctrl_.Task(&context_);
			pFoward->Task(&context_);
			// 手動描画プライオリティ変換～ｗ
			pInput->Task(&context_);

		/*#if SPECIAL_WINDOW==2
			// セカンダリに二分の一縮小描き込み
			GetDraw()->GetSecondary()->BltNatural(&triBuffer,0,0,&size);
		#endif*/

			// flip
			GetDraw()->OnDraw();

		#ifdef BMW_CAPTURE
			if(pInput->getKeyBoard().IsKeyPush(DIK_F5)) bCap_ = !bCap_;
			if(bCap_)
			{// 画面のキャプチャ
				CDIBitmap bmp;
				bmp.CreateSurface(640,480,globalData_.getColor());
				bmp.GetSurfaceInfo()->GeneralBlt(CSurfaceInfo::eSurfaceBltFast,
					GetDraw()->GetSecondary()->GetSurfaceInfo(),([]{ static CSurfaceInfo::CBltInfo b; return &b; }()),0);

				string sCapFile = sCapFolder + CStringScanner::NumToStringZ(nCapCounter++,6) + ".bmp";
				bmp.Save(sCapFile);
			}
		#endif
		}

		// BGMの動作
		pBgm->OnSound();
		// SEの再生
		se.OnPlay();
		// フレーム調整
		t.WaitFrame();

	/*#if BMW_DEBUG
		sprintf(fps,"FPS:%d",t.GetRealFPS());
		::SetWindowText(CAppManager::GetMyWindow()->GetHWnd(),fps);
	#endif*/
	}

	// グローバルデータの保存
	saveGlobal(BMW::sSaveFolder+"\\");

	// 入力タスクの削除
	DELETE_SAFE(pInput_);
	// サウンドの削除
	DELETE_SAFE(pBgm_);
	// Fowardの削除
	DELETE_SAFE(pFoward_);
}

void CApp::end()
{
	::SendMessage(GetMyApp()->GetHWnd(),WM_CLOSE,0,0);
}

void CApp::animeSkip()
{// アニメーション時のスキップは、設定と同じ
	bSkip_=getGlobal().IsSkip();
}

void CApp::clearSeCache()
{
	// SEローダ読み直し(キャッシュクリアとも言う)
	getSeDB().getSeLoader()->ReleaseAll();
	// SE_IDの方は、何度も読み込むとマップが増えていくだけなので、Loaderの方だけを読み直せば良い
	getSeDB().getSeLoader()->Set(BMW::Config::Const::configDB_.getConfigFileStr("SE"));
}

void CApp::setupGlobal()
{
	// コンフィグデータ位置替え
	// ゲームフォルダ直下を、savedata直下へ
	string sFile;
	CFile::MakeFullName(BMW::sSaveFolder+"\\"+BMW::Save::sGlobal, sFile);
	if(!CDir().IsFileExist(sFile))
	{// ファイルが見つからない
		CFile::MakeFullName(BMW::Save::sGlobal, sFile);
		if(CDir().IsFileExist(sFile))
		// 旧データがあるなら、そいつをロード
			loadGlobal("");
		
		// ファイル生成
		saveGlobal(BMW::sSaveFolder+"\\");
	}
	// グローバルデータロード
	loadGlobal(BMW::sSaveFolder+"\\");
}

void CApp::saveGlobal(const string& sSave)
{
	CSerialize s;
	s << globalData_;

	string sFile;
	CFile::MakeFullName(sSave + BMW::Save::sGlobal, sFile);

	s.Save(sFile);
}

void CApp::loadGlobal(const string& sSave)
{
	CSerialize s;
	s.SetStoring(false);

	string sFile;
	CFile::MakeFullName(sSave + BMW::Save::sGlobal, sFile);

	s.Load(sFile);
	s << globalData_;
}

//	これがmain windowのためのクラス。
class CAppMainWindow : public CAppBase 
{	//	アプリケーションクラスから派生
	virtual void MainThread(){			   //  これがワーカースレッド
		CApp().Start();
	}

	LRESULT OnPreCreate(CWindowOption& opt);
	LRESULT OnPreClose();
};

LRESULT CAppMainWindow::OnPreCreate(CWindowOption& opt)
{
	opt.caption		= "BattleMoonWars銀";
	opt.classname	= "BMW_BY_WERK";

#if SPECIAL_WINDOW>=1 // 可変ウインドウ
	opt.size_x		=  SPECIAL_WINDOW_WIDTH;
	opt.size_y		=  SPECIAL_WINDOW_HEIGHT;
#else
	opt.size_x		=  BMW::WINDOW_WIDTH;
	opt.size_y		=  BMW::WINDOW_HEIGHT;
#endif

	opt.style		= WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU;

	return 0;
}

LRESULT CAppMainWindow::OnPreClose()
{
	CAppManager::GetMyWindow()->ShowCursor(true);

	if(MessageBox(GetHWnd(), "本当に終了してよろしいですか？", "BattleMoonWars銀", MB_OKCANCEL)==IDOK)
		Close();

	CAppManager::GetMyWindow()->ShowCursor(false);
	return 1;
}

//	言わずと知れたWinMain
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	CObjectCreater* pCreater;

	CSingleApp sapp;

	HMODULE hModule;
	void (__stdcall *dwm_enable_comp) (LPCTSTR);

	hModule = LoadLibrary("dwmapi.dll");
	if(hModule)
	{
		dwm_enable_comp = (void (__stdcall *)(LPCTSTR))GetProcAddress(hModule, "DwmEnableComposition");
		if(dwm_enable_comp)
			dwm_enable_comp(FALSE);
	}

	{
	#ifdef BMW_DEBUG
		Err.Debug(); // デバッグ出力On！！
	#endif

		CAppInitializer init(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
		//	↑必ず書いてね
		
		// ここでロードする
		pCreater = CObjectCreater::GetObj();
		pCreater->LoadPlugIn("ogg.dll");

#ifndef BMW_DEBUG
		if (sapp.IsValid())
#endif
		{
			CThreadManager::CreateThread(new CAppMainWindow);
			//	上で定義したメインのウィンドゥを作成
//			CThreadManager::CreateThread(new CAppMainWindow);
//			↑複数書くと、複数ウィンドゥが生成されるのだ

		}
		//	ここでCAppInitializerがスコープアウトするのだが、このときに
		//	すべてのスレッドの終了を待つことになる
	}
	
	// ここでレリース(笑)する
	// こうしないと、CAppFrameのデストラクタで落ちる事になる
	// （DLLを解放すると、そこから来たオブジェクトが不正になる）
	pCreater->ReleasePlugIn("ogg.dll");

	return 0;
}