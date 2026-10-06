/*
	katze 05/05/01
	顔マップパーサー
*/
#pragma once

#pragma warning(disable:4512) // 代入演算子作れね
//#include "ConstFace.h"

namespace BMW{
namespace Face{

class CFaceMap;
struct CFaceMapParser : public boost::spirit::grammar<CFaceMapParser>
{
	CFaceMapParser(CFaceMap& db):db_(db){}
	CFaceMap& db_;

	template<typename S>
	struct definition
	{
		definition(const CFaceMapParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// スタート
			start_ = xml_ >> facemap_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <facemap>
			//  <face id="" name="" src="" />*n
			// </facemap>
			facemap_	=	str_p("<facemap>")
						>>	eps_p[var(nCount_)=0]
						>>	*(face_ | eq_)
						>>	str_p("</facemap>")
						;

			// <face id="" src="" />
			face_		=	str_p("<face")
						>>	id_[bind(&definition::newDB)(var(*this),var(self.db_),arg1)]
						>>	name_[bind(&Face::CFaceDB::setName)(var(pDB_),arg1)]
						>>	src_[face_.val=arg1]
						>>	str_p("/>")[bind(&Face::CFaceDB::setFaceDB)(var(pDB_),face_.val)]
						;

			// <eq id="">
			//	 <face id="" />
			// </eq>
			eq_			=	str_p("<eq") >> id_[var(nFaceID_)=bind(&katzeSDK::Misc::CStringMap::getValue)(var(Face::Const::faceID_),arg1)] >> '>'
						>>	+(str_p("<face") 
							>> id_ [bind(&definition::addEq)(var(*this),var(self.db_),arg1)]
						>> str_p("/>"))
						>>	str_p("</eq>")
						;			

			// id属性
			id_	= str_p("id=\"")	>> (*(anychar_p - '"'))[id_.val = construct_<string>(arg1,arg2)] >> '"';

			// name属性
			name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';

			// src属性
			src_	= str_p("src=\"")	>> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';
		};

		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>										rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>	rule_s;

		// rule
		rule	start_,xml_,facemap_;
		rule_s	face_,id_,name_,src_,eq_;

		// 一時データ
		CFaceDB* pDB_;
		int		 nFaceID_; // EQ用
		// カウンタ
		int nCount_;
		// データ生成
		void newDB(CFaceMap& db, const string& sID)
		{
			Face::Const::faceID_.writeMap(sID,nCount_);
			pDB_=new CFaceDB();
			db.setFaceDB(pDB_,nCount_++);
		}

		void addEq(CFaceMap& db, const string& sEQ)
		{
			int nEQ		= Face::Const::faceID_.getValue(sEQ);
			db.addEqFaceMap(nFaceID_,nEQ);
		}
	};
};

} // namespace Face end
} // namespace BMW end

#pragma warning(default:4512) // 代入演算子作れね