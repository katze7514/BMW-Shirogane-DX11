/*
	katze 06/04/19
	update 07/01/20
	SLGスクリプト部分パーサー
*/
#pragma once

#include "../../../Sound/SoundCmd.h"
#include "../../../Sound/Code/CSoundCmdFactory.h"

#include "../../../ADV/IDADV.h"
#include "../../../ADV/CAdvCmdFactory.h"

#include "../../IDRule.h"
#include "../../VM/CSlgCmdFactory.h"
#include "../../Code/CCode_cond_call.h"
#include "../../Code/CCode_slg_end.h"
#include "../../Code/CCode_slg2valid.h"

#include "../../Phase/CPhase_init.h"
#include "../../various/CData_ref.h"

#include "../../Effect/CSlgEffectScriptLoader.h"

#include "../CSLGDef.h"
#include "../CSLGContext.h"

#include "CSlgCondLoader.h"
#include "ISlgCond.h"

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

#include "../../../Sound/Code/sound_symbol.h"
#include "../../../Sound/Code/sound_closure.h"

#include "../../../ADV/adv_closure.h"
#include "../../../ADV/adv_symbol.h"

#include "slg_closure.h"
#include "slg_symbol.h"

namespace BMW{
namespace SLG{
class CSLGDef;

struct battle_closure : public boost::spirit::closure<battle_closure, Code::CCmdBattle>
{
	member1 val;
};

struct chara_closure : public boost::spirit::closure<chara_closure, Code::CCmdChara, int>
{
	member1 val;
	member2 n;
};

struct sally_closure : public boost::spirit::closure<sally_closure, Code::CCmdSally>
{
	member1 val;
};

struct vic_closure : public boost::spirit::closure<vic_closure, Code::CCmdVicChange>
{
	member1 val;
};

struct changechara_closure : public boost::spirit::closure<changechara_closure, Code::CCmdChangeChara, string>
{
	member1 val;
	member2 s;
};

struct CSlgScriptParser : public boost::spirit::grammar<CSlgScriptParser>
{
	CSlgScriptParser(CSLGDef& def,CSLGContext* p, VM::CScript* pScript):def_(def),p_(p),pScript_(pScript){ cond_.setDef(&def_); effect_.setSLGContext(p); }
	CSLGDef&		def_;
	CSLGContext*	p_;
	VM::CScript*	pScript_;
	CSlgCondLoader	cond_;
	Effect::CSlgEffectScriptLoader effect_;

	template<typename S>
	struct definition
	{
		definition(const CSlgScriptParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// コンテキストの設定
			pContext_ = self.p_;
			pScript_  = self.pScript_;
			pLoader_  = const_cast<CSlgCondLoader*>(&self.cond_);
			pEffect_  = const_cast<Effect::CSlgEffectScriptLoader*>(&self.effect_);

			// name属性
			name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';

			// 各種コマンド
			start_ =*(msg_ | state_ | bgm_ | se_ | fade_ | wait_ | cursol_ | call_ | cond_call_
					| addchara_ | addmap_ | delchara_ | delmap_ | setweapon_ | chara_ | cond_ | battle_ | effect_ | vic_
					| phase_ | sally_ | changechara_
					| slg_end_ | slg2valid_)
					;


			// <cond (expert=""|hero="")>*script_</cond>
			cond_	= str_p("<cond") 
						>> (
							(str_p("expert=\"") >> expertSymbol_[cond_.val=arg1] >> '"' >> '>'
								>> if_p(bind(&definition::IsExpert)(var(*this),cond_.val))
								   [start_]
								   .else_p
								   [*(anychar_p - "</cond>")]
							)	   
							|
							(str_p("hero=\"") >> heroSymbol_[cond_.val=arg1] >> '"'	>> '>'
								>> if_p(bind(&definition::IsHero)(var(*this),cond_.val))
								   [start_]
								   .else_p
								   [*(anychar_p - "</cond>")]
							)
						   )
					>> str_p("</cond>")
					;

			// <msg side="" (no=""|slg=""|chara="") face="" !mask="" >
			//	TEXT
			// </msg>
			msg_	= str_p("<msg")
						>> str_p("side=\"") 
							>> sideID_[bind(&SLG::Code::CCmdMsg::setSide)(msg_.val,arg1)]
						>> '"'
						>>(	no_[bind(&SLG::Code::CCmdMsg::setID)(msg_.val,arg1)]
						|	slg_id_[bind(&SLG::Code::CCmdMsg::setSlg)(msg_.val,arg1)]
						|	( str_p("chara=\"") 
								>> (*(anychar_p - '"'))[msg_.s = construct_<string>(arg1,arg2)]
								>> '"'
								>> eps_p[bind(&SLG::Code::CCmdMsg::setChara)(msg_.val,msg_.s)]
							)
						) 
						>> str_p("face=\"") >> (*(anychar_p - '"'))[msg_.s = construct_<string>(arg1,arg2)] >> '"'
						>> eps_p[bind(&SLG::Code::CCmdMsg::setFace)(msg_.val,msg_.s)]
						>> !(str_p("mask=\"") >> boolSymbol_[bind(&SLG::Code::CCmdMsg::mask)(msg_.val,arg1)] >> '"')
					>> '>'
					>> (*(anychar_p - "</msg>"))[msg_.s = construct_<string>(arg1,arg2)]
					>> eps_p[bind(&SLG::Code::CCmdMsg::setMsg)(msg_.val,msg_.s)]
					>> str_p("</msg>")
					>> eps_p[bind(&definition::newMsg)(var(*this),msg_.val)]
					;

			// id属性
			id_		= str_p("id=\"") >> (*(anychar_p - '"'))[id_.val = construct_<string>(arg1,arg2)] >> '"';

			// <state visible="" />
			state_	= str_p("<state")
						>> str_p("visible=\"") >> boolSymbol_[bind(&definition::newMsgState)(var(*this),arg1)] >> '"'
					>> str_p("/>")
					;

			// <bgm ctrl="" !id="" !fade="" />
			bgm_	= str_p("<bgm")
					>> str_p("ctrl=\"") >> ctrlID_[bind(&Sound::Code::CCmdSound::setCtrl)(bgm_.val,arg1)] >> '"'
					>> !(id_[bind(&Sound::Code::CCmdSound::setBgm)(bgm_.val,arg1)])
					>> !( str_p("fade=\"") >> int_p[bind(&Sound::Code::CCmdSound::setFade)(bgm_.val,arg1)] >> '"')
					>> str_p("/>")
					>> eps_p[bind(&definition::newBgm)(var(*this),bgm_.val)]
					;

			// <se ctrl="" no="" />
			se_		= str_p("<se")
					>> str_p("ctrl=\"") >> ctrlID_[bind(&Sound::Code::CCmdSound::setCtrl)(se_.val,arg1)] >> '"'
					>> id_[bind(&Sound::Code::CCmdSound::setBgm)(se_.val,arg1)]
					>> str_p("/>")
					>> eps_p[bind(&definition::newSe)(var(*this),se_.val)]
					;

			// <fade ctrl="" frame="" !color="" />
			fade_	= str_p("<fade")
					>> str_p("ctrl=\"") >> fadeID_[bind(&ADV::CCmdFade::setCtrl)(fade_.val,arg1)] >> '"'
					>> str_p("frame=\"") >> int_p[bind(&ADV::CCmdFade::setFrame)(fade_.val,arg1)] >> '"'
					>> !(str_p("color=\"") >> colorID_[bind(&ADV::CCmdFade::setColor)(fade_.val,arg1)] >> '"')
					>> str_p("/>")
					>> eps_p[bind(&definition::newFade)(var(*this),fade_.val)]
					;

			// <wait kind=""|frame="" />
			wait_	= str_p("<wait")
					>> (
						(str_p("kind=\"") >> waitID_[bind(&definition::newWait)(var(*this),arg1)] >> '"')
						|
					    (str_p("frame=\"") >> int_p[bind(&definition::newWaitFrame)(var(*this),arg1)] >> '"')
					   )
					>> str_p("/>")
					;

			// <cursol enable="" />
			cursol_	= str_p("<cursol")
					>> str_p("enable=\"") 
						>> boolSymbol_[bind(&definition::newCursolEnable)(var(*this),arg1)] 
					>> '"'
					>> str_p("/>")
					;

			// <call name="" />
			call_	= str_p("<call") >> name_[call_.val = arg1]	>> str_p("/>")
					>> eps_p[bind(&definition::newCall)(var(*this),call_.val,var(self.def_))]
					;

			// <cond_call name="">
			//  cond
			// </cond_call>
			cond_call_	= str_p("<cond_call") >> name_[cond_call_.val = arg1] >> str_p(">")
							>>  eps_p[bind(&definition::newCondCall)(var(*this),cond_call_.val,var(self.def_))]
							>>	(*(anychar_p - "</cond_call>"))[cond_call_.val = construct_<string>(arg1,arg2)]
							>>	eps_p[var(pCondList_) = bind(&CSlgCondLoader::createCond)(var(pLoader_),cond_call_.val)]
							>>	eps_p[bind(&Code::CCode_cond_call::setCond)(var(pCall_),var(pCondList_))]
						>> str_p("</cond_call>")
						;

			// 思考ルーチン定義部
			// action="" !wait="" !(move="" slg="") !e_move="" !e_atk="" !e_hp="" *param=""
			action_ =	str_p("action=\"") 
							>> actionID_[bind(&SLG::Code::CCmdAction::setAction)(action_.val,arg1)] 
						>> '"'
						// 基本パラメタ
						>> !(str_p("wait=\"") 
							>> int_p[bind(&SLG::Code::CCmdAction::addParam)(action_.val,arg1)] 
						>> '"')
						>> !(str_p("move=\"") 
								>> moveType_[bind(&SLG::Code::CCmdAction::addParam)(action_.val,arg1)] 
							>> '"'
						>> slg_id_[bind(&SLG::Code::CCmdAction::setTargetChara)(action_.val,arg1)]
							)
						// 評価関数は、評価いじるやつの時だけ必須
						>> if_p(bind(&Code::CCmdAction::getAction)(action_.val)==Action::NORMAL_EVAL
							||  bind(&Code::CCmdAction::getAction)(action_.val)==Action::FIELD_EVAL)
							[
								(str_p("e_move=\"") 
									>> evalMoveID_[bind(&SLG::Code::CCmdAction::addParam)(action_.val,arg1)] 
								>> '"')
							>> 	(str_p("e_atk=\"") 
									>> evalMoveID_[bind(&SLG::Code::CCmdAction::addParam)(action_.val,arg1)] 
								>> '"')
							>>	(str_p("e_hp=\"") 
									>> evalHpID_[bind(&SLG::Code::CCmdAction::addParam)(action_.val,arg1)] 
								>> '"')
							]
						// 任意のパラメタ
						>> *(str_p("param=\"") 
							>> int_p[bind(&SLG::Code::CCmdAction::addParam)(action_.val,arg1)] 
							>> '"')
						;

			// <addchara (no=""|slg="") train="" chara="" phase="" action="" !wait="" !(move="" slg="") !e_move="" !e_atk="" !e_hp="" *param="" />
			addchara_	= str_p("<addchara")
						>>( no_[bind(&SLG::Code::CCmdAddChara::setID)(addchara_.val,arg1)]
							| slg_id_[bind(&SLG::Code::CCmdAddChara::setSlg)(addchara_.val,arg1)]
							)
						>> str_p("train=\"") 
							>> int_p[bind(&SLG::Code::CCmdAddChara::setTrain)(addchara_.val,arg1)] 
						>> '"'
						>> str_p("chara=\"") 
							>> (*(anychar_p - '"'))[addchara_.s = construct_<string>(arg1,arg2)]
							>> eps_p[bind(&SLG::Code::CCmdAddChara::setChara)(addchara_.val,addchara_.s)]
						>> '"'
						>> str_p("phase=\"") 
							>> phaseID_[bind(&SLG::Code::CCmdAddChara::setPhase)(addchara_.val,arg1)] 
						>> '"'
						// 思考ルーチン
						>> action_[bind(&Code::CCmdAddChara::setActionInfo)(addchara_.val,arg1)]

						>> str_p("/>")
						>> eps_p[bind(&definition::newAddChara)(var(*this),addchara_.val)]
						;

			// <addmap (no=""|slg="") (index=""|(target="" on="")) way="" !effect="" />
			addmap_	= str_p("<addmap")
						>>(	no_[bind(&SLG::Code::CCmdAddCharaMap::setID)(addmap_.val,arg1)]
						  | slg_id_[bind(&SLG::Code::CCmdAddCharaMap::setSlg)(addmap_.val,arg1)])
						>> ((str_p("index=\"") >> int_p[bind(&SLG::Code::CCmdAddCharaMap::setIndex)(addmap_.val,arg1)] >> '"')
						   | (target_[bind(&SLG::Code::CCmdAddCharaMap::setTarget)(addmap_.val,arg1)]
							  >> !(str_p("on=\"") >> wayID_[bind(&SLG::Code::CCmdAddCharaMap::setOn)(addmap_.val,arg1)] >> '"')
							  ))
						>> str_p("way=\"")
							>> wayID_[bind(&SLG::Code::CCmdAddCharaMap::setWay)(addmap_.val,arg1)]
						>> '"'
						>> !(str_p("effect=\"")
							>> addEffectID_[bind(&SLG::Code::CCmdAddCharaMap::setEffect)(addmap_.val,arg1)]
						>> '"')
					>> str_p("/>")
					>> eps_p[bind(&definition::newAddCharaMap)(var(*this),addmap_.val)]
					;

			// <delchara (no=""|slg="") />
			delchara_	= str_p("<delchara")
						>> (no_[bind(&definition::newDelCharaID)(var(*this),arg1)]
							| slg_id_[bind(&definition::newDelChara)(var(*this),arg1)]
							)
						>> str_p("/>")
						;

			// <delmap (no=""|slg="") effect="" />
			delmap_	= str_p("<delmap")
						>> (no_[bind(&SLG::Code::CCmdDelCharaMap::setID)(delmap_.val,arg1)]
							| slg_id_[bind(&SLG::Code::CCmdDelCharaMap::setSlg)(delmap_.val,arg1)]
							)
						>> str_p("effect=\"")
							>> delEffectID_[bind(&SLG::Code::CCmdDelCharaMap::setEffect)(delmap_.val,arg1)]
						>> '"'
					>> str_p("/>")
					>> eps_p[bind(&definition::newDelCharaMap)(var(*this),delmap_.val)]
					;

			// <setweapon (no=""|slg="") />
			setweapon_	= str_p("<setweapon")
							>>(	no_[bind(&definition::newSetWeaponID)(var(*this),arg1)]
								| slg_id_[bind(&definition::newSetWeapon)(var(*this),arg1)]
								)
						>> str_p("/>")
						;

			// <chara target="" kind="" !type="" value="" />
			// <chara target="" kind="LOVE" slg="" />
			// <chara target="" kind="ACTION" action="" !wait="" !(move="" slg="") *param="" />
			chara_ = str_p("<chara") 
						>> target_[bind(&Code::CCmdChara::sChara_)(chara_.val) = arg1]
						>> str_p("kind=\"") >> kind_[bind(&Code::CCmdChara::nKind_)(chara_.val)=chara_.n=arg1] >> '"'
						>> if_p(chara_.n == Code::CCode_chara::ACT)
							[// 行動状態
								str_p("value=\"") >> actvalue_[bind(&SLG::Code::CCmdChara::addParam)(chara_.val,arg1)]  >> '"'
							]
							.else_p
							[
								if_p(bind(&definition::IsCharaKind)(var(*this),chara_.n))
								[// HP・EN・気力
									str_p("type=\"") >>	valuetype_[bind(&Code::CCmdChara::nType_)(chara_.val)=arg1] >> '"'
									>> str_p("value=\"") >> int_p[bind(&SLG::Code::CCmdChara::addParam)(chara_.val,arg1)] >> '"'
								]
								.else_p
								[
									if_p(chara_.n == Code::CCode_chara::ACTION)
									[// 思考ルーチン
										action_[bind(&Code::CCmdChara::action_)(chara_.val)=arg1]
									]
									.else_p
									[
										if_p(chara_.n == Code::CCode_chara::WAY)
										[// 向き
											str_p("value=\"") >> wayID_[bind(&SLG::Code::CCmdChara::addParam)(chara_.val,arg1)] >> '"'
										]
										.else_p
										[
											if_p(chara_.n == Code::CCode_chara::VALID)
											[// コマンド有効状態
												str_p("type=\"") >> validtype_[bind(&Code::CCmdChara::nType_)(chara_.val)=arg1] >> '"'
												>> str_p("value=\"") >> boolSymbol_[bind(&SLG::Code::CCmdChara::addParam)(chara_.val,arg1)] >> '"'
											]
											.else_p
											[
												if_p(chara_.n == Code::CCode_chara::LOVE)
												[// 告白キャラ
													slg_id_[bind(&SLG::Code::CCmdChara::setTarget)(chara_.val,arg1)]
												]
												.else_p
												[// Apper
													str_p("value=\"") >> boolSymbol_[bind(&SLG::Code::CCmdChara::addParam)(chara_.val,arg1)] >> '"'
												]
											]
										]
									]
								]
							]
						>> str_p("/>")[bind(&definition::newChara)(var(*this),chara_.val)]
						;

			// <phase intro="" />
			phase_	= str_p("<phase") 
						>> str_p("intro=\"") >> boolSymbol_[bind(&definition::newPhaseBall)(var(*this),arg1)] >> '"'
					>> str_p("/>")
					;

			// slg_id_属性
			slg_id_		= str_p("slg=\"") >> (*(anychar_p - '"'))[slg_id_.val = construct_<string>(arg1,arg2)] >> '"';

			// target_属性
			target_		= str_p("target=\"") >> (*(anychar_p - '"'))[target_.val = construct_<string>(arg1,arg2)] >> '"';

			// no属性
			no_		= str_p("no=\"") >> int_p[no_.val = arg1] >> '"';

			// <battle name="" demo="" side="" />
			battle_	= str_p("<battle")
							>> name_[bind(&Code::CCmdBattle::nID_)(battle_.val) = bind(&CSLGDef::getBattleEventID)(var(self.def_),arg1)]
							>> str_p("demo=\"") >> boolSymbol_[bind(&Code::CCmdBattle::bDemo_)(battle_.val)=arg1] >> '"'
							>> str_p("side=\"") >> sideID_[bind(&Code::CCmdBattle::nSide_)(battle_.val)=arg1] >> '"'
						>> str_p("/>")
						>> eps_p[bind(&definition::newEventBattle)(var(*this),battle_.val)]
						;

			// <effect name="">*</effect>
			effect_	= str_p("<effect") >> !name_ >> '>'
						>> (*(anychar_p - "</effect>"))[effect_.val = construct_<string>(arg1,arg2)]
						>> str_p("</effect>")[bind(&Effect::CSlgEffectScriptLoader::createEffectScript)(var(pEffect_),effect_.val,var(pScript_))]
					;

			// <sally max="" index="">
			//	*<chara slg="" />
			//	<map>数値,数値,・・・,数値</map>
			// </sally>
			sally_ = str_p("<sally") 
						>> str_p("max=\"") >> int_p[bind(&Code::CCmdSally::setMaxNum)(sally_.val,arg1)] >> '"'
						>> str_p("index=\"") >> int_p[bind(&Code::CCmdSally::setIndex)(sally_.val,arg1)] >> '"'
					>> ch_p('>')
					>> *(str_p("<chara") >> slg_id_[bind(&Code::CCmdSally::addChara)(sally_.val,arg1)] >> str_p("/>"))
					>> *(str_p("<out") >> slg_id_[bind(&Code::CCmdSally::addOut)(sally_.val,arg1)] >> str_p("/>"))
					>> !(str_p("<map>") 
						>> int_p[bind(&Code::CCmdSally::addIndex)(sally_.val,arg1)] 
						>> *(',' >> int_p[bind(&Code::CCmdSally::addIndex)(sally_.val,arg1)])
					>> str_p("</map>"))
					>> str_p("</sally>")[bind(&definition::newSally)(var(*this),sally_.val)]
					;

			// <vic vic="" lose="" expert="" !apper="" />
			vic_ = str_p("<vic")
					//>> str_p("type=\"") >> vicType_[bind(&Code::CCmdVicChange::nType_)(vic_.val)=arg1] >> '"'
					>> str_p("vic=\"") >> int_p[bind(&Code::CCmdVicChange::nVic_)(vic_.val)=arg1] >> '"'
					>> str_p("lose=\"") >> int_p[bind(&Code::CCmdVicChange::nLose_)(vic_.val)=arg1] >> '"'
					>> str_p("expert=\"") >> int_p[bind(&Code::CCmdVicChange::nExpert_)(vic_.val)=arg1] >> '"'
					>> !(str_p("apper=\"") >> int_p[bind(&Code::CCmdVicChange::nApper_)(vic_.val)=arg1] >> '"')
				>> str_p("/>")[bind(&definition::newVic)(var(*this),vic_.val)]
				;

			// <change_chara slg="" chara="" !(train="" !(lv="" !offset="")) />
			changechara_	= str_p("<change_chara")
							>> ( slg_id_[bind(&Code::CCmdChangeChara::sID_)(changechara_.val)=arg1]
							   | no_[bind(&Code::CCmdChangeChara::nID_)(changechara_.val)=arg1])
							>> str_p("chara=\"") 
								>> (*(anychar_p - '"'))[changechara_.s = construct_<string>(arg1,arg2)]
								>> eps_p[bind(&Code::CCmdChangeChara::sCharaID_)(changechara_.val)=changechara_.s]
							>> '"'
							>> !(str_p("train=\"") >> int_p[bind(&Code::CCmdChangeChara::nTrain_)(changechara_.val)=arg1] >> '"'
									>> !(str_p("flag=\"") >> lv_flag_[bind(&Code::CCmdChangeChara::nFlag_)(changechara_.val)=arg1] >> '"'
										>> !(str_p("offset=\"") >> int_p[bind(&Code::CCmdChangeChara::nOffset_)(changechara_.val)=arg1] >> '"')
									)
							
							)
							>> str_p("/>")[bind(&definition::newChangeChara)(var(*this),changechara_.val)]
							;

			// <slg_end !save="" />
			slg_end_	=  str_p("<slg_end")[slg_end_.val = false]
						>> !(str_p("save=\"") >> boolSymbol_[slg_end_.val = arg1] >> '"')
						>> str_p("/>")[bind(&definition::newSlgEnd)(var(*this),slg_end_.val)]
						;

			// <slg2valid />
			slg2valid_	=str_p("<slg2valid") >> str_p("/>")[bind(&definition::newSlg2Valid)(var(*this))];

			// <enemy_reset />
			//enemy_	= str_p("<enemy_reset") >> str_p("/>")[bind(&definition::newEnemy)(var(*this))]
					;

			// <phase_init />
			//phase_	= str_p("<phase_init") >> str_p("/>")
			//		>> eps_p[bind(&definition::newPhase)(var(*this))]
			//		;

			// <data_ref />
			//ref_	= str_p("<data_ref") >> str_p("/>")
			//		>> eps_p[bind(&definition::newRef)(var(*this))]
			//		;
		}
		
		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>											rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>		rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>			rule_i;
		typedef boost::spirit::rule<S, Parser::bool_closure::context_t>			rule_b;

		typedef boost::spirit::rule<S, ADV::fade_closure::context_t>			rule_f;
		typedef boost::spirit::rule<S, Sound::Code::sound_closure::context_t>	rule_sd;
		typedef boost::spirit::rule<S, SLG::addChara_closure::context_t>		rule_ac;
		typedef boost::spirit::rule<S, SLG::addCharaMap_closure::context_t>		rule_acm;
		typedef boost::spirit::rule<S, SLG::delCharaMap_closure::context_t>		rule_dcm;
		typedef boost::spirit::rule<S, SLG::msg_closure::context_t>				rule_msg;
		typedef boost::spirit::rule<S, battle_closure::context_t>				rule_btl;
		typedef boost::spirit::rule<S, chara_closure::context_t>				rule_c;
		typedef boost::spirit::rule<S, sally_closure::context_t>				rule_sally;
		typedef boost::spirit::rule<S, vic_closure::context_t>					rule_vic;
		typedef boost::spirit::rule<S, changechara_closure::context_t>			rule_cc;
		typedef boost::spirit::rule<S, action_closure::context_t>				rule_act;
		// rule
		rule		start_;
		rule_i		cond_;
		rule_s		id_,name_,slg_id_,target_;
		rule_s		call_,cond_call_;
		rule_ac		addchara_;
		rule_acm	addmap_;
		rule_s		delchara_;
		rule_dcm	delmap_;
		rule_s		setweapon_;
		rule_c		chara_;
		rule_i		no_;
		rule_msg	msg_;
		rule		state_;
		rule		wait_;
		rule		cursol_;
		rule_f		fade_;
		rule_sd		bgm_,se_;
		rule		fantazm_;
		rule_btl	battle_;
		rule_s		effect_;
		rule		phase_;
		rule_sally	sally_;
		rule_vic	vic_;
		rule_cc		changechara_;
		rule_act	action_;
		rule_b		slg_end_;
		rule		slg2valid_;
		//rule		enemy_;
		//rule		phase_,ref_;

		// シンボル
		Parser::boolsym					boolSymbol_;
		ADV::side_symbol				sideID_;
		ADV::wait_symbol				waitID_;
		Sound::Code::sound_ctrl_symbol	ctrlID_;
		ADV::fade_ctrl_symbol			fadeID_;
		ADV::fade_color_symbol			colorID_;
		SLG::factory_symbol				factoryID_;
		SLG::phase_symbol				phaseID_;
		SLG::action_symbol				actionID_;
		action_evalmove_symbol			evalMoveID_;
		action_evalhp_symbol			evalHpID_;
		SLG::action_move_symbol			moveType_;
		SLG::way_symbol					wayID_;
		SLG::add_effect_symbol			addEffectID_;
		SLG::del_effect_symbol			delEffectID_;
		SLG::chara_kind_symbol			kind_;
		SLG::chara_valuetype_symbol		valuetype_;
		SLG::chara_validtype_symbol		validtype_;
		SLG::chara_statetype_symbol		statetype_;
		SLG::chara_actvalue_symbol		actvalue_;
		SLG::expert_symbol				expertSymbol_;
		SLG::hero_symbol				heroSymbol_;
		SLG::vic_symbol					vicType_;
		SLG::lv_flag_symbol				lv_flag_;

		// 一時データとか
		CSLGContext*		pContext_;
		VM::CScript*		pScript_;

		CSlgCondLoader*						pLoader_;
		BMW::SLG::Code::CCode_cond_call*	pCall_;
		ISlgCond*							pCondList_;

		Effect::CSlgEffectScriptLoader*		pEffect_;
		// カウンタ
		int nID_;
		int nCount_;

		// 熟練度判定
		bool IsExpert(int nExpert)
		{
			// 全滅プレイ後はNORMAL扱い
			return (pContext_->getValue(Flag::WIPEOUT)
					? SLG::Expert::NORMAL
					: pContext_->getScenarioData()->getExpertRank(pContext_->getApp()->getExec().getExpert())
					)
					== nExpert;
		}

		// 主人公判定
		bool IsHero(int nHero)
		{
			return pContext_->getApp()->getExec().getHero()==nHero;
		}

		void endScript()
		{// 安全のためretを自動挿入
			BMW::VM::Code::CCode_ret* pRet = new BMW::VM::Code::CCode_ret();
			pScript_->addCode(pRet);
		}

		void newCall(const string sName, SLG::CSLGDef& def)
		{// 他のサブルーチン呼び出し
			BMW::VM::Code::CCode_call* pCall = new BMW::VM::Code::CCode_call();
			pCall->setState(def.getScriptID(sName));
			pScript_->addCode(pCall);
		}

		void newCondCall(const string sName, SLG::CSLGDef& def)
		{// 他のサブルーチン呼び出し
			pCall_ = new Code::CCode_cond_call();
			pCall_->setState(def.getScriptID(sName));
			pScript_->addCode(pCall_);
		}

		void newMsg(SLG::Code::CCmdMsg& cmd)
		{
			SLG::CSlgCmdFactory::createMsg(cmd,*pContext_,pScript_);
		}

		void newMsgState(bool bVisible)
		{
			SLG::CSlgCmdFactory::createMsgState(bVisible,pScript_);
		}

		void newWait(int nWait)
		{
		
			VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
			int nCall;
			if(nWait==ADV::Wait::INPUT)	nCall=Rule::WAIT_INPUT;
			//ef(nWait==ADV::Wait::SALLY) nCall=Rule::WAIT_SALLY;
			else						nCall=Rule::WAIT_FADE;

			pCall->setState(nCall);
			pScript_->addCode(pCall);
		}

		void newCursolEnable(bool bEnable)
		{
			VM::Code::CCode_cursol_enable* pCursol = new VM::Code::CCode_cursol_enable();
			pCursol->visible(bEnable);
			pScript_->addCode(pCursol);
		}

		void newBgm(Sound::Code::CCmdSound& cmd)
		{
			Sound::Code::CSoundCmdFactory::createBgm(cmd, pContext_, pScript_);
		}

		void newSe(Sound::Code::CCmdSound& cmd)
		{
			if(cmd.getCtrl()==Sound::Ctrl::WAIT)
				ADV::CAdvCmdFactory::createSeWait(cmd,pScript_,false);
			else
				Sound::Code::CSoundCmdFactory::createSe(cmd, pContext_, pScript_);
		}

		void newFade(ADV::CCmdFade& fade)
		{
			ADV::CAdvCmdFactory::createFade(fade,*pContext_,pScript_);
		}

		void newAddChara(SLG::Code::CCmdAddChara& add)
		{
			SLG::CSlgCmdFactory::createAddChara(add,*pContext_,pScript_);
		}

		void newAddCharaMap(SLG::Code::CCmdAddCharaMap& add)
		{
			SLG::CSlgCmdFactory::createAddCharaMap(add,*pContext_,pScript_);
		}

		void newDelCharaID(int nSlg)
		{
			SLG::CSlgCmdFactory::createDelChara(nSlg,pScript_);
		}

		void newDelChara(const string& sSlg)
		{
			SLG::CSlgCmdFactory::createDelChara(sSlg,*pContext_,pScript_);
		}

		void newDelCharaMap(SLG::Code::CCmdDelCharaMap& cmd)
		{
			SLG::CSlgCmdFactory::createDelCharaMap(cmd,*pContext_,pScript_);
		}

		void newSetWeaponID(int nSlg)
		{
			SLG::CSlgCmdFactory::createSetWeapon(nSlg,pScript_);
		}

		void newSetWeapon(const string& sSlg)
		{
			SLG::CSlgCmdFactory::createSetWeapon(sSlg,*pContext_,pScript_);
		}

		bool IsCharaKind(int nID)
		{
			return nID==Code::CCode_chara::HP 
				|| nID==Code::CCode_chara::EN
				|| nID==Code::CCode_chara::SP
				|| nID==Code::CCode_chara::MENTAL;
		}

		void newChara(Code::CCmdChara& cmd)
		{
			SLG::CSlgCmdFactory::createChara(cmd,*pContext_,pScript_);
		}

		void newEventBattle(Code::CCmdBattle& cmd)
		{
			SLG::CSlgCmdFactory::cerateEventBattle(cmd,pScript_);
		}

		void newPhaseBall(bool b)
		{
			SLG::CSlgCmdFactory::createPhaseBall(b,pScript_);
		}

		void newSally(Code::CCmdSally& cmd)
		{
			SLG::CSlgCmdFactory::createSally(cmd,*pContext_,pScript_);
		}

		void newVic(Code::CCmdVicChange& cmd)
		{
			SLG::CSlgCmdFactory::createVic(cmd,pScript_);
		}

		void newChangeChara(Code::CCmdChangeChara& cmd)
		{
			SLG::CSlgCmdFactory::createChangeChara(cmd,*pContext_,pScript_);
		}

		void newWaitFrame(int nFrame)
		{
			SLG::CSlgCmdFactory::createWaitFrame(nFrame,pScript_);
		}

		void newSlgEnd(bool bSave)
		{
			pScript_->addCode(new Code::CCode_slg_end(bSave));
		}

		void newSlg2Valid()
		{
			pScript_->addCode(new Code::CCode_slg2valid());
		}

		/*void newEnemyReset()
		{
			SLG::CSlgCmdFactory::createEnemyReset(pScript_);
		}

		void newPhase()
		{
			pScript_->addCode(new Phase::CPhase_init());
		}

		void newRef()
		{
			pScript_->addCode(new Data::CData_ref());
		}*/
	};
};

} // namespace SLG end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね