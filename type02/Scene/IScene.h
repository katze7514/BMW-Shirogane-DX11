/*
	katze 06/02/13
	トップレベルシーンのテンプレート基底クラス
*/
#pragma once

#include "../GUI/DB/CGuiDefDB.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace Scene{

class IScene : public Task::CTaskList
{/**
	シーン基底
 */
public:
	// デストラクタ
	virtual ~IScene(){}

	BMW::GUI::CGuiDefDB& getGuiDefDB(){ return guiDef_; }
	void				 setGuiDefDB(const string& sID){ guiDef_.setGuiDef(Config::Const::configDB_.getConfigFileStr(sID)); }

protected:
	// GUIはシーンごとにもつで
	BMW::GUI::CGuiDefDB guiDef_;
};

} // namespace Scene end
} // namespace BMW end