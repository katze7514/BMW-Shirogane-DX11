/*
	katze 05/03/09
	WeaponParser
*/
#pragma once

#include "../../Chara/ConstChara.h"
#include "../ConstWeapon.h"
#include "../CDataWeaponInit.h"

#include "../../Sound/ConstSound.h"
#include "../../Save/CExecData.h"
#include "CWeaponDB.h"

namespace BMW{
namespace Weapon{
struct CWeaponParser : public boost::spirit::grammar<CWeaponParser>
{
	CWeaponParser(CWeaponDB& db):db_(db){}
	CWeaponDB& db_;
	
	template<typename S>
	struct definition
	{
		definition(const CWeaponParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// スタートルール
			start_ = xml_ >> weapon_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <weapon>
			//	<weaponinfo></weaponinfo>*n
			// </weapon>
			weapon_ = str_p("<weapon>")
						>> *(weaponinfo_ | include_)
					>> str_p("</weapon>")
					;

			// <include src="" />
			include_	= str_p("<include")
							>> src_[bind(&CWeaponDB::setWeaponDB)(var(self.db_),arg1)]
						>> str_p("/>")
						;

			// <weaponinfo id="" name="">
			//	<demo />
			//	!<bgm />
			//  <kind />
			//	<attr />
			//	<attack />
			//	<corect />
			//  !<need />
			//	<train />
			//	!<aid></aid>
			// </weaponinfo>
			weaponinfo_ = str_p("<weaponinfo")
							>> id_[weaponinfo_.val=bind(&katzeSDK::Misc::CStringMap::getValue)(var(Weapon::Const::weaponID_),arg1)]
							>> name_[weaponinfo_.name=arg1]
							>> ch_p('>')
							>> eps_p[bind(&definition::newInit)(var(*this),var(self.db_),weaponinfo_.val)]
							>> eps_p[bind(&Weapon::CDataWeaponInit::setName)(var(pInit_),weaponinfo_.name)]
						>> demo_
						>> !bgm_
						>> kind_
						>> attr_
						>> attack_
						>> corect_
						>> !need_
						>> train_
						>> !aid_
						>> str_p("</weaponinfo>")
						;

			// id属性
			id_	= str_p("id=\"")	>> (*(anychar_p - '"'))[id_.val = construct_<string>(arg1,arg2)] >> '"';

			// src属性
			src_	= str_p("src=\"")	>> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';

			// <demo src="" />
			demo_ = str_p("<demo") 
						>> src_[bind(&Weapon::CDataWeaponInit::setDemoID)(var(pInit_),arg1)]
					>> str_p("/>")
					;

			// <bgm id="" />
			bgm_ = str_p("<bgm") 
				>> id_[bgm_.val=bind(&katzeSDK::Misc::CStringMap::getValue)(var(Sound::Const::bgmID_),arg1)]
				>> str_p("/>")
				>> eps_p[bind(&Weapon::CDataWeaponInit::setBgmID)(var(pInit_),bgm_.val)] 
				;

			// <kind type="" />
			kind_ = str_p("<kind")
					>> str_p("type=\"") >> kindType_[bind(&Weapon::CDataWeaponInit::setKind)(var(pInit_),arg1)]	>> '"'
					>> str_p("/>")
					;
			// <attr p="" m="" t="" !f="" />
			attr_ = str_p("<attr")
					>> str_p("p=\"") >> boolSym_[bind(&Weapon::CDataWeaponInit::p)(var(pInit_),arg1)]	>> '"'
					>> str_p("m=\"") >> boolSym_[bind(&Weapon::CDataWeaponInit::m)(var(pInit_),arg1)]	>> '"'
					>> str_p("t=\"") >> boolSym_[bind(&Weapon::CDataWeaponInit::t)(var(pInit_),arg1)]	>> '"'
					>> !(str_p("f=\"") >> boolSym_[bind(&Weapon::CDataWeaponInit::f)(var(pInit_),arg1)]	>> '"')
					>> str_p("/>")
					;

			// <attack>
			//	<power value="" />
			//	<all min="" max="" />
			//	<core min="" max="" />
			//	<height value="" />
			// </attack>
			attack_ = str_p("<attack>")
						>> str_p("<power")
							>>	str_p("value=\"") >> int_p[bind(&Weapon::CDataWeaponInit::setAttack)(var(pInit_),arg1)]	>> '"'
						>> str_p("/>")
						>> str_p("<all")
							>> str_p("min=\"") >> int_p[bind(&Weapon::CDataWeaponInit::setMin)(var(pInit_),arg1)]	>> '"'
							>> str_p("max=\"") >> int_p[bind(&Weapon::CDataWeaponInit::setMax)(var(pInit_),arg1)]	>> '"'
						>> str_p("/>")
						>> str_p("<core")
							>> str_p("min=\"") >> int_p[bind(&Weapon::CDataWeaponInit::setCoreMin)(var(pInit_),arg1)]	>> '"'
							>> str_p("max=\"") >> int_p[bind(&Weapon::CDataWeaponInit::setCoreMax)(var(pInit_),arg1)]	>> '"'
						>> str_p("/>")
						>> str_p("<height")
							>> str_p("value=\"") >> int_p[bind(&Weapon::CDataWeaponInit::setHeight)(var(pInit_),arg1)]	>> '"'
						>> str_p("/>")
					>> str_p("</attack>")
					;

			// <corect hit="" ct="" />
			corect_ = str_p("<corect")
						>> str_p("hit=\"") >> int_p[bind(&Weapon::CDataWeaponInit::setHit)(var(pInit_),arg1)]	>> '"'
						>> str_p("ct=\"") >> int_p[bind(&Weapon::CDataWeaponInit::setCT)(var(pInit_),arg1)]	>> '"'
					>> str_p("/>")
					;

			// <need en="" ballet="" mental="" />
			need_ = str_p("<need")
						>> !(str_p("en=\"") >> int_p[bind(&Weapon::CDataWeaponInit::setEN)(var(pInit_),arg1)]	>> '"')
						>> !(str_p("ballet=\"") >> int_p[bind(&Weapon::CDataWeaponInit::setBallet)(var(pInit_),arg1)]	>> '"')
						>> !(str_p("mental=\"") >> int_p[bind(&Weapon::CDataWeaponInit::setMental)(var(pInit_),arg1)]	>> '"')
					>> str_p("/>")
					;

			// <train type="" />
			train_ = str_p("<train")
						>> str_p("type=\"") >> costType_[bind(&Weapon::CDataWeaponInit::setTrainingType)(var(pInit_),arg1)]	>> '"'
					>> str_p("/>")
					;

			// <aid></aid>
			aid_ = str_p("<aid") >> *(anychar_p - '>') >> '>'
					>> !(cond_ | collab_ | status_) >> !map_ >> !special_
				>> str_p("</aid>")
				;

			// <chara />
			collab_ = repeat_p(2,5)[chara_];

			// <chara id="" weapon="" />
			chara_ = str_p("<chara")
					>> id_[chara_.val=bind(&katzeSDK::Misc::CStringMap::getValue)(var(Chara::Const::charaID_),arg1)]
					>> weapon_c_[chara_.weapon=bind(&katzeSDK::Misc::CStringMap::getValue)(var(Weapon::Const::weaponID_),arg1)]
					>> eps_p[bind(&Weapon::CDataWeaponInit::setCollabChara)(var(pInit_),chara_.val,chara_.weapon)]
					>> str_p("/>")
					;
			weapon_c_ = str_p("weapon=\"") >> (*(anychar_p - '"'))[weapon_c_.val = construct_<string>(arg1,arg2)] >> '"';

			// <fund /><mental />
			status_ = !fund_[bind(&Weapon::CDataWeaponInit::setFundAid)(var(pInit_),arg1)]
					>> !(str_p("<mental")
						>> str_p("value=\"") >> int_p[bind(&Weapon::CDataWeaponInit::setMentalAid)(var(pInit_),arg1)] >> '"'
						>> str_p("/>")
						)
					;

			// <cond type="" value="" />
			cond_ = str_p("<cond")
						>> str_p("type=\"") >> condType_[bind(&Weapon::CDataWeaponInit::setCond)(var(pInit_),arg1)] >> '"'
						>> str_p("value=\"") >> int_p[bind(&Weapon::CDataWeaponInit::setCondValue)(var(pInit_),arg1)] >> '"'
					>> str_p("/>")
					;

			// <field type="" friend="" !size="" src="" />
			map_ = str_p("<field")
						>> str_p("type=\"") >> fieldType_[bind(&Weapon::CDataWeaponInit::setField)(var(pInit_),arg1)] >> '"'
						>> str_p("friend=\"") >> boolSym_[bind(&Weapon::CDataWeaponInit::fieldFriend)(var(pInit_),arg1)] >> '"'
						>> if_p(bind(&Weapon::CDataWeaponInit::getField)(var(pInit_))==Weapon::Field::THROW)
						   [// 投げ込みタイプの時はこいつを設定する
							str_p("size=\"") >> int_p[bind(&Weapon::CDataWeaponInit::setFieldSize)(var(pInit_),arg1)]	>> '"'
						   ]
						>> src_[bind(&Weapon::CDataWeaponInit::setFieldFile)(var(pInit_),arg1)]
					>> str_p("/>")
					;

			// <special type="" />
			special_ = str_p("<special")
						>> str_p("type=\"") >> specialType_[bind(&Weapon::CDataWeaponInit::setSpecial)(var(pInit_),arg1)] >> '"'
					>> str_p("/>")
					;

			// 名前属性
			name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';
		}

		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>											rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>		rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>			rule_i;
		typedef boost::spirit::rule<S, weapon_info_closure::context_t>			rule_info;
		typedef boost::spirit::rule<S, collab_closure::context_t>				rule_collab;
		
		// ルール
		rule		start_,xml_;
		rule		weapon_,include_;
		rule_info	weaponinfo_;
		rule_s		id_,src_;
		rule_s		name_,demo_;
		rule_i		bgm_;
		rule		kind_,attr_,attack_,corect_,need_,train_,aid_;
		rule		collab_,status_,cond_,map_,special_;
		rule_collab	chara_;
		rule_s		weapon_c_;

		// 文法
		Chara::fund_grammar fund_;
		
		// シンボル
		Parser::planesym	charaID_,weaponID_;
		Parser::boolsym		boolSym_;
		kindsym				kindType_;
		condsym				condType_;
		costsym				costType_;
		fieldsym			fieldType_;
		specialsym			specialType_;
		
		// 一時オブジェクト
		CDataWeaponInit* pInit_;
		// 一時オブジェクト設定メソッド
		void newInit(CWeaponDB& db,int nID){ pInit_=new CDataWeaponInit(); db.addData(pInit_, nID); }
	};
};

} // namespace Weapon end
} // namespace BMW end