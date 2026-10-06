/**
	katze 08/08/18
	キャラ辞典パーサー
*/
#pragma once

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね


#include "CDictCharaContext.h"

namespace BMW{
namespace Dict{

struct CDictCharaParser : public boost::spirit::grammar<CDictCharaParser>
{
	CDictCharaParser(CDictCharaContext* p):pContext_(p){}
	CDictCharaContext*	pContext_;

	template<typename S>
	struct definition
	{
		definition(const CDictCharaParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			pContext_	= self.pContext_;

			// スタート
			start_ = xml_ >> dict_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <dictionary><chara></chara></dictionary>
			dict_ =	str_p("<dictionary>")
						>> *chara_
					>> str_p("</dictionary>")
					;
			
			// <chara>...</chara>
			chara_ = str_p("<chara>")

						>> str_p("<no>")
						>> int_p[bind(&definition::newItem)(var(*this),arg1)]
						>> str_p("</no>")

						>> str_p("<id>")
							>> (*(anychar_p - "</id>"))[chara_.val=construct_<string>(arg1,arg2)] 
						>> str_p("</id>")[bind(&CDictCharaItem::setCharaID)(var(pItem_),chara_.val)]

						>> str_p("<name>")
							>> (*(anychar_p - "</name>"))[chara_.val=construct_<string>(arg1,arg2)] 
						>> str_p("</name>")[bind(&CDictCharaItem::setName)(var(pItem_),chara_.val)]

						>> str_p("<ruby>")
							>> (*(anychar_p - "</ruby>"))[chara_.val=construct_<string>(arg1,arg2)] 
						>> str_p("</ruby>")[bind(&CDictCharaItem::setRuby)(var(pItem_),chara_.val)]

						>> str_p("<origin>")
							>> (*(anychar_p - "</origin>"))[chara_.val=construct_<string>(arg1,arg2)] 
						>> str_p("</origin>")[bind(&CDictCharaItem::setOrigin)(var(pItem_),chara_.val)]

						>> str_p("<ncv>")
							>> (*(anychar_p - "</ncv>"))[chara_.val=construct_<string>(arg1,arg2)] 
						>> str_p("</ncv>")[bind(&CDictCharaItem::setNcv)(var(pItem_),chara_.val)]

						>> str_p("<ncv_in>")
							>> (*(anychar_p - "</ncv_in>"))
						>> str_p("</ncv_in>")

						>> str_p("<profile>")
							>> (*(anychar_p - "</profile>"))[chara_.val=construct_<string>(arg1,arg2)] 
						>> str_p("</profile>")[bind(&CDictCharaItem::setProfile)(var(pItem_),chara_.val)]

						>> str_p("<intro>")
							>> (*(anychar_p - "</intro>"))[chara_.val=construct_<string>(arg1,arg2)] 
						>> str_p("</intro>")[bind(&CDictCharaItem::setIntro)(var(pItem_),chara_.val)]

						>> str_p("<comment>")
							>> (*(anychar_p - "</comment>"))[chara_.val=construct_<string>(arg1,arg2)] 
						>> str_p("</comment>")[bind(&CDictCharaItem::setComment)(var(pItem_),chara_.val)]

					>> str_p("</chara>")
					;
		}

		const boost::spirit::rule<S>& start() const { return start_; }

		typedef boost::spirit::rule<S>	rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>	rule_s;
			
		rule start_, xml_, dict_;
		rule_s chara_;


		// データ
		CDictCharaItem* pItem_;
		CDictCharaContext* pContext_;

		void newItem(int nNo)
		{
			pItem_ = new CDictCharaItem();
			pItem_->setNo(nNo);
			pContext_->addDictCharaItem(nNo,pItem_);
		}
	};
};

} // namespace Dict end
} // namespace BMW end
