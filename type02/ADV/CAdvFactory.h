/*
	katze 05/05/18
	ADV VM用のFactory
*/
#pragma once

#include "CAdvMap.h"

namespace BMW{
namespace ADV{
class CADVContext;
class CAdvFactory : public Task::ITaskListFactory
{/**
	ADV VM用のFactory
 */
public:
	typedef map<int, smart_ptr<Task::ITaskList> > api_map;
	typedef map<int, smart_ptr<VM::CScript> > script_map;

	smart_ptr<Task::ITaskList> createTaskList(int nID);

	// 設定子
	void OnInit(CADVContext& p);
	void setScript(const string& sFile, CADVContext& p);

	// 操作
	void addMain(const smart_ptr<VM::CScript>& pScript)
	{
		mapScript_.insert(pair<int,smart_ptr<VM::CScript> >(scriptID_.getValue("MAIN"), pScript));
	}
	void addScript(const string& sID, const smart_ptr<VM::CScript>& pScript)
	{
		mapScript_.insert(pair<int,smart_ptr<VM::CScript> >(scriptID_.addMap(sID), pScript));
	}
	int getScriptID(const string& sID)
	{
		return scriptID_.getValue(sID);
	}

private:
	// ADVでちょっとだけあるAPIマップ
	api_map		mapApi_;
	// こいつが、ADVシーンの実体スクリプト
	script_map	mapScript_;
	// ↑のマップ
	CAdvMap		scriptID_;
};

} // namespace ADV end
} // namespace BMW end