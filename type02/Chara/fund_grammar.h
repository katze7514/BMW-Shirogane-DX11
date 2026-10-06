/*
	katze 05/03/05
	基礎ステータス解析器
*/
#pragma once

namespace BMW{
namespace Chara{

struct fund_grammar : public boost::spirit::grammar<fund_grammar, fund_closure::context_t>
{
	template<typename S>
	struct definition
	{
		definition(const fund_grammar& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// <fund strength="" magic="" hit="" avoid="" defence="" skill="" sp="" />
			start_ = str_p("<fund")
					>> str_p("strength=\"") >> int_p[bind(&Chara::CStatusFund::setStrength)(self.val,arg1)] >> '"'
					>> str_p("magic=\"")	>> int_p[bind(&Chara::CStatusFund::setMagic)(self.val,arg1)]	>> '"'
					>> str_p("hit=\"")		>> int_p[bind(&Chara::CStatusFund::setHit)(self.val,arg1)]		>> '"'
					>> str_p("avoid=\"")	>> int_p[bind(&Chara::CStatusFund::setAvoid)(self.val,arg1)]	>> '"' 
					>> str_p("defence=\"")	>> int_p[bind(&Chara::CStatusFund::setDefence)(self.val,arg1)]	>> '"' 
					>> str_p("skill=\"")	>> int_p[bind(&Chara::CStatusFund::setSkill)(self.val,arg1)]	>> '"'
					>> str_p("sp=\"")		>> int_p[bind(&Chara::CStatusFund::setSP)(self.val,arg1)]		>> '"'
					>> str_p("/>")
					;
		}

		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S> rule;
		rule start_;
	};
};

} // namespace Chara end
} // namespace BMW end