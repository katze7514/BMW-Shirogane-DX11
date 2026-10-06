/*
	katze 06/05/24
	SLGスクリプト部分パーサー
*/
#pragma once

#include "../Context/CSLGDef.h"
#include "../Context/CSLGContext.h"

#include "CSlgEffectScriptFactory.h"

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

#include "effect_symbol_closure.h"

namespace BMW{
namespace SLG{
class CSLGDef;

namespace Effect{

struct CSlgEffectScriptParser : public boost::spirit::grammar<CSlgEffectScriptParser>
{
	CSlgEffectScriptParser(CSLGContext* p, VM::CScript* pScript):p_(p),pScript_(pScript){}
	CSLGContext*	p_;
	VM::CScript*	pScript_;

	template<typename S>
	struct definition
	{
		definition(const CSlgEffectScriptParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// コンテキストの設定
			pContext_ = self.p_;
			pScript_  = self.pScript_;

			// はじめ
			start_ = !(str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>"))
					>> !str_p("<effect>")
						>> *(load_ | addevent_ | addmap_ | del_ | wait_ | ctrl_ | nop_ | scroll_ | back_ | chip_ | skip_ | help_)
					>> !str_p("</effect>")
					;

			// <load no="" symbol="" !effect="" />
			load_ = str_p("<load") 
					>> no_[bind(&CCmdLoad::nNo_)(load_.val)=arg1]
					>> symbol_[bind(&CCmdLoad::sSymbol_)(load_.val)=arg1]
					>> !(str_p("effect=\"") >> loadeffect_[bind(&CCmdLoad::nEffect_)(load_.val)=arg1] >> '"')
					>> str_p("/>")[bind(&definition::newLoad)(var(*this),load_.val)]
					;

			// <addevent no="" !target="" !(index=""|chara="") !x="" !y="" />
			addevent_ = str_p("<addevent") 
						>> no_[bind(&CCmdAddSymbol::nNo_)(addevent_.val)=arg1]
						>> !(str_p("target=\"") >> eventtarget_[bind(&CCmdAddSymbol::nTarget_)(addevent_.val)=addevent_.target=arg1] >> '"')
						>> if_p(addevent_.target!=CCode_add_event_symbol::EVENT)
							[
								if_p(addevent_.target==CCode_add_event_symbol::EVENT_MAP)
								[index_[bind(&CCmdAddSymbol::nIndex_)(addevent_.val)=arg1]]
								.else_p
								[chara_[bind(&CCmdAddSymbol::sChara_)(addevent_.val)=arg1]]
							]
						>> !x_[bind(&CCmdAddSymbol::nX_)(addevent_.val)=arg1]
						>> !y_[bind(&CCmdAddSymbol::nY_)(addevent_.val)=arg1]
						>> str_p("/>")[bind(&definition::newAddEvent)(var(*this),addevent_.val)]
						;

			// <addmap no="" !target="" (index=""|chara="") !x="" !y="" />
			addmap_ = str_p("<addmap") 
						>> no_[bind(&CCmdAddSymbol::nNo_)(addmap_.val)=arg1]
						>> !(str_p("target=\"") >> maptarget_[bind(&CCmdAddSymbol::nTarget_)(addmap_.val)=addmap_.target=arg1] >> '"')
						>> if_p(addmap_.target==CCode_add_map_symbol::MAP)
							[index_[bind(&CCmdAddSymbol::nIndex_)(addmap_.val)=arg1]]
							.else_p
							[chara_[bind(&CCmdAddSymbol::sChara_)(addmap_.val)=arg1]]
						>> !x_[bind(&CCmdAddSymbol::nX_)(addmap_.val)=arg1]
						>> !y_[bind(&CCmdAddSymbol::nY_)(addmap_.val)=arg1]
					>> str_p("/>")[bind(&definition::newAddMap)(var(*this),addmap_.val)]
					;

			// <del no="" />
			del_ = str_p("<del") >> no_[bind(&definition::newDel)(var(*this),arg1)] >> str_p("/>");

			// <wait no="" />
			wait_ = str_p("<wait") >> no_[bind(&definition::newWait)(var(*this),arg1)] >> str_p("/>");

			// <ctrl no="" type="" value="" />
			ctrl_ = str_p("<ctrl") 
						>> no_[bind(&CCmdCtrl::nNo_)(ctrl_.val)=arg1]
						>> str_p("type=\"") >> ctrltype_[bind(&CCmdCtrl::nType_)(ctrl_.val)=arg1] >> '"'
						>> str_p("value=\"") >> bool_[bind(&CCmdCtrl::nValue_)(ctrl_.val)=arg1] >> '"'
					>> str_p("/>")[bind(&definition::newCtrl)(var(*this),ctrl_.val)]
					;

			// <nop />
			nop_ = str_p("<nop") >> str_p("/>")[bind(&definition::newNop)(var(*this))];

			// <scroll type="" (index=""|chara=""|(!x="" !y="" frame="" edging="")) />
			scroll_ = str_p("<scroll")
						>> !(str_p("type=\"") >> mapscrolltype_[bind(&CCmdMapScroll::nType_)(scroll_.val)=scroll_.type=arg1] >> '"')
						>> if_p(scroll_.type==Code::CCode_map_scroll::MAP)
							[// インデックス
								index_[bind(&CCmdMapScroll::nIndex_)(scroll_.val)=arg1]
							]
							.else_p
							[
								if_p(scroll_.type==Code::CCode_map_scroll::CHARA)
								[// キャラ
									chara_[bind(&CCmdMapScroll::sChara_)(scroll_.val)=arg1]
								]
								.else_p
								[// 座標
									!x_[bind(&CCmdMapScroll::nX_)(scroll_.val)=arg1]
								>>	!y_[bind(&CCmdMapScroll::nY_)(scroll_.val)=arg1]
								>>	str_p("frame=\"") >> int_p[bind(&CCmdMapScroll::nFrame_)(scroll_.val)=arg1] >> '"'
								>>	str_p("edging=\"") >> int_p[bind(&CCmdMapScroll::nEdging_)(scroll_.val)=arg1] >> '"'
								]
							]
					>> str_p("/>")[bind(&definition::newMapScroll)(var(*this),scroll_.val)]
					;

			// <back symbol="" />
			back_	= str_p("<back") 
						>> symbol_[bind(&CCmdLoad::sSymbol_)(back_.val)=arg1]
					>> str_p("/>")[bind(&definition::newBack)(var(*this),back_.val)]
					;

			// <mapchip !type="" (chara="" visible=""| ctrl="" symbol="" index="" priority="" !visible="") />
			chip_	= str_p("<mapchip") 
						>> !(str_p("type=\"") >> mapchiptype_[bind(&CCmdMapChip::nType_)(chip_.val)=arg1] >> '"')
						>> if_p(CCode_ctrl_map_chip::CHARA==bind(&CCmdMapChip::nType_)(chip_.val))
							[
								chara_[bind(&CCmdMapChip::sSlg_)(chip_.val)=arg1]
								>> str_p("visible=\"") >> bool_[bind(&CCmdMapChip::bVisible_)(chip_.val)=arg1] >> '"'
							]
							.else_p
							[
								str_p("ctrl=\"") >> mapchipctrl_[bind(&CCmdMapChip::nCtrl_)(chip_.val)=arg1] >> '"'
							>>	!symbol_[bind(&CCmdMapChip::sSlg_)(chip_.val)=arg1]
							>>	str_p("index=\"") >> int_p[bind(&CCmdMapChip::nIndex_)(chip_.val)=arg1] >> '"'
							>>	str_p("priority=\"") >> int_p[bind(&CCmdMapChip::nPriority_)(chip_.val)=arg1] >> '"'
							>> !(str_p("visible=\"") >> bool_[bind(&CCmdMapChip::bVisible_)(chip_.val)=arg1] >> '"')
							]
					>> str_p("/>")[bind(&definition::newMapChip)(var(*this),chip_.val)]
					;

			// <skip flag="" />
			skip_	= str_p("<skip")
						>> str_p("flag=\"") >> skipFlag_[bind(&definition::newSkip)(var(*this),arg1)] >> '"'
					>> str_p("/>")
					;

			// <help type="" />
			help_	= str_p("<help")
						>> str_p("type=\"") >> helpID_[bind(&definition::newHelp)(var(*this),arg1)] >> '"'
					>> str_p("/>")
					;

			// symbol属性
			symbol_	= str_p("symbol=\"")	>> (*(anychar_p - '"'))[symbol_.val = construct_<string>(arg1,arg2)] >> '"';

			// chara_属性
			chara_		= str_p("chara=\"") >> (*(anychar_p - '"'))[chara_.val = construct_<string>(arg1,arg2)] >> '"';

			// no属性
			no_		= str_p("no=\"") >> int_p[no_.val = arg1] >> '"';

			// index属性
			index_		= str_p("index=\"") >> int_p[index_.val = arg1] >> '"';

			// x属性
			x_		= str_p("x=\"") >> int_p[x_.val = arg1] >> '"';

			// y属性
			y_		= str_p("y=\"") >> int_p[y_.val = arg1] >> '"';
		}
		
		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>											rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>		rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>			rule_i;
		typedef boost::spirit::rule<S, load_symbol_closure::context_t>			rule_load;
		typedef boost::spirit::rule<S, add_symbol_closure::context_t>			rule_add;
		typedef boost::spirit::rule<S, ctrl_symbol_closure::context_t>			rule_ctrl;
		typedef boost::spirit::rule<S, map_scroll_closure::context_t>			rule_scroll;
		typedef boost::spirit::rule<S, map_chip_closure::context_t>				rule_chip;
		
		// rule
		rule		start_;
		rule_s		chara_,symbol_;
		rule_i		no_,index_,x_,y_;
		rule_load	load_,back_;
		rule_add	addevent_,addmap_;
		rule_i		del_,wait_;
		rule_ctrl	ctrl_;
		rule		nop_,skip_;
		rule_scroll	scroll_;
		rule_chip	chip_;
		rule		help_;

		// symbol
		Parser::boolsym			bool_;
		load_effect_symbol		loadeffect_;
		add_event_target_symbol	eventtarget_;
		add_map_target_symbol	maptarget_;
		ctrl_symbol_symbol		ctrltype_;
		map_scroll_symbol		mapscrolltype_;
		skip_flag_symbol		skipFlag_;
		help_symbol				helpID_;
		mapchip_type_symbol		mapchiptype_;
		mapchip_ctrl_symbol		mapchipctrl_;
		
		// 一時データとか
		CSLGContext*		pContext_;
		VM::CScript*		pScript_;

		void newLoad(CCmdLoad& cmd)
		{
			CSlgEffectScriptFactory::createLoad(cmd, pContext_, pScript_);
		}

		void newAddEvent(CCmdAddSymbol& cmd)
		{
			CSlgEffectScriptFactory::createAddEvent(cmd, pContext_, pScript_);
		}

		void newAddMap(CCmdAddSymbol& cmd)
		{
			CSlgEffectScriptFactory::createAddMap(cmd, pContext_, pScript_);
		}

		void newDel(int n)
		{
			CSlgEffectScriptFactory::createDel(n,pScript_);
		}

		void newWait(int n)
		{
			CSlgEffectScriptFactory::createWait(n,pScript_);
		}

		void newCtrl(CCmdCtrl& cmd)
		{
			CSlgEffectScriptFactory::createCtrl(cmd,pScript_);
		}

		void newNop()
		{
			pScript_->addCode(new VM::Code::CCode_nop());
		}

		void newMapScroll(CCmdMapScroll& cmd)
		{
			CSlgEffectScriptFactory::createMapScroll(cmd, pContext_, pScript_);
		}

		void newBack(CCmdLoad& cmd)
		{
			CSlgEffectScriptFactory::createBack(cmd, pContext_, pScript_);
		}

		void newMapChip(CCmdMapChip& cmd)
		{
			CSlgEffectScriptFactory::createMapChip(cmd, pContext_, pScript_);
		}

		void newSkip(int nSkip)
		{
			CSlgEffectScriptFactory::createSkip(nSkip,pScript_);
		}

		void newHelp(int nHelp)
		{
			CSlgEffectScriptFactory::createHelp(nHelp,pScript_);
		}
	};
};

} // namespace Effect end
} // namespace SLG end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね