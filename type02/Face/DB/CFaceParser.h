/*
	katze 05/03/25
	顔DBの解析
*/
#pragma once

#include "../../Draw/DB/CSpriteDB.h"
#include "CFaceDB.h"

namespace BMW{
namespace Face{
struct CFaceParser : public boost::spirit::grammar<CFaceParser>
{
	CFaceParser(CFaceDB& db,Draw::CSpriteDB& sp):db_(db),spriteDB_(sp){}
	Draw::CSpriteDB&	spriteDB_;
	CFaceDB&			db_;

	template<typename S>
	struct definition
	{
		definition(const CFaceParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// スタート
			start_ = !xml_ >> face_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <face>
			//  <spritedef />
			//	<faceinfo />*n
			// </face>
			face_	= str_p("<face>")
					>> spritedef_
					>> eps_p[var(nCount_)=0]
					>> *faceinfo_
					>> str_p("</face>")
					;

			// <spritedef src="" />
			spritedef_	= str_p("<spritedef") 
						>> src_[spritedef_.val=arg1]
						>> str_p("/>")[bind(&Draw::CSpriteDB::setSpriteDB)(var(self.spriteDB_),spritedef_.val)]
						;

			// src属性
			src_	= str_p("src=\"")	>> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';

			// <faceinfo name="">
			//	<sprite />*2
			// </faceinfo>
			faceinfo_ = str_p("<faceinfo") >> name_[bind(&Face::CFaceDB::writeFaceIDMap)(var(self.db_),arg1,var(nCount_))] >> '>'
						>> for_p(faceinfo_.count=0,faceinfo_.count<2,++faceinfo_.count)
							[
								sprite_[bind(&Face::CDataFace::setFaceID)(faceinfo_.val,arg1,faceinfo_.count)]
							]
						>> str_p("</faceinfo>")[bind(&Face::CFaceDB::addFaceData)(var(self.db_),faceinfo_.val,var(nCount_)),++var(nCount_)]
						;

			// <sprite name="" />
			sprite_	= str_p("<sprite") >> name_[sprite_.val = bind(&Draw::CSpriteDB::getSpriteID)(var(self.spriteDB_),arg1)] >> str_p("/>");

			// 名前属性
			name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';
		}

		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>										rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>	rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>		rule_i;
		typedef boost::spirit::rule<S, Face::face_closure::context_t>		rule_f;

		// rule
		rule	start_,xml_,face_;
		rule_s	spritedef_;
		rule_f	faceinfo_;
		rule_i	sprite_;
		rule_s	src_,name_;

		// カウンタ
		int nCount_;
	};
};

} // namespace Face end
} // namespace BMW end