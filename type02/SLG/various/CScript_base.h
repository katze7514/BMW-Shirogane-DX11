/*
	katze 05/07/20
	スクリプト制御の基底クラス
*/
#pragma once

namespace BMW{
namespace SLG{
class CSLGContext;

namespace Script{

class CScript_base : public Task::ITaskList
{/**
	スクリプト制御の基底クラス
 */
public:
	// デストラクタ
	virtual ~CScript_base(){}

	// 操作
	static int	getFlag(int nFlag,Task::CTaskContext*);
	static void	setFlag(int nValue, int nFlag,Task::CTaskContext*);
	static int	getFlag(const string& sFlag,Task::CTaskContext*);
	static void	setFlag(int nValue, const string& sFlag,Task::CTaskContext*);
	static int	getFlag(const string& sFlag,CSLGContext*);
	static void	setFlag(int nValue, const string& sFlag,CSLGContext*);
	void		callScript(const string& sID,Task::CTaskContext*);
	static int	getScriptID(const string& sID,Task::CTaskContext*);
	static int	getSlgID(const string& sID,Task::CTaskContext*);

	static CSLGContext*	getSlgContext(Task::CTaskContext*);
	static bool IsAct(int nID, int nAct, CSLGContext& context);
	static bool IsAct(const string& sID, int nAct, CSLGContext& context);
};

} // namespace Script end
} // namespace SLG end
} // namespace BMW end