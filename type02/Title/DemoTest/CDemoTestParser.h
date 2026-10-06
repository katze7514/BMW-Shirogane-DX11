/*
	katze 05/08/04
	デモテストパーサー
*/
#pragma once

#ifdef BMW_DEBUG

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね


#include "../../ADV/adv_symbol.h"
#include "../../SLG/Context/DB/slg_symbol.h"
#include "../../SLG/Map/map_symbol.h"

namespace BMW{
namespace DemoTest{

class CDemoTestData
{/**
 デモテストデータ
 */
public:
	CDemoTestData():bBgm_(false),nAttackDamage_(0),nAttackEN_(0){}

	int		getSide()const{ return nSide_; }
	void	setSide(int nSide){ nSide_=nSide; }
	const string&	getBack()const{ return sBack_; }
	void			setBack(const string& sBack){ sBack_=sBack; }

	bool	IsBgm()const{ return bBgm_; }
	void	bgm(bool bBgm){ bBgm_=bBgm; }

	int		getAttackPhase()const{ return nAttackPhase_; }
	void	setAttackPhase(int nAttackPhase){ nAttackPhase_=nAttackPhase; }
	int		getAttackChara()const{ return nAttackChara_; }
	void	setAttackChara(const string& sAttackChara)
	{ 
		nAttackChara_ = Chara::Const::charaID_.getValue(sAttackChara);
	}
	int		getAttackWeapon()const{ return nAttackWeapon_; }
	void	setAttackWeapon(const string& sAttackWeapon)
	{ 
		nAttackWeapon_ = Weapon::Const::weaponID_.getValue(sAttackWeapon);
	}
	int		getAttackDamage()const{ return nAttackDamage_; }
	void	setAttackDamage(int nAttackDamage){ nAttackDamage_=nAttackDamage; }
	int		getAttackEN()const{ return nAttackEN_; }
	void	setAttackEN(int nAttackEN){ nAttackEN_=nAttackEN; }

	int		getDefPhase()const{ return nDefPhase_; }
	void	setDefPhase(int nDefPhase){ nDefPhase_=nDefPhase; }
	int		getDefChara()const{ return nDefChara_; }
	void	setDefChara(const string& sDefChara)
	{ 
		nDefChara_ = Chara::Const::charaID_.getValue(sDefChara);
	}
	int		getDefAction()const{ return nDefAction_; }
	void	setDefAction(int nDefAction){ nDefAction_=nDefAction; }

private:
	// 攻撃サイド
	int nSide_;
	// 背景
	string sBack_;
	// 音楽
	bool bBgm_;

	// 攻撃側
	int nAttackPhase_;
	int nAttackChara_;
	int nAttackWeapon_;
	int nAttackDamage_;
	int nAttackEN_;

	// 防御側
	int nDefPhase_;
	int nDefChara_;
	int nDefAction_;
};

struct action_symbol : public boost::spirit::symbols<>
{
	action_symbol()
	{
		add
			("HIT",SLG::Battle::HIT)
			("DEF",SLG::Battle::DEFENCE)
			("AVOID",SLG::Battle::AVOID)
			;
	}
};

struct CDemoTestParser : public boost::spirit::grammar<CDemoTestParser>
{
	CDemoTestParser(CDemoTestData& data):data_(data){}
	CDemoTestData& data_;

	template<typename S>
	struct definition
	{
		definition(const CDemoTestParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			start_ = xml_ >> test_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <test>
			//	<side id="" />
			//	<back id="" />
			//	!<bgm on="" />
			//	<attack phase="" chara="" weapon="" !damage="" !en="" />
			//	<def phase="" chara="" action="" />
			// </test>
			test_	= str_p("<test>")
					>> side_
					>> back_
					>> !bgm_
					>> attack_
					>> def_
					>> str_p("</test>")
					;

			side_	= str_p("<side") 
						>> str_p("id=\"") 
						>> sideID_[bind(&CDemoTestData::setSide)(var(self.data_),arg1)]
						>> '"'
						>> str_p("/>")
						;

			back_	= str_p("<back") 
						>> src_[bind(&CDemoTestData::setBack)(var(self.data_),arg1)]
						>> str_p("/>")
						;

			bgm_	=  str_p("<bgm") 
					>> str_p("on=\"") >> bool_[bind(&CDemoTestData::bgm)(var(self.data_),arg1)] >> '"'
					>> str_p("/>")
						;

			attack_	= str_p("<attack") 
					>> str_p("phase=\"") >> phaseID_[bind(&CDemoTestData::setAttackPhase)(var(self.data_),arg1)] >> '"'
					>> chara_[bind(&CDemoTestData::setAttackChara)(var(self.data_),arg1)]
					>> weapon_[bind(&CDemoTestData::setAttackWeapon)(var(self.data_),arg1)]
					>> !(str_p("damage=\"") >> int_p[bind(&CDemoTestData::setAttackDamage)(var(self.data_),arg1)] >> '"')
					>> !(str_p("en=\"") >> int_p[bind(&CDemoTestData::setAttackEN)(var(self.data_),arg1)] >> '"')
					>> str_p("/>")
					;

			def_	= str_p("<def") 
					>> str_p("phase=\"") >> phaseID_[bind(&CDemoTestData::setDefPhase)(var(self.data_),arg1)] >> '"'
					>> chara_[bind(&CDemoTestData::setDefChara)(var(self.data_),arg1)]
					>> str_p("action=\"") >> actionID_[bind(&CDemoTestData::setDefAction)(var(self.data_),arg1)] >> '"'
					>> str_p("/>")
					;

			// キャラ属性
			chara_ = str_p("chara=\"") >> (*(anychar_p - '"'))[chara_.val = construct_<string>(arg1,arg2)] >> '"';
			// 武器属性
			weapon_ = str_p("weapon=\"") >> (*(anychar_p - '"'))[weapon_.val = construct_<string>(arg1,arg2)] >> '"';
			// src属性
			src_	= str_p("src=\"")	>> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';
		}

		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>											rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>		rule_s;
		
		// rule
		rule	start_,xml_;
		rule	test_,side_,back_,attack_,def_,bgm_;
		rule_s	chara_,weapon_,src_;

		// symbol
		ADV::side_symbol	sideID_;
		SLG::phase_symbol	phaseID_;
		SLG::Map::map_symbol backID_;
		action_symbol		actionID_;
		Parser::boolsym		bool_;
	};
};

} // namespace DemoTest end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね

#endif