#include "stdafx.h"
#include "resource.h"
#include "CGlobalData.h"
#include "capp.h"
void	CApp::MainThread() {
/**
	起動設定用Dialog
*/

	CGlobalData data;
	CSerialize s;
	CDir dir;
	if(!dir.IsFileExist("savedata\\config.ini"))
	{// ファイルないし
		if(dir.IsFileExist("config.ini"))
		// 旧データがある
			s.Load("config.ini");
		else // 旧データもないならデフォ値で
			s << data;

		s.Save("savedata\\config.ini");
		s.Clear();
	}

	s.SetStoring(false);
	s.Load("savedata\\config.ini");
	s << data;
	
	CFPSTimer timer;
	timer.SetFPS(10);

	//	これをメインプリにする（終了するときに、他のウィンドゥをすべて閉じる）
	SetMainApp(true);

	CDialogHelper dialog(GetMyApp()->GetMyWindow());

	int nOK = dialog.HookButtonOnClick(ID_BUTTON2);
	int nCancel = dialog.HookButtonOnClick(ID_BUTTON);


	dialog.SetCheck(IDC_FULL, data.IsFull()?1:0);
	dialog.SetCheck(IDC_WIN, data.IsFull()?0:1);

	dialog.SetCheck(IDC_32, data.getColor()==CGlobalData::COLOR_32?1:0);
	dialog.SetCheck(IDC_16, data.getColor()==CGlobalData::COLOR_32?0:1);
	
	while (IsThreadValid()){
		if(dialog.GetPoolInfo(nOK)->isPool())
		{
			//	OKボタンおされとる！
			dialog.GetPoolInfo(nOK)->reset();
			// ダイアログから情報を取得
			// 起動モード
			int nMode = dialog.GetCheck(IDC_FULL);
			data.full(nMode==1);
			
			nMode = dialog.GetCheck(IDC_32);
			if(nMode==1)
				data.setColor(CGlobalData::COLOR_32);
			else
				data.setColor(CGlobalData::COLOR_16);

			// データセーブ
			s.Clear();
			s.SetStoring(true);
			s << data;
			s.Save("savedata\\config.ini");
			break;
		}
		else if(dialog.GetPoolInfo(nCancel)->isPool())
		{
			// キャンセルされた・・・
			dialog.GetPoolInfo(nCancel)->reset();
			break;
		}

		timer.WaitFrame();
	}
}

//	これがmain windowのためのクラス。
class CAppMainWindow : public CAppBase {	//	アプリケーションクラスから派生
	virtual void MainThread(){			   //  これがワーカースレッド
		CApp().Start();
	}
	virtual LRESULT OnPreCreate(CWindowOption &opt){
		opt.dialog = MAKEINTRESOURCE(IDD_DIALOG1);	//	ダイアログなのだ！
		return 0;
	}
};


//	言わずと知れたWinMain
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	{
		/*
		{	//	エラーログをファイルに出力するのら！
			CTextOutputStreamFile* p = new CTextOutputStreamFile;
			p->SetFileName("Error.txt");
			Err.SelectDevice(smart_ptr<ITextOutputStream>(p));
		}
		*/

		CAppInitializer init(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
		//	↑必ず書いてね

		CSingleApp sapp;
		if (sapp.IsValid()) {
			CThreadManager::CreateThread(new CAppMainWindow);
		}
		//	ここでCAppInitializerがスコープアウトするのだが、このときに
		//	すべてのスレッドの終了を待つことになる
	}
	return 0;
}
