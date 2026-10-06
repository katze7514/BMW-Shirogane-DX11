/*
	katze 05/06/28
	update 06/04/09
	update 06/04/19
	SLGパーサー
*/
#pragma once

#include "../../Chara/DB/train_grammar.h"

#include "../../Scene/IScene.h"

#include "DB/CSlgCondLoader.h"
#include "DB/ISlgCond.h"
#include "DB/CSlgBattleEventLoader.h"
#include "DB/CSlgScriptLoader.h"

#include "CSLGContext.h"

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

#include "slg_closure.h"
#include "slg_symbol.h"

namespace BMW{
namespace SLG{
class CSLGDef;

struct CSlgParser : public boost::spirit::grammar<CSlgParser>
{
	CSlgParser(CSLGDef& def,CSLGContext* p):def_(def),p_(p){ cond_.setDef(&def); battle_.setDef(&def); script_.setDef(&def_); script_.setSLGContext(p); }
	CSLGDef&		def_;
	CSLGContext*	p_;
	CSlgCondLoader	cond_;
	CSlgCondLoader* getCondLoader(){ return &cond_; }
	CSlgBattleEventLoader	battle_;
	CSlgBattleEventLoader* getBattleLoader(){ return &battle_; }
	CSlgScriptLoader	script_;
	CSlgScriptLoader*	getScriptLoader(){ return &script_; }

	template<typename S>
	struct definition
	{
		definition(const CSlgParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// コンテキストの設定
			pContext_ = self.p_;

			start_ = xml_ 
					>> eps_p[var(pLoader_) = const_cast<CSlgParser&>(self).getCondLoader()]
					>> eps_p[var(pBattleLoader_) = const_cast<CSlgParser&>(self).getBattleLoader()]
					>> eps_p[var(pScriptLoader_) = const_cast<CSlgParser&>(self).getScriptLoader()]
					>> slg_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <slg>
			// !<factory no="" />
			// <map src="" />
			// <victory src="" />
			// !<effect src="" />
			// <train></train>*n
			// !<flag></flag>
			//	<id></id>
			//  !<def></def>
			// <vic></vic>
			// <lose></lose>
			// <expert></expert>
			// <battle>*n
			// <fun ></fun>*n
			// </slg>
			slg_ = str_p("<slg>")
					>>	!factory_
					>>	map_
					>>	victory_
					>>	!effect_
					>>	eps_p[var(nCount_)=0]
					>>	*(cond_ | train_[bind(&definition::setTrain)(var(*this),var(self.def_),arg1)])
					>>	!flagdef_
					>>	id_map_
					>>	!def_
					>>	!(vic_ >> lose_ >> !expert_)
					>>	*battle_
					>>	*fun_
				>> str_p("</slg>")
			;

			// <factory no="" />
			factory_ = str_p("<factory") 
						>> str_p("id=\"") 
							>> factoryID_[bind(&SLG::CSLGDef::setFactory)(var(self.def_),arg1)]
						>> '"'
					>> str_p("/>")
					;

			// <map src="" />
			map_	= str_p("<map")
					>> src_[map_.val=arg1]
					>> str_p("/>")
					>> eps_p[bind(&SLG::CSLGDef::setMap)(var(self.def_),map_.val)]
					;

			// <victory src="" />
			victory_= str_p("<victory")
					>> src_[bind(&definition::setVictory)(var(*this),arg1)]
					>> str_p("/>")
					;

			// <effect src="" />
			effect_	= str_p("<effect")
						>> src_[effect_.val=arg1]
					>> str_p("/>")
					>> eps_p[bind(&SLG::CSLGDef::setEffect)(var(self.def_),effect_.val)]
					;

			// src属性
			src_	= str_p("src=\"")	>> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';

			// <flag>
			//	*<id name="" init="" />
			// </flag>
			flagdef_ = str_p("<flag>")
						>> eps_p[var(nID_)=0]
						>> *(
								str_p("<id")
									>> name_[bind(&SLG::CSLGDef::setFlagID)(var(self.def_),arg1,var(nID_))]
									>> !(str_p("init=\"") >> int_p[bind(&CSLGContext::initValue)(var(pContext_),arg1,var(nID_))] >> '"')
									>> eps_p[++var(nID_)]
								>> str_p("/>")
							)
					>> str_p("</flag>")
					;

			// <id>
			//	*<map />
			// </id>
			id_map_ = str_p("<id>")
						>> eps_p[var(nID_)=0]
						>> *map_id_
					>> str_p("</id>")
					;

			// <map slg="" />
			map_id_	= str_p("<map")
						>> slg_id_[bind(&SLG::CSLGDef::setSlgID)(var(self.def_),arg1,var(nID_)++)]
					>> str_p("/>")
					;

			// <def>
			//	<fun name="" />
			// </def>
			def_	= str_p("<def>")
						>> *fun_def_
					>> str_p("</def>")
					;

			// <vic !init="">+<check></check></vic>
			vic_	= str_p("<vic")
						>> !(str_p("init=\"") >> int_p[bind(&CSLGContext::initVictory)(var(pContext_),arg1)] >> '"')
						>> !(str_p("hard=\"") >> int_p[bind(&CSLGContext::initVictoryHard)(var(pContext_),arg1)] >> '"')
					>> str_p(">")
						>> +check_[bind(&definition::setVicCond)(var(*this),var(self.def_))]
					>> str_p("</vic>")
					;

			// <lose init="">+<check></check></lose>
			lose_	= str_p("<lose")
						>> !(str_p("init=\"") >> int_p[bind(&CSLGContext::initLose)(var(pContext_),arg1)] >> '"')
						>> !(str_p("hard=\"") >> int_p[bind(&CSLGContext::initLoseHard)(var(pContext_),arg1)] >> '"')
					>> str_p(">")
						>> +check_[bind(&definition::setLoseCond)(var(*this),var(self.def_))]
					>> str_p("</lose>")
					;

			// <expert init="">+<check></check></expert>
			expert_	= str_p("<expert") 
						>> !(str_p("init=\"") >> int_p[bind(&CSLGContext::initExpertCond)(var(pContext_),arg1)] >> '"')
						>> !(str_p("hard=\"") >> int_p[bind(&CSLGContext::initExpertHard)(var(pContext_),arg1)] >> '"')
					>> str_p(">")
						>> +check_[bind(&definition::setExpertCond)(var(*this),var(self.def_))]
					>> str_p("</expert>")
					;

			// <check>cond</check>
			check_	= str_p("<check>")
						>>	(*(anychar_p - "</check>"))[check_.val = construct_<string>(arg1,arg2)]
						>>	eps_p[var(pCondList_) = bind(&CSlgCondLoader::createCond)(var(pLoader_),check_.val)]
					>> str_p("</check>")
					;
			
			// <fun name="" />
			fun_def_= str_p("<fun") 
						>> name_[bind(&SLG::CSLGDef::setScriptID)(var(self.def_),arg1)]
					>> str_p("/>")
					;

			// <battle name="">
			// </battle>
			battle_	= str_p("<battle") >> name_[battle_.pre=arg1] >> '>'
						>>	(*(anychar_p - "</battle>"))[battle_.val = construct_<string>(arg1,arg2)]
						>>	eps_p[var(pBattleEvent_) = bind(&CSlgBattleEventLoader::createBattleEvent)(var(pBattleLoader_),battle_.val)]
						>>	eps_p[bind(&CSLGDef::setBattleEvent)(var(self.def_),battle_.pre,var(pBattleEvent_))]
					>> str_p("</battle>")
					;

			// <fun name="">
			// *script_
			// </fun>
			fun_	= str_p("<fun") >> name_[fun_.pre = arg1] >> '>'
						>>	(*(anychar_p - "</fun>"))[fun_.val = construct_<string>(arg1,arg2)]
						>>	eps_p[var(pScript_) = bind(&CSlgScriptLoader::createScript)(var(pScriptLoader_),fun_.val)]
					>> str_p("</fun>")
					>> eps_p[bind(&definition::newScript)(var(*this),fun_.pre,var(self.def_))]
					;

			// name属性
			name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';

			// slg_id_属性
			slg_id_	= str_p("slg=\"") >> (*(anychar_p - '"'))[slg_id_.val = construct_<string>(arg1,arg2)] >> '"';
			
			// <cond expert="">*script_</cond>
			cond_	= str_p("<cond") 
					>> (str_p("expert=\"") >> expertSymbol_[cond_.val=arg1] >> '"' >> '>'
							>> if_p(bind(&definition::IsExpert)(var(*this),cond_.val))
							   [*train_[bind(&definition::setTrain)(var(*this),var(self.def_),arg1)]]
							   .else_p
							   [*(anychar_p - "</cond>")])
				>> str_p("</cond>")
				;

		}
		
		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>											rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>		rule_s;
		typedef boost::spirit::rule<S, Parser::ss_closure::context_t>			rule_ss;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>			rule_i;

		// rule
		rule		start_,xml_,slg_;
		rule		factory_,victory_;
		rule_s		map_;
		rule_s		effect_;
		rule		flagdef_,id_map_,map_id_;
		rule		def_,fun_def_;
		rule		vic_,lose_,expert_;
		rule_s		check_;
		rule_ss		battle_,fun_;
		rule_i		cond_;
		rule_s		src_,id_,name_,slg_id_;
		
		// grammar
		Chara::train_grammar train_;

		// シンボル
		Parser::boolsym		boolSymbol_;
		SLG::factory_symbol	factoryID_;
		SLG::expert_symbol	expertSymbol_;
		
		// 一時データとか
		CSLGContext*		pContext_;
		CSlgScriptLoader*	pScriptLoader_;
		VM::CScript*		pScript_;

		CSlgCondLoader*						pLoader_;
		ISlgCond*							pCondList_;

		CSlgBattleEventLoader*				pBattleLoader_;
		Event::CBattleEventData*			pBattleEvent_;

		// カウンタ
		int nID_;
		int nCount_;

		// 熟練度判定
		bool IsExpert(int nExpert)
		{
			// 全滅プレイ後はNORMAL扱い
			return ((pContext_->getApp()->getExec().getFlag("VICTORY",0) || pContext_->getValue(Flag::WIPEOUT))
					? Expert::NORMAL
					: Expert::HARD
					)
					== nExpert;
		}

		// 設定
		void setTrain(SLG::CSLGDef& def, const Chara::CDataCharaTrain& train)
		{
			def.setTrain(nCount_++,train);
		}

		void setVictory(const string& sSrc)
		{// 勝利条件のパネルデータを読み込む
			pContext_->getScene()->getGuiDefDB().setGuiDef(sSrc);
		}

		// スクリプト生成
		void newScript(const string sName, SLG::CSLGDef& def)
		{
			// 安全のためretを自動挿入
			BMW::VM::Code::CCode_ret* pRet = new BMW::VM::Code::CCode_ret();
			pScript_->addCode(pRet);
			def.setScript(sName,pScript_);
		}

		// 勝利条件
		void setVicCond(SLG::CSLGDef& def)
		{
			def.setVictory(pCondList_);
		}
		void setLoseCond(SLG::CSLGDef& def)
		{
			def.setLose(pCondList_);
		}
		void setExpertCond(SLG::CSLGDef& def)
		{
			def.setExpert(pCondList_);
		}
	};
};

} // namespace SLG end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね