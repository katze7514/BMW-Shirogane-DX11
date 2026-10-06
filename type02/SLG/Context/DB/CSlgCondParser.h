/*
	katze 06/04/06
	SLG判定パーサー
*/
#pragma once

#include "../../../Scene/ConstScene.h"
#include "CSlgCondList.h"
#include "CCondChara.h"
#include "CCondPhase.h"
#include "CCondBattle.h"
#include "CCondFlag.h"
#include "CCondDeath.h"
#include "CCondVictory.h"

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

#include "slg_symbol.h"

namespace BMW{
namespace SLG{
struct phase_symbol;

struct characond_symbol : public boost::spirit::symbols<>
{
	characond_symbol()
	{
		add
			("HP",CCondChara::HP)
			("EXIST",CCondChara::EXIST)
			("INDEX",CCondChara::INDEX)
			("CHARA",CCondChara::CHARA)
			("DAMAGE",CCondChara::DAMAGE)
			("ID",CCondChara::ID)
			;
	}
};

struct battlekind_symbol : public boost::spirit::symbols<>
{
	battlekind_symbol()
	{
		add
			("CHARA",CCondBattle::CHARA)
			("DAMAGE",CCondBattle::DAMAGE)
			;
	}
};

struct flagtype_symbol : public boost::spirit::symbols<>
{
	flagtype_symbol()
	{
		add
			("COND",		CCondFlag::COND)
			("COND_GLOBAL",	CCondFlag::COND_GLOBAL)
			("SET",			CCondFlag::SET)
			("CALC",		CCondFlag::CALC)
			("SET_GLOBAL",	CCondFlag::SET_GLOBAL)
			("CALC_GLOBAL",	CCondFlag::CALC_GLOBAL)
			;
	}
};

struct death_symbol : public boost::spirit::symbols<>
{
	death_symbol()
	{
		add
			("ALL",CCondDeath::ALL)
			("ONE",CCondDeath::ONE)
			("ALL_ALIVE",CCondDeath::ALL_ALIVE)
			("ONE_ALIVE",CCondDeath::ONE_ALIVE)
			;
	}
};

struct condphase_symbol : public boost::spirit::symbols<>
{
	condphase_symbol()
	{
		add
			("EQ",CCondPhase::EQ)
			("GT",CCondPhase::GT)
			("LT",CCondPhase::LT)
			;
	}
};

struct victype_symbol : public boost::spirit::symbols<>
{
	victype_symbol()
	{
		add
			("VICTORY",CCondVictory::VICTORY)
			("LOSE",CCondVictory::LOSE)
			("EXPERT",CCondVictory::EXPERT)
			;
	}
};

struct CSlgCondParser : public boost::spirit::grammar<CSlgCondParser>
{
	CSlgCondList* pList_;
	CSLGDef* p_;

	// アクセッサ
	void setCondList(CSlgCondList* pList){ pList_=pList; }
	void setSlgDef(CSLGDef* pDef){ p_=pDef; }

	template<typename S>
	struct definition
	{
		definition(const CSlgCondParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// 初期化
			pList_ = self.pList_;

			//start_ = *(not_ | cond_ );

			cond_ = *(flag_ | phase_ | death_live_ | death_ | chara_ | battle_ | victory_ | or_ | and_ | not_);

			// <chara id="" type="" (value=""|id="") />
			chara_	= str_p("<chara")
						>> eps_p[bind(&definition::newChara)(var(*this))]
						>> id_[bind(&definition::setCharaID)(var(*this),arg1,const_cast<CSlgCondParser&>(self).p_)]
						>> str_p("type=\"") >> charatype_[bind(&CCondChara::setType)(var(pChara_),arg1)] >> '"'
						>> if_p(bind(&CCondChara::getType)(var(pChara_))==CCondChara::CHARA)
							[// キャラID
								id_[chara_.val=bind(&katzeSDK::Misc::CStringMap::getValue)(var(Chara::Const::charaID_),arg1),
									bind(&CCondChara::setValue)(var(pChara_),chara_.val)]
							]
							.else_p
							[
								if_p(bind(&CCondChara::getType)(var(pChara_))==CCondChara::ID)
								[// SLG ID
									id_[chara_.val=bind(&CSLGDef::getSlgID)(var(*(self.p_)),arg1),
										bind(&CCondChara::setValue)(var(pChara_),chara_.val)]
								]
								.else_p
								[
									str_p("value=\"") >> int_p[bind(&CCondChara::setValue)(var(pChara_),arg1)] >> '"'
								]
							]
					>> str_p("/>")
					;
			// <phase !cond="" !turn="" !phase="" />
			phase_	= str_p("<phase") 
						>> eps_p[bind(&definition::newPhase)(var(*this))]
						>> !(str_p("cond=\"") >>  condphasetype_[bind(&CCondPhase::setCond)(var(pPhase_),arg1)] >> '"')
						>> !(str_p("turn=\"") >>  int_p[bind(&CCondPhase::setTurn)(var(pPhase_),arg1)] >> '"')
						>> !(str_p("phase=\"") >> phasetype_[bind(&CCondPhase::setPhase)(var(pPhase_),arg1)] >> '"')
					>> str_p("/>")
					;
			// <battle id="" !(kind="" value="") />
			battle_	= str_p("<battle") 
						>> eps_p[bind(&definition::newBattle)(var(*this))]
						>> id_[battle_.val=bind(&CSLGDef::getSlgID)(var(*self.p_),arg1),
							   bind(&CCondBattle::setChara)(var(pBattle_),battle_.val)]
						>> !(str_p("kind=\"") >> battlekind_[bind(&CCondBattle::setKind)(var(pBattle_),arg1)] >> '"'
						>>	 str_p("value=\"") >> int_p[bind(&CCondBattle::setValue)(var(pBattle_),arg1)] >> '"')
					>> str_p("/>")
					;
			// <flag !type="" name="" value="" />
			flag_	= str_p("<flag")
						>> eps_p[bind(&definition::newFlag)(var(*this))]
						>> !(str_p("type=\"") >> flagtype_[bind(&CCondFlag::setType)(var(pFlag_),arg1)] >> '"')
						>> if_p(bind(&CCondFlag::getType)(var(pFlag_))==CCondFlag::COND_GLOBAL
							 || bind(&CCondFlag::getType)(var(pFlag_))==CCondFlag::SET_GLOBAL
							 || bind(&CCondFlag::getType)(var(pFlag_))==CCondFlag::CALC_GLOBAL)
								[name_[flag_.val=bind(&katzeSDK::Misc::CStringMap::getValue)(var(Scene::Const::flagID_),arg1)]]
								.else_p
								[name_[flag_.val=bind(&CSLGDef::getFlagID)(var(*self.p_),arg1)]]
						>> eps_p[bind(&CCondFlag::setFlag)(var(pFlag_),flag_.val)]
						>> str_p("value=\"") >> int_p[bind(&CCondFlag::setValue)(var(pFlag_),arg1)] >> '"'
					>> str_p("/>")
					;
			// <death type="" phase="" />
			death_	= str_p("<death") 
						>> eps_p[bind(&definition::newDeath)(var(*this))]
						>> str_p("type=\"") >>  deathtype_[bind(&CCondDeath::setType)(var(pDeath_),arg1)] >> '"'
						>> str_p("phase=\"") >> phasetype_[bind(&CCondDeath::setPhase)(var(pDeath_),arg1)] >> '"'
					>> str_p("/>")
					;
			// <death_live phase="">
			//	+<chara id="" />
			// </death_live>
			death_live_	= str_p("<death_live")[bind(&definition::newDeathLive)(var(*this))]
							>> str_p("phase=\"") >> phasetype_[bind(&CCondDeathLive::setPhase)(var(pDeathLive_),arg1)] >> '"'
						>> str_p(">")
							>> +(str_p("<chara") >> id_[bind(&definition::setDeathLive)(var(*this),arg1,var(*self.p_))] >> str_p("/>"))
						>> str_p("</death_live>") 
						;
			// <victory !type="" />
			victory_	= str_p("<victory")[victory_.val=CCondVictory::VICTORY]
							>> !(str_p("type=\"") >> victype_[victory_.val=arg1] >> '"')
						>> str_p("/>")[bind(&definition::newVictory)(var(*this),victory_.val)]
						;

			// <or>*cond</or>
			or_		= str_p("<or>")[bind(&definition::newListOr)(var(*this))]
						>> cond_
					>> str_p("</or>")[bind(&definition::popList)(var(*this))]
					;

			// <and>*cond</and>
			and_	= str_p("<and>")[bind(&definition::newList)(var(*this))]
						>> cond_
					>> str_p("</and>")[bind(&definition::popList)(var(*this))]
					;

			// <not>cond</not>
			not_	= str_p("<not>") >> cond_ >> str_p("</not>")[bind(&definition::newNot)(var(*this))];

			// id属性
			id_	= str_p("id=\"")	>> (*(anychar_p - '"'))[id_.val = construct_<string>(arg1,arg2)] >> '"';

			// name属性
			name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';
		}
		
		const boost::spirit::rule<S>& start() const { return cond_; }

		// typedef
		typedef boost::spirit::rule<S>											rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>		rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>			rule_i;
		
		// rule
		rule		start_,phase_,death_,death_live_,cond_,or_,and_,not_;
		rule_i		chara_,battle_,flag_,victory_;
		rule_s		id_,name_;

		// シンボル
		phase_symbol		phasetype_;
		characond_symbol	charatype_;
		battlekind_symbol	battlekind_;
		flagtype_symbol		flagtype_;
		death_symbol		deathtype_;
		condphase_symbol	condphasetype_;
		victype_symbol		victype_;

		CCondChara*		pChara_;
		CCondPhase*		pPhase_;
		CCondBattle*	pBattle_;
		CCondFlag*		pFlag_;
		CCondDeath*		pDeath_;
		CCondDeathLive*	pDeathLive_;
		CSlgCondList*	pList_;
		stack<CSlgCondList*> pOldStack_;
		//ISlgCond*		pCond_;
		
		// 設定関数
		void newList()
		{
			pOldStack_.push(pList_);
			pList_ = new CSlgCondList();
			pOldStack_.top()->addCond(pList_);
		}

		void newListOr()
		{
			pOldStack_.push(pList_);
			pList_ = new CSlgCondListOr();
			pOldStack_.top()->addCond(pList_);
		}

		void popList()
		{
			pList_ = pOldStack_.top();
			pOldStack_.pop();
		}

		void newChara()
		{
			pChara_ = new CCondChara();
			pList_->addCond(pChara_);
		}

		void setCharaID(const string& sID, CSLGDef* p)
		{
			if(sID=="TARGET") pChara_->setChara(CCondChara::TARGET);
			ef(sID=="CTRL") pChara_->setChara(CCondChara::CTRL);
			else pChara_->setChara(p->getSlgID(sID));
		}

		void newPhase()
		{
			pPhase_ = new CCondPhase();
			pList_->addCond(pPhase_);
		}

		void newBattle()
		{
			pBattle_ = new CCondBattle();
			pList_->addCond(pBattle_);
		}

		void newFlag()
		{
			pFlag_ = new CCondFlag();
			pList_->addCond(pFlag_);
		}

		void newDeath()
		{
			pDeath_ = new CCondDeath();
			pList_->addCond(pDeath_);
		}

		void newDeathLive()
		{
			pDeathLive_ = new CCondDeathLive();
			pList_->addCond(pDeathLive_);
		}

		void setDeathLive(const string& sID, CSLGDef& def)
		{
			pDeathLive_->setSetChara(def.getSlgID(sID));
		}

		void newVictory(int nType)
		{
			pList_->addCond(new CCondVictory(nType));
		}

		void newNot()
		{
			// こいつの一つ前に、NotにすべきCondが入ってる
			pList_->addCond(new CSlgCondNot(pList_->popBackCond()));
		}
	};
};

} // namespace SLG end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね