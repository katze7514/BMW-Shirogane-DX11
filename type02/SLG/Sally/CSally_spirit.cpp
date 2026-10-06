#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "../../Scene/GUI/CNumCtrl.h"
#include "../../Scene/GUI/CNumRemain.h"
#include "../../Scene/GUI/CGage.h"

#include "../../Spirit/IDSpirit.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/CMapSymbolDB.h"

#include "CSally_spirit.h"

namespace BMW{
namespace SLG{
namespace Sally{

void CSally_spirit::OnReset(Task::CTaskContext* pContext)
{
	GUI::CGuiDefDB& db = pContext->getScene()->getGuiDefDB();
	// インターフェイス
	pPanel_ = db.createInterfaceCast<GUI::CPanel>("PANEL_KENSAKU");
	addTask(pPanel_,INTERFACE);

	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CSally_spirit::eventButton);
	// ボタン設定
	pChange_ = pPanel_->getWidgetRecCast<GUI::CButton>("CHANGE/BUTTON");
	GUI::CButton::setButtonEvent(pChange_,fun,CHANGE);
	// 数字
	pPage_ = pPanel_->getWidgetRecCast<GUI::INum>("CHANGE/PAGE");
	// 精神設定
	initSpirit(pPanel_->getWidgetCast<GUI::CPanel>("COMMAND"),static_cast<CSLGContext*>(pContext));
}

void CSally_spirit::OnInit(Task::CTaskContext* pContext)
{
	// とりあえず、キャラは出さない
	if(pCharaPanel_!=NULL)
	{
		pCharaPanel_->valid(false);
		pCharaPanel_->visible(false);
	}
	pChange_->valid(false);
	pChange_->visible(false);
	pPage_->visible(false);

	pContext->getInput()->guard(false);
	setState(NORMAL);
}

namespace{
__inline bool releaseCancel(Task::CTaskContext* pContext)
{
	using Input::IInput;
	return pContext->getInput()->getInputState(IInput::CANCEL)==IInput::RELEASE;
}
} // namespace end

void CSally_spirit::OnAction(Task::CTaskContext* pContext)
{
	if(getState()==NORMAL)
	{
		if(releaseCancel(pContext))
		{// キャンセルされた
			pContext->push(-1);
			setState(END);
		}
	}
	ef(getState()==END)
	{
		pContext->getInput()->guard(true);
		getTaskListCtrl()->returnTaskList();
		pContext->getApp()->getFoward()->clearPopUp();
	}
}

////////////////////////////////////////////////
// イベントリスナー
////////////////////////////////////////////////
void CSally_spirit::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{
		if(pButton->getValue()==CHANGE)
		{
			int nPage = pPage_->getNum();
			if(++nPage>nDiv_) nPage=1;
			pPage_->setNum(nPage);
			static_cast<GUI::CPanelCtrl*>(pCharaPanel_)->validWidget(nPage-1);
		}
		else
		{// 精神ボタンが押された
			createChipPanel(pButton->getValue(), static_cast<CSLGContext*>(pContext));
		}
	}
}

void CSally_spirit::eventChara(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// キャラが選択された
		pContext->push(pButton->getValue());
		setState(END);
	}
}

/////////////////////////////////////////////////
// キャラインターフェイス生成
/////////////////////////////////////////////////
namespace{
#pragma warning(disable:4512) // 代入演算子作れね
//////////////////////////////////////
// 精神コマンド持ってない
//////////////////////////////////////
struct IsNoHasSpirit : public unary_function<int,bool>
{
	bool operator() (int nID)
	{
		CDataCharaSLG* pChara = p_.getCharaData(nID);
		return pChara==NULL 
			|| !pChara->IsExist()
			|| !pChara->getBattle().IsHasSpirit(nSpirit_);
	}

	IsNoHasSpirit(int nSpirit, CSLGContext& p):nSpirit_(nSpirit),p_(p){}
	CSLGContext& p_;
	int nSpirit_;
};

////////////////////////////////////
// SP昇順
////////////////////////////////////
struct sort_SpUp2 : public binary_function<int, int, bool>
{
	sort_SpUp2(CSLGContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst, int nSecond)
	{
		return mapChara.find(nFirst)->second->getBattle().getSP() < mapChara.find(nSecond)->second->getBattle().getSP();
	}

	CSLGContext::chara_map& mapChara;
};
#pragma warning(default:4512) // 代入演算子作れね
////////////////////////////////////
// SPキャラ設定
////////////////////////////////////
void setCharaSpChip(GUI::CPanel* pPanel, CDataCharaSLG& chara, int nSP)
{
	GUI::CGage* pGage = pPanel->getWidgetCast<GUI::CGage>("GAGE");
	pGage->getSlashGui()->visible(false);
	pGage->getMaxNumGui()->visible(false);
	pGage->actionChangeNum(chara.getBattle().getSP(),chara.getBattle().getMaxSP());
	// 消費SPの方が高かったら赤文字
	pGage->getRemain()->getCurrentNumGui()->validNumGui(nSP>chara.getBattle().getSP() ? GUI::CNumRemain::RED : GUI::CNumRemain::WHITE);
}

} // namespace end

void CSally_spirit::setCharaChip(int nID, int nPos, int nSpirit, const GUI::CButton::ButtonEvent& fun, GUI::CPanel* pPanel, CSLGContext* p)
{
	CDataCharaSLG* pChara;
	GUI::CPanel* pChip;
	GUI::CButtonSymbol* pButton;
	GUI::CPanel* pSort;
	
	pChara = p->getCharaData(nID);
	pChip = pPanel->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("CHARASP",nPos));
	pButton = pChip->getWidgetCast<GUI::CButtonSymbol>("CHIP");
	pChara->getMapSymbol()->setButtonGui(pButton,pChara->getState().getAct()!=Act::BEFORE?"BUTTON_SLG":"BUTTON_INTER");			
	pSort = pChip->getWidgetCast<GUI::CPanel>("SP_CHARA");
	GUI::CButton::setButtonEvent(pButton, fun, pChara->getID());
	// 消費SP計算
	int nSP = pChara->getBattle().hasSpirit(nSpirit);
	if(pChara->getBattle().hasSkill(Ability::CONCENT)>=0) nSP=(nSP*4)/5;
	// チップ設定
	setCharaSpChip(pSort,*pChara,nSP);
	// POPUP設定
	pButton->setPopUp("消費SP："+ CStringScanner::NumToString(nSP));
}

void CSally_spirit::createChipPanel(int nSpirit, CSLGContext* p)
{
	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CSally_spirit::eventChara);

	// 持っているキャラだけを取得
	list<int> listChara = p->getPlayerPhaseList();
	listChara.remove_if(IsNoHasSpirit(nSpirit,*p));

	if(listChara.empty())
	{// 空だったら
		if(pCharaPanel_!=NULL)
		{
			pCharaPanel_->valid(false);
			pCharaPanel_->visible(false);
		}
		pPage_->visible(false);
		pChange_->valid(false);
		pChange_->visible(false);
		return;
	}

	// 残りSPでソート
	listChara.sort(sort_SpUp2(p->getCharaMap()));
	
	// 必要なキャラ数に合わせて選択パネルを生成
	int nSize = (int)listChara.size();
	nDiv_ = (int)ceil((double)nSize / 5.0);

	GUI::CPanel* pPanel;
	list<int>::iterator it;
	int nPos=1;
	if(nDiv_==1)
	{// 1枚ならCtrlじゃなくて、Panel一枚で生成
		// CHANGEボタン使用しない
		pChange_->valid(false);
		pChange_->visible(false);
		pPage_->visible(false);
		// とりあえず、ひな形生成
		pPanel = p->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("CHARASP_5");
		pCharaPanel_ = pPanel;

		// パネル設定ループ
		for(it=listChara.begin(); it!=listChara.end(); ++it)
			setCharaChip(*it,nPos++,nSpirit,fun,pPanel,p);
		
	}
	else
	{// 2枚以上なら、コントローラで生成
		// CHANGEボタン使用する
		pChange_->valid(true);
		pChange_->visible(true);
		pPage_->visible(true);

		GUI::CPanelCtrl* pCtrl = new GUI::CPanelCtrl();
		pCharaPanel_ = pCtrl;

		int nDiv=0;
		pPanel = p->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("CHARASP_5");
		pCtrl->addWidget(pPanel, CStringScanner::NumToString(nDiv++));

		for(it=listChara.begin(); it!=listChara.end(); ++it)
		{// パネル設定ループ
			setCharaChip(*it,nPos++,nSpirit,fun,pPanel,p);

			if(nPos>5)
			{// 5個越えたら次
				nPos=1;
				pPanel = p->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("CHARASP_5");	
				pCtrl->addWidget(pPanel, CStringScanner::NumToString(nDiv++));
			}
		}
		// とりあえず、1を表示
		pPage_->setNum(1);
		pCtrl->validWidget(0);
	}

	// 最後のパネルの余った部分のタスクは余計なので、削除してしまう
	for(int i=nPos; i<=5; i++)
		pPanel->delWidget(Misc::linkStrAndNum("CHARASP",i));

	// パネルを設定
	pPanel_->swapWidget(pCharaPanel_,"CHARASP_5");
}

void CSally_spirit::initSpirit(GUI::CPanel* pPanel, CSLGContext* p)
{
	Spirit::CSpiritDB& db = p->getApp()->getSpirit();
	GUI::CButton::ButtonEvent fun(this,&CSally_spirit::eventButton);
	GUI::CButtonSymbol* pButton;
	// 精神ボタン設定
	// 奇跡
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("KISEKI");
	db.setButtonHolder(pButton, Spirit::MIRACLE, fun, Spirit::MIRACLE);
	// 挑発
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("CHOHATSU");
	db.setButtonHolder(pButton, Spirit::PROVO, fun, Spirit::PROVO);
	// 覚醒
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("KAKUSEI");
	db.setButtonHolder(pButton, Spirit::AWAKE, fun, Spirit::AWAKE);
	// 再動
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("SAIDOU");
	db.setButtonHolder(pButton, Spirit::AGAIN, fun, Spirit::AGAIN);
	// 脱力
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("DATSURYOKU");
	db.setButtonHolder(pButton, Spirit::WEAK, fun, Spirit::WEAK);
	// 激励
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("GEKIREI");
	db.setButtonHolder(pButton, Spirit::ENCOURAGE, fun, Spirit::ENCOURAGE);
	// 気合
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("KIAI");
	db.setButtonHolder(pButton, Spirit::POWER, fun, Spirit::POWER);
	// 祈り
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("INORI");
	db.setButtonHolder(pButton, Spirit::PRAY, fun, Spirit::PRAY);
	// 信念
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("SHINNEN");
	db.setButtonHolder(pButton, Spirit::FAITH, fun, Spirit::FAITH);
	// 偵察
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("TEISATSU");
	db.setButtonHolder(pButton, Spirit::SPY, fun, Spirit::SPY);
	// 跳躍
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("CHOYAKU");
	db.setButtonHolder(pButton, Spirit::JUMP, fun, Spirit::JUMP);
	// 加速
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("KASOKU");
	db.setButtonHolder(pButton, Spirit::ACC, fun, Spirit::ACC);
	// 熱血
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("NEKKETSU");
	db.setButtonHolder(pButton, Spirit::FIREBALL, fun, Spirit::FIREBALL);
	// 魂
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("TAMASHII");
	db.setButtonHolder(pButton, Spirit::SPIRIT, fun, Spirit::SPIRIT);
	// 集中
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("SYUUTYUU");
	db.setButtonHolder(pButton, Spirit::CONCENT, fun, Spirit::CONCENT);
	// 必中
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("HITTYUU");
	db.setButtonHolder(pButton, Spirit::HIT, fun, Spirit::HIT);
	// 感応
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("KANNOU");
	db.setButtonHolder(pButton, Spirit::SYNC, fun, Spirit::SYNC);
	// ひらめき
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("HIRAMEKI");
	db.setButtonHolder(pButton, Spirit::AVOID, fun, Spirit::AVOID);
	// 狙撃
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("SOGEKI");
	db.setButtonHolder(pButton, Spirit::SNIPE, fun, Spirit::SNIPE);
	// 直撃
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("CHOKUGEKI");
	db.setButtonHolder(pButton, Spirit::DIRECT, fun, Spirit::DIRECT);
	// 突撃
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("TOTSUGEKI");
	db.setButtonHolder(pButton, Spirit::CHARGE, fun, Spirit::CHARGE);
	// 手加減
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("TEKAGEN");
	db.setButtonHolder(pButton, Spirit::EASYON, fun, Spirit::EASYON);
	// 不屈
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("FUKUTSU");
	db.setButtonHolder(pButton, Spirit::TOUGH, fun, Spirit::TOUGH);
	// 鉄壁
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("TEPPEKI");
	db.setButtonHolder(pButton, Spirit::DEFENCE, fun, Spirit::DEFENCE);
	// 補給
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("HOKYUU");
	db.setButtonHolder(pButton, Spirit::SUPPLY, fun, Spirit::SUPPLY);
	// 信頼
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("SHINRAI");
	db.setButtonHolder(pButton, Spirit::TRUST, fun, Spirit::TRUST);
	// 友情
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("YUJOU");
	db.setButtonHolder(pButton, Spirit::FRIEND, fun, Spirit::FRIEND);
	// 根性
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("KONJO");
	db.setButtonHolder(pButton, Spirit::GUTS, fun, Spirit::GUTS);
	// 期待
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("KITAI");
	db.setButtonHolder(pButton, Spirit::HOPE, fun, Spirit::HOPE);
	// ド根性
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("DOKONJO");
	db.setButtonHolder(pButton, Spirit::VERYGUTS, fun, Spirit::VERYGUTS);
	// 努力
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("DORYOKU");
	db.setButtonHolder(pButton, Spirit::EFFORT, fun, Spirit::EFFORT);
	// 応援
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("OUEN");
	db.setButtonHolder(pButton, Spirit::CHEER, fun, Spirit::CHEER);
	// 祝福
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("SYUKUFUKU");
	db.setButtonHolder(pButton, Spirit::BLESS, fun, Spirit::BLESS);
	// 幸運
	pButton = pPanel->getWidgetCast<GUI::CButtonSymbol>("KOUUN");
	db.setButtonHolder(pButton, Spirit::FORTUNE, fun, Spirit::FORTUNE);
}

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end