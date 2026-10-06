/*
	katze 05/05/18
	背景DBパーサー
*/
#pragma once

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

#include "CBackDB.h"
namespace BMW{
namespace ADV{
class CBackDB;

struct CBackParser : public boost::spirit::grammar<CBackParser>
{
	CBackParser(CBackDB& db,Draw::CSpriteDB& sprite):db_(db),spriteDB_(sprite){}
	CBackDB&			db_;
	Draw::CSpriteDB&	spriteDB_;

	template<typename S>
	struct definition
	{
		definition(const CBackParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// スタート
			start_ = xml_ >> back_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <back>
			//	<spritedef />
			//	<backinfo />*n
			// </back>
			back_	=	str_p("<back>")
						>> spritedef_
						>> eps_p[var(nCount)=0]
						>> *backinfo_
						>> str_p("</back>")
						;
						

			// <spritedef src="" />
			spritedef_	= str_p("<spritedef")
						>> src_[bind(&Draw::CSpriteDB::setSpriteDB)(var(self.spriteDB_),arg1)]
						>> str_p("/>")
						;

			// src属性
			src_	= str_p("src=\"")	>> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';

			//	<backinfo id="" name="">
			//		<sprite name="" />
			//	</backinfo>
			backinfo_ = str_p("<backinfo")
						>> eps_p[bind(&definition::newBack)(var(*this),var(self.db_))]
							>> id_[bind(&katzeSDK::Misc::CStringMap::writeMap)(var(ADV::Const::backID_),arg1,var(nCount))]
							>> eps_p[var(nCount)++]
							>> name_[bind(&ADV::CDataBack::setName)(var(pBack),arg1)]
						>> '>'
						>> sprite_[bind(&ADV::CDataBack::setSpriteID)(var(pBack),arg1)]
						>> str_p("</backinfo>")
						;

			// id属性
			id_	= str_p("id=\"")	>> (*(anychar_p - '"'))[id_.val = construct_<string>(arg1,arg2)] >> '"';

			// name属性
			name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';

			// <sprite name="" />
			sprite_	= str_p("<sprite") >> name_[sprite_.val = bind(&Draw::CSpriteDB::getSpriteID)(var(self.spriteDB_),arg1)] >> str_p("/>");
		}

		
		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>										rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>	rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>		rule_i;

		// ルール
		rule	start_,xml_,back_,backinfo_,spritedef_;
		rule_s	src_,id_,name_;
		rule_i	sprite_;

		// カウンタ
		int nCount;
		
		// 一時データ
		CDataBack* pBack;

		// 一時データ生成
		void newBack(CBackDB& db)
		{
			pBack = new CDataBack();
			db.setBackData(pBack, nCount);
		}
	};
};

} // namespace ADV end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね