/**
	katze 05/03/22
	コンフィグ（設定ファイル）DB
*/
#pragma once

#include "../../Misc/CIntMap.h"
#include "../../Misc/CStringMap.h"

namespace BMW{
namespace Config{
// using宣言
using katzeSDK::Misc::CIntMap;
using katzeSDK::Misc::CStringMap;

class CConfigDB
{/**
	設定ファイルDB
 */
public:
	// 操作
	void setConfigDB(const string& sFile);

	// Fileに対する操作
	void			writeMapFile(int nID, const string& sValue){ configFile_.writeMap(nID,sValue); }
	const string&	getConfigFile(int nID) const { return configFile_.getValue(nID); }
	// IDに対する操作
	void			writeMapID(const string& sID, int nValue){ configID_.writeMap(sID,nValue); }
	int				getConfigID(const string& sID) const { return configID_.getValue(sID); }
	// ヘルパ
	const string&	getConfigFileStr(const string& sID) const { return configFile_.getValue(getConfigID(sID)); }

private:
	CIntMap		configFile_;
	CStringMap	configID_;
};

} // namespace BMW end
} // namespace Config end