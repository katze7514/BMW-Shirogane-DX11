/*
	katze 05/12/27

*/

#pragma once

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

#include "plane_closure.h"

namespace BMW{
namespace Draw{
class CPlaneLoader2;

struct CPlaneParser : public boost::spirit::grammar<CPlaneParser>
{
	CPlaneParser(CPlaneLoader2& def, const string& sPrefix):def_(def),sPrefix_(sPrefix){}
	CPlaneLoader2&		def_;
	string				sPrefix_;

	template<typename S>
	struct definition
	{
		definition(const CPlaneParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			start_ = xml_ >> eps_p[bind(&definition::setPrefix)(var(*this),self.sPrefix_)] >> planedef_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <planedef>
			//	*<include src="" />
			//	*plane_
			// </planedef>
			planedef_ = str_p("<planedef>")
						>> *include_
						>> *plane_
					  >> str_p("</planedef>")
					  ;

			// <include src="" !pre="" />
			include_	= str_p("<include")
						>> src_[bind(&pair<string,string>::first)(include_.val)=arg1]
						>> !pre_[bind(&pair<string,string>::second)(include_.val)=arg1]
						>> str_p("/>")
						>> eps_p[bind(&definition::Set)(var(*this),var(self.def_),include_.val)]
						;

			// <include src="" />
			//include_	= str_p("<include") 
			//				>> src_[bind(&definition::Set)(var(self.def_),arg1)]
			//			>> str_p("/>")
			//			;

			// <plane name="" src="" />
			plane_	=	str_p("<plane")
						>> name_[bind(&pair<string,string>::first)(plane_.val)=arg1]
						>> src_[bind(&pair<string,string>::second)(plane_.val)=arg1]
					>>	str_p("/>")[bind(&definition::setObj)(var(*this),var(self.def_),plane_.val)]
					;

			// name属性
			name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';

			// src属性
			src_	= str_p("src=\"")	>> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';

			// pre属性
			pre_	= str_p("pre=\"")	>> (*(anychar_p - '"'))[pre_.val = construct_<string>(arg1,arg2)] >> '"';
		}
		
		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>											rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>		rule_s;
		typedef boost::spirit::rule<S, Draw::s2_closure::context_t>				rule_ss;
		
		// rule
		rule		start_,xml_,planedef_;
		rule_ss		include_,plane_;		
		rule_s		name_,src_,pre_;

		string		sPrefix_;

		void setPrefix(const string& sPrefix)
		{
			sPrefix_=sPrefix;
		}

		void Set(CPlaneLoader2& db, const pair<string,string>& p)
		{
			db.SetPre(p.first, sPrefix_+p.second, true);
		}

		// セット
		void setObj(CPlaneLoader2& db, const pair<string,string>& p)
		{
#ifdef BMW_DEBUG
			if(db.getPlaneID(sPrefix_+p.first)>0)
				CDbg().Out("PLANE %s はすでに存在してます",(sPrefix_+p.first).c_str());
#endif
			db.setPlaneID(sPrefix_+p.first);
			CLoadCacheInfo* pInfo = new CLoadCacheInfo();
			pInfo->strFileName = db.GetReadDir() + p.second;
			smart_obj obj(pInfo);
			db.GetIDObjectManager()->setObject(db.getPlaneID(sPrefix_+p.first),obj);
		}
	};
};

} // namespace Draw end
} // namespace BMW end


#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね