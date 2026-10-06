/*
	katze 06/04/19
	SLG戦闘イベントパーサー
*/
#pragma once

#include "../../../Chara/DB/chara_symbol.h"
#include "../../IDSLG.h"
#include "../CSLGDef.h"

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

namespace BMW{
namespace SLG{

struct actiontype_symbol : public boost::spirit::symbols<>
{
	actiontype_symbol()
	{
		add
			("HIT",Battle::HIT)
			("DEF",Battle::DEFENCE)
			("AVOID",Battle::AVOID)
			;
	}
};

struct damagetype_symbol : public boost::spirit::symbols<>
{
	damagetype_symbol()
	{
		add
			("ABS",Event::CBattleEventAttack::ABS)
			("RATIO",Event::CBattleEventAttack::RATIO)
			;
	}
};

struct CSlgBattleEventParser : public boost::spirit::grammar<CSlgBattleEventParser>
{
	Event::CBattleEventData* pBattle_;
	CSLGDef* p_;

	// アクセッサ
	void setBattleEvent(Event::CBattleEventData* pBattle){ pBattle_=pBattle; }
	void setSlgDef(CSLGDef* pDef){ p_=pDef; }

	template<typename S>
	struct definition
	{
		definition(const CSlgBattleEventParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			//	<attack></attack>
			//	<counter></counter>
			//	!<attack_back></attack_back>
			//	!<counter_back></counter_back>
			start_ = attack_ >> counter_ >> !attack_back_ >> !counter_back_;

			//	<attack></attack>
			attack_ = str_p("<attack>")[bind(&definition::setData)(var(*this),Event::CBattleEventData::ATTACK,var(self.pBattle_))]
						>> data_ 
					>> str_p("</attack>");
			//	<counter></counter>
			counter_ = str_p("<counter>")[bind(&definition::setData)(var(*this),Event::CBattleEventData::COUNTER,var(self.pBattle_))]
						>> data_
					>> str_p("</counter>");
			//	<attack_back></attack_back>
			attack_back_ = str_p("<attack_back>")[bind(&definition::setData)(var(*this),Event::CBattleEventData::ATTACK_BACK,var(self.pBattle_))]
							>> data_
						>> str_p("</attack_back>");
			//	<counter_back></counter_back>
			counter_back_ = str_p("<counter_back>")[bind(&definition::setData)(var(*this),Event::CBattleEventData::COUNTER_BACK,var(self.pBattle_))]
							>> data_
						>> str_p("</counter_back>");

			// <chara id="" />
			//	!atk
			//	!def
			data_ = str_p("<chara")
						>> id_[data_.val = bind(&CSLGDef::getSlgID)(var(*self.p_),arg1), 
							   bind(&Event::CBattleEventBase::setChara)(var(pBase_),data_.val)]
					>> str_p("/>")
					>> !atk_ >> !def_;

			// <atk>
			//	<weapon id="" !en="" !ct="" !death="" />
			//	!<damage !type="" value="" />
			//	abiltiy
			// </atk>
			atk_ = str_p("<atk>")
					>> str_p("<weapon")
							>> id_[atk_.val=bind(&katzeSDK::Misc::CStringMap::getValue)(var(Weapon::Const::weaponID_),arg1),
								   bind(&Event::CBattleEventAttack::setWeaponID)(var(pAtk_),atk_.val)]
							>> !(str_p("en=\"") >> int_p[bind(&Event::CBattleEventAttack::setEN)(var(pAtk_),arg1)] >> '"')
							>> !(str_p("ct=\"") >> bool_[bind(&Event::CBattleEventAttack::ct)(var(pAtk_),arg1)] >> '"')
							>> !(str_p("death=\"") >> bool_[bind(&Event::CBattleEventAttack::death)(var(pAtk_),arg1)] >> '"')
					>> str_p("/>")
					>> !(str_p("<damage")
							>> !(str_p("type=\"") >> damagetype_[bind(&Event::CBattleEventAttack::setType)(var(pAtk_),arg1)] >> '"')
							>> str_p("value=\"") >> int_p[bind(&Event::CBattleEventAttack::setDamage)(var(pAtk_),arg1)] >> '"'
					>>str_p("/>"))
					>> eps_p[var(pAbility_) = var(pAtk_)]
					>> ability_
				>> str_p("</atk>")
				;

			// <def>
			//	<action id="" />
			//	ability
			// </def>
			def_ = str_p("<def>")
					>> str_p("<action")
						>> str_p("id=\"") >> actiontype_[bind(&Event::CBattleEventDefence::setAction)(var(pDef_),arg1)] >> '"'
					>> str_p("/>")[var(pAbility_) = var(pDef_)]
					>> ability_
				>> str_p("</def>")
				;

			// *<ability id="" !en="" />
			// !<msg id="" />
			// !<msg_backup id="" />
			ability_ = *(str_p("<ability")
							>> str_p("id=\"") >> abilityid_[bind(&Event::CBattleEventAbility::setAbility)(var(pAbility_),arg1)] >> '"'
							>> !(str_p("en=\"") >> int_p[bind(&Event::CBattleEventAbility::calcEnAbility)(var(pAbility_),arg1)] >> '"')
						>> str_p("/>"))
						>> !(str_p("<msg") >> id_[bind(&Event::CBattleEventAbility::setMsgList)(var(pAbility_),arg1)] >> str_p("/>"))
						>> !(str_p("<msg_backup") >> id_[bind(&Event::CBattleEventAbility::setMsgListBackup)(var(pAbility_),arg1)] >> str_p("/>"))
						;

			// id属性
			id_	= str_p("id=\"")	>> (*(anychar_p - '"'))[id_.val = construct_<string>(arg1,arg2)] >> '"';

			// name属性
			//name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';
		}
		
		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>											rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>		rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>			rule_i;
		
		// rule
		rule		start_,attack_,counter_,attack_back_,counter_back_;
		rule_i		data_,atk_;
		rule		def_,ability_;
		rule_s		id_;

		// シンボル
		Parser::boolsym		bool_;
		actiontype_symbol	actiontype_;
		damagetype_symbol	damagetype_;
		Chara::abilitysym	abilityid_;

		// 一時データ
		Event::CBattleEventBase*	pBase_;
		Event::CBattleEventAbility*	pAbility_;
		Event::CBattleEventAttack*	pAtk_;
		Event::CBattleEventDefence*	pDef_;

		// データ設定
		void setData(int nIndex, Event::CBattleEventData* pBattle)
		{
			pBase_ = pBattle->getEventBasePtr(nIndex);
			pAtk_ = pBase_->getAtkPtr();
			pDef_ = pBase_->getDefPtr();
		}
	};
};

} // namespace SLG end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね