/*
	katze 05/03/05
	update 06/01/18
	キャラ初期値とキャラ成長の解析器
*/
#pragma once

#include "../../Weapon/ConstWeapon.h"
#include "../../Face/ConstFace.h"
#include "../../Scene/ConstScene.h"
#include "../ConstChara.h"
#include "../CValidSkill.h"
#include "CDataCharaData.h"

#include "../../Sound/ConstSound.h"
#include "../../Save/CExecData.h"
#include "CCharaDB.h"

namespace BMW{
namespace Chara{
struct CCharaParser : public boost::spirit::grammar<CCharaParser>
{
	CCharaParser(CCharaDB& db):db_(db){}
	CCharaDB& db_;

	template<typename S>
	struct definition
	{
		definition(const CCharaParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// スタートルール
			start_	= !xml_ 
						>> chara_
					;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <chara>
			//	<character></character>*n
			// </chara>
			chara_	= str_p("<chara>")
					>> *(character_ | include_)
					>> str_p("</chara>")
					;

			// <include src="" />
			include_	= str_p("<include")
							>> src_[bind(&CCharaDB::setCharaDB)(var(self.db_),arg1)]
						>> str_p("/>")
						;

			// parent属性
			parent_	= str_p("parent=\"") >> (*(anychar_p - '"'))[parent_.val = construct_<string>(arg1,arg2)] >> '"';

			// train属性
			train_	= str_p("train=\"") >> (*(anychar_p - '"'))[train_.val = construct_<string>(arg1,arg2)] >> '"';

			// hide属性
			//hide_	= str_p("hide=\"") >> (*(anychar_p - '"'))[hide_.val = construct_<string>(arg1,arg2)] >> '"';

			// <character id="" !parent="" !train="">
			//	<init></init>
			//	<growth></growth>
			// </character>
			character_ = str_p("<character")
					>> id_[character_.val=bind(&katzeSDK::Misc::CStringMap::getValue)(var(Chara::Const::charaID_),arg1)]
					>> eps_p[bind(&definition::newData)(var(*this),var(self.db_),character_.val)]
					>> !parent_[bind(&definition::newDataParent)(var(*this),var(self.db_),arg1)]
					>> !train_[bind(&definition::newDataTrain)(var(*this),arg1,character_.val)]
					//>> !hide_[bind(&definition::newDataHide)(var(*this),arg1,character_.val)]
					>> '>'
					>> init_[bind(&Chara::CDataCharaData::setInit)(var(pData_),arg1)]
					>> eps_p[bind(&definition::setID)(var(*this),character_.val)]
					>> !growth_[bind(&Chara::CDataCharaData::setAbility)(var(pData_),arg1)]
					>> str_p("</character>")
					;

			// id属性
			id_	= str_p("id=\"") >> (*(anychar_p - '"'))[id_.val = construct_<string>(arg1,arg2)] >> '"';

			// src属性
			src_	= str_p("src=\"") >> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';

			// <init !name="">
			//	!<face />
			//	!<map />
			//	!<demo src="" symbol="" />
			//	!<bgm />
			//	!<nature />
			//  !<growth />
			//	!<penalty />
			//	!<fp />
			//	!<fund />
			//	!<battle />
			//	!<skill />
			//	!<talent id="" !attr="" />*n
			//	!<weapon id="" />*n
			//	!<weaponcost type="" />
			//	!<item num="" />
			// </init>
			init_ = str_p("<init") >> !name_ >> '>'
						>> !face_[bind(&Chara::CDataCharaInit::setFaceID)(init_.val,arg1)]

						>> !(str_p("<map")
							>> src_[bind(&Chara::CDataCharaInit::setMapSymbolID)(init_.val,arg1)]
							>> str_p("/>"))

						>> !(str_p("<demo")
							>> src_[bind(&Chara::CDataCharaInit::setDemoID)(init_.val,arg1)]
							>> symbol_[bind(&Chara::CDataCharaInit::setSymbolID)(init_.val,arg1)]
							>> str_p("/>"))

						>> !bgm_[bind(&Chara::CDataCharaInit::setBgmID)(init_.val,arg1)]
						>> !nature_[bind(&Chara::CDataCharaInit::setChara)(init_.val,arg1)]
						>> !growth_type_[bind(&Chara::CDataCharaInit::setGrowth)(init_.val,arg1)]
						>> !penalty_[bind(&Chara::CDataCharaInit::setPena)(init_.val,arg1)]
						>> !fp_[bind(&Chara::CDataCharaInit::setFP)(init_.val,arg1)]
						>> !fund_[bind(&Chara::CDataCharaInit::setFund)(init_.val,arg1)]
						>> !battle_[bind(&Chara::CDataCharaInit::setBattle)(init_.val,arg1)]
						>> !skill_v_/*[bind(&Chara::CDataCharaInit::setSkillValid)(init_.val,arg1)]*/
						>> *talent_[bind(&Chara::CDataCharaInit::addTalent)(init_.val,arg1)]
						>> *weapon_[bind(&Chara::CDataCharaInit::addWeapon)(init_.val,arg1)]
						>> !weaponcost_[bind(&Chara::CDataCharaInit::setWeaponCost)(init_.val,arg1)]
						>> !item_[bind(&Chara::CDataCharaInit::setItemMax)(init_.val,arg1)]
					>> str_p("</init>")
					;

			//	<face id="" />
			face_			= str_p("<face")
							>> id_[face_.val = bind(&katzeSDK::Misc::CStringMap::getValue)(var(Face::Const::faceID_),arg1)]
							>> str_p("/>")
							;
			//	<bgm id="" />
			bgm_ = str_p("<bgm") 
				>> id_[bgm_.val=bind(&katzeSDK::Misc::CStringMap::getValue)(var(Sound::Const::bgmID_),arg1)]
				>> str_p("/>")
				;
			
			//	<nature type="" />
			nature_			= str_p("<nature") >> str_p("type=\"") >> natureType_[nature_.val = arg1] >> '"' >> str_p("/>");
			//  <growth type="" />
			growth_type_	= str_p("<growth") >> str_p("type=\"") >> growthType_[growth_type_.val = arg1] >> '"' >> str_p("/>");

			//  <penalty value="" />
			penalty_	= str_p("<penalty") >> str_p("value=\"") >> int_p[penalty_.val = arg1] >> '"' >> str_p("/>");
			//  <fp value="" />
			fp_			= str_p("<fp") >> str_p("value=\"") >> int_p[fp_.val = arg1] >> '"' >> str_p("/>");
			
			// <battle !hp="" !en="" !tough="" !quick="" !move="" !jump="" />
			battle_ = str_p("<battle")
						>> !(str_p("hp=\"")		>> int_p[bind(&Chara::CStatusBattle::setHP)(battle_.val,arg1)]		>> '"')
						>> !(str_p("en=\"")		>> int_p[bind(&Chara::CStatusBattle::setEN)(battle_.val,arg1)]		>> '"')
						>> !(str_p("tough=\"")	>> int_p[bind(&Chara::CStatusBattle::setTough)(battle_.val,arg1)]	>> '"')
						>> !(str_p("quick=\"")	>> int_p[bind(&Chara::CStatusBattle::setQuick)(battle_.val,arg1)]	>> '"')
						>> !(str_p("move=\"")	>> int_p[bind(&Chara::CStatusBattle::setMove)(battle_.val,arg1)]	>> '"')
						>> !(str_p("jump=\"")	>> int_p[bind(&Chara::CStatusBattle::setJump)(battle_.val,arg1)]	>> '"')
					>> str_p("/>")
					;

			// <talent id="" attr="" />
			talent_ = str_p("<talent")
						>> str_p("id=\"") >> abilityType_[bind(&Chara::CStatusAbility::setID)(talent_.val,arg1)] >> '"'
						>> !(str_p("attr=\"") >> int_p[bind(&Chara::CStatusAbility::setAttr)(talent_.val,arg1)] >> '"')
					>> str_p("/>")
					;

			// <skill !fundpower="" !counter="" !attack="" !defence="" !spup="" />
			skill_v_ = str_p("<skill")
					>> !(str_p("fundpower=\"") >> int_p/*[bind(&Chara::CValidSkill::valid)(skill_v_.val,arg1,val(Chara::CValidSkill::FUNDPOWER))]*/ >> '"')
					>> !(str_p("counter=\"") >> int_p/*[bind(&Chara::CValidSkill::valid)(skill_v_.val,arg1,val(Chara::CValidSkill::COUNTER))]*/ >> '"')
					>> !(str_p("attack=\"") >> int_p/*[bind(&Chara::CValidSkill::valid)(skill_v_.val,arg1,val(Chara::CValidSkill::BACKUPATTACK))]*/ >> '"')
					>> !(str_p("defence=\"") >> int_p/*[bind(&Chara::CValidSkill::valid)(skill_v_.val,arg1,val(Chara::CValidSkill::BACKUPDEFENCE))]*/ >> '"')
					>> !(str_p("spup=\"") >> int_p/*[bind(&Chara::CValidSkill::valid)(skill_v_.val,arg1,val(Chara::CValidSkill::SPUP))]*/ >> '"')
					>> str_p("/>")
					;

			// <weapon id="" />
			weapon_ = str_p("<weapon")
						>> id_[weapon_.val=bind(&katzeSDK::Misc::CStringMap::getValue)(var(Weapon::Const::weaponID_),arg1)]
					>> str_p("/>")
					;

			// <weaponcost type="" />
			weaponcost_ = str_p("<weaponcost")
						>> str_p("type=\"") >> costType_[weaponcost_.val=arg1] >> '"'
					>> str_p("/>")
					;

			// <item num="" />
			item_ = str_p("<item")
						>> str_p("num=\"") >> int_p[item_.val=arg1] >> '"'
					>> str_p("/>")
					;
			
			// <growth>
			//	<spirit lv="" id="" attr="" />×6
			//	<skill lv="" id="" attr="" />*n
			// </growth>
			growth_ = str_p("<growth>")
						>>	for_p(growth_.count=0,growth_.count<6,growth_.count++)
							[
								spirit_[bind(&Chara::CDataCharaGrowthAbility::setSpirit)(growth_.val,arg1,growth_.count)]
							]
						>> *skill_[bind(&Chara::CDataCharaGrowthAbility::addSkill)(growth_.val,arg1)]
					>> str_p("</growth>")
					;

			spirit_ = str_p("<spirit")
						>> str_p("lv=\"")	>> int_p[bind(&Chara::CStatusGrowthAbility::setLv)(spirit_.val,arg1)]				>> '"'
						>> str_p("id=\"")	>> spiritType_[bind(&Chara::CStatusGrowthAbility::setAbilityID)(spirit_.val,arg1)]	>> '"'
						>> str_p("attr=\"")	>> int_p[bind(&Chara::CStatusGrowthAbility::setAbilityAttr)(spirit_.val,arg1)]		>> '"'
					>> str_p("/>")
					;

			skill_ = str_p("<skill")
						>> str_p("lv=\"")		>> int_p[bind(&Chara::CStatusGrowthAbility::setLv)(skill_.val,arg1)]				>> '"'
						>> str_p("id=\"")		>> abilityType_[bind(&Chara::CStatusGrowthAbility::setAbilityID)(skill_.val,arg1)]	>> '"'
						>> !(str_p("attr=\"")	>> int_p[bind(&Chara::CStatusGrowthAbility::setAbilityAttr)(skill_.val,arg1)]		>> '"')
					>> str_p("/>")
					;

			// 名前属性
			name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';

			// symbol属性
			symbol_	= str_p("symbol=\"")	>> (*(anychar_p - '"'))[symbol_.val = construct_<string>(arg1,arg2)] >> '"';
		}

		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S> rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>	rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>		rule_i;
		typedef boost::spirit::rule<S, init_closure::context_t>				rule_init;
		typedef boost::spirit::rule<S, ability_closure::context_t>			rule_a;
		typedef boost::spirit::rule<S, ability_g_closure::context_t>		rule_ag;
		typedef boost::spirit::rule<S, battle_closure::context_t>			rule_b;
		typedef boost::spirit::rule<S, c_ability_g_closure::context_t>		rule_ca;
		typedef boost::spirit::rule<S, skill_closure::context_t>			rule_sk;

		// rule
		rule		start_;
		rule		xml_,chara_,include_;
		rule_i		character_;
		rule_s		src_,parent_,train_/*,hide_*/;
		rule_init	init_;
		rule_s		id_;
		rule_i		face_;
		rule_i		/*map_,*/bgm_;
		rule_s		symbol_/*,msg_*/;
		rule_i		nature_;
		rule_i		growth_type_;
		rule_i		penalty_,fp_;
		rule_s		name_;
		rule_b		battle_;
		rule_a		talent_;
		rule_sk		skill_v_;
		rule_i		weapon_;
		rule_i		weaponcost_;
		rule_i		item_;
		rule_ca		growth_;
		rule_ag		spirit_;
		rule_ag		skill_;

		// こいつを通るとarg1として、CStatusFundがゲットできる
		fund_grammar fund_;

		// シンボル
		naturesym	natureType_;
		growthsym	growthType_;
		spiritsym	spiritType_;
		abilitysym	abilityType_;

		//weponsym	weaponType_;
		Weapon::costsym		costType_;

		// 現在、設定中のキャラID
		// 一時オブジェクト
		CDataCharaData* pData_;
		// 一時オブジェクト設定メソッド
		void newData(CCharaDB& db,int nID){ pData_=new Chara::CDataCharaData(); db.addCharaData(pData_,nID); }
		void newDataParent(CCharaDB& db,const string& sParentID)
		{
			CDataCharaData* pData = db.getCharaData(Chara::Const::charaID_.getValue(sParentID));
		#ifdef BMW_DEBUG
			if(pData==NULL) CDbg().Out("親キャラ %s が定義されていません", sParentID.c_str());
		#endif
			pData_->setParentData(smart_ptr<CDataCharaData>(pData,false));
		}
		void newDataTrain(const string& sTrainID, int nID)
		{// 養成マップに追加
			Save::CExecData::addTrainMap(nID, Const::charaID_.getValue(sTrainID));
		}
		/*void newDataHide(const string& sFlag, int nID)
		{// ハイドマップに追加
			Save::CExecData::addHideMap(nID, Scene::Const::flagID_.getValue(sFlag));
		}*/
		void setID(int nID){ pData_->getInitPtr()->setID(nID); }
	};
};

} // namespace Chara end
} // namespace BMW end