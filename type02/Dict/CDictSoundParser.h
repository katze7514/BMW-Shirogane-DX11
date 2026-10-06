/**
	katze 08/08/18
	サウンド辞典パーサー
*/
#pragma once

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね


#include "CDictSoundContext.h"

namespace BMW{
namespace Dict{

struct CDictSoundParser : public boost::spirit::grammar<CDictSoundParser>
{
	CDictSoundParser(CDictSoundContext* p):pContext_(p){}
	CDictSoundContext*	pContext_;

	template<typename S>
	struct definition
	{
		definition(const CDictSoundParser& self)
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
						>> *sound_
					>> str_p("</dictionary>")
					;
			
			// <chara>...</chara>
			sound_ = str_p("<sound>")

						>> str_p("<no>")
						>> int_p[bind(&definition::newItem)(var(*this),arg1)]
						>> str_p("</no>")

						>> str_p("<id>")
							>> (*(anychar_p - "</id>"))[sound_.val=construct_<string>(arg1,arg2)] 
						>> str_p("</id>")[bind(&CDictSoundItem::setID)(var(pItem_),sound_.val)]

						>> str_p("<title>")
							>> (*(anychar_p - "</title>"))[sound_.val=construct_<string>(arg1,arg2)] 
						>> str_p("</title>")[bind(&CDictSoundItem::setTitle)(var(pItem_),sound_.val)]

						>> str_p("<composer>")
							>> (*(anychar_p - "</composer>"))[sound_.val=construct_<string>(arg1,arg2)] 
						>> str_p("</composer>")[bind(&CDictSoundItem::setComposer)(var(pItem_),sound_.val)]

						>> str_p("<use>")
							>> (*(anychar_p - "</use>"))[sound_.val=construct_<string>(arg1,arg2)] 
						>> str_p("</use>")[bind(&CDictSoundItem::setUse)(var(pItem_),sound_.val)]

						>> str_p("<comment>")
							>> (*(anychar_p - "</comment>"))[sound_.val=construct_<string>(arg1,arg2)] 
						>> str_p("</comment>")[bind(&CDictSoundItem::setComment)(var(pItem_),sound_.val)]

					>> str_p("</sound>")
					;
		}

		const boost::spirit::rule<S>& start() const { return start_; }

		typedef boost::spirit::rule<S>	rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>	rule_s;
			
		rule start_, xml_, dict_;
		rule_s sound_;


		// データ
		CDictSoundItem* pItem_;
		CDictSoundContext* pContext_;

		void newItem(int nNo)
		{
			pItem_ = new CDictSoundItem();
			pItem_->setNo(nNo);
			pContext_->addSoundItem(nNo,pItem_);
		}
	};
};

} // namespace Dict end
} // namespace BMW end
