#include "stdafx.h"

#include "IPanelParser.h"
#include "IPanel.h"

namespace BMW{
namespace GUI{

void IPanel::setID(const string& sID)
{
	if(getID(sID)>=0) return;
	priorityID_.writeMap(sID,priorityID_.getMapSize());
}

Task::ITaskBase* IPanel::getWidgetRec(const string& sID)
{/**
	本当なら再帰関数にするのが本道だが、
	どうせ末尾再帰なので、ループにしちゃう

	ちなみに再帰で書くと

	Task::ITaskBase* getWidgetRec(const string& sID, Task::ITaskBase* pBase)
	{
		if(sID.empty()) return pBase;
		
		string sID2 = sIDを左から見ていき/にぶつかるまで取得
		string sID3 = sID - sID2
		return getWidgetRec(sID3, static_cast<IPanel*>(pBase)->getWidget(sID2));
	}
 */
	using namespace boost::spirit;
	using namespace phoenix;

	// 構文解析
	IPanelParser ps;
	parse(sID.c_str(), ps);

	// 解析結果を使って検索
	Task::ITaskBase* pBase;
	list<string>::iterator it=ps.listPath_.begin();
	pBase = getWidget(*it++);
	while(it!=ps.listPath_.end())
	{// パスが残ってたら一個パネルを下がる
		// そいつの中で検索
		pBase = static_cast<IPanel*>(pBase)->getWidget(*it++);
	}

	return pBase;
}

} // namespace GUI end
} // namespace BMW end