/*
	katze 05/06/29
	養成データ
*/
#pragma once

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

#include "chara_closure.h"
#include "chara_symbol.h"

namespace BMW{
namespace Chara{

struct train_grammar : public boost::spirit::grammar<train_grammar, train_closure::context_t>
{
	template<typename S>
	struct definition
	{
		definition(const train_grammar& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// <train !name="">
			//	<lv />
			//	<exp />
			//	!<fp />
			//	!<kill />
			//	!<fund />
			//	!<battle />
			//	!<weapon />
			//	<skill />*n
			//	<item />*n
			// </train>
			start_ = str_p("<train") >> !(str_p("name=\"") >> *(anychar_p -'"') >> '"') >> ch_p('>')
					>>	lv_[bind(&Chara::CDataCharaTrain::setLv)(self.val,arg1)]
					>>	exp_[bind(&Chara::CDataCharaTrain::setExp)(self.val,arg1)]
					>>	!penalty_[bind(&Chara::CDataCharaTrain::setPenalty)(self.val,arg1)]
					>>	!fp_[bind(&Chara::CDataCharaTrain::setFP)(self.val,arg1)]
					>>	!kill_[bind(&Chara::CDataCharaTrain::setKill)(self.val,arg1)]
					>>	!fund_[bind(&Chara::CDataCharaTrain::setFund)(self.val,arg1)]
					>>	!battle_[bind(&Chara::CDataCharaTrain::setBattle)(self.val,arg1)]
					>>	!weapon_[bind(&Chara::CDataCharaTrain::setWeapon)(self.val,arg1)]
					>>	*skill_[bind(&Chara::CDataCharaTrain::addSkillAbi)(self.val,arg1)]
					>>	*talent_[bind(&Chara::CDataCharaTrain::addTalent)(self.val,arg1)]
					>>	*item_[bind(&Chara::CDataCharaTrain::addItemAbi)(self.val,arg1)]
					>> str_p("</train>")
					;

			// <lv value="" />
			lv_	= str_p("<lv") >> str_p("value=\"") >> int_p[lv_.val = arg1] >> '"' >> str_p("/>");
			// <exp value="" />
			exp_	= str_p("<exp") >> str_p("value=\"") >> int_p[exp_.val = arg1] >> '"' >> str_p("/>");
			// <penalty value="" />
			penalty_	= str_p("<penalty") >> str_p("value=\"") >> int_p[penalty_.val = arg1] >> '"' >> str_p("/>");
			// <fp value="" />
			fp_	= str_p("<fp") >> str_p("value=\"") >> int_p[fp_.val = arg1] >> '"' >> str_p("/>");
			// <kill value="" />
			kill_	= str_p("<kill") >> str_p("value=\"") >> int_p[kill_.val = arg1] >> '"' >> str_p("/>");

			// <battle hp="" en="" tough="" quick="" />
			battle_ = str_p("<battle")
						>> str_p("hp=\"")		>> int_p[bind(&Chara::CStatusBattle::setHP)(battle_.val,arg1)]		>> '"'
						>> str_p("en=\"")		>> int_p[bind(&Chara::CStatusBattle::setEN)(battle_.val,arg1)]		>> '"'
						>> str_p("tough=\"")	>> int_p[bind(&Chara::CStatusBattle::setTough)(battle_.val,arg1)]	>> '"'
						>> str_p("quick=\"")	>> int_p[bind(&Chara::CStatusBattle::setQuick)(battle_.val,arg1)]	>> '"'
					>> str_p("/>")
					;

			// <weapon value="" />
			weapon_	= str_p("<weapon") >> str_p("value=\"") >> int_p[weapon_.val = arg1] >> '"' >> str_p("/>");

			// <skill id="" attr="" />
			skill_	= str_p("<skill")
						>> str_p("id=\"") >> skillID_[bind(&Chara::CStatusAbility::setID)(skill_.val,arg1)] >> '"'
						>> !(str_p("attr=\"") >> int_p[bind(&Chara::CStatusAbility::setAttr)(skill_.val,arg1)] >> '"')
					>> str_p("/>")
					;

			// <item id="" attr="" />
			item_	= str_p("<item")
						>> str_p("id=\"") >> itemID_[bind(&Chara::CStatusAbility::setID)(item_.val,arg1)] >> '"'
						>> str_p("attr=\"") >> int_p[bind(&Chara::CStatusAbility::setAttr)(item_.val,arg1)] >> '"'
					>> str_p("/>")
					;

			// <talent id="" />
			talent_	= str_p("<talent")
						>> str_p("id=\"") >> skillID_[bind(&Chara::CStatusAbility::setID)(talent_.val,arg1)] >> '"'
					>> str_p("/>")
					;
		}

		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>											rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>		rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>			rule_i;
		typedef boost::spirit::rule<S, Chara::battle_closure::context_t>		rule_b;
		typedef boost::spirit::rule<S, Chara::ability_closure::context_t>		rule_ab;

		// ルール
		rule	start_;
		rule_i	lv_,exp_,penalty_,fp_,kill_,weapon_;
		rule_b	battle_;
		rule_ab	item_,skill_,talent_;

		// grammar
		fund_grammar fund_;

		// シンボル
		Chara::abilitysym	skillID_;
		Chara::itemsym		itemID_;
	};
};

} // namespace Chara end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね