#include "stdafx.h"

#include "../Sound/SoundCode.h"

#include "../SLG/various/CWait_frame.h"

#include "IDADV.h"
#include "CADVContext.h"

#include "AdvApi.h"
#include "CAdvCmdFactory.h"

#include "CAdvParser.h"

#include "CAdvFactory.h"

namespace BMW{
namespace ADV{

smart_ptr<Task::ITaskList> CAdvFactory::createTaskList(int nID)
{
	if(nID<Rule::MAIN)
	{
		return mapApi_[nID];
	}
	else
	{
		VM::CScriptExec* pExec = new VM::CScriptExec();
		pExec->setScript(mapScript_[nID]);
		return smart_ptr<Task::ITaskList>(pExec);
	}
}

void CAdvFactory::OnInit(CADVContext& p)
{// ようは、スクリプトを解釈して、実行系列を作るだけ

	// API生成
	// メッセージ変更
	API::CMsg_change* pMsg = new API::CMsg_change();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MSG,smart_ptr<Task::ITaskList>(pMsg)));
	// 背景変更
	API::CBack_change* pBack = new API::CBack_change();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::BACK,smart_ptr<Task::ITaskList>(pBack)));
	// 入力待ち
	API::CWait_input* pInput = new API::CWait_input();
	pInput->setState(Rule::MSG_BACK);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::WAIT_INPUT,smart_ptr<Task::ITaskList>(pInput)));	
	// BGMフェード待ち
	Sound::Code::CCode_bgm_wait* pBgmWait = new Sound::Code::CCode_bgm_wait();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::WAIT_BGM,smart_ptr<Task::ITaskList>(pBgmWait)));
	// SE再生終了待ち
	Sound::Code::CCode_se_wait* pSeWait = new Sound::Code::CCode_se_wait();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::WAIT_SE,smart_ptr<Task::ITaskList>(pSeWait)));
	// フェード待ち
	API::CWait_fade* pFadeWait = new API::CWait_fade();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::WAIT_FADE,smart_ptr<Task::ITaskList>(pFadeWait)));
	// フレーム待ち
	SLG::CWait_frame* pFrameWait = new SLG::CWait_frame();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::WAIT_FRAME,smart_ptr<Task::ITaskList>(pFrameWait)));
	// シナリオ
	API::CScenario_select* pScenario = new API::CScenario_select();
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::SCENARIO,smart_ptr<Task::ITaskList>(pScenario)));
	// アイテム追加
	API::CItem_ctrl* pItem = new API::CItem_ctrl();
	pItem->OnReset(&p);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::ITEM_CTRL,smart_ptr<Task::ITaskList>(pItem)));
	// バックログ
	API::CMsg_back* pMsgBack = new API::CMsg_back();
	pMsgBack->OnReset(&p);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(Rule::MSG_BACK,smart_ptr<Task::ITaskList>(pMsgBack)));
}

void CAdvFactory::setScript(const string& sFile, CADVContext& context)
{
	using namespace boost::spirit;
	using namespace phoenix;

#ifdef BMW_DEBUG
	CDbg().Out("ADV %s",sFile.c_str());
#endif
	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(sFile);
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	// 構文解析
	CAdvParser ps(*this,&context);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("%s 読み込み失敗！！",/*sFile.c_str()*/r.stop);
#endif
}

} // namespace ADV end
} // namespace BMW end