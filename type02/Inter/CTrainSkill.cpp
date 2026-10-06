#include "stdafx.h"

#include "../Ability/IDAbility.h"
#include "../Status/status_fun.h"

#include "../Scene/IScene.h"
#include "../Scene/GUI/CNumCtrl.h"

#include "IDInter.h"
#include "CInterContext.h"
#include "CInterChara.h"

#include "CChara.h"
#include "CTrainSkill.h"

namespace BMW{
namespace Inter{
namespace Chara{

namespace{
__inline void setSkillEventHandler(GUI::CPanel* pPanel, const GUI::CButton::ButtonEvent& fun, int nValue)
{
	GUI::CButton::setButtonEvent(pPanel->getWidgetCast<GUI::CButton>("SHORTBAR"),fun,nValue);
}

void setHaveBarEventHandler(GUI::CPanel* pPanel, const GUI::CButton::ButtonEvent& fun, int nValue)
{
	for(int i=0; i<6; ++i)
		setSkillEventHandler(pPanel->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("ROW",i+1)),fun,nValue+i);
}

void initGetRow(GUI::CPanel* pPanel, const GUI::CButton::ButtonEvent& fun, int nValue, Ability::CAbilityDB& db)
{
	GUI::CPanel* pRow;
	GUI::CTextPopUp* pText;
	for(int i=0; i<7; ++i)
	{
		pRow = pPanel->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("ROW",i+1));
		// イベントハンドラ
		setSkillEventHandler(pRow,fun,nValue+i);
		// 技能名
		pText = pRow->getWidgetCast<GUI::CTextPopUp>("SKILLNAME");

		db.setTextPopUpHolder(pText, CTrainSkill::get2Ability(nValue+i));
		pText->UpdateTextAA();
		// 必要FP
		pRow->getWidgetCast<GUI::INum>("SKILLPOINT")->setNum(db.getGetFP(CTrainSkill::get2Ability(nValue+i)));
	}
}

__inline void updateSkillBar(GUI::CPanel* pPanel, int nID, int nAttr, bool bValid, Ability::CAbilityDB& db, bool bPoint=false, bool bSenten=false)
{
	// 上書き可・不可フラグ
	Task::ITaskBase* pButton = pPanel->getWidget("SHORTBAR");
	pButton->setState(GUI::CButton::NORMAL);
	pButton->valid(bValid);
	// テキスト設定
	GUI::CTextPopUp* pText = pPanel->getWidgetCast<GUI::CTextPopUp>("SKILLNAME");
	db.setTextPopUpHolder(pText,nID);
	Status::addLvStr(nID, nAttr, pText->getText());
	// 先天技能だったら、*を追加
	if(bSenten) pText->setText("*" + pText->getText());
	pText->UpdateTextAA();

	if(bPoint)
	{// 必要FP
		GUI::INum* pNum = pPanel->getWidgetCast<GUI::INum>("SKILLPOINT");
		if(nAttr<0) pPanel->getWidget("SKILLPOINT")->visible(false);
		else pNum->setNum(db.getGetFP(nID,nAttr));
	}
}

__inline void updateSkillBar(GUI::CPanel* pPanel, const BMW::Chara::CStatusAbility& skill, bool bValid, Ability::CAbilityDB& db, bool bPoint=false, bool bSenten=false)
{
	// 上書き可・不可フラグ
	//Task::ITaskBase* pButton = pPanel->getWidget("SHORTBAR");
	//pButton->setState(GUI::CButton::NORMAL);
	//pButton->valid(bValid);
	// テキスト設定
	//GUI::CTextPopUp* pText = pPanel->getWidgetCast<GUI::CTextPopUp>("SKILLNAME");
	//db.setTextPopUpHolder(pText,skill.getID());
	//Status::addLvStr(skill, pText->getText());
	//pText->UpdateTextAA();
	updateSkillBar(pPanel,skill.getID(),skill.getAttr(),bValid,db, bPoint, bSenten);
}

__inline void resetGetRow(GUI::CPanel* pPanel)
{
	GUI::CPanel* pBar;
	Task::ITaskBase* pButton;
	for(int i=1; i<=7; ++i)
	{// リセット
		pBar = pPanel->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("ROW",i));
		pButton = pBar->getWidget("SHORTBAR");
		pButton->setState(GUI::CButton::NORMAL);
		pButton->valid(true);
		pBar->getWidget("SKILLPOINT")->visible(true);
	}
}

__inline bool IsLvSkill(int nID)
{// LV制技能や獲得不可能な技能判定
	switch(nID)
	{
	case Ability::SPECTER:
	case Ability::MAGICIAN:
	case Ability::VAMPIRE:
	case Ability::FUNDPOWER:
	case Ability::COUNTER:
	case Ability::BACKUPATTACK:
	case Ability::BACKUPDEFENCE:
	case Ability::SPUP:				
	case Ability::LUCKY:
	case Ability::MOVE_UP:			return true;
	default:						return false;
	}
}

} // namespace end
__inline int CTrainSkill::getGetFP(int nID, Ability::CAbilityDB& db)
{
	switch(nID)
	{
	case POWER:		return db.getGetFP(Ability::FUNDPOWER,nPower_);
	case COUNTER:	return db.getGetFP(Ability::COUNTER,nCounter_);
	case BACKUP_ATT:return db.getGetFP(Ability::BACKUPATTACK,nBackAtk_);
	case BACKUP_DEF:return db.getGetFP(Ability::BACKUPDEFENCE,nBackDef_);
	case SPUP:		return db.getGetFP(Ability::SPUP,nSpUp_);
	case MOVE_UP:	return db.getGetFP(Ability::MOVE_UP,nMoveUp_);
	default:		return db.getGetFP(get2Ability(nID));
	}
}

__inline void CTrainSkill::calcLv(int nID, int nValue)
{
	switch(nID)
	{
	case Ability::FUNDPOWER:		nPower_+=nValue;	break;
	case Ability::COUNTER:			nCounter_+=nValue;	break;
	case Ability::BACKUPATTACK:		nBackAtk_+=nValue;	break;
	case Ability::BACKUPDEFENCE:	nBackDef_+=nValue;	break;
	case Ability::SPUP:				nSpUp_+=nValue;		break;
	case Ability::MOVE_UP:			nMoveUp_+=nValue;	break;
	default: break;
	}
}
__inline int CTrainSkill::getAttr(int nID)
{
	switch(nID)
	{
	case POWER:			return nPower_;
	case COUNTER:		return nCounter_;
	case BACKUP_ATT:	return nBackAtk_;
	case BACKUP_DEF:	return nBackDef_;
	case SPUP:			return nSpUp_;
	case MOVE_UP:		return nMoveUp_;
	default:			return 0;
	}
}
int	CTrainSkill::ability2Get(int nID)
{
	switch(nID)
	{
	case Ability::FUNDPOWER:	return POWER;
	case Ability::COUNTER:		return COUNTER;
	case Ability::BACKUPATTACK:	return BACKUP_ATT;
	case Ability::BACKUPDEFENCE:return BACKUP_DEF;
	case Ability::LINKAGEATTACK:return LINKAGE;
	case Ability::HITAWAY:		return HIT_ADN_WAY;
	case Ability::SPUP:			return SPUP;
	case Ability::SPRECOVER:	return SP_RECOVER;
	case Ability::CONCENT:		return CONCENT;
	case Ability::MAGICSAVE:	return MAGICSAVE;
	case Ability::BATTLESPIRIT:	return BATTLESPIRIT;
	case Ability::FIGHTUP:		return FIGHTUP;
	case Ability::RITHM:		return RITHM;
	case Ability::AGAINST:		return AGAINST;
	case Ability::ATTACKER:		return ATTACKER;
	case Ability::REVENGE:		return REVENGE;
	case Ability::GUARD:		return GUARD;
	case Ability::BREAKLINE:	return BREAKLINE;
	case Ability::BALLETSAVE:	return BALLETSAVE;
	case Ability::IKIYOYO:		return IKIYOYO;
	case Ability::MOVE_UP:		return MOVE_UP;
	default:					return -1;
	}
}

int	CTrainSkill::get2Ability(int nID)
{
	switch(nID)
	{
	case POWER:			return Ability::FUNDPOWER;
	case COUNTER:		return Ability::COUNTER;
	case BACKUP_ATT:	return Ability::BACKUPATTACK;
	case BACKUP_DEF:	return Ability::BACKUPDEFENCE;
	case LINKAGE:		return Ability::LINKAGEATTACK;
	case HIT_ADN_WAY:	return Ability::HITAWAY;
	case SPUP:			return Ability::SPUP;
	case SP_RECOVER:	return Ability::SPRECOVER;
	case CONCENT:		return Ability::CONCENT;
	case MAGICSAVE:		return Ability::MAGICSAVE;
	case BATTLESPIRIT:	return Ability::BATTLESPIRIT;
	case FIGHTUP:		return Ability::FIGHTUP;
	case RITHM:			return Ability::RITHM;
	case AGAINST:		return Ability::AGAINST;
	case ATTACKER:		return Ability::ATTACKER;
	case REVENGE:		return Ability::REVENGE;
	case GUARD:			return Ability::GUARD;
	case BREAKLINE:		return Ability::BREAKLINE;
	case BALLETSAVE:	return Ability::BALLETSAVE;
	case IKIYOYO:		return Ability::IKIYOYO;
	case MOVE_UP:		return Ability::MOVE_UP;
	default:			return -1;
	}
}

///////////////////////////////////////////////////
// タスク
///////////////////////////////////////////////////
void CTrainSkill::OnInit(Task::CTaskContext* pContext)
{
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_YOUSEI_SKILL");
	pPanel_->setParent(getParent());
	// インターフェイス展開
	// 所持
	pHave_ = pPanel_->getWidgetCast<GUI::CPanel>("HAVE");
	// イベントハンドラ設定
	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CTrainSkill::eventHave);
	setHaveBarEventHandler(pHave_, fun, HAVE_0);

	// 獲得
	fun.set(this,&CTrainSkill::eventButton);
	Ability::CAbilityDB& db = pContext->getApp()->getAbility();
	// 一行目
	pGet1_ = pPanel_->getWidgetCast<GUI::CPanel>("GET1");
	initGetRow(pGet1_,fun,POWER,db);
	// 二行目
	pGet2_ = pPanel_->getWidgetCast<GUI::CPanel>("GET2");
	initGetRow(pGet2_,fun,SPUP,db);
	// 三行目
	pGet3_ = pPanel_->getWidgetCast<GUI::CPanel>("GET3");
	initGetRow(pGet3_,fun,BATTLESPIRIT,db);

	// OK
	pOK_=pPanel_->getWidgetCast<GUI::CButton>("OK");
	fun.set(this,&CTrainSkill::eventOK);
	GUI::CButton::setButtonEvent(pOK_,fun,0);

	// FP残
	pRemain_ = pPanel_->getWidgetCast<GUI::CNumCtrl>("REMAIN");
}

void CTrainSkill::OnReset(Task::CTaskContext* pContext)
{
	CInterContext* p = static_cast<CInterContext*>(pContext);
	p->setCtrlAbility(-1);
	p->setTargetAbility(-1);
	Ability::CAbilityDB& db = p->getApp()->getAbility();
	// 残りFP設定
	nFP_=0;
	pRemain_->setNum(p->getApp()->getExec().getFP());
	pRemain_->validNumGui(0/*白*/);

	// キャラデータ取得
	CInterChara* pChara = p->getTargetCharaData();
	BMW::Chara::CDataCharaInter& inter = *pChara->getData();

	// 獲得技能リセット
	resetLv();
	resetGetRow(pGet1_);
	resetGetRow(pGet2_);
	resetGetRow(pGet3_);

	// 所持技能設定
	bool bValid = inter.sizeSkill()+inter.sizeSkillAcqu() >= 6;
	int nPos=1;
	GUI::CPanel* pPanel;
	inter.beginSkill();
	while(!inter.endSkill())
	{// 先天技能
		const BMW::Chara::CStatusAbility& skill = *inter.nextSkill();
		// ID保存
		nHaveSkill_[nPos-1]=skill.getID();
		pPanel = pHave_->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("ROW",nPos++));
		// 先天なので上書き不可
		updateSkillBar(pPanel,skill,false,db,false,true);
		// 持っているのは獲得できない
		invalidGet(skill,db,true);
	}
	inter.beginSkillAcqu();
	while(!inter.endSkillAcqu())
	{//	後天技能
		const BMW::Chara::CStatusAbility& skill = *inter.nextSkillAcqu();
		// ID保存
		nHaveSkill_[nPos-1]=skill.getID();
		pPanel = pHave_->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("ROW",nPos++));
		// 後天なので上書き可
		updateSkillBar(pPanel,skill,bValid,db);
		// 持っているのは獲得できない
		invalidGet(skill,db);
	}

	if(!bValid)
	{// 空きあるなら
		p->setCtrlAbility(nPos-1); //	とりあえず、はじめの空きをセット
		Task::ITaskBase* pButton;
		GUI::CTextPopUp* pText;
		for(int i=nPos; i<=6; ++i)
		{// 残りは空き
			// ID保存
			nHaveSkill_[nPos-1]=-1;
			pPanel = pHave_->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("ROW",nPos++));
			pButton = pPanel->getWidget("SHORTBAR");
			pButton->setState(GUI::CButton::NORMAL);
			pButton->valid(false);
			// テキスト設定
			pText = pPanel->getWidgetCast<GUI::CTextPopUp>("SKILLNAME");
			pText->setText("-------");
			pText->UpdateTextAA();
		}
	}

	// 最後にLv制技能の反映
	updateLvSkill(inter,db);

	// とりあえず、OKボタンは無効
	pOK_->valid(false);
	pOK_->visible(false);
}

// アクション
void CTrainSkill::actionUpdateFP(Task::CTaskContext* pContext)
{
	pRemain_->setNum(pContext->getApp()->getExec().getFP() - nFP_);
	pRemain_->validNumGui(pRemain_->getNum()>=0?0/*白*/:1/*赤*/);
}

//////////////////////////////////////////////////////////
// イベントハンドラ
//////////////////////////////////////////////////////////
namespace{
__inline void validOK(Task::ITaskBase* pOK, Task::CTaskContext* pContext)
{
	if(pContext->getValue(Flag::CTRL_ABILITY)>=0
	&& pContext->getValue(Flag::TARGET_ABILITY)>=0)
	{// 状況が整ってたら使用OK
		pOK->valid(true);
		pOK->visible(true);
	}
	else
	{// 状況が整ってたら使用できない
		pOK->valid(false);
		pOK->visible(false);
	}
}

} // namespace end
void CTrainSkill::eventHave(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 押された
		if(pContext->getValue(Flag::CTRL_ABILITY)>=0
		&& pContext->getValue(Flag::CTRL_ABILITY)!=INT_MAX)
		{// 今まで押されてたのをキャンセル
			GUI::CPanel* pBase = pHave_->getWidgetCast<GUI::CPanel>(pContext->getValue(Flag::CTRL_ABILITY));
			pBase->getWidget("SHORTBAR")->setState(GUI::CButton::NORMAL);
		}
		pContext->setValue(pButton->getValue(),Flag::CTRL_ABILITY);
	}
	ef(GUI::IsCancel(pButton))
	{// キャンセルされた
		pContext->setValue(-1,Flag::CTRL_ABILITY);
	}
	// OKボタンの処理
	validOK(pOK_,pContext);
}

void CTrainSkill::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 押された
		// 今まで押されてたのをキャンセル
		GUI::CPanel* pBase = getBar(pContext->getValue(Flag::TARGET_ABILITY));
		if(pBase!=NULL)	pBase->getWidget("SHORTBAR")->setState(GUI::CButton::NORMAL);

		pContext->setValue(pButton->getValue(),Flag::TARGET_ABILITY);
		pRemain_->setNum(pRemain_->getNum() + nFP_);
		// FP残設定
		nFP_=getGetFP(pButton->getValue(),pContext->getApp()->getAbility());
		pRemain_->setNum(pRemain_->getNum() - nFP_);
		// 残FPによって色替え
		pRemain_->validNumGui(pRemain_->getNum()>=0 ? 0/*白*/ : 1/*赤*/);

		if(pContext->getValue(Flag::CTRL_ABILITY)<0
		|| pContext->getValue(Flag::CTRL_ABILITY)==INT_MAX)
		{// Lv制技能の場合は、上書きできるかもしれない
			switch(pButton->getValue())
			{
			case POWER:
			case COUNTER:
			case MOVE_UP:
			case BACKUP_ATT:
			case BACKUP_DEF:
			case SPUP:
			{// その技能を持ってるかをチェック
				CInterContext* p = static_cast<CInterContext*>(pContext);
				CInterChara* pChara = p->getTargetCharaData();
				BMW::Chara::CDataCharaInter* inter = pChara->getData();
				if(inter->hasSkillAcqu(get2Ability(pButton->getValue()))>0)
					// どうせ上書き処理はされるから、CTRLを正にしとけば良い
					pContext->setValue(INT_MAX,Flag::CTRL_ABILITY);
				else
					pContext->setValue(-1,Flag::CTRL_ABILITY);
			}
			break;

			default: pContext->setValue(-1,Flag::CTRL_ABILITY); break;
			}
		}
	}
	ef(GUI::IsCancel(pButton))
	{// キャンセルされた
		// 今まで押されてたのをキャンセル
		GUI::CPanel* pBase = getBar(pContext->getValue(Flag::TARGET_ABILITY));
		if(pBase!=NULL)	pBase->getWidget("SHORTBAR")->setState(GUI::CButton::NORMAL);

		pContext->setValue(-1,Flag::TARGET_ABILITY);
		pRemain_->setNum(pRemain_->getNum() + nFP_);
		nFP_=0;
		// 残FPによって色替え
		pRemain_->validNumGui(pRemain_->getNum()>=0 ? 0/*白*/ : 1/*赤*/);
	}
	// OKボタンの処理
	validOK(pOK_,pContext);
}

namespace{

__inline void addSkill(int nAbility, int nHaveAbility, int nAttr, BMW::Chara::CDataCharaInter* inter)
{
	if(nHaveAbility>=0
	&& inter->hasSkillAcqu(nAbility)<0) // 取得したい技能を持っていない
	{// 上書き
		int nLv;
		if(nHaveAbility==Ability::SPUP)
		{// SPUPの上書きなら、その分減らす
			nLv = inter->hasSkillAcqu(Ability::SPUP);
			inter->setSP(inter->getMaxSP() - 6*nLv);
		}
		ef(nHaveAbility==Ability::MOVE_UP)
		{// MOVE_UPの上書きなら、その分減らす
			nLv = inter->hasSkillAcqu(Ability::MOVE_UP);
			inter->setMove(inter->getSourceMove() - 1*nLv); 
			inter->setJump(inter->getSourceJump() - 2*nLv); 
		}
		inter->addSkillAcqu(nHaveAbility, nAbility, nAttr);
	}
	else // 空きがあるのでただ追加
	{	inter->addSkillAcqu(nAbility, nAttr); }

	// 養成したのが、SPアップ・移動力だとパラメタが直に変わるのでそれを変更
	// また、どうせ1Lvのアップなのでそれだけ増やす
	if(nAbility==Ability::SPUP)
	{// SPを6増やす
		inter->setSP(inter->getMaxSP() + 6);
	}
	ef(nAbility==Ability::MOVE_UP)
	{// Move+1 Jump+2
		inter->setMove(inter->getSourceMove() + 1);
		inter->setJump(inter->getSourceJump() + 2);
	}
}

__inline bool IsChild(CInterContext& context, int nChild, const string& sParent)
{
	return context.getApp()->getChara().IsChild(nChild,BMW::Chara::Const::charaID_.getValue(sParent));
}
__inline void metamorSkill(int nAbility, int nHaveAbility, int nAttr, int nCharaID, CInterContext* p)
{// 変身対応キャラだったら、そいつのinterを取得しaddSkill
	if(IsChild(*p,nCharaID,"PLAYER_ARC_FTS"))
	{// ファンタズムーンの養成だったら、エクリプスがいたらそっちにも適用
		CInterChara* pChara = p->getCharaData(BMW::Chara::Const::charaID_.getValue("PLAYER_ECLIPS"));
		if(pChara!=NULL) addSkill(nAbility, nHaveAbility, nAttr, pChara->getData());
		// アルクは？
	}
	ef(IsChild(*p,nCharaID,"PLAYER_ECLIPS"))
	{// エクリプスだったら、ファンタズムーンは？
		CInterChara* pChara = p->getCharaData(BMW::Chara::Const::charaID_.getValue("PLAYER_ARC_FTS"));
		if(pChara!=NULL) addSkill(nAbility, nHaveAbility, nAttr, pChara->getData());
		// アルクは？
	}
	// アルクだったら？
	ef(IsChild(*p,nCharaID,"PLAYER_RIN_KALEIDO"))
	{// 凛だったら、カレイドは？
		CInterChara* pChara = p->getCharaData(BMW::Chara::Const::charaID_.getValue("PLAYER_KALEIDO"));
		if(pChara!=NULL) addSkill(nAbility, nHaveAbility, nAttr, pChara->getData());
	}
	ef(IsChild(*p,nCharaID,"PLAYER_KALEIDO"))
	{// カレイドだったら、凛は？
		CInterChara* pChara = p->getCharaData(BMW::Chara::Const::charaID_.getValue("PLAYER_RIN_KALEIDO"));
		if(pChara!=NULL) addSkill(nAbility, nHaveAbility, nAttr, pChara->getData());
	}
	ef(IsChild(*p,nCharaID,"PLAYER_KOHAKU_2"))
	{// 琥珀だったら、アンバーは？
		CInterChara* pChara = p->getCharaData(BMW::Chara::Const::charaID_.getValue("PLAYER_AMBER_2"));
		if(pChara!=NULL) addSkill(nAbility, nHaveAbility, nAttr, pChara->getData());
	}
	ef(IsChild(*p,nCharaID,"PLAYER_KOHAKU"))
	{// 琥珀だったら、アンバーは？
		CInterChara* pChara = p->getCharaData(BMW::Chara::Const::charaID_.getValue("PLAYER_AMBER"));
		if(pChara!=NULL) addSkill(nAbility, nHaveAbility, nAttr, pChara->getData());
	}
	ef(IsChild(*p,nCharaID,"PLAYER_AMBER_2"))
	{// アンバーだったら、琥珀は？
		CInterChara* pChara = p->getCharaData(BMW::Chara::Const::charaID_.getValue("PLAYER_KOHAKU_2"));
		if(pChara!=NULL) addSkill(nAbility, nHaveAbility, nAttr, pChara->getData());
	}
	ef(IsChild(*p,nCharaID,"PLAYER_AMBER"))
	{// アンバーだったら、琥珀は？
		CInterChara* pChara = p->getCharaData(BMW::Chara::Const::charaID_.getValue("PLAYER_KOHAKU"));
		if(pChara!=NULL) addSkill(nAbility, nHaveAbility, nAttr, pChara->getData());
	}
	ef(IsChild(*p,nCharaID,"PLAYER_SABER_AVALON"))
	{// セイバーだったら、リリィは？
		CInterChara* pChara = p->getCharaData(BMW::Chara::Const::charaID_.getValue("PLAYER_LILY_AVALON"));
		if(pChara!=NULL) addSkill(nAbility, nHaveAbility, nAttr, pChara->getData());
	}
	ef(IsChild(*p,nCharaID,"PLAYER_SABER_EX"))
	{// セイバーだったら、リリィは？
		CInterChara* pChara = p->getCharaData(BMW::Chara::Const::charaID_.getValue("PLAYER_LILY_EX"));
		if(pChara!=NULL) addSkill(nAbility, nHaveAbility, nAttr, pChara->getData());
	}
	ef(IsChild(*p,nCharaID,"PLAYER_SABER"))
	{// セイバーだったら、リリィは？
		CInterChara* pChara = p->getCharaData(BMW::Chara::Const::charaID_.getValue("PLAYER_LILY"));
		if(pChara!=NULL) addSkill(nAbility, nHaveAbility, nAttr, pChara->getData());
	}
	ef(IsChild(*p,nCharaID,"PLAYER_LILY_AVALON"))
	{// リリィだったら、セイバーは？
		CInterChara* pChara = p->getCharaData(BMW::Chara::Const::charaID_.getValue("PLAYER_SABER_AVALON"));
		if(pChara!=NULL) addSkill(nAbility, nHaveAbility, nAttr, pChara->getData());
	}
	ef(IsChild(*p,nCharaID,"PLAYER_LILY_EX"))
	{// リリィだったら、セイバーは？
		CInterChara* pChara = p->getCharaData(BMW::Chara::Const::charaID_.getValue("PLAYER_SABER_EX"));
		if(pChara!=NULL) addSkill(nAbility, nHaveAbility, nAttr, pChara->getData());
	}
	ef(IsChild(*p,nCharaID,"PLAYER_LILY"))
	{// リリィだったら、セイバーは？
		CInterChara* pChara = p->getCharaData(BMW::Chara::Const::charaID_.getValue("PLAYER_SABER"));
		if(pChara!=NULL) addSkill(nAbility, nHaveAbility, nAttr, pChara->getData());
	}
}

} // namespace end

void CTrainSkill::eventOK(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 決定！
		if(pRemain_->getNum()>=0
		&& pContext->getValue(Flag::CTRL_ABILITY)>=0
		&& pContext->getValue(Flag::TARGET_ABILITY)>=0)
		{// 状況が整っている
			CInterContext* p = static_cast<CInterContext*>(pContext);
			CInterChara* pChara = p->getTargetCharaData();
			BMW::Chara::CDataCharaInter* inter = pChara->getData();
			// 獲得対象
			int nTarget = p->getTargetAbility();
			// 設定スロットの状態にあわせて、処理がちょっと分岐
			int nAbility = get2Ability(nTarget);
			int nHaveAbility = nHaveSkill_[p->getCtrlAbility()];
			int nAttr = getAttr(nTarget);
			// 技能追加
			addSkill(nAbility, nHaveAbility, nAttr, inter);

			// 変身対応キャラだったら、そいつのinterを取得しaddSkill
			metamorSkill(nAbility, nHaveAbility, nAttr, inter->getID(), p);

			// セーブデータへ反映
			//p->getApp()->getExec().getTrainData(pChara->getData()->getID(),false)->back(pChara->getData());
			// FPを減らす
			p->getApp()->getExec().setFP(pRemain_->getNum());
			// ヘッダへ反映
			fun_(CChara::S_HEAD,p);
			// 各種ステータスへの反映
			fun_(CChara::S_BASE,p);
			fun_(CChara::S_FP,p);

			if(nAbility==Ability::SPUP
			|| nHaveAbility==Ability::SPUP)
				fun_(CChara::S_EASY,p);
			if(nAbility==Ability::MOVE_UP
			|| nHaveAbility==Ability::MOVE_UP)
				fun_(CChara::S_BATTLE,p);
			if(nAbility==Ability::MAGICSAVE
			|| nAbility==Ability::BALLETSAVE
			|| nHaveAbility==Ability::MAGICSAVE
			|| nHaveAbility==Ability::BALLETSAVE)
				fun_(CChara::S_WEAPON,p);
			// 状況設定し直し
			OnReset(p);
			// ポップアップクリア
			pContext->getApp()->getFoward()->clearPopUp();
		}
	}
}

///////////////////////////////////////////////////////////
// 獲得技能処理
///////////////////////////////////////////////////////////
GUI::CPanel* CTrainSkill::getBar(int nID)
{
	if(nID<7)	return pGet1_->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("ROW",nID+1));
	ef(nID<14)	return pGet2_->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("ROW",nID+1-7));
	else		return pGet3_->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("ROW",nID+1-14));
}

void CTrainSkill::invalidGet(const BMW::Chara::CStatusAbility& skill, Ability::CAbilityDB& db, bool bTalent)
{
	if(IsLvSkill(skill.getID()))
	{// LV制スキルや獲得できないスキル
		// とりあえず、LVを加算しておく
		// 先天技能の場合は加算しない
		if(!bTalent) calcLv(skill.getID(),skill.getAttr());
	}
	else
	{// そうじゃなければ重複してとれない
		// IDから属する列を取得
		int nID = ability2Get(skill.getID());
		GUI::CPanel* pBar = getBar(nID);
		// ボタン動作を停止し、数字の表示を赤にする
		Task::ITaskBase* pButton;
		pButton = pBar->getWidget("SHORTBAR");
		pButton->setState(GUI::CButton::NORMAL);
		pButton->valid(false);
		//pBar->getWidgetCast<GUI::CNumCtrl>("SKILLPOINT")->validNumGui(1/*赤*/);
		pBar->getWidget("SKILLPOINT")->visible(false);
	}
}

void CTrainSkill::updateLvSkill(const BMW::Chara::CDataCharaInter& chara, Ability::CAbilityDB& db)
{
	GUI::CPanel* pBar;
	bool bCan; // 取得可能フラグ
	// 底力
	pBar=getBar(POWER);
	// 取得できるか？
	bCan = nPower_<=chara.IsSkillValid(BMW::Chara::CValidSkill::FUNDPOWER);
	// ↑にあわせて設定
	updateSkillBar(pBar, Ability::FUNDPOWER, bCan?nPower_:-1, bCan, db, true);

	// カウンター
	pBar=getBar(COUNTER);
	// 取得できるか？
	bCan = nCounter_<=chara.IsSkillValid(BMW::Chara::CValidSkill::COUNTER);
	// ↑にあわせて設定
	updateSkillBar(pBar, Ability::COUNTER, bCan?nCounter_:-1, bCan, db, true);

	// 移動力アップ
	pBar=getBar(MOVE_UP);
	// 取得できるか？
	bCan = nMoveUp_<=chara.IsSkillValid(BMW::Chara::CValidSkill::MOVE_UP);
	// ↑にあわせて設定
	updateSkillBar(pBar, Ability::MOVE_UP, bCan?nMoveUp_:-1, bCan, db, true);

	// 援護攻撃
	pBar=getBar(BACKUP_ATT);
	// 取得できるか？
	bCan = nBackAtk_<=chara.IsSkillValid(BMW::Chara::CValidSkill::BACKUPATTACK);
	// ↑にあわせて設定
	updateSkillBar(pBar, Ability::BACKUPATTACK, bCan?nBackAtk_:-1, bCan, db, true);

	// 援護防御
	pBar=getBar(BACKUP_DEF);
	// 取得できるか？
	bCan = nBackDef_<=chara.IsSkillValid(BMW::Chara::CValidSkill::BACKUPDEFENCE);
	// ↑にあわせて設定
	updateSkillBar(pBar, Ability::BACKUPDEFENCE, bCan?nBackDef_:-1, bCan, db, true);

	// SPUP
	pBar=getBar(SPUP);
	// 取得できるか？
	bCan = nSpUp_<=chara.IsSkillValid(BMW::Chara::CValidSkill::SPUP);
	// ↑にあわせて設定
	updateSkillBar(pBar, Ability::SPUP, bCan?nSpUp_:-1, bCan, db, true);
}


} // namespace Chara end
} // namespace Inter end
} // namespace BMW end