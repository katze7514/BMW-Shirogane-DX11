/*
	katze 05/03/04
	スプライト部分の文法
*/
#pragma once

#include "CPlaneLoader2.h"
#include "CSpriteDB.h"

namespace BMW{
namespace Draw{
struct CSpriteParser : public boost::spirit::grammar<CSpriteParser, Parser::string_closure::context_t>
{
	CSpriteParser(CSpriteDB& db, const string& sPrefix):db_(db),sPrefix_(sPrefix){}
	CSpriteDB& db_;
	string sPrefix_;

	template<typename S>
	struct definition
	{
		definition(const CSpriteParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// スタート
			start_ = !xml_ >> eps_p[bind(&definition::setPrefix)(var(*this),self.sPrefix_)] >> db_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <spritedef>
			// *(<planedef /> | <include />)
			// <sprite></sprite>*n
			// </spritedef>
			db_		= str_p("<spritedef>")
						>> *(loader_ | include_)
						>> *sprite_
						>> str_p("</spritedef>")
					;

			// PlaneLoaderに渡す設定ファイル
			// <planedef src="" />
			loader_	= str_p("<planedef")
						>> src_[loader_.val=arg1]
						>> str_p("/>")
						>> eps_p[bind(&definition::setPlane)(var(*this),var(self.db_.getLoader()),loader_.val)]
					;

			// <include src="" !pre="" />
			include_	= str_p("<include")
						>> src_[include_.val=arg1]
						>> eps_p[bind(static_cast<void(string::*)()>(&string::clear))(include_.pre)]
						>> !pre_[include_.pre=arg1] 
						>> str_p("/>")
						>> eps_p[bind(&CSpriteDB::setSpriteDBPre)(var(self.db_),include_.val, var(sPrefix_)+include_.pre)]
						;

			// <include src="" />
			//include_	= str_p("<include") 
			//				>> src_[bind(&CSpriteDB::setSpriteDB)(var(self.db_),arg1)]
			//			>> str_p("/>")
			//			;

			// src属性
			src_	= str_p("src=\"")	>> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';

			// pre属性
			pre_	= str_p("pre=\"")	>> (*(anychar_p - '"'))[pre_.val = construct_<string>(arg1,arg2)] >> '"';

			// スプライト定義
			sprite_	= str_p("<sprite")	>> name_[sprite_.name=arg1] >> '>'
						>> plane_[bind(&Draw::CSpriteInfoBase::setGraphicID)(sprite_.val,arg1)]
						>> rect_[bind(&Draw::CSpriteInfoBase::setRect)(sprite_.val,arg1)]
						>> !offset_[bind(&Draw::CSpriteInfoBase::setOffsetPos)(sprite_.val,arg1)]
						>> str_p("</sprite>")
						>> eps_p[bind(&Draw::CSpriteDB::addSpriteDataStr)(var(self.db_),sprite_.val,var(sPrefix_)+sprite_.name)]
					;

			// 名前属性
			name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';

			// グラフィック
			plane_	= str_p("<plane")
						//>> str_p("name=\"") >> planeSym_[plane_.val=arg1] >> '"' 
						>> name_[plane_.val=bind(&Draw::CPlaneLoader2::getPlaneID)(var(self.db_.getLoader()),var(sPrefix_)+arg1)]
						>> str_p("/>")
					;

			// 矩形
			rect_	= str_p("<rect")
						>> left_[bind(&RECT::left)(rect_.val)=arg1]
						>> top_[bind(&RECT::top)(rect_.val)=arg1]
						>> right_[bind(&RECT::right)(rect_.val)=arg1]
						>> bottom_[bind(&RECT::bottom)(rect_.val)=arg1]
						>> str_p("/>")
					;

			left_	= str_p("left=\"")	>> int_p[left_.val = arg1] >> '"';
			top_	= str_p("top=\"")	>> int_p[top_.val = arg1] >> '"';
			right_	= str_p("right=\"")	>> int_p[right_.val = arg1] >> '"';
			bottom_	= str_p("bottom=\"")>> int_p[bottom_.val = arg1] >> '"';

			// オフセット
			offset_	= str_p("<offset")	>> x_[bind(&POINT::x)(offset_.val)=arg1]
										>> y_[bind(&POINT::y)(offset_.val)=arg1]
						>> str_p("/>")
					;

			x_	= str_p("x=\"") >> int_p[x_.val = arg1] >> '"';
			y_	= str_p("y=\"") >> int_p[y_.val = arg1] >> '"';
		}

		const boost::spirit::rule<S>& start() const { return start_; }

		// 各種ルールtypedef
		typedef boost::spirit::rule<S>										rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>	rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>		rule_i;
		typedef boost::spirit::rule<S, Parser::ss_closure::context_t>		rule_ss;
		typedef boost::spirit::rule<S, Draw::sprite_closure::context_t>		rule_sp;
		typedef boost::spirit::rule<S, Draw::rect_closure::context_t>		rule_r;
		typedef boost::spirit::rule<S, Draw::point_closure::context_t>		rule_p;

		// rule
		rule	start_,xml_,db_;
		rule_ss	include_;
		rule_s	loader_;
		rule_sp	sprite_;
		rule_s	name_,src_,pre_;
		rule_i	plane_;
		rule_r	rect_;
		rule_i	left_, top_, right_, bottom_;
		rule_p	offset_;
		rule_i	x_,y_;

		// 名前のprefix
		string sPrefix_;

		void setPrefix(const string& sPrefix)
		{
			sPrefix_=sPrefix;
		}

		void setPlane(CPlaneLoader2& db, const string& sFile)
		{
			db.SetPre(sFile,sPrefix_,true);
		}
	};
};

} // namespace Draw end
} // namespace BMW end