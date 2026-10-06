/*
	katze 06/02/21
	パネルのパス指定を分解する文法
*/
#pragma once

namespace BMW{
namespace GUI{

struct IPanelParser : public boost::spirit::grammar<IPanelParser>
{
	// 分解されたIDリストが格納される
	list<string> listPath_;

	template <typename S>
	struct definition
	{
		definition(const IPanelParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// IDを/で区切って指定する。左側を親として検索していく
			start_ = str_[push_back_a(const_cast<IPanelParser&>(self).listPath_)]
						>> *str2_;

			str_ = (*(anychar_p - '/'))[str_.val = construct_<string>(arg1,arg2)];

			str2_ = '/' >> str_[push_back_a(const_cast<IPanelParser&>(self).listPath_)];
		}
		
		const boost::spirit::rule<S>& start() const { return start_; }

		typedef boost::spirit::rule<S>										rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>	rule_s;
		
		rule	start_,str2_;
		rule_s	str_;
	};
};

} // namespace GUI end
} // namespace BMW end