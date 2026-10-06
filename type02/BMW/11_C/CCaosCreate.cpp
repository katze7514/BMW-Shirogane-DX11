#include "stdafx.h"

#include "../../SLG/IDSLG.h"
#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Action/IDAction.h"
#include "../../SLG/VM/CSlgCmdFactory.h"
#include "../../SLG/VM/CSlgVM.h"
#include "../../SLG/Code/CCode_parent_state.h"
#include "../../SLG/Sally/CSally_add_chara_map.h"
#include "../../SLG/various/CScript_base.h"


#include "CCaosCreate.h"

namespace BMW{
namespace SLG{
namespace C_11{

CCaosCreate::CCaosCreate()
{
	pExec_ = new VM::CScriptExec();
	pExec_->setParent(smart_ptr<Task::ITaskBase>(this,false));
}

CCaosCreate::~CCaosCreate()
{
	DELETE_SAFE(pExec_);
}

void CCaosCreate::OnInit(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	createAddCaos(p);
}

void CCaosCreate::OnAction(Task::CTaskContext* pContext)
{
	if(getState()==NORMAL)
		pExec_->Task(pContext);
	else
		getTaskListCtrl()->returnTaskList();
}

namespace{
int getCaosIndex()
{
	switch(CApp::rand_.Get(3))
	{
	case 0:	return 37;
	case 1:	return 45;
	default:return 53;
	}
}
} // namespace end

void CCaosCreate::createAddCaos(CSLGContext* p)
{
	setState(NORMAL);
	VM::CScript* pScript = new VM::CScript();
	pExec_->setScript(smart_ptr<VM::CScript>(pScript));
	pExec_->setTaskListCtrl(getTaskListCtrl());

	// カオス追加
	int nChara=CApp::rand_.Get(4);
	if(nChara>=3)
	{// 4分の1でカオスは出てこない
		setState(END);
		return;
	}
	int nID = Script::CScript_base::getFlag("CAOS_BATTLE",p);
	Script::CScript_base::setFlag(nID+1, "CAOS_BATTLE",p);
	nID+=1000;
	list<int> listParam;
	nChara += Chara::Const::charaID_.getValue("ENEMY_CAOS_SHIKA_2");
	CSlgCmdFactory::createAddChara(nID,
								   2,
								   nChara,
								   Phase::ENEMY,
								   Action::NORMAL,
								   listParam,
								   pScript);
	CSlgCmdFactory::createSetWeapon(nID,pScript);

	// マップに追加
	CSlgCmdFactory::createAddCharaMap(nID, 
									  getCaosIndex(),
									  Way::LEFT,
									  Sally::CSally_add_chara_map::NORMAL,
									  pScript);
	// 最後に状態変化を突っ込む
	pScript->addCode(new Code::CCode_parent_state(END));
	pExec_->OnReset(p);
}

} // namespace C_11 end
} // namespace SLG end
} // namespace BMW end
