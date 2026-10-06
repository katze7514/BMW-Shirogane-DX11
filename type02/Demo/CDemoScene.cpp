#include "stdafx.h"

#include "../Movie/Code/CCode_movie_end.h"

#include "../Chara/CValidCond.h"

#include "../Weapon/IDWeapon.h"
#include "../Weapon/CDataWeaponBattle.h"
#include "../Ability/IDAbility.h"
#include "../SLG/IDSLG.h"
#include "../SLG/Context/CDataBattle.h"
#include "../SLG/Context/CDataCharaSLG.h"

#include "GUI/CDemoEasyStatus.h"
#include "GUI/CDemoMsgBoard.h"
#include "GUI/CDemoDamage.h"
#include "Back/CDemoBack.h"
#include "Back/IDemoBackLine.h"
#include "Code/CCode_ct.h"
#include "Code/CCode_curtain.h"

#include "IDDemo.h"
#include "CDemoMovieClip.h"
#include "CDemoSymbolDB.h"
#include "CLayerChara.h"
#include "CDemoScene.h"

namespace BMW{
namespace Demo{

CDemoScene::CDemoScene()
{
	// 技能IDマップ
	// 左
	abilityID_[0].writeMap(Ability::MADRED,"SKILL_KURENAI_L_G");
	abilityID_[0].writeMap(Ability::MAGICBARRIER_A,"SKILL_TAIMARYOKU_L_G");
	abilityID_[0].writeMap(Ability::MAGICBARRIER_B,"SKILL_TAIMARYOKU_L_G");
	abilityID_[0].writeMap(Ability::MAGICBARRIER_C,"SKILL_TAIMARYOKU_L_G");
	abilityID_[0].writeMap(Ability::MAGICRELEASE,"SKILL_MARYOKUHOU_L_G");
	abilityID_[0].writeMap(Ability::BATTLEFOLLOW,"SKILL_SENTOUZOKKOU_L_G");
	abilityID_[0].writeMap(Ability::FUTUREEYE,"SKILL_MIRAISHI_L_G");
	abilityID_[0].writeMap(Ability::ALTER_EGO,"SKILL_BUNSHIN_L_G");
	abilityID_[0].writeMap(Ability::TWELVECROSS,"SKILL_GODHAND_L_G");
	abilityID_[0].writeMap(Ability::ROAIAS,"SKILL_ROUAIAS_L_G");
	abilityID_[0].writeMap(Ability::FUTUREEYE_SECOND,"SKILL_MIRAISHI2_L_G");
	abilityID_[0].writeMap(Ability::OPEN_GET,"SKILL_OPENGETS_L_G");
	abilityID_[0].writeMap(Ability::MUDAI_SHIELD,"SKILL_MUDAISHIELD_L_G");
	abilityID_[0].writeMap(Ability::HIYOKURENRI,"SKILL_HIYOKURENRI_L_G");
	abilityID_[0].writeMap(Ability::COLA_BARRIAR,"SKILL_CORABARIYA_L_G");
	abilityID_[0].writeMap(Ability::MEKA_BARRIAR,"SKILL_MHISUIBARIYA_L_G");
	abilityID_[0].writeMap(Ability::AVALON,"SKILL_AVARON_L_G");
	abilityID_[0].writeMap(Ability::MEKA_BARRIAR_WEAK,"SKILL_MHISUIBARIYA_L_G");
	// 右
	abilityID_[1].writeMap(Ability::MADRED,"SKILL_KURENAI_R_G");
	abilityID_[1].writeMap(Ability::MAGICBARRIER_A,"SKILL_TAIMARYOKU_R_G");
	abilityID_[1].writeMap(Ability::MAGICBARRIER_B,"SKILL_TAIMARYOKU_R_G");
	abilityID_[1].writeMap(Ability::MAGICBARRIER_C,"SKILL_TAIMARYOKU_R_G");
	abilityID_[1].writeMap(Ability::MAGICRELEASE,"SKILL_MARYOKUHOU_R_G");
	abilityID_[1].writeMap(Ability::BATTLEFOLLOW,"SKILL_SENTOUZOKKOU_R_G");
	abilityID_[1].writeMap(Ability::FUTUREEYE,"SKILL_MIRAISHI_R_G");
	abilityID_[1].writeMap(Ability::ALTER_EGO,"SKILL_BUNSHIN_R_G");
	abilityID_[1].writeMap(Ability::DEATH_EX,"SKILL_TYOKUSHI_R_G");
	abilityID_[1].writeMap(Ability::DEATH_TRUE,"SKILL_TYOKUSHI_R_G");
	abilityID_[1].writeMap(Ability::ROAIAS,"SKILL_ROUAIAS_R_G");
	abilityID_[1].writeMap(Ability::FUTSUNO,"SKILL_FUTSUNO_R_G");
	abilityID_[1].writeMap(Ability::SCAPEGOAT,"SKILL_MIGAWARI_R_G");
	abilityID_[1].writeMap(Ability::FUTOU,"SKILL_FUTOUFUKUTSU_R_G");
	abilityID_[1].writeMap(Ability::MEKA_BARRIAR,"SKILL_MHISUIBARIYA_R_G");
	abilityID_[1].writeMap(Ability::CHALICE_CONECT,"SKILL_SEIHAIRENKETSU_R_G");
	abilityID_[1].writeMap(Ability::AVALON,"SKILL_AVARON_R_G");
	abilityID_[1].writeMap(Ability::MEKA_BARRIAR_WEAK,"SKILL_MHISUIBARIYA_R_G");
}

CDemoScene::~CDemoScene()
{
	DELETE_SAFE(pChara_);
	DELETE_SAFE(pBack_);
	DELETE_SAFE(pCurtain_[0]);
	DELETE_SAFE(pCurtain_[1]);
	DELETE_SAFE(pStatus_[0]);
	DELETE_SAFE(pStatus_[1]);
	DELETE_SAFE(pMsg_);
	DELETE_SAFE(pDamage_);
}

void CDemoScene::OnInit(Task::CTaskContext* pContext)
{
	setContext(pContext);
	// インターフェイス読み込み
	setGuiDefDB("DEMO");

	// 戦闘データ
	context_.setBattleData(pContext->getBattleData());
	context_.setDemoScene(smart_ptr<CDemoScene>(this,false));

	// 各種インスタンス生成
	// 背景
	pBack_ = backLoader_.createDemoBack(context_.getBattleData()->getBack());
	// ステータス
	pStatus_[0] = new CDemoEasyStatus();
	pStatus_[0]->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pStatus_[0]->setSide(SLG::CStatusCharaVeryEasy::LEFT);
	pStatus_[0]->OnInit(&context_);
	context_.setEasyStatus(smart_ptr<CDemoEasyStatus>(pStatus_[0],false),0);
	pStatus_[1] = new CDemoEasyStatus();
	pStatus_[1]->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pStatus_[1]->setSide(SLG::CStatusCharaVeryEasy::RIGHT);
	pStatus_[1]->OnInit(&context_);
	context_.setEasyStatus(smart_ptr<CDemoEasyStatus>(pStatus_[1],false),1);
	// メッセージ
	pMsg_ = new CDemoMsgBoard();
	pMsg_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pMsg_->OnInit(&context_);
	context_.setMsgBoard(smart_ptr<CDemoMsgBoard>(pMsg_,false));
	// デモシンボルDBへ設定
	CDemoSymbolDB::setMsgBoard(smart_ptr<CDemoMsgBoard>(pMsg_,false));
	// ダメージ
	pDamage_ = new CDemoDamage();
	pDamage_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pDamage_->OnInit(&context_);
	pDamage_->visible(false);
	context_.setDamage(smart_ptr<CDemoDamage>(pDamage_,false));

	// キャラ
	pChara_ = new CLayerChara();
	pChara_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	setChara();
	pChara_->OnReset(&context_);

	// カーテン
	nCurtain_=-1;
	pCurtain_[0] = static_cast<CDemoMovieClip*>(getGuiDefDB().getSymbolDB().createSymbolStr("CURTAIN_R"));
	pCurtain_[1] = static_cast<CDemoMovieClip*>(getGuiDefDB().getSymbolDB().createSymbolStr("CURTAIN_L"));

	// 初期はATTACK、とCOUNTER
	setStatus(ATTACK);
	setStatus(COUNTER);

	// まずは、フェードイン
	Scene::CFoward::FaderEvent fun;
	fun.set(this,&CDemoScene::eventFade);
	Scene::CFoward* pFoward = pContext->getApp()->getFoward();
	pFoward->setFaderHandler(fun);
	pFoward->fadeOut();
	setState(FADE_IN);

	// アニメ中のskipはフラグしだい
	pContext->getApp()->animeSkip();
	pContext->getInput()->guard(false);
	// イベントモード？
	bEvent_ = context_.getBattleData()->IsEvent();
	// 背景は表示だよ
	IDemoBackLine::backVisible(true);
	IDemoBackLine::forwardVisible(true);
}

void CDemoScene::callTaskAction(Task::CTaskContext* pContext)
{
	pChara_->Task(pContext);
	pBack_->Task(pContext);
	if(getCurtain()>=0) pCurtain_[getCurtain()]->Task(pContext);
	pStatus_[0]->Task(pContext);
	pStatus_[1]->Task(pContext);
	pMsg_->Task(pContext);
	pDamage_->Task(pContext);
}

void CDemoScene::callTaskDraw(Task::CTaskContext* pContext)
{
	if(IDemoBackLine::IsBackVisible()) 
		pBack_->TaskBack(pContext);

	pChara_->Task(pContext);

	if(IDemoBackLine::IsForwardVisible()) 
		pBack_->TaskForward(pContext);

	if(getCurtain()>=0) pCurtain_[getCurtain()]->Task(pContext);

	pStatus_[0]->Task(pContext);
	pStatus_[1]->Task(pContext);
	pMsg_->Task(pContext);
	pDamage_->Task(pContext);
}

void CDemoScene::OnAction(Task::CTaskContext* pContext)
{// 再生状況に合わせて、再生を制御する
 // つまり、ムービー割り込みなどを行う
	switch(getState())
	{
	case NORMAL:
		if(pChara_->IsEnd()
		|| (!bEvent_ && pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE))
		{ 
			// フェードアウト
			pContext->getApp()->getFoward()->fadeIn();
			setState(FADE_OUT);
			pContext->getInput()->guard(true);
		}
	break;

	case RETURN:
		getTaskListCtrl()->returnTaskList();
		// 終了したらskipを有効に
		pContext->getApp()->skip(true);
	break;

	default: break;
	}
}

void CDemoScene::eventFade(Task::CTaskContext* pContext)
{
	if(getState()==FADE_IN)
	{	setState(NORMAL); }
	else
	{
		pContext->getApp()->getSeDB().StopAll();
		setState(RETURN);
	}
}

///////////////////////////////////////////////////
// 設定
///////////////////////////////////////////////////
void CDemoScene::setStatus(int nSymbol)
{
	SLG::CDataBattleBase& base = context_.getBattleData()->getBattleData(nSymbol);
	smart_ptr<SLG::CDataCharaSLG>& pChara = base.getChara();
	// 攻撃側・反撃側は同じサイドであり、良く考えると偶奇が揃う
	// よって、攻撃側の偶奇を計算すれば、自ずとsideが導出できる
	int nSide = context_.getBattleData()->getSide() + nSymbol;
	nSide %= 2;
	int nHP=0,nEN=0;
	if(nSymbol==ATTACK || nSymbol==COUNTER)
	{ nHP=getSub(nSymbol,HP); nEN=getSub(nSymbol,EN); }

	pStatus_[nSide]->actionReset(*pChara.getPointer(),SLG::CStatusCharaVeryEasy::ACTION, nSymbol>1?nSymbol+2:nSymbol,nHP,nEN);
}

void CDemoScene::loadData(int nBattle, int nFlag, SLG::CDataBattleBase& data, CDemoSymbolDB* pDB, CDemoContext* p)
{
	if(!data.getAttack().getWeaponData().isNull())
	{
		// 防御側のを設定
		def_[nBattle][ATK].getSymbolDB().setDefSymbolDB(smart_ptr<CDemoSymbolDB>(pDB,false));
		// アニメ投入ー
		def_[nBattle][ATK].setDemoDef(data.getAttack().getWeaponData()->getDemoID(),&context_);
	}

	// フラグ設定
	context_.setFlag(nFlag,
					 data.getAttack().getDamage(),
					 data.getAttack().getDamageEN(),
					 data.getAttack().getEN(),
					 data.getAttack().getEnAbility(),
					 data.getDefence().getEnAbility());
}

namespace{
string getBattleID2SymbolID(int nID)
{
	switch(nID)
	{
	case SLG::Battle::AVOID:	return "AVOID";
	case SLG::Battle::DEFENCE:	return "DEFENCE";
	default:					return "HIT";
	}
}
} // namespace end

//////////////////////////////////////////////////////////
// ムービー生成
//////////////////////////////////////////////////////////
namespace{

__inline const string& getMsgList(SLG::CDataBattleAbility& abi, const string& sID, bool bBackup=false)
{
	const string& msg = bBackup ? abi.getMsgListBackup() : abi.getMsgList();

	return msg.empty() ? sID : msg;
}

// 防御側メッセージID取得
__inline void getDefMsg(int nDamage, SLG::CDataBattleDefence& def, string& sMsg)
{
	if(nDamage==0)
	{
		if(def.IsAlterEgo())
		{// 分身発動してる
			sMsg="ALTER_AGO";
		}
		ef(def.IsBarriar())
		{
			sMsg="BARRIAR";
		}
		else
		{// 通常回避
			sMsg = "AVOID";
		}
	}
	else
	{// ダメージ受けた
		sMsg = "DEFENCE";
	}
}

} // namespace end

void CDemoScene::setChara()
{
	smart_ptr<SLG::CDataBattle>& battle = context_.getBattleData();
	// 戦闘アニメ抽選
	for(int i=ATTACK; i<=COUNTER_BACK; ++i)
		nRatio_[i]=CApp::rand_.Get(100)+1;

	// 戦闘データ
	// 攻撃と反撃の設定
	SLG::CDataBattleBase& attack = battle->getBattleData(SLG::CDataBattle::ATTACK);

	// フィール武器か？
	if(attack.getAttack().getWeaponData()->IsF())
	{// この場合は、前半だけ
		context_.setValue(1,Flag::FIELD);
		// 攻撃
		def_[ATTACK][DEF].setDemoDef(attack.getChara()->getBattle().getDemoID(),&context_);
		// アニメ投入ー
		def_[ATTACK][ATK].setDemoDef(attack.getAttack().getWeaponData()->getDemoID(),&context_);

	#ifdef BMW_DEBUG
		def_[COUNTER][DEF].setDemoDef(battle->getBattleData(SLG::CDataBattle::COUNTER).getChara()->getBattle().getDemoID(),&context_);
		loadData(ATTACK, Flag::ATTACK_HP, attack, def_[COUNTER][DEF].getSymbolDBPtr(), &context_);
	#endif

		// フラグ設定
		context_.setFlag(Flag::ATTACK_HP,
						 attack.getAttack().getDamage(),
						 0,
						 attack.getAttack().getEN(),
						 attack.getAttack().getEnAbility(),
						 attack.getDefence().getEnAbility());
		pChara_->addTask(def_[ATTACK][DEF].createMovieClip("INTRO"));
		// WAITしつつ技能EN消費
		CDemoMovieClip* pMovie = def_[ATTACK][DEF].createMovieClip("WAIT_ATTACK_SKILL");
		setAbilityAtk(context_.getBattleData()->getSide(),pMovie, attack.getAttack());
		pChara_->addTask(pMovie);
		// WAITしつつ武器EN消費
		pChara_->addTask(def_[ATTACK][DEF].createMovieClip("WAIT_ATTACK_WEAPON"));
	
		def_[ATTACK][ATK].setMsgList(getMsgList(attack.getAttack(),"ATTACK"),
									 0,
									 *attack.getChara().getPointer(),
									 *attack.getChara().getPointer(),
									 &context_,
									 !(attack.getAttack().getMsgList().empty()));
		pChara_->addTask(def_[ATTACK][ATK].createMovieClip("ATTACK",nRatio_[ATTACK]));

		// 初期背景速度
		IDemoBackLine::setBackState(context_.getBattleData()->getSide()==SLG::CDataBattle::LEFT
									? IDemoBackLine::LEFT_5 : IDemoBackLine::RIGHT_5);
		// 自分EasyステータスON
		pStatus_[battle->getSide()]->valid(true);
		pStatus_[battle->getSide()]->visible(true);
		// 敵側EasyステータスOFF
		pStatus_[1-battle->getSide()]->valid(false);
		pStatus_[1-battle->getSide()]->visible(false);
		// 差分データリセット
		resetSub();
		return; // フィールド武器ん時はここで終了
	}
	
	context_.setValue(0,Flag::FIELD);
	// フィールドじゃない普通の武器はこっち
	SLG::CDataBattleBase& counter = battle->getBattleData(SLG::CDataBattle::COUNTER);
	SLG::CDataBattleBase& attackBack = battle->getBattleData(SLG::CDataBattle::ATTACK_BACK);
	SLG::CDataBattleBase& counterBack = battle->getBattleData(SLG::CDataBattle::COUNTER_BACK);
	// 攻撃パターン判定
	// 味方対象？
	bool bFriend = attack.getAttack().getWeaponData()->getKind()==Weapon::Kind::STATUS;
	// 反撃有り？
	bool bCounter = attack.getDefence().getAction()!=SLG::Battle::NO;
	// カウンター発動？
	bool bCounterA = bCounter ? counter.getAttack().IsAbility(Ability::COUNTER) : false;
	// 援護防御は？
	bool bBackDef = counterBack.getDefence().getAction()!=SLG::Battle::NO;
	context_.setValue(bBackDef ? 1 : 0, Flag::BACK_DEF);
	// 援護攻撃は？
	bool bBackAtk = attackBack.getDefence().getAction()!=SLG::Battle::NO;
	
	// データロード
	// 防御サイド
	// 攻撃
	def_[ATTACK][DEF].setDemoDef(attack.getChara()->getBattle().getDemoID(),&context_);
	// 反撃
	def_[COUNTER][DEF].setDemoDef(counter.getChara()->getBattle().getDemoID(),&context_);
	// 援護攻撃
	if(bBackAtk) def_[ATTACK_BACK][DEF].setDemoDef(attackBack.getChara()->getBattle().getDemoID(),&context_);
	// 援護防御
	if(bBackDef) def_[COUNTER_BACK][DEF].setDemoDef(counterBack.getChara()->getBattle().getDemoID(),&context_);

	// 攻撃サイド
	// 攻撃
	loadData(ATTACK, Flag::ATTACK_HP, attack, def_[bBackDef?COUNTER_BACK:COUNTER][DEF].getSymbolDBPtr(), &context_);
	// 反撃
	if(bCounter)
		loadData(COUNTER, Flag::COUNTER_HP, counter, def_[ATTACK][DEF].getSymbolDBPtr(), &context_);
	// 援護攻撃
	if(bBackAtk)
		loadData(ATTACK_BACK, Flag::ATTACK_BACK_HP, attackBack, def_[COUNTER][DEF].getSymbolDBPtr(), &context_);
	// 援護防御
	if(bBackDef)
		loadData(COUNTER_BACK, Flag::COUNTER_BACK_HP, counterBack, NULL, &context_);
	
	// まず、カウンター技能発動ある？
	CDemoMovieClip* pMovie;
	if(!bCounterA)
	{// 無い
		// 攻撃側前半
		pChara_->addTask(def_[ATTACK][DEF].createMovieClip("INTRO"));
		setAttack(bBackDef, attack, counter, counterBack);
		
		// 防御側登場
		if(bFriend)
		{// 味方からの援護武器
			// 援護を受けるキャラが登場
			pChara_->addTask(def_[COUNTER][DEF].createMovieClip("INTRO_REVERSE"));

			// 後半
			pChara_->addTask(def_[ATTACK][ATK].createMovieClip("HIT",nRatio_[ATTACK]));
		}
		else
		{// 通常の攻撃
			setCounterDef(bBackDef, counter, counterBack, attack);
		}

		// 反撃側
		// 反撃があって生きていれば
		if(bCounter
		&& (bBackDef || !attack.getAttack().IsDeath()))
		{// 反撃があれば
			if(bBackDef)
			{// 援護防御があったら、入れ替え
				pChara_->addTask(def_[COUNTER_BACK][DEF].createMovieClip("EXIT"));
				pChara_->addTask(def_[COUNTER][DEF].createMovieClip("INTRO_REVENGE"));
			}
			// 反撃攻撃
			setCounterAttack(counter,attack);
		}
	}
	else
	{// カウンター有り
		// 反撃側から
		pMovie = def_[COUNTER][DEF].createMovieClip("INTRO_COUNTER");
		setAbilityIntro(1-battle->getSide(), pMovie, COUNTER_INTRO);
		pChara_->addTask(pMovie);
		// 反撃攻撃
		setCounterAttack(counter,attack);

		// 攻撃側
		if(!counter.getAttack().IsDeath())
		{// 生きてればね
			// 反撃側退場
			//pChara_->addTask(def_[COUNTER][DEF].createMovieClip("EXIT"));
			// 攻撃側前半
			setAttack(bBackDef, attack, counter, counterBack);
			// 攻撃側後半
			setCounterDef(bBackDef, counter, counterBack, attack);
		}
	}

	// 援護攻撃
	if((bBackDef || !attack.getAttack().IsDeath())
	&& !counter.getAttack().IsDeath()
	&& bBackAtk)
	{// あるよ！
		// 状況に応じてキャラの出し入れ
		if(!bCounter || (bCounter&&bCounterA))
		{// 反撃が無し、もしくは、反撃有りカウンター有り
		 // つまり、反撃側でアニメが終了している
			if(bBackDef) // 援護防御有
				pChara_->addTask(def_[COUNTER_BACK][DEF].createMovieClip("EXIT"));
			else
				pChara_->addTask(def_[COUNTER][DEF].createMovieClip("EXIT"));
		}
		else
		{// 反撃有りでカウンター無し
		 // つまり、攻撃側でアニメが終了している
			pChara_->addTask(def_[ATTACK][DEF].createMovieClip("EXIT"));
		}

		// 登場
		// 援護攻撃入る時メッセージ
		def_[ATTACK_BACK][DEF].setMsgList(getMsgList(counterBack.getDefence(), "BACK_ATK", true),
											0,
											*attackBack.getChara().getPointer(),
											*attack.getChara().getPointer(), // 被援護者にしておく
											&context_,
											!attackBack.getDefence().getMsgList().empty());
		pMovie = def_[ATTACK_BACK][DEF].createMovieClip("INTRO_BACK_ATTACK");
		setAbilityIntro(battle->getSide(), pMovie, BACK_ATK);
		pChara_->addTask(pMovie);
		// WAITしつつ技能EN消費
		pMovie = def_[ATTACK_BACK][DEF].createMovieClip("WAIT_ATTACK_BACK_SKILL");
		setAbilityAtk(battle->getSide(),pMovie, attackBack.getAttack());
		pChara_->addTask(pMovie);
		// WAITしつつ武器EN消費
		pChara_->addTask(def_[ATTACK_BACK][DEF].createMovieClip("WAIT_ATTACK_BACK_WEAPON"));
		// 攻撃開始！
		def_[ATTACK_BACK][ATK].setMsgList(getMsgList(attackBack.getAttack(),"ATTACK"),
										  0,
										  *attackBack.getChara().getPointer(),
										  *counter.getChara().getPointer(),
										  &context_,
										  !attackBack.getAttack().getMsgList().empty());
		pChara_->addTask(def_[ATTACK_BACK][ATK].createMovieClip("ATTACK",nRatio_[ATTACK_BACK]));
		// カーテン投入
		pChara_->addTask(createCurtain(ATTACK));
		// 防御側登場
		pChara_->addTask(def_[COUNTER][DEF].createMovieClip("INTRO_REVENGE"));
		// 防御WAIT
		// WAITしつつ技能EN消費
		pMovie = def_[COUNTER][DEF].createMovieClip("WAIT_ATTACK_BACK_DEF");
		setAbilityDef(1-battle->getSide(), pMovie, attackBack.getDefence());
		pChara_->addTask(pMovie);
		
		// 後半
		pChara_->addTask(def_[ATTACK_BACK][ATK].createMovieClip(getBattleID2SymbolID(attackBack.getDefence().getAction()),nRatio_[ATTACK_BACK]));

		// ダメージがあって状態変化武器だったら状態Symbol追加
		if((context_.getValue(Flag::ATTACK_HP)+context_.getValue(Flag::ATTACK_BACK_HP))>0) setCondSymbol(COUNTER,attackBack);

		// ダメージ
		string sMsg;
		getDefMsg(context_.getValue(Flag::ATTACK_HP)+context_.getValue(Flag::ATTACK_BACK_HP), attackBack.getDefence(), sMsg);
		def_[COUNTER][DEF].setMsgList(getMsgList(attackBack.getDefence(),sMsg),
									  context_.getValue(Flag::ATTACK_HP)+context_.getValue(Flag::ATTACK_BACK_HP),
									  *counter.getChara().getPointer(),
									  *attackBack.getChara().getPointer(),
									  &context_,
									  !(attackBack.getDefence().getMsgList().empty()));
		pMovie = def_[COUNTER][DEF].createMovieClip("WAIT_ATTACK_BACK_HP");
		setAbilityDamage(1-battle->getSide(), pMovie, attack.getDefence(), attackBack.getAttack().IsCT());
		pChara_->addTask(pMovie);
	}

	// 初期背景速度
	IDemoBackLine::setBackState(context_.getBattleData()->getSide()==SLG::CDataBattle::LEFT
								&& !bCounterA
								? IDemoBackLine::LEFT_5 : IDemoBackLine::RIGHT_5);
	// 自分EasyステータスON
	pStatus_[battle->getSide()]->valid(true);
	pStatus_[battle->getSide()]->visible(true);
	// 敵側EasyステータスON
	pStatus_[1-battle->getSide()]->valid(true);
	pStatus_[1-battle->getSide()]->visible(true);
	// 差分データリセット
	resetSub();
}

void CDemoScene::setAttack(bool bBackDef, SLG::CDataBattleBase& attack,SLG::CDataBattleBase& counter, SLG::CDataBattleBase& counterBack)
{
	// WAITしつつ技能EN消費
	CDemoMovieClip* pMovie = def_[ATTACK][DEF].createMovieClip("WAIT_ATTACK_SKILL");
	setAbilityAtk(context_.getBattleData()->getSide(),pMovie, attack.getAttack());
	pChara_->addTask(pMovie);
	// WAITしつつ武器EN消費
	pChara_->addTask(def_[ATTACK][DEF].createMovieClip("WAIT_ATTACK_WEAPON"));
	// 攻撃開始！
#ifdef BMW_DEBUG
	CDbg().Out("EVENT %s", attack.getAttack().getMsgList().c_str());
#endif

	def_[ATTACK][ATK].setMsgList(getMsgList(attack.getAttack(),"ATTACK"),
								 0,
								 *attack.getChara().getPointer(),
								 bBackDef ? *counterBack.getChara().getPointer() : *counter.getChara().getPointer(),
								 &context_,
								 !(attack.getAttack().getMsgList().empty()));
	pChara_->addTask(def_[ATTACK][ATK].createMovieClip("ATTACK", nRatio_[ATTACK]));
	// カーテン投入
	pChara_->addTask(createCurtain(ATTACK));
}

void CDemoScene::setCounterDef(bool bBackDef, SLG::CDataBattleBase& counter, SLG::CDataBattleBase& counterBack, SLG::CDataBattleBase& attack)
{
	CDemoMovieClip* pMovie;
	int nDef;
	int nSide = 1-context_.getBattleData()->getSide();
	if(bBackDef)
	{// 援護防御ある？
		// 援護防御キャラ登場with援護防御パネル
		// 援護防御入る時メッセージ
		def_[COUNTER_BACK][DEF].setMsgList(getMsgList(counterBack.getDefence(), "BACK_DEF", true),
											0,
											*counterBack.getChara().getPointer(),
											*counter.getChara().getPointer(), // 被援護者にしておく
											&context_,
											!counterBack.getDefence().getMsgList().empty());
		pMovie = def_[COUNTER_BACK][DEF].createMovieClip("INTRO_BACK_DEF");
		setAbilityIntro(nSide, pMovie, BACK_DEF);
		pChara_->addTask(pMovie);
		// 防御WAIT
		// WAITしつつ技能EN消費
		pMovie = def_[COUNTER_BACK][DEF].createMovieClip("WAIT_COUNTER_BACK_DEF");
		setAbilityDef(nSide, pMovie, counterBack.getDefence());
		pChara_->addTask(pMovie);
		// 反撃行動取得
		nDef = counterBack.getDefence().getAction();
	}
	else
	{// なし
		// 反撃キャラが普通に登場
		pChara_->addTask(def_[COUNTER][DEF].createMovieClip("INTRO_REVENGE"));
		// 防御WAIT
		// WAITしつつ技能EN消費
		pMovie = def_[COUNTER][DEF].createMovieClip("WAIT_COUNTER_DEF");
		setAbilityDef(nSide, pMovie, counter.getDefence());
		pChara_->addTask(pMovie);
		// 反撃行動取得
		nDef = counter.getDefence().getAction();
	}

	// 後半
	pChara_->addTask(def_[ATTACK][ATK].createMovieClip(getBattleID2SymbolID(nDef),nRatio_[ATTACK]));

	// 状態変化武器だったら状態Symbol追加
	if(context_.getValue(Flag::ATTACK_HP)>0) setCondSymbol(bBackDef?COUNTER_BACK:COUNTER,attack);

	// ダメージ
	SLG::CDataBattleBase& c = bBackDef?counterBack:counter;
	string sMsg;
	getDefMsg(context_.getValue(Flag::ATTACK_HP), c.getDefence(), sMsg);
	def_[bBackDef?COUNTER_BACK:COUNTER][DEF].setMsgList(getMsgList(c.getDefence(), sMsg),
														context_.getValue(Flag::ATTACK_HP),
														*c.getChara().getPointer(),
														*attack.getChara().getPointer(),
														&context_,
														!c.getDefence().getMsgList().empty());
	pMovie = def_[bBackDef?COUNTER_BACK:COUNTER][DEF].createMovieClip("WAIT_ATTACK_HP");
	setAbilityDamage(nSide, pMovie, c.getDefence(), attack.getAttack().IsCT());
	pChara_->addTask(pMovie);
}

void CDemoScene::setCounterAttack(SLG::CDataBattleBase& counter, SLG::CDataBattleBase& attack)
{
	CDemoMovieClip* pMovie;
	int nSide = context_.getBattleData()->getSide();
	// WAITしつつ技能EN消費
	pMovie = def_[COUNTER][DEF].createMovieClip("WAIT_COUNTER_SKILL");
	setAbilityAtk(1-nSide, pMovie, counter.getAttack());
	pChara_->addTask(pMovie);
	// WAITしつつ武器EN消費
	pChara_->addTask(def_[COUNTER][DEF].createMovieClip("WAIT_COUNTER_WEAPON"));
	// 攻撃開始！
	def_[COUNTER][ATK].setMsgList(getMsgList(counter.getAttack(),"ATTACK"),0,
								  *counter.getChara().getPointer(),
								  *attack.getChara().getPointer(),
								  &context_,
								  !counter.getAttack().getMsgList().empty());
	pChara_->addTask(def_[COUNTER][ATK].createMovieClip("ATTACK",nRatio_[COUNTER]));
	// カーテン投入
	pChara_->addTask(createCurtain(COUNTER));
	// 防御側登場
	pChara_->addTask(def_[ATTACK][DEF].createMovieClip("INTRO"));
	// 防御WAIT
	// WAITしつつ技能EN消費
	pMovie = def_[ATTACK][DEF].createMovieClip("WAIT_ATTACK_DEF");
	setAbilityDef(nSide, pMovie, attack.getDefence());
	pChara_->addTask(pMovie);
		
	// 後半
	pChara_->addTask(def_[COUNTER][ATK].createMovieClip(getBattleID2SymbolID(attack.getDefence().getAction()),nRatio_[COUNTER]));

	// ダメージをくらって状態変化武器だったら状態Symbol追加
	if(context_.getValue(Flag::COUNTER_HP)>0) setCondSymbol(ATTACK,counter);

	// ダメージ
	string sMsg;
	getDefMsg(context_.getValue(Flag::COUNTER_HP), attack.getDefence(), sMsg);
	def_[ATTACK][DEF].setMsgList(getMsgList(attack.getDefence(),sMsg),
								 context_.getValue(Flag::COUNTER_HP),
								 *attack.getChara().getPointer(),
								 *counter.getChara().getPointer(),
								 &context_,
								 !attack.getDefence().getMsgList().empty());
	pMovie = def_[ATTACK][DEF].createMovieClip("WAIT_COUNTER_HP");
	setAbilityDamage(1-nSide, pMovie, attack.getDefence(), counter.getAttack().IsCT());
	pChara_->addTask(pMovie);
}

/////////////////////////////////////////////
// 技能パネル追加系
/////////////////////////////////////////////
namespace{
Movie::CTween* createTweenAbility(Task::ITaskBase* pBase, int nSide, int nFrame, int nStage=0)
{
	const int nY = 88+35*nStage;
	// TWEEN設定
	Movie::CTween* pTween = new Movie::CTween();
	pTween->setTask(pBase);
	pTween->share(pBase==NULL);
	pTween->setState(nFrame);
	// モーション設定
	LONG lWidth, lHeight;
	pBase->getSize(lWidth,lHeight);
	Movie::CMotion motion;
	motion.setStep(nFrame);
	if(nSide==0)
	{
		motion.setStart(-lWidth,nY);
		motion.setEnd(0,nY);
	}
	else
	{
		motion.setStart(640+lWidth,nY);
		motion.setEnd(640,nY);
	}
	pTween->setMotion(motion);

	return pTween;
}

Movie::CKeyFrame* createKeyFrame(Task::ITaskBase* pBase, int nSide, int nFrame, int nStage=0)
{
	Movie::CKeyFrame* pKey = new Movie::CKeyFrame();
	pKey->setTask(pBase);
	pKey->share(pBase==NULL);
	pKey->setState(nFrame);
	pKey->setX(nSide==0?0:640);
	pKey->setY(88+35*nStage);

	return pKey;
}

const string asIntroID[2][3]=
{
	{"SKILL_ENGO_DEF_L_G","SKILL_ENGO_ATK_L_G","SKILL_COUNTER_L_G"},
	{"SKILL_ENGO_DEF_R_G","SKILL_ENGO_ATK_R_G","SKILL_COUNTER_R_G"}
};

} // namesapce end
void CDemoScene::setAbilityIntro(int nSide, CDemoMovieClip* pClip, int nID)
{// 登場時技能パネル追加
 // 援護防御・援護攻撃・カウンター
	// レイヤー
	Movie::CLayer* pLayer = new Movie::CLayer();
	pLayer->resizeFrame(3);
	pClip->addTask(pLayer,INT_MAX);
	
	// IDに合わせて生成
	Task::ITaskBase* pBase = getGuiDefDB().getSymbolDB().createSymbolStr(asIntroID[nSide][nID]);
	// tween
	pLayer->setKeyFrame(createTweenAbility(pBase,nSide,10),0);
	// キーフレ
	pLayer->setKeyFrame(createKeyFrame(NULL,nSide,20),1);
	// キーフレ(END)
	pLayer->setKeyFrame(createKeyFrame(new Movie::Code::CCode_movie_end(),nSide,1),2);
}

void CDemoScene::setAbility(int nSide, CDemoMovieClip* pClip, SLG::CDataBattleAbility& data, set<int>& validSet, int nStage)
{// 技能パネルの追加
	// 技能セットと、追加が有効なセットをもらって追加する
	// レイヤー
	Movie::CLayer* pLayer;
	Task::ITaskBase* pBase;
	set<int>& setID = data.getAbilitySet();
	set<int>::iterator it;
	
	for(it=setID.begin(); it!=setID.end(); ++it)
	{
		if(validSet.find(*it)!=validSet.end())
		{// 生成OKリストに入っていれば生成
			pLayer = new Movie::CLayer();
			pLayer->resizeFrame(2);
			pClip->addTask(pLayer,INT_MAX-nStage);
			// IDに合わせて生成
			pBase = getGuiDefDB().getSymbolDB().createSymbolStr(abilityID_[nSide].getValue(*it));
			// tween
			pLayer->setKeyFrame(createTweenAbility(pBase,nSide,10,nStage),0);
			// キーフレ
			pLayer->setKeyFrame(createKeyFrame(NULL,nSide,20,nStage),1);

			++nStage;
		}
	}
}

void CDemoScene::setAbilityAtk(int nSide, CDemoMovieClip* pClip, SLG::CDataBattleAbility& data)
{// 攻撃時技能パネルの追加
 // 魔力放出・直死の魔眼・改・聖杯連結・未来視・弐
	set<int> validSet;
	validSet.insert(Ability::MAGICRELEASE);
	validSet.insert(Ability::DEATH_EX);
	validSet.insert(Ability::FUTUREEYE_SECOND);
	validSet.insert(Ability::CHALICE_CONECT);
	setAbility(nSide,pClip,data, validSet);
}

void CDemoScene::setAbilityDef(int nSide, CDemoMovieClip* pClip, SLG::CDataBattleAbility& data)
{// 防御時技能パネルの追加
 // 紅赤朱・対魔力障壁・未来視・分身・直死の魔眼・真・ローアイアス・十二の試練・布津ノ加護
 // オープンゲット・ムダイシールド・コーラバリア・メカヒスイバリア・アヴァロン
	set<int> validSet;
	validSet.insert(Ability::MADRED);
	validSet.insert(Ability::MAGICBARRIER_A);
	validSet.insert(Ability::MAGICBARRIER_B);
	validSet.insert(Ability::MAGICBARRIER_C);
	validSet.insert(Ability::FUTUREEYE);
	validSet.insert(Ability::ALTER_EGO);
	validSet.insert(Ability::DEATH_TRUE);
	validSet.insert(Ability::TWELVECROSS);
	validSet.insert(Ability::FUTSUNO);
	validSet.insert(Ability::ROAIAS);
	validSet.insert(Ability::OPEN_GET);
	validSet.insert(Ability::MUDAI_SHIELD);
	validSet.insert(Ability::COLA_BARRIAR);
	validSet.insert(Ability::MEKA_BARRIAR);
	validSet.insert(Ability::AVALON);
	setAbility(nSide,pClip,data, validSet);
}

void CDemoScene::setAbilityDamage(int nSide, CDemoMovieClip* pClip, SLG::CDataBattleAbility& data, bool bCT)
{// ダメージ時技能パネルの追加
 // クリティカル・戦闘続行・身代わり・不撓不屈・比翼連理
	if(bCT)
	{// クリティカルコード挿入
		Movie::CLayer* pLayer = new Movie::CLayer();
		Movie::CKeyFrame* pKey = new Movie::CKeyFrame();
		pKey->setTask(new Code::CCode_ct());
		pLayer->resizeFrame(1);
		pLayer->setKeyFrame(pKey,0);
		pClip->addTask(pLayer,INT_MAX);
	}

	// その他、ダメージ時技能
	set<int> validSet;
	validSet.insert(Ability::BATTLEFOLLOW);
	validSet.insert(Ability::SCAPEGOAT);
	validSet.insert(Ability::FUTOU);
	validSet.insert(Ability::HIYOKURENRI);
	setAbility(nSide,pClip,data,validSet,bCT?1:0);
}

Task::ITaskBase* CDemoScene::createCurtain(int nSide)
{
	return new Code::CCode_curtain(nSide==ATTACK ? context_.getBattleData()->getSide() : 1-context_.getBattleData()->getSide(), this);
}

void CDemoScene::setCondSymbol(int nBattle, SLG::CDataBattleBase& attack)
{
	smart_ptr<Weapon::CDataWeaponBattle>& pWeapon = attack.getAttack().getWeaponData();
	// 状態変化武器じゃなかったら何もしない。もしくはダメージ0
	if(!(pWeapon->getKind()==Weapon::Kind::FIGHT_COND || pWeapon->getKind()==Weapon::Kind::MAGIC_COND)
	|| attack.getAttack().getDamage()<=0) return;

	// SymbolID
	const string sCondSymbolID[]={"WAIT_JOUTAI_ACTION","WAIT_JOUTAI_MOVE","WAIT_JOUTAI_DEFENCE","WAIT_JOUTAI_HIT","WAIT_JOUTAI_AVOID","WAIT_JOUTAI_EN"};

	pChara_->addTask(def_[nBattle][DEF].createMovieClip(sCondSymbolID[pWeapon->getCond()]));
}

} // namespace Demo end
} // namespace BMW end