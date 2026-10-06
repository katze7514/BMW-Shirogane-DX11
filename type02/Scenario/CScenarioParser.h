/*
	katze 05/06/15
	シナリオパーサ
*/
#pragma once

#include "CDataScenario.h"

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

#include "scenario_symbol.h"
#include "scenario_closure.h"

namespace BMW{
namespace Scenario{

class CScenarioDB;
struct CScenarioParser : public boost::spirit::grammar<CScenarioParser>
{
	CScenarioParser(CScenarioDB& db):db_(db){}
	CScenarioDB& db_;

	template<typename S>
	struct definition
	{
		definition(const CScenarioParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// スタートルール
			start_	= !xml_ 
					>> game_
					;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <game>
			//  <scenario>*n
			// </game>
			game_	= str_p("<game>")
					>> *(scenario_ | include_)
					>> str_p("</game>")
					;
			// <include src="" />
			include_	= str_p("<include") 
							>> src_[bind(&CScenarioDB::setScenarioDB)(var(self.db_),arg1)]
						>> str_p("/>")
						;
			
			// <scenario id="">
			// <info >
			// <expert />
			// !(<event>
			//	+<chara id="" />
			// </event>)
			// *(<adv src="" /> | <slg src="" /> | <ed src="" /> | <edr src="" /> | <end src="" />)
			// </scenario>
				scenario_	= str_p("<scenario")
						>> id_[scenario_.val=arg1]
						>> '>'
						>> eps_p[bind(&definition::newData)(var(*this),var(self.db_),scenario_.val)]
						>> info_
						>> expert_
						>> !event_
						>> eps_p[var(nFile_)=0]
						>> *base_[bind(&Scenario::CDataScenario::addBase)(var(pData_),arg1)]
						>> str_p("</scenario>")
						;

			// <title no="">タイトル</title>
			info_	= str_p("<title")
					>> str_p("no=\"") 
					>> int_p[bind(&Scenario::CDataScenario::setNo)(var(pData_),arg1)]
					>> '"'
					>> ch_p('>')
					>> (*(anychar_p - "</title>"))[info_.val=construct_<string>(arg1,arg2)]
					>> eps_p[bind(&Scenario::CDataScenario::setTitle)(var(pData_),info_.val)]
					>> str_p("</title>")
					;

			// <expert normal="" !hard="" />
			expert_	= str_p("<expert")
					>> str_p("normal=\"") >> int_p[bind(&Scenario::CDataScenario::setNormal)(var(pData_),arg1)] >> '"'
					>> !(str_p("hard=\"") >> int_p[bind(&Scenario::CDataScenario::setHard)(var(pData_),arg1)] >> '"')
					>> str_p("/>")
					;

			// <adv src="" /> | <slg src="" /> | <ed src="" /> | <edr src="" />
			base_	=
					( str_p("<adv")[bind(&Scenario::CDataScenarioBase::setScene)(base_.val,Scenario::CDataScenarioBase::ADV)]
					| str_p("<slg")[bind(&Scenario::CDataScenarioBase::setScene)(base_.val,Scenario::CDataScenarioBase::SLG)]
					| str_p("<edr")[bind(&Scenario::CDataScenarioBase::setScene)(base_.val,Scenario::CDataScenarioBase::ED_RETURN)]
					| str_p("<ed")[bind(&Scenario::CDataScenarioBase::setScene)(base_.val,Scenario::CDataScenarioBase::ED)]
					| str_p("<end")[bind(&Scenario::CDataScenarioBase::setScene)(base_.val,Scenario::CDataScenarioBase::END)])
					>> eps_p[bind(&Scenario::CDataScenarioBase::setID)(base_.val,var(nFile_))]
					>> src_[bind(&Scenario::CDataScenario::setScenarioFile)(var(pData_),arg1,var(nFile_))]
					>> eps_p[var(nFile_)++]
					>> str_p("/>")
					;

			// <event><chara id="" /></event>
			event_ = str_p("<event>")
					>> +(str_p("<chara") >> id_[bind(&definition::setEventCharaID)(var(*this),arg1)] >> str_p("/>"))
					>> str_p("</event>")
					;

			// id属性
			id_	= str_p("id=\"") >> (*(anychar_p - '"'))[id_.val = construct_<string>(arg1,arg2)] >> '"';

			// src属性
			src_	= str_p("src=\"") >> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';
		}

		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S> rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>	rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>		rule_i;
		typedef boost::spirit::rule<S, Scenario::scenario_base_closure::context_t>	rule_b;

		// rule
		rule		start_;
		rule		xml_,game_,include_;
		rule_s		scenario_;
		rule_s		info_;
		rule		expert_;
		rule		event_;
		rule_b		base_;
		rule_s		id_,src_;

		// 一時データ
		int nFile_;
		CDataScenario* pData_;
		// 一時データ生成
		void newData(CScenarioDB& db,const string& sID)
		{
			pData_=new CDataScenario();
			db.setScenario(pData_,sID);
		}
		// 操作
		void setEventCharaID(const string& sID)
		{
			pData_->setEventCharaID(Chara::Const::charaID_.getValue(sID));
		}
	};
};


} // namespace Scenario end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね