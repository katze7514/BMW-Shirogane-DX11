/*
	SLG・Inter共通の関係の設定子定義
*/

#include "stdafx.h"

#include "../GUI/DB/CGuiDefDB.h"
#include "../Spirit/ConstSpirit.h"
#include "../Item/ConstItem.h"
#include "../Chara/CDataCharaBase.h"
#include "../Weapon/CalcWeapon.h"
#include "../Weapon/CDataWeaponBattle.h"

#include "../SLG/IDSLG.h"

#include "status_fun.h"

namespace BMW{
namespace Status{
///////////////////////////////////////////////////
// クリアヘッダ
///////////////////////////////////////////////////
void setClearHeaderBase(GUI::CPanel* pPanel, Task::CTaskContext& p, bool bInter)
{
	const smart_ptr<Scenario::CDataScenario>& pData = p.getScenarioData();
	GUI::CPanelCtrl* pCtrl;
	// 話数
	pCtrl = pPanel->getWidgetCast<GUI::CPanelCtrl>("CHAPTER_NUMBER");
	int nNo = pData->getNo();
	pCtrl->validWidget(nNo<10?"1_DIGIT":"2_DIGIT");
	pCtrl->getValidWidgetCast<GUI::INum>()->setNum(nNo);

	// 話タイトル
	GUI::CText* pText = pPanel->getWidgetCast<GUI::CText>("CHAPTER_TITLE");
	pText->setText("『" + pData->getTitle() + "』");
	pText->getFontConf().SetWeight(700);
	pText->UpdateTextAA();

	// 難易度表示
	pCtrl = pPanel->getWidgetCast<GUI::CPanelCtrl>("GAMELEVEL");
	int nRank;
	// セーブデータのVICTORYフラグに寄る
	nRank = p.getApp()->getExec().getFlag("VICTORY",1) ? SLG::Expert::HARD : SLG::Expert::NORMAL;
	pCtrl->validWidget(nRank);

	// 敵養成段階
	// 周回プレイ時のみ有効
	Save::CExecData& exec = p.getApp()->getExec();
	bool bTekiTrain = exec.getFlag("HANDOVER",1) || exec.getFlag("HANDOVER",2);

	pText = pPanel->getWidgetCast<GUI::CText>("TEKI_YOUSEI");
	pText->visible(bTekiTrain);

	pText = pPanel->getWidgetCast<GUI::CText>("TEKI_YOUSEI_NUM");
	pText->visible(bTekiTrain);
	if(bTekiTrain)
	{
		int nTrain=0;
		exec.getFlag(Scene::Const::flagID_.getValue("ENEMY_TRAIN"),nTrain);
		pText->setText(CStringScanner::NumToString(nTrain));
		pText->UpdateTextAA();
	}
}
///////////////////////////////////////////////////
// 基本ステータス
///////////////////////////////////////////////////
void addLvStr(const Chara::CStatusAbility& skill, string& sName)
{
	addLvStr(skill.getID(),skill.getAttr(),sName);
}

void addLvStr(int nID, int nAttr, string& sName)
{
	switch(nID)
	{
	case Ability::SPECTER:
	case Ability::MAGICIAN:
	case Ability::VAMPIRE:
	case Ability::COUNTER:
	case Ability::BACKUPATTACK:
	case Ability::BACKUPDEFENCE:
	case Ability::FUNDPOWER:
	case Ability::SPUP:
	case Ability::MOVE_UP:
		if(nAttr>0) sName += CStringScanner::NumToString(nAttr);
	break;

	default: break;
	}
}

void setStatusBasicFund(GUI::CPanel* pPanel, const Chara::CDataCharaBase& chara)
{// 基礎能力
	// 腕力
	pPanel->getWidgetCast<GUI::INum>("STR")->setNum(chara.getStrength());
	// 魔力
	pPanel->getWidgetCast<GUI::INum>("MGC")->setNum(chara.getMagic());
	// 命中
	pPanel->getWidgetCast<GUI::INum>("HIT")->setNum(chara.getHit());
	// 回避
	pPanel->getWidgetCast<GUI::INum>("AVD")->setNum(chara.getAvoid());
	// 防御
	pPanel->getWidgetCast<GUI::INum>("DEF")->setNum(chara.getDefence());
	// 技量
	pPanel->getWidgetCast<GUI::INum>("SKL")->setNum(chara.getSkill());
}

void setStatusBasicTalent(GUI::CPanel* pPanel, const Chara::CDataCharaBase& chara, Task::CTaskContext& p)
{// 固有能力
	pPanel->validAll(false);
	pPanel->visibleAll(false);

	int nCount=0;
	Ability::CAbilityDB& aDB = p.getApp()->getAbility();
	GUI::CTextPopUp* pText;
	chara.beginTalent();
	while(!chara.endTalent())
	{
		const Chara::CStatusAbility& talent= *chara.nextTalent();
		pText = pPanel->getWidgetCast<GUI::CTextPopUp>(nCount++);
		aDB.setTextPopUpHolder(pText,talent.getID());	
		pText->UpdateTextAA();
		pText->valid(true);
		pText->visible(true);
	}
}

void setStatusBasicSpirit(GUI::CPanel* pPanel, const Chara::CDataCharaBase& chara, Task::CTaskContext& p)
{// 精神
	pPanel->validAll(false);
	pPanel->visibleAll(false);
	Spirit::CSpiritDB& sDB = p.getApp()->getSpirit();
	bool bConcent = chara.hasSkill(Ability::CONCENT)>=0;
	
	GUI::CPanel*		pSpirit;
	GUI::CGraphicPopUp*	pIcon;
	GUI::CNum*			pSpend;
	GUI::CText*			pName;

	for(int i=0; i<6; i++)
	{
		const Chara::CStatusAbility& spirit= chara.getSpirit(i);
		pSpirit = pPanel->getWidgetCast<GUI::CPanel>(i);
		pSpirit->visible(true);

		if(spirit.getID()<0)
		{// 精神ねー
			// アイコン
			pIcon = pSpirit->getWidgetCast<GUI::CGraphicPopUp>("ICON");
			pIcon->visible(false);
			// 消費SP
			pSpend = pSpirit->getWidgetCast<GUI::CNum>("SP_SPEND");
			pSpend->visible(false);
		}
		else
		{// あるでー
			pSpirit->valid(true);
			// アイコン
			pIcon = pSpirit->getWidgetCast<GUI::CGraphicPopUp>("ICON");
			sDB.setGraphicHolder(pIcon, spirit.getID());
			pIcon->visible(true);
			// 消費SP
			pSpend = pSpirit->getWidgetCast<GUI::CNum>("SP_SPEND");
			int nSP = bConcent ? (spirit.getAttr()*4/5) : spirit.getAttr();
			if(nSP<=0) nSP=1;
			pSpend->setNum(nSP);
			pSpend->visible(true);
		}
		// 名前
		pName = pSpirit->getWidgetCast<GUI::CText>("SPIRITNAME");
		pName->setText(Spirit::Const::spiritName_.getValue(spirit.getID()));
		pName->UpdateTextAA();
	}
}

void setStatusBasicItem(GUI::CPanel* pPanel, const Chara::CDataCharaBase& chara, Task::CTaskContext& p)
{// アイテム
	pPanel->validAll(false);
	pPanel->visibleAll(false);

	Item::CItemDB& iDB = p.getApp()->getItem();
	
	int nCount=0;
	GUI::CGraphicPopUp*	pIcon;
	
	chara.sortItem();
	chara.beginItem();
	while(!chara.endItem())
	{
		const Chara::CStatusAbility& item = *chara.nextItem();
		pIcon = pPanel->getWidgetCast<GUI::CGraphicPopUp>(nCount++);
		pIcon->valid(true);
		pIcon->visible(true);
		// アイコン
		iDB.setGraphicHolder(pIcon, item.getID());
	}
}

//////////////////////////////////////////////////////
// 武器ステータス
//////////////////////////////////////////////////////
namespace{
__inline void setRange(GUI::CPanelCtrl* pPanel, int nMin, int nMax)
{
	if(nMin==nMax)
	{// 最小と最大が同じ時
		pPanel->validWidget("RANGE1");
		pPanel->getValidWidgetCast<GUI::INum>()->setNum(nMin);
	}
	else
	{// そうで無いとき
		pPanel->validWidget(nMax<10?"RANGE1_1":"RANGE1_2");
		GUI::CPanel* pPanel2 = pPanel->getValidWidgetCast<GUI::CPanel>();
		// 最小
		pPanel2->getWidgetCast<GUI::INum>("MIN")->setNum(nMin);
		// 最大
		pPanel2->getWidgetCast<GUI::INum>("MAX")->setNum(nMax);
	}
}

} // namespace end

void setWeaponIcon(GUI::CPanel* pPanel, const Weapon::CDataWeaponBattle& weapon)
{
	// アイコンの設定
	GUI::CPanelCtrl* pCtrl;
	pCtrl = pPanel->getWidgetCast<GUI::CPanelCtrl>("ICON");
	pCtrl->validWidget(Weapon::getIconID(weapon.getKind(),pCtrl) + weapon.getRank());
	pCtrl->valid(false); // ボタン動作はしない
	// 属性の設定
	pCtrl = pPanel->getWidgetCast<GUI::CPanelCtrl>("PMT");
	pCtrl->validWidget(Weapon::getAttrID(weapon.IsP(),
										  weapon.IsM(),
										  weapon.IsT(),
										  weapon.IsF(),
										  pCtrl));
	// ペケはもちろん非表示
	pPanel->getWidget("PEKE")->visible(false);
}

void setStatusWeaponLine(GUI::CPanel* pPanel, const Weapon::CDataWeaponBattle& weapon, bool bIcon)
{// 武器ライン
	// アイコン設定
	if(bIcon) setWeaponIcon(pPanel->getWidgetCast<GUI::CPanel>("ICON"),weapon);
	// 武器名
	GUI::CText* pText = pPanel->getWidgetCast<GUI::CText>("WEAPONNAME");
	pText->setText(weapon.getName());
	if(!bIcon) pText->getFontConf().SetWeight(700);
	pText->UpdateTextAA();
	// 攻撃力
	pPanel->getWidgetCast<GUI::INum>("ATK")->setNum(weapon.getAttack());
	// 中心射程
	setRange(pPanel->getWidgetCast<GUI::CPanelCtrl>("CORE_RANGE"),weapon.getCoreMin(), weapon.getCoreMax());
	// 全射程
	setRange(pPanel->getWidgetCast<GUI::CPanelCtrl>("ALL_RANGE"),weapon.getMin(), weapon.getMax());
	// 到達
	pPanel->getWidgetCast<GUI::INum>("HEIGHT")->setNum(weapon.getHeight(false));
}

void setStatusWeaponTextCond(GUI::CText* pText, const Weapon::CDataWeaponBattle& weapon)
{
	string& s = pText->getText();

	// 状態変化系
	if(weapon.getCond()==Chara::CValidCond::ACTION)
	{
		s = CStringScanner::NumToString(weapon.getCondValue()) + "回行動不能にする\n";
	}
	else if(weapon.getCond()==Chara::CValidCond::EN)
	{
		s = "ENを" + CStringScanner::NumToString(weapon.getCondValue()) + "減少させる\n";
	}
	else if(weapon.getCond()==Chara::CValidCond::MENTAL)
	{
		s = "気力を" + CStringScanner::NumToString(-weapon.getCondValue()) + "減少させる\n";
	}
	else
	{
		s = CStringScanner::NumToString(weapon.getCondValue()) + "ターンの間\n";
		switch(weapon.getCond())
		{
		case Chara::CValidCond::MOVE:		s+="Moveを半分"; break;
		case Chara::CValidCond::DEFENCE:	s+="Toughを半分"; break;
		case Chara::CValidCond::HIT:		s+="命中率を半分"; break;
		case Chara::CValidCond::AVOID:		s+="回避率を半分"; break;
		}
		s+="にする\n";
	}
}

void appendTextStatus(const string& sText, int nValue, string& s, int& n)
{
	if(nValue<=0) return;
	if(n>0) s += "・";
	s += sText + CStringScanner::NumToString(nValue);
	if(++n>1)
	{	
		s += "\n";
		n=0;
	}
}

void setStatusWeaponTextStatus(GUI::CText* pText, const Weapon::CDataWeaponBattle& weapon)
{
	string& s = pText->getText();
	s.clear();
	int n=0;
	appendTextStatus("腕力+",weapon.getStrengthAid(),s,n);
	appendTextStatus("魔力+",weapon.getMagicAid(),s,n);
	appendTextStatus("命中+",weapon.getHitAid(),s,n);
	appendTextStatus("回避+",weapon.getAvoidAid(),s,n);
	appendTextStatus("防御+",weapon.getDefenceAid(),s,n);
	appendTextStatus("技量+",weapon.getSkillAid(),s,n);
	appendTextStatus("気力+",weapon.getMentalAid(),s,n);
		
}

void setStatusWeaponText(GUI::CText* pText, const Weapon::CDataWeaponBattle& weapon)
{
	pText->visible(true);
	pText->getFontConf().SetHeight(16);
	pText->getText().clear();

	// 行カウント
	int nLine=0;

	if(weapon.IsWeaponID("MOUSOU_SHINON"))
	{// 妄想心音
		pText->setText("この武器の攻撃は、\n攻撃対象のToughが半減扱いとなる");
		nLine+=2;
	}
	ef(weapon.IsWeaponID("FURAGA"))
	{// フラガラック
		pText->setText("反撃専用\nカウンター・クリティカルが必ず発動する\n相手の攻撃力が4500以上の場合、ダメージが1.5倍");
		nLine+=3;
	}
	ef(weapon.IsWeaponID("FURAGA_ENEMY"))
	{// フラガラック
		pText->setText("反撃専用\nカウンターが必ず発動する");
		nLine+=2;
	}
	ef(weapon.IsWeaponID("AVESTER"))
	{// アヴェスター
		pText->setText("反撃専用\n受けたダメージをそのまま返す\n特殊防御無視で必ず命中する");
		nLine+=3;
	}
	ef(weapon.IsWeaponID("AVESTER_ENEMY"))
	{// アヴェスター
		pText->setText("反撃専用\n受けたダメージをそのまま返す\n特殊防御無視で必ず命中する");
		nLine+=3;
	}
	ef(weapon.IsWeaponID("HIMAWARI"))
	{// 素敵衛星HIMAWARI
		pText->setText("すべての基礎能力+5\n");
		nLine+=1;
	}
	else
	{// それ以外
		switch(weapon.getKind())
		{
		case Weapon::Kind::FIGHT_COND:
		case Weapon::Kind::MAGIC_COND: // 状態変化
			setStatusWeaponTextCond(pText,weapon);
			nLine+=2;
		break;

		case Weapon::Kind::STATUS:	// ステータスアップ
			setStatusWeaponTextStatus(pText,weapon);
			nLine+=1;
		break;

		case Weapon::Kind::CURE:	// 治療
		{
			string& s = pText->getText();
			s = "現在のLvに応じて\n味方のHPを回復します\n";
			nLine+=2;
		}
		break;

		case Weapon::Kind::REFILL:	// 補給
		{
			string& s = pText->getText();
			s = "味方のEN・弾数を\n全回復します\n";
			nLine+=2;
		}
		break;

		default: pText->visible(false); break;
		}
	}

	// フィールド武器？
	if(weapon.IsF())
	{// フィールド武器
		string& s = pText->getText();
		switch(weapon.getField())
		{
		case Weapon::Field::CENTER:
			s+="自分中心型";
		break;

		case Weapon::Field::LINE:
			s+="ライン型";
		break;

		case Weapon::Field::THROW:
			s +="投げ込み型";
			if(0==nLine){ s += "\n"; nLine+=1; }
			else		{ s += "　"; }

			s +="投げ込み範囲：" + CStringScanner::NumToString(weapon.getFieldSize());
			nLine+=1;
		break;

		default: break;
		}
		
		if(0==nLine) s += "\n";
		else		 s += "　";
				
		if(weapon.IsFieldFriend())
		{// 味方認識
			if(weapon.getKind()!=Weapon::Kind::STATUS)
				s+= "味方にあたらない";
			else
				s+= "敵にあたらない";
		}

		pText->visible(true);
	}

	// テキスト化
	pText->UpdateTextAA();
}

void setStatusWeaponDetail(GUI::CPanel* pPanel, const Weapon::CDataWeaponBattle& weapon, int nEN, int nMental)
{// 詳細情報
	// 命中
	GUI::INum* pNum = pPanel->getWidgetCast<GUI::INum>("HIT");
	pNum->plus(true);
	pNum->setNum(weapon.getHit());
	// CT
	pNum = pPanel->getWidgetCast<GUI::INum>("CT");
	pNum->plus(true);
	pNum->setNum(weapon.getCT());
	// 特殊
	setStatusWeaponText(pPanel->getWidgetCast<GUI::CText>("SPECIAL_EF"),weapon);
	// 最大弾数
	pPanel->getWidgetCast<GUI::INum>("TAMA_MAX")->setNum(weapon.getBallet());
	// 残弾数
	pPanel->getWidgetCast<GUI::INum>("TAMA_NOW")->setNum(weapon.getBalletRest());
	// 現在EN
	pPanel->getWidgetCast<GUI::INum>("EN_NOW")->setNum(nEN);
	// 消費EN
	pPanel->getWidgetCast<GUI::INum>("EN_SPEND")->setNum(weapon.getEN());
	// 現在KI
	pPanel->getWidgetCast<GUI::INum>("KI_NOW")->setNum(nMental);
	// 消費KI
	pPanel->getWidgetCast<GUI::INum>("KI_WANT")->setNum(weapon.getMental());
}

} // namespace Status end
} // namespace BMW end