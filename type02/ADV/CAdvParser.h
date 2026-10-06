/*
	katze 05/05/21
	ADVスクリプトパーサー
*/
#pragma once

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

#include "../Chara/DB/chara_symbol.h"

#include "../Sound/SoundCmd.h"
#include "../Sound/Code/CSoundCmdFactory.h"

#include "../Sound/Code/sound_symbol.h"
#include "../Sound/Code/sound_closure.h"

#include "../SLG/Context/DB/slg_symbol.h"

#include "Code/CCode_next.h"
#include "IDADV.h"
#include "adv_closure.h"
#include "adv_symbol.h"

#include "CAdvFactory.h"
namespace BMW{
namespace ADV{
class CADVContext;
class CAdvFactory;

struct CAdvParser : public boost::spirit::grammar<CAdvParser>
{
	CAdvParser(CAdvFactory& fct,CADVContext* p):fct_(fct),pContext_(p){}
	CAdvFactory&				fct_;
	CADVContext*				pContext_;

	template<typename S>
	struct definition
	{
		definition(const CAdvParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// データの取得
			pContext_	= self.pContext_;

			// スタート
			start_ = xml_ >> adv_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <adv>
			//	<backdef />
			//	<include />
			//	<fun></fun>*n
			//	<main></main>
			// </adv>
			adv_	= str_p("<adv>")
					>> !backdef_[bind(&ADV::CADVContext::setBackDB)(var(*self.pContext_),arg1)]
					>> *include_
					>> *fun_
					>> !main_
					>> str_p("</adv>")
				;

			// <backdef src="" />
			backdef_	= str_p("<backdef")
						>> src_[backdef_.val=arg1]
						>> str_p("/>")
						;

			// <include src="" />
			include_	= str_p("<include") 
							>> src_[bind(&CAdvFactory::setScript)(var(self.fct_),arg1,var(*self.pContext_))]
						>> str_p("/>")
						;

			// src属性
			src_	= str_p("src=\"")	>> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';

			// <fun name="">
			//	script系
			// </fun>
			fun_	= str_p("<fun") >> name_[fun_.val=arg1] >> '>'
					>> eps_p[bind(&definition::newScript)(var(*this),var(self.fct_),fun_.val)]
						>> *script_
					>> str_p("</fun>")
					>> eps_p[bind(&definition::endScript)(var(*this))]
					;

			// <main> script系 </main>
			main_	= str_p("<main>")
					>> eps_p[bind(&definition::newMain)(var(*this),var(self.fct_))]
						>> *script_
					>> str_p("</main>")
					>> eps_p[bind(&definition::endScript)(var(*this))]
					;

			// <msg></msg> | <state /> | <wait /> | <back/> | <bgm/> | <se/> | <fade /> | <next> | <valid> | <scenario> | <call /> | <cond /> | <train />
			script_ = msg_ | state_ | wait_ | back_ | bgm_ | se_ | fade_ | next_ | valid_ | scenario_ | call_ | cond_ | train_all_ | train_ | item_ctrl_;

			// <cond hero="">*script_</cond>
			// <cond expert="">*script_</cond>
			// <cond flag="" value="">*script_</cond>
			// 
			cond_	= str_p("<cond") 
						>> 	((str_p("hero=\"") >> heroSymbol_[cond_.val=arg1] >> '"' >> '>'
							>> if_p(bind(&definition::IsHero)(var(*this),cond_.val))
								[*script_]
								.else_p
								[*(anychar_p - "</cond>")]
							)
							|
							(str_p("expert=\"") >> expertSymbol_[cond_.val=arg1] >> '"' >> '>'
								>> if_p(bind(&definition::IsExpert)(var(*this),cond_.val))
								   [*script_]
								   .else_p
								   [*(anychar_p - "</cond>")]
							)
							|
							(str_p("flag=\"") >> (*(anychar_p - '"'))[cond_.str=construct_<string>(arg1,arg2)] >> '"'
							>> str_p("value=\"") >> int_p[cond_.val=arg1] >> '"'
							>> '>'
							>> if_p(bind(&definition::IsFlag)(var(*this),cond_.str,cond_.val))
								[*script_]
								.else_p
								[*(anychar_p - "</cond>")]
							))
					>> str_p("</cond>")
					;

			// <next id="" />
			next_	= str_p("<next") >> id_[bind(&definition::newNext)(var(*this),arg1)] >> str_p("/>");

			// <scenario />
			scenario_	= str_p("<scenario") 
						>> (str_p("type=\"") >> scnType_[scenario_.val=arg1] >> '"')
						>> str_p("/>")[bind(&definition::newScenario)(var(*this),scenario_.val)]
						;

			// <valid ctrl="">
			// *<chara id="" />
			// </valid>
			valid_	= str_p("<valid") >> str_p("ctrl=\"") >> validCtrlID_[bind(&ADV::CCmdValid::setCtrl)(valid_.val,arg1)] >> '"' 
					>> if_p(bind(&CCmdValid::getCtrl)(valid_.val)==Code::CCode_valid::CLEAR)
					[
						str_p("/>")
					].
					else_p
					[
						ch_p('>')
							>> *(str_p("<chara") >> id_[bind(&ADV::CCmdValid::addCharaList)(valid_.val,arg1)] >> str_p("/>"))
						>> str_p("</valid>")
					]
					>> eps_p[bind(&definition::newValid)(var(*this),valid_.val)]
					;

			// <msg side="" chara="" face="" !mask="">
			//	TEXT
			// </msg>
			msg_	= str_p("<msg")
						>> str_p("side=\"") >> sideID_[bind(&ADV::CCmdMsg::setSide)(msg_.val,arg1)] >> '"'
						>> str_p("chara=\"") >> (*(anychar_p - '"'))[msg_.s = construct_<string>(arg1,arg2)] >> '"'
						>> eps_p[bind(&ADV::CCmdMsg::setChara)(msg_.val,msg_.s)]
						>> str_p("face=\"") >> (*(anychar_p - '"'))[msg_.s = construct_<string>(arg1,arg2)] >> '"'
						>> !(str_p("mask=\"") >> boolSymbol_[bind(&ADV::CCmdMsg::mask)(msg_.val,arg1)] >> '"')
						>> eps_p[bind(&ADV::CCmdMsg::setFace)(msg_.val,msg_.s)]
					>> '>'
					>> (*(anychar_p - "</msg>"))[msg_.s = construct_<string>(arg1,arg2)]
					>> eps_p[bind(&ADV::CCmdMsg::setText)(msg_.val,msg_.s)]
					>> str_p("</msg>")
					>> eps_p[bind(&definition::newMsg)(var(*this),msg_.val)]
					;
			
			// <state side="" ctrl="" !flag="" />
			state_	= str_p("<state")
						>> str_p("side=\"") >> sideID_[bind(&ADV::CCmdMsgState::setSide)(state_.val,arg1)] >> '"'
						>> str_p("ctrl=\"") >> stateID_[bind(&ADV::CCmdMsgState::setCtrl)(state_.val,arg1)] >> '"'
						>> !(str_p("flag=\"") >> boolSymbol_[bind(&ADV::CCmdMsgState::flag)(state_.val,arg1)] >> '"')
					>> str_p("/>")
					>> eps_p[bind(&definition::newState)(var(*this),state_.val)]
					;
					

			// <wait (kind=""|frame="") />
			wait_	= str_p("<wait")
					>> (
						(str_p("kind=\"")
							>> waitID_[bind(&definition::newWait)(var(*this),arg1)]
						>> '"')
						|
						(str_p("frame=\"")
							>> int_p[bind(&definition::newWaitFrame)(var(*this),arg1)]
						>> '"')
						)
					>> str_p("/>")
					;

			// <back id="" />
			back_	= str_p("<back")
					>> id_[bind(&ADV::CCmdBack::setBack)(back_.val,arg1)]
					>> str_p("/>")
					>> eps_p[bind(&definition::newBack)(var(*this),back_.val)]
					;

			// id属性
			id_		= str_p("id=\"") >> (*(anychar_p - '"'))[id_.val = construct_<string>(arg1,arg2)] >> '"';

			// <bgm ctrl="" !id="" !fade="" />
			bgm_	= str_p("<bgm")
					>> str_p("ctrl=\"") >> ctrlID_[bind(&Sound::Code::CCmdSound::setCtrl)(bgm_.val,arg1)] >> '"'
					>> !(id_[bind(&Sound::Code::CCmdSound::setBgm)(bgm_.val,arg1)])
					>> !( str_p("fade=\"") >> int_p[bind(&Sound::Code::CCmdSound::setFade)(bgm_.val,arg1)] >> '"')
					>> str_p("/>")
					>> eps_p[bind(&definition::newBgm)(var(*this),bgm_.val)]
					;

			// <se ctrl="" id="" />
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
					>> str_p("/>")[bind(&definition::newFade)(var(*this),fade_.val)]
					;

			// <call name="" />
			call_	= str_p("<call")
						>> name_[call_.val = arg1]
					>> str_p("/>")[bind(&definition::newCall)(var(*this),call_.val,var(self.fct_))]
					;

			// <train id="" kind="" !type="" value="" !max="" />
			train_	= str_p("<train")
						>> id_[bind(&CCmdTrain::sTarget_)(train_.val)=arg1]
						>> str_p("kind=\"") >> trainKind_[bind(&CCmdTrain::nKind_)(train_.val)=arg1] >> '"'
						>> !(str_p("type=\"") >> trainType_[bind(&CCmdTrain::nType_)(train_.val)=arg1] >> '"')
						>> str_p("value=\"")
						>> if_p(bind(&CCmdTrain::nKind_)(train_.val)==Code::CCode_train::CHANGE
							 || bind(&CCmdTrain::nKind_)(train_.val)==Code::CCode_train::COPY) // キャラチェンジだったらID
							[(*(anychar_p-"\""))[bind(&CCmdTrain::sValue_)(train_.val)=construct_<string>(arg1,arg2)]]
						   .else_p // それ以外は整数
							[int_p[bind(&CCmdTrain::nValue_)(train_.val)=arg1]]
						>> '"'
						>> !(str_p("max=\"") >> int_p[bind(&CCmdTrain::nMax_)(train_.val)=arg1] >> '"')
					>> str_p("/>")[bind(&definition::newTrain)(var(*this),train_.val)]
					;

			// <train_all ctrl="" kind="" !type="" value="" !max="">
			//	<chara id="" />*n
			// </train_all>
			train_all_	= str_p("<train_all")
							>> !(str_p("ctrl=\"") >> trainCtrl_[bind(&CCmdTrainAll::nCtrl_)(train_all_.val)=arg1] >> '"')
							>> str_p("kind=\"") >> trainKind_[bind(&CCmdTrainAll::nKind_)(train_all_.val)=arg1] >> '"'
							>> !(str_p("type=\"") >> trainType_[bind(&CCmdTrainAll::nType_)(train_all_.val)=arg1] >> '"')
							>> str_p("value=\"") >> int_p[bind(&CCmdTrainAll::nValue_)(train_all_.val)=arg1] >> '"'
							>> !(str_p("max=\"") >> int_p[bind(&CCmdTrainAll::nMax_)(train_all_.val)=arg1] >> '"')
						>> str_p(">")
							>> *(str_p("<chara") >> id_[bind(&CCmdTrainAll::addTarget)(train_all_.val,arg1)] >> str_p("/>"))
						>> str_p("</train_all>")[bind(&definition::newTrainAll)(var(*this),train_all_.val)]
						;

			// <item_ctrl ctrl="">
			// <item id="" value="" />*n
			// </item_ctrl>
			item_ctrl_	= str_p("<item_ctrl") 
							>> !(str_p("ctrl=\"") >> itemCtrl_[bind(&CCmdItemCtrl::nCtrl_)(item_ctrl_.val)=arg1] >> '"')
							>> ">"
						>> +item_[bind(&CCmdItemCtrl::setItem)(item_ctrl_.val,arg1)]
						>> str_p("</item_ctrl>")[bind(&definition::newItemCtrl)(var(*this),item_ctrl_.val)]
						;
			item_	= str_p("<item") 
						>> str_p("id=\"") >> itemID_[bind(&pair<int,int>::first)(item_.val)=arg1] >> '"'
						>> str_p("value=\"") >> int_p[bind(&pair<int,int>::second)(item_.val)=arg1] >> '"'
						>> str_p("/>")
						;

			// name属性
			name_		= str_p("name=\"") >> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';
		}

		
		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>											rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>		rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>			rule_i;
		typedef boost::spirit::rule<S, Parser::int_str_closure::context_t>		rule_is;
		typedef boost::spirit::rule<S, ADV::msg_closure::context_t>				rule_m;
		typedef boost::spirit::rule<S, ADV::msg_state_closure::context_t>		rule_ms;
		typedef boost::spirit::rule<S, ADV::back_closure::context_t>			rule_b;
		typedef boost::spirit::rule<S, ADV::fade_closure::context_t>			rule_f;
		typedef boost::spirit::rule<S, ADV::valid_closure::context_t>			rule_v;
		typedef boost::spirit::rule<S, ADV::train_closure::context_t>			rule_t;
		typedef boost::spirit::rule<S, ADV::train_all_closure::context_t>		rule_ta;
		typedef boost::spirit::rule<S, ADV::item_ctrl_closure::context_t>		rule_ia;
		typedef boost::spirit::rule<S, ADV::item_closure::context_t>			rule_it;
		typedef boost::spirit::rule<S, Sound::Code::sound_closure::context_t>	rule_sd;

		// ルール
		rule	start_,xml_,adv_,main_,script_,include_;
		rule_s	fun_;
		rule_s	src_,id_;
		rule_s	backdef_;
		rule_is	cond_;
		rule	next_;
		rule_i  scenario_;
		rule_m	msg_;
		rule_ms state_;
		rule_b	back_;
		rule	wait_;
		rule_f	fade_;
		rule_v	valid_;
		rule_t	train_;
		rule_ta	train_all_;
		rule_sd	bgm_,se_;
		rule_s	call_,name_;
		rule_ia item_ctrl_;
		rule_it	item_;

		// シンボル
		Parser::boolsym					boolSymbol_;
		ADV::side_symbol				sideID_;
		ADV::wait_symbol				waitID_;
		Sound::Code::sound_ctrl_symbol	ctrlID_;
		ADV::state_ctrl_symbol			stateID_;
		ADV::fade_ctrl_symbol			fadeID_;
		ADV::valid_symbol				validCtrlID_;
		ADV::fade_color_symbol			colorID_;
		ADV::hero_symbol				heroSymbol_;
		ADV::train_kind_symbol			trainKind_;
		ADV::train_type_symbol			trainType_;
		ADV::train_ctrl_symbol			trainCtrl_;
		ADV::item_ctrl_symbol			itemCtrl_;
		ADV::scn_type_symbol			scnType_;
		Chara::itemsym					itemID_;
		SLG::expert_symbol				expertSymbol_;

		// データ
		VM::CScript*	pScript_;
		CADVContext*	pContext_;

		// スクリプト生成
		void newMain(CAdvFactory& fct)
		{
			pScript_ = new VM::CScript();
			fct.addMain(smart_ptr<VM::CScript>(pScript_));
		}
		void newScript(CAdvFactory& fct, const string& sID)
		{
			pScript_ = new VM::CScript();
			fct.addScript(sID, smart_ptr<VM::CScript>(pScript_));
		}
		void endScript()
		{// 忘れずにretしる！
			BMW::VM::Code::CCode_ret* pRet = new BMW::VM::Code::CCode_ret();
			pScript_->addCode(pRet);
		}

		// 主人公判定
		bool IsHero(int nHero)
		{
			return pContext_->getApp()->getExec().getHero()==nHero;
		}

		// 熟練度判定
		bool IsExpert(int nExpert)
		{
			int nExpertNum = pContext_->getApp()->getExec().getExpert();
			smart_ptr<BMW::Scenario::CDataScenario>& pScn = pContext_->getScenarioData();

			// その話の熟練度を取る前にALLなのか、50がMAX
			switch(nExpert)
			{
			case SLG::Expert::ALL_BEFORE: return EXPERT_MAX==nExpertNum || nExpertNum==pScn->getNo()-1;
			// その話の熟練度を取ったかもしれない時にALLなのか、50がMAX
			case SLG::Expert::ALL_AFTER:  return EXPERT_MAX==nExpertNum || nExpertNum==pScn->getNo();
			// 全熟練度の9割(45)以上かどうか
			case SLG::Expert::OVER_45:    return nExpertNum>=45;
			// 普通の難易度判定
			default:					  return pScn->getExpertRank(nExpertNum)==nExpert;
			}
		}

		// フラグ判定
		bool IsFlag(const string& sFlag, int nValue)
		{			
			return pContext_->getApp()->getExec().getFlag(sFlag,nValue);
		}

		void newNext(const string& sID)
		{
			ADV::Code::CCode_next* pNext = new ADV::Code::CCode_next();
			pNext->setState(pContext_->getApp()->getScenario().getScenarioID(sID));
			pScript_->addCode(pNext);
		}

		void newScenario(int nType)
		{
			ADV::CAdvCmdFactory::createScenario(nType,pScript_);
		}

		void newValid(CCmdValid& cmd)
		{
			ADV::CAdvCmdFactory::createValid(cmd,pScript_);
		}

		void newMsg(ADV::CCmdMsg& cmd)
		{
			ADV::CAdvCmdFactory::createMsg(cmd,*pContext_,pScript_);
		}

		void newState(ADV::CCmdMsgState& cmd)
		{
			ADV::CAdvCmdFactory::createMsgState(cmd,*pContext_,pScript_);
		}

		void newWait(int nWait)
		{
			VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
			int nCall;
			if(nWait==Wait::INPUT)	nCall=Rule::WAIT_INPUT;
			ef(nWait==Wait::BGM)	nCall=Rule::WAIT_BGM;
			else					nCall=Rule::WAIT_FADE;

			pCall->setState(nCall);
			pScript_->addCode(pCall);
		}

		void newWaitFrame(int nFrame)
		{
			ADV::CAdvCmdFactory::createFrameWait(nFrame,*pContext_,pScript_);
		}

		void newBack(ADV::CCmdBack& cmd)
		{
			ADV::CAdvCmdFactory::createBack(cmd,*pContext_,pScript_);
		}

		void newBgm(Sound::Code::CCmdSound& cmd)
		{
			Sound::Code::CSoundCmdFactory::createBgm(cmd, pContext_, pScript_);
		}

		void newSe(Sound::Code::CCmdSound& cmd)
		{
			if(cmd.getCtrl()==Sound::Ctrl::WAIT)
				ADV::CAdvCmdFactory::createSeWait(cmd,pScript_);
			else
				Sound::Code::CSoundCmdFactory::createSe(cmd, pContext_, pScript_);
		}

		void newFade(ADV::CCmdFade& fade)
		{
			ADV::CAdvCmdFactory::createFade(fade,*pContext_,pScript_);
		}

		void newCall(const string& sName, CAdvFactory& fct)
		{// 他のサブルーチン呼び出し
			BMW::VM::Code::CCode_call* pCall = new BMW::VM::Code::CCode_call();
			pCall->setState(fct.getScriptID(sName));
			pScript_->addCode(pCall);
		}

		void newTrain(CCmdTrain& cmd)
		{
			ADV::CAdvCmdFactory::createTrain(cmd,pScript_);
		}

		void newTrainAll(CCmdTrainAll& cmd)
		{
			ADV::CAdvCmdFactory::createTrainAll(cmd,pScript_);
		}

		void newItemCtrl(CCmdItemCtrl& cmd)
		{
			ADV::CAdvCmdFactory::createItemCtrl(cmd,pScript_);
		}
	};

};

} // namespace ADV end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね