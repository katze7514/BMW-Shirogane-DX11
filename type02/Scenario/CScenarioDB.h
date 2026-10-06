/*
	katze 05/06/13
	ゲーム進行データのDB
*/
#pragma once

namespace BMW{
namespace Scenario{
class CDataScenario;

class CScenarioDB
{/*
	ゲーム進行データDB
 */
public:
	typedef map<int, CDataScenario*> scenario_map;
	// デストラクタ
	~CScenarioDB();

	// 設定
	void			setScenarioDB(const string& sFile);
	CDataScenario*  getScenario(int nID)
	{ 
		scenario_map::iterator it = mapScenario_.find(nID);
		return it!=mapScenario_.end() ? it->second : NULL;
	}
	CDataScenario*  getScenario(const string& sID){ return getScenario(scenarioID_.getValue(sID)); }
	void			setScenario(CDataScenario* pScenario,int nID)
	{
		mapScenario_.insert(pair<int, CDataScenario*>(nID,pScenario));
	}
	void			setScenario(CDataScenario* pScenario, const string& sID)
	{
		setScenarioID(sID);
		pScenario->setID(getScenarioID(sID));
		mapScenario_.insert(pair<int, CDataScenario*>(pScenario->getID(),pScenario));
	}

	int				getScenarioID(const string& sID){ return scenarioID_.getValue(sID); }
	void			setScenarioID(const string& sID, int nID){ scenarioID_.writeMap(sID,nID); }
	void			setScenarioID(const string& sID){ scenarioID_.writeMap(sID,scenarioID_.getMapSize()); }

	scenario_map&	getScenarioDB(){ return mapScenario_; }

	// 取得
	int				getNo(int nID);
	const string&	getTitle(int nID);

private:
	scenario_map				mapScenario_;
	katzeSDK::Misc::CStringMap	scenarioID_;
};

} // namespace Game end
} // namespace BMW end