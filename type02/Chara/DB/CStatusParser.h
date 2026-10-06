/*
	katze 05/03/05
	成長ステータスの解析器
*/
#pragma once

#include "CCharaDB.h"

namespace BMW{
namespace Chara{
struct CStatusParser : public boost::spirit::grammar<CStatusParser>
{
	CStatusParser(CCharaDB& db):db_(db){}
	CCharaDB& db_;

	template<typename S>
	struct definition
	{
		definition(const CStatusParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// スタートルール
			start_	= !xml_ 
						>> status_
					;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// ルートタグ
			// <status>
			//	<growth></growth>*16
			// </status>
			status_ = str_p("<status>")
						>> *growth_
					>> str_p("</status>")
					;

			// 成長
			// <growth type="">
			//	<growthinfo lv=""></growthinfo>*n
			// </growth>
			growth_	= str_p("<growth") >> str_p("type=\"") >> growthType_[growth_.val=arg1] >> '"' >> ch_p('>')
					>> *growthinfo_[bind(&Chara::CCharaDB::addStatusData)(var(self.db_),arg1,growth_.val)]
					>> str_p("</growth>")
					;

			// <growthinfo lv="">
			//	<fund />
			// </growthinfo>
			growthinfo_ = str_p("<growthinfo") >> str_p("lv=\"") >> int_p[bind(&Chara::CStatusGrowthStatus::setLv)(growthinfo_.val,arg1)] >> '"' >> ch_p('>')
							>> fund_[bind(&Chara::CStatusGrowthStatus::setFund)(growthinfo_.val,arg1)]
						>> str_p("</growthinfo>")
						;
		}

		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>									rule;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>	rule_i;
		typedef boost::spirit::rule<S, status_g_closure::context_t>		rule_st;

		// ルール
		rule		start_;
		rule		xml_,status_;
		rule_i		growth_;
		rule_st		growthinfo_;

		// 文法
		fund_grammar	fund_;

		// シンボル
		growthsym	growthType_;
	};
};

} // namespace Chara end
} // namespace BMW end