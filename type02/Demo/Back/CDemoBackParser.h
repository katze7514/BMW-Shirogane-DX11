/*
	katze 06/03/23
	デモ背景パーサー
*/
#pragma once

#include "CDemoBackLine.h"

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

namespace BMW{
namespace Demo{

enum eDemoBack{
	BACK,
	FORWARD,
};

struct linetype_symbol : public boost::spirit::symbols<>
{
	linetype_symbol()
	{
		add
			("BACK",BACK)
			("FORWARD",FORWARD)
		;
	}
};

struct CDemoBackParser : public boost::spirit::grammar<CDemoBackParser>
{
	CDemoBackParser(CBackSymbolDB* db, CDemoBack* pBack):db_(db),pBack_(pBack){}
	CBackSymbolDB* db_;
	CDemoBack* pBack_;
	

	template<typename S>
	struct definition
	{
		definition(const CDemoBackParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// スタート
			start_ = !xml_
					>> eps_p[var(db_) = const_cast<CDemoBackParser&>(self).db_]
					>> eps_p[var(pBack_) = const_cast<CDemoBackParser&>(self).pBack_]
					>> demo_back_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <demo_back forward="" back="">
			//	<symboldef />
			//	<line />
			// </demo_back>
			demo_back_ = str_p("<demo_back")
							>> str_p("forward=\"") >> int_p[bind(&CDemoBack::resizeForward)(var(pBack_),arg1)] >> '"'
							>> str_p("back=\"") >> int_p[bind(&CDemoBack::resizeBack)(var(pBack_),arg1)] >> '"'
							>> ch_p('>')
						>> symboldef_
						>> +line_
						>> str_p("</demo_back>");


			// <symboldef src="" />
			symboldef_	= str_p("<symboldef")
						>> src_[bind(&CBackSymbolDB::setSymbol)(var(db_),arg1)]
						>> str_p("/>")
						;

			// src属性
			src_	= str_p("src=\"")	>> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';

			// name属性
			name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';

			// <line type="" no="" y="">
			//	<symbol name="" x="" left="" right="" move= "" />
			//	<symbol name="" x="" left="" right="" move= "" />
			//	<vel value="" />*10
			// </line>
			line_ = str_p("<line")
						>> str_p("type=\"") >> linetype_[line_.val = arg1] >> '"'
						>> str_p("no=\"") >> int_p[bind(&definition::newLine)(var(*this),line_.val,arg1)] >> '"'
						>> str_p("y=\"") >> int_p[bind(&CDemoBackLine::setY)(var(pLine_),arg1)] >> '"'
					>> str_p(">")
					>> eps_p[var(nNo_)=0]
					>> symbol_
					>> eps_p[var(nNo_)=1]
					>> symbol_
					>> vel_
				>> str_p("</line>");

			//	<symbol name="" x="" left="" right="" move= "" />
			symbol_	= str_p("<symbol") >> name_[bind(&definition::newSymbol)(var(*this),arg1)]
						>> str_p("x=\"") >> int_p[bind(&CDemoBackLine::setSymbolX)(var(pLine_),arg1,var(nNo_))] >> '"'
						>> str_p("left=\"") >> int_p[bind(&CDemoBackLine::setLeft)(var(pLine_),arg1,var(nNo_))] >> '"'
						>> str_p("right=\"") >> int_p[bind(&CDemoBackLine::setRight)(var(pLine_),arg1,var(nNo_))] >> '"'
						>> str_p("move=\"") >> int_p[bind(&CDemoBackLine::setMove)(var(pLine_),arg1,var(nNo_))] >> '"'
					  >> str_p("/>");

			// <vel>
			//	<1 value="" />～<10 value="" />
			// </vel>
			vel_ =  str_p("<vel>")
					>> for_p(vel_.val=1,vel_.val<=10,vel_.val++)
					[
					   eps_p[bind(&definition::createTag)(var(*this),vel_.val,vel_.str)]
					   >> f_str_p(vel_.str)
						   >> str_p("value=\"") >> real_p[bind(&CDemoBackLine::setVelFloat)(var(pLine_),arg1,vel_.val-1)] >> '"'
					   >> str_p("/>")
					]
					>> str_p("</vel>");
		}

		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>										rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>	rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>		rule_i;
		typedef boost::spirit::rule<S, Parser::int_str_closure::context_t>	rule_is;

		// ルール
		rule	start_,xml_,symboldef_,demo_back_;
		rule_s	src_,name_;
		rule	symbol_;
		rule_i	line_;
		rule_is	vel_;

		// シンボル
		linetype_symbol linetype_;

		// バック
		CDemoBack* pBack_;
		// ライン
		int nNo_;
		CDemoBackLine* pLine_;
		// シンボル
		CBackSymbolDB* db_;
		Task::ITaskBase* pSymbol_;

		void newLine(int nType, int nNo)
		{
			pLine_ = new CDemoBackLine();
			if(nType==BACK)	pBack_->setBackLine(pLine_,nNo);
			else			pBack_->setForwardLine(pLine_,nNo);
		}

		void newSymbol(const string& sID)
		{
			pSymbol_ = db_->createSymbolStr(sID);
			pLine_->setSymbol(pSymbol_,nNo_);
		}

		void createTag(int n, string& s)
		{
			s = "<" + CStringScanner::NumToString(n);
		}
	};
};

} // namespace Demo end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね