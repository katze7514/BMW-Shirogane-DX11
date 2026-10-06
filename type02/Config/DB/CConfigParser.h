/**
	katze 05/03/22
	コンフィグ（設定ファイル）DBの解析器
*/
#pragma once

#include "CConfigDB.h"

namespace BMW{
namespace Config{
struct CConfigParser : public boost::spirit::grammar<CConfigParser>
{
	CConfigParser(CConfigDB& db):db_(db){}
	CConfigDB& db_;

	template<typename S>
	struct definition
	{
		definition(const CConfigParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// スタート
			start_ = xml_ >> config_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <config>
			// <confinfo />*n
			// </config>
			config_	= str_p("<config>")
					>> eps_p[var(nID_)=0]
					>> *confinfo_
					>> str_p("</config>")
					;

			// <confinfo id="" src="" />
			confinfo_ = str_p("<confinfo")
						>> id_[bind(&definition::setID)(var(*this),var(self.db_),arg1)]
						>> src_[bind(&definition::setFile)(var(*this),var(self.db_),arg1)]
						>> str_p("/>")
						>> eps_p[var(nID_)++]
						;

			// id属性
			id_	= str_p("id=\"")
						>> (*(anychar_p - '"'))[id_.val = construct_<string>(arg1,arg2)] 
						>> '"'
					;

			// ソース属性
			src_	= str_p("src=\"")
						>> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] 
						>> '"'
					;
		}

		const boost::spirit::rule<S>& start() const { return start_; } 

		// typedef
		typedef boost::spirit::rule<S> rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t> rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t> rule_i;

		// ルール
		rule	start_,xml_;
		rule	config_;
		rule	confinfo_;
		rule_s	id_,src_;

		// 一時データ
		int nID_;
		// 設定
		void setID(CConfigDB& db,const string& sID){ db.writeMapID(sID,nID_); }
		void setFile(CConfigDB& db,const string& sFile){ db.writeMapFile(nID_,sFile); }
	};
};

} // namespace Config end
} // namespace BMW end