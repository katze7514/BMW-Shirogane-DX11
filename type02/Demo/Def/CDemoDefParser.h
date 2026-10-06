/*
	katze 06/03/25
	デモ定義パーサ
*/
#pragma once

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

#include "CDemoDef.h"
namespace BMW{
namespace Demo{
class CDemoDef;

struct msgcondtype_symbol : public boost::spirit::symbols<>
{
	msgcondtype_symbol()
	{
		add
			("HP",		CDemoMsgCondBase::HP)
			("CHARA",	CDemoMsgCondBase::CHARA)
			("RANDOM",	CDemoMsgCondBase::RANDOM)
		;
	}
};

struct CDemoDefParser : public boost::spirit::grammar<CDemoDefParser>
{
	CDemoDefParser(CDemoDef& def, CDemoContext* p):def_(def),p_(p){}
	CDemoDef& def_;
	CDemoContext* p_;
	
	template<typename S>
	struct definition
	{
		definition(const CDemoDefParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// スタート
			start_ = xml_ >> demodef_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// name属性
			name_		= str_p("name=\"") >> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';

			// src属性
			src_		= str_p("src=\"") >> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';

			// <demodef>
			//	<symboldef src="" />
			//	<attack></attack>
			//	<hit></hit>
			//	<defence></defence>
			//	<avoid></avoid>
			//	<msgdef src="" />
			//	<msg_cond name=""></msg_cond>
			//	<include src="" />
			// </demodef>
			demodef_ = str_p("<demodef>")
						>> !(str_p("<symboldef") 
								>> src_[bind(&CDemoSymbolDB::setSymbol)(var(self.def_.getSymbolDB()),arg1)] 
							>> str_p("/>"))
						>> !attack_ >> !hit_ >> !defence_ >> !avoid_
						>> !(str_p("<msgdef") 
								>> src_[bind(&CDemoMsgDB::setDemoMsg)(var(self.def_.getMsgDB()),arg1,var(self.p_))] 
							>> str_p("/>"))
						>>	*msg_cond_
						>>	*(str_p("<include")
							>> src_[bind(&CDemoDef::setDemoDef)(var(self.def_),arg1,var(self.p_))]
							>> str_p("/>"))
					>> str_p("</demodef>");

			// <attack> +<symbol name="" ratio="" /> </attack>
			attack_	= str_p("<attack>")
						>> eps_p[var(nSymbolCond_)=CDemoDef::ATTACK]
						>> +symbol_
					>> str_p("</attack>");
			// <hit> +<symbol name="" ratio="" /> </hit>
			hit_	= str_p("<hit>")
						>> eps_p[var(nSymbolCond_)=CDemoDef::HIT]
						>> +symbol_
					>> str_p("</hit>");
			// <defence> +<symbol name="" ratio="" /> </defence>
			defence_	= str_p("<defence>")
						>> eps_p[var(nSymbolCond_)=CDemoDef::DEFENCE]
						>> +symbol_
					>> str_p("</defence>");
			// <avoid> +<symbol name="" ratio="" /> </avoid>
			avoid_	= str_p("<avoid>")
						>> eps_p[var(nSymbolCond_)=CDemoDef::AVOID]
						>> +symbol_
					>> str_p("</avoid>");

			// <symbol name="" ratio="" />
			symbol_	= str_p("<symbol") 
						>> eps_p[bind(&definition::newSymbolCond)(var(*this),var(self.def_))]
						>> name_[symbol_.val=bind(&CDemoSymbolDB::getID)(var(self.def_.getSymbolDB()),arg1)]
						>> eps_p[bind(&CDemoSymbolCond::setSymbolID)(var(pSymbolCond_),symbol_.val)]
						>> str_p("ratio=\"") >> int_p[bind(&CDemoSymbolCond::setRatio)(var(pSymbolCond_),arg1)] >> '"'
					>> str_p("/>");

			// <msg_cond name="">
			//	+<cond type="" cond="" !rand="" msg="" />
			// </msg_cond>
			msg_cond_ = str_p("<msg_cond") >> name_[bind(&definition::newMsgCond)(var(*this),var(self.def_),arg1)] >> '>'
						>> +cond_
					  >> str_p("</msg_cond>");

			// msg属性
			msg_		= str_p("msg=\"") >> (*(anychar_p - '"'))[msg_.val = construct_<string>(arg1,arg2)] >> '"';

			// <cond type="" cond="" !rand="" msg="" />
			cond_ = str_p("<cond")
					>> eps_p[bind(&definition::newMsgCondBase)(var(*this))]
					>> str_p("type=\"") >> type_[bind(&CDemoMsgCondBase::setType)(var(pMsgCondBase_),arg1)] >> '"'
					>> str_p("cond=\"")
					>> if_p(bind(&CDemoMsgCondBase::getType)(var(pMsgCondBase_))==CDemoMsgCondBase::CHARA)
					[str_[cond_.val=bind(&katzeSDK::Misc::CStringMap::getValue)(var(Chara::Const::charaID_),arg1)]]
					.else_p
					[int_p[cond_.val=arg1]]
					>> '"'
					>> eps_p[bind(&CDemoMsgCondBase::setCond)(var(pMsgCondBase_),cond_.val)]
					>> !(str_p("rand=\"") >> int_p[bind(&CDemoMsgCondBase::setRand)(var(pMsgCondBase_),arg1)] >> '"')
					>> msg_[cond_.val = bind(&CDemoMsgDB::getDemoMsgID)(var(self.def_.getMsgDB()),arg1)]
					>> eps_p[bind(&CDemoMsgCondBase::setMsgID)(var(pMsgCondBase_),cond_.val)]
				 >> str_p("/>");

			str_ = (*(anychar_p - '"'))[str_.val = construct_<string>(arg1,arg2)];
		}

		
		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>											rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>		rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>			rule_i;
		
		// ルール
		rule	start_,xml_,demodef_;
		rule_s	name_,src_,msg_;
		rule	attack_,hit_,defence_,avoid_;
		rule_i	symbol_;
		rule	msg_cond_;
		rule_i	cond_;
		rule_s	str_;

		// シンボル
		msgcondtype_symbol type_;

		// 生成
		// シンボル
		int					nSymbolCond_; // 現在設定中のIndex
		CDemoSymbolCond*	pSymbolCond_;
		// メッセージ
		CDemoMsgCond*		pMsgCond_;
		CDemoMsgCondBase*	pMsgCondBase_;

		// メソッド
		void newSymbolCond(CDemoDef& def)
		{
			pSymbolCond_ = new CDemoSymbolCond();
			def.addSymbolCond(pSymbolCond_,nSymbolCond_);
		}

		void newMsgCond(CDemoDef& def, const string& sName)
		{
			pMsgCond_ = new CDemoMsgCond();
			def.addMsgCond(pMsgCond_,sName);
		}

		void newMsgCondBase()
		{
			pMsgCondBase_ = new CDemoMsgCondBase();
			pMsgCond_->addMsgCond(pMsgCondBase_);
		}
	};
};


} // namespace Demo end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね