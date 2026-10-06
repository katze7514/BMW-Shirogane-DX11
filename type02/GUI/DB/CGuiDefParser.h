/*
	katze 06/01/20
	GUI定義解析
*/
#pragma once

#include "../../Movie/DB/CSymbolDB.h"
#include "CDataGuiDefPanel.h"
#include "CDataGuiDefGage.h"
#include "CDataGuiDefRemain.h"
#include "CDataGuiDefCircle.h"
#include "CDataGuiDefAction.h"
#include "CDataGuiDefGui.h"
#include "CDataGuiDefWidget.h"

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

#include "../../Movie/DB/symbol_closure.h"
#include "guidef_symbol.h"

#ifdef BMW_DEBUG
//#define BMW_DEBUG_INTER
#endif

struct str_int_closure : public boost::spirit::closure<str_int_closure, string, int>
{
	member1 str;
	member2 val;
};

#include "CGuiDefDB.h"
namespace BMW{
namespace GUI{

class CGuiDefDB;
struct CGuiDefParser : public boost::spirit::grammar<CGuiDefParser>
{
	CGuiDefParser(CGuiDefDB& db,Movie::CSymbolDB& symbolDB):db_(db),symbolDB_(symbolDB){}

	CGuiDefDB& db_;
	Movie::CSymbolDB& symbolDB_;

	template<typename S>
	struct definition
	{
		definition(const CGuiDefParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;
			
			// スタート
			start_ = !xml_ >> interface_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <interface>
			//	*(<include /> | <symboldef />)
			//	*(<panel> | <action> | <remain> | <gage> | <circle> | <button> | <text> | <graphic>)
			// </interface>
			interface_	= str_p("<interface>")
							>> *(symboldef_ | include_)
							>> *(panel_ | button_ | graphic_ | remain_ | gage_ | circle_ | text_g_ | action_)
						>> str_p("</interface>")
						;

			// src属性
			src_	= str_p("src=\"")	>> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';

			// <include src="" />
			include_	= str_p("<include") 
							>> src_[bind(&CGuiDefDB::setGuiDef)(var(self.db_),arg1)]
						>> str_p("/>")
						;
			
			// <symboldef src="" />
			symboldef_	= str_p("<symboldef")
						>> src_[bind(&Movie::CSymbolDB::setSymbol)(var(self.symbolDB_),arg1)]
						>> str_p("/>")
						;

			// id属性
			//id_	= str_p("id=\"")	>> (*(anychar_p - '"'))[id_.val = construct_<string>(arg1,arg2)] >> '"';

			// name属性
			name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';

			// <panel name="" !type="">
			//	!<pos x="" y="" />
			//	*<widget></widget>
			// </panel>
			panel_	= str_p("<panel") 
							>> name_[bind(&definition::newPanel)(var(*this),var(self.db_),arg1)]
							>> !(str_p("type=\"") >> panelType_[bind(&CDataGuiDefPanel::setPanelType)(var(pDefPanel_),arg1)] >> '"')
						>> '>'
						>> !pos_[bind(&CDataGuiDefPanel::setPos)(var(pDefPanel_),arg1)]
						>> *widget_
					>> str_p("</panel>")
					;

			// <widget name="">
			//	*(<symbol> | <button> | <num> | <text> | <panel> | <obj> |<gage> | <remain> | <face> | <name> | <graphic>)
			//	!<pos x="" y="" />
			// </widget>
			widget_	= str_p("<widget") >> name_[widget_.val=arg1] >> '>'
						>> *(symbol_ | button_w_| graphic_w_ | num_ | text_ | panel_w_ | obj_ | gage_w_ | remain_w_ | face_ | name_w_)
						>> eps_p[bind(&IDataGuiDefWidget::setID)(var(pWidget_),widget_.val)]
						>> !pos_[bind(&IDataGuiDefWidget::setPos)(var(pWidget_),arg1)]
						>> eps_p[bind(&definition::resetWidget)(var(*this))]
					>> str_p("</widget>")
					;

			// <symbol name="" />
			symbol_	= str_p("<symbol")
					>> eps_p[bind(&definition::newSymbol)(var(*this))]
					>> name_[symbol_.val = bind(&Movie::CSymbolDB::getID)(var(self.symbolDB_),arg1)]
					>> eps_p[bind(&CDataGuiDefSymbol::setSymbolID)(var(pSymbol_),symbol_.val)]
					>> str_p("/>")
					;

			// <button (name=""|symbol="") !type="" />
			button_w_	= str_p("<button")
						>> eps_p[bind(&definition::newWidgetButton)(var(*this))]
						>> (name_[bind(&CDataGuiDefWidgetButton::setButtonID)(var(pWidgetButton_),arg1)]
							| 
							(str_p("symbol=\"") >> (*(anychar_p - '"'))[button_w_.str = construct_<string>(arg1,arg2)] >> '"'
								>> eps_p[button_w_.val=bind(&Movie::CSymbolDB::getID)(var(self.symbolDB_),button_w_.str)]
								>> eps_p[bind(&CDataGuiDefWidgetButton::setSymbolID)(var(pWidgetButton_),button_w_.val)])
							)
						>> !(str_p("type=\"") >> buttonactSymbol_[bind(&CDataGuiDefWidgetButton::setAct)(var(pWidgetButton_),arg1)] >> '"')
					>> str_p("/>")
					;

			// <graphic name="" />
			graphic_w_	= str_p("<graphic")
						>> eps_p[bind(&definition::newWidgetGraphic)(var(*this))]
						>> name_[bind(&CDataGuiDefWidgetGraphic::setGraphicID)(var(pWidgetGraphic_),arg1)]
						>> str_p("/>")
						;

			// <num +name="" />
			num_	= str_p("<num")
						>> eps_p[bind(&definition::newNum)(var(*this))]
						>> +(name_[num_.val = bind(&Movie::CSymbolDB::getID)(var(self.symbolDB_),arg1)]
							>> eps_p[bind(&CDataGuiDefNum::setNumID)(var(pNum_),num_.val)])
					>> str_p("/>")
					; 

			// <text !font="" !size="" !side="" !color="" !type="" /> | <text !font="" !size="" !side="" !color="" !type="">TEXT</text>
			// <text name="" />
			text_	= str_p("<text")
					>> eps_p[bind(&definition::newText)(var(*this))]
					>> 
						(
						!(str_p("font=\"") >> fontSymbol_[bind(&CDataGuiDefText::setFont)(var(pText_),arg1)] >> '"')
						>> !(str_p("size=\"") >> int_p[bind(&CDataGuiDefText::setSize)(var(pText_),arg1)] >> '"')
						>> !(str_p("side=\"") >> sidefSymbol_[bind(&CDataGuiDefText::setSide)(var(pText_),arg1)] >> '"')
						>> !(str_p("color=\"") 
								>> (*(anychar_p -'"'))[text_.val=construct_<string>(arg1,arg2),bind(&CDataGuiDefText::setColorRGB)(var(pText_),text_.val)] 
						>> '"')
						>> !(str_p("type=\"") >> texttypeSymbol_[bind(&CDataGuiDefText::setType)(var(pText_),arg1)] >> '"')
						>> (
							str_p("/>")
							|
							('>' >> (*(anychar_p - "</text>"))[text_.val=construct_<string>(arg1,arg2)] 
									>> eps_p[bind(&CDataGuiDefText::setText)(var(pText_),text_.val)] 
								>> str_p("</text>")
							)
						   )
						)
						|
						(	
							name_[bind(&CDataGuiDefText::setText)(var(pText_),arg1)]
							>> eps_p[bind(&CDataGuiDefText::setType)(var(pText_),Text::NAME)]
							>> str_p("/>")
						)
					;

			// <panel name="" />
			panel_w_= str_p("<panel")
						>> eps_p[bind(&definition::newWidgetPanel)(var(*this))]
						>> name_[bind(&CDataGuiDefWidgetPanel::setPanelID)(var(pWidgetPanel_),arg1)]
					>> str_p("/>")
					; 

			// <obj !type="" />
			obj_	= str_p("<obj")
						>> eps_p[bind(&definition::newObj)(var(*this))]
						>> !(str_p("type=\"") >> objType_[bind(&CDataGuiDefObj::setType)(var(pObj_),arg1)] >> '"')
					>> str_p("/>")
					; 
			

			// <remain name="" !turn="" />
			remain_w_	= str_p("<remain")
						>> eps_p[bind(&definition::newWidgetRemain)(var(*this))]
						>> name_[bind(&CDataGuiDefWidgetRemain::setRemainID)(var(pWidgetRemain_),arg1)]
						>> !(str_p("turn=\"") >> int_p[bind(&CDataGuiDefWidgetRemain::setTurn)(var(pWidgetRemain_),arg1)] >> '"')
					>> str_p("/>")
					;

			// <gage name="" !left="" />
			gage_w_	= str_p("<gage")
						>> eps_p[bind(&definition::newWidgetGage)(var(*this))]
						>> name_[bind(&CDataGuiDefWidgetGage::setGageID)(var(pWidgetGage_),arg1)]
						>> !(str_p("left=\"") >> boolSymbol_[bind(&CDataGuiDefWidgetGage::left)(var(pWidgetGage_),arg1)] >> '"')
					>> str_p("/>")
					; 

			// <face toward="" battle="" />
			face_	= str_p("<face")
						>> eps_p[bind(&definition::newFace)(var(*this))]
						>> str_p("toward=\"") >> sideSymbol_[bind(&CDataGuiDefFace::setToward)(var(pFace_),arg1)] >> '"'
						>> str_p("battle=\"") >> boolSymbol_[bind(&CDataGuiDefFace::battle)(var(pFace_),arg1)] >> '"'
					>> str_p("/>")
					;

			// <name !toward="" />
			name_w_	= str_p("<name") 
						>> eps_p[bind(&definition::newName)(var(*this))]
						>> str_p("toward=\"") >> sideSymbol_[bind(&CDataGuiDefName::setToward)(var(pName_),arg1)] >> '"'
					>> str_p("/>")
					;

			// <pos x="" y="" />
			pos_	=  str_p("<pos")
					>> eps_p[bind(&POINT::x)(pos_.val)=0] >> eps_p[bind(&POINT::y)(pos_.val)=0]
					>> !(str_p("x=\"") >> int_p[bind(&POINT::x)(pos_.val)=arg1] >> '"')
					>> !(str_p("y=\"") >> int_p[bind(&POINT::y)(pos_.val)=arg1] >> '"')
					>> str_p("/>")
					;

			// <action name="" panel="">
			//	*(<state> | <tween>)
			// </action>
			action_	= str_p("<action")
						>> name_[action_.val=arg1]
						>> eps_p[bind(&definition::newAction)(var(*this),var(self.db_),action_.val)]
						>> panel_a_[bind(&CDataGuiDefAction::setTargetID)(var(pAction_),arg1)]
						>> '>'
						>> *(state_ | tween_)
					>> str_p("</action>")
					;

			// panel属性
			panel_a_ = str_p("panel=\"") >> (*(anychar_p -'"'))[panel_a_.val=construct_<string>(arg1,arg2)] >> '"';

			// <state>
			//	<draw>
			// </state>
			state_	= str_p("<state>")
					>> eps_p[bind(&definition::newState)(var(*this))]
						>> draw_[bind(&CDataGuiDefActionState::setDrawInfo)(var(pState_),arg1)]
					>> str_p("</state>")
					;

			// <tween>
			//	<draw><draw><frame><edging>
			// </tween>
			tween_	= str_p("<tween>")
						>> eps_p[bind(&definition::newTween)(var(*this))]
						>> draw_[bind(&CDataGuiDefActionTween::setDrawInfo)(var(pTween_),arg1)]
						>> draw_[bind(&CDataGuiDefActionTween::setEnd)(var(pTween_),arg1)]
						>> str_p("<frame")
							>> str_p("value=\"") >> int_p[bind(&CDataGuiDefActionTween::setFrame)(var(pTween_),arg1)] >> '"'
						>> str_p("/>")
						>> str_p("<edging") 
							>> str_p("value=\"") >> int_p[bind(&CDataGuiDefActionTween::setEdging)(var(pTween_),arg1)] >> '"'
						>> str_p("/>")
					>> str_p("</tween>")
					;

			// <draw x="" y="" alpha="" widht="" height="" angle="" />
			draw_	= str_p("<draw")
					>> !(str_p("x=\"") 
						>> int_p[bind(&Draw::CDrawInfo::setX)(draw_.val,arg1)]
						>> '"')
					>> !(str_p("y=\"") 
						>> int_p[bind(&Draw::CDrawInfo::setY)(draw_.val,arg1)] 
						>> '"')
					>> !(str_p("alpha=\"")
						>> int_p[bind(&Draw::CDrawInfo::setAlpha)(draw_.val,(arg1*Draw::CDrawInfo::SCALE_ALPHA)/100)]
						>> '"')
					>> !(str_p("width=\"")
						>> int_p[bind(&Draw::CDrawInfo::setWidth)(draw_.val,(arg1*Draw::CDrawInfo::SCALE_RATE)/100)]
						>> '"')
					>> !(str_p("height=\"")
						>> int_p[bind(&Draw::CDrawInfo::setHeight)(draw_.val,(arg1*Draw::CDrawInfo::SCALE_RATE)/100)]
						>> '"')
					>> !(str_p("angle=\"")
						>> int_p[bind(&Draw::CDrawInfo::setAngle)(draw_.val,(arg1*Draw::CDrawInfo::SCALE_ANGLE)/360)]
						>> '"')
					>> str_p("/>")
					;

			// <remain name="">
			//	!<turn value="" />
			//	<current_num_n name="" />
			//	<current_num_e name="" />
			//	!<max_num name="" x="" y="" />
			//	!<slash name="" x="" y="" />
			//</remain>
			remain_	= str_p("<remain") >> name_[remain_.str=arg1] >> '>'
					>> eps_p[bind(&definition::newRemain)(var(*this),var(self.db_),remain_.str)]
					>> !(str_p("<turn")
						>> str_p("value=\"") >> int_p[bind(&CDataGuiDefRemain::setTurn)(var(pRemain_),arg1)] >> '"'
						>> str_p("/>"))
					>> str_p("<current_num_n") 
						>> name_[remain_.val=bind(&Movie::CSymbolDB::getID)(var(self.symbolDB_),arg1)]
						>> eps_p[bind(&CDataGuiDefRemain::setCurrentNumN)(var(pRemain_),remain_.val)]
						>> str_p("/>")
					>> str_p("<current_num_e") 
						>> name_[remain_.val=bind(&Movie::CSymbolDB::getID)(var(self.symbolDB_),arg1)]
						>> eps_p[bind(&CDataGuiDefRemain::setCurrentNumE)(var(pRemain_),remain_.val)]
						>> str_p("/>")
					>> !(str_p("<max_num") 
						>> name_[remain_.val=bind(&Movie::CSymbolDB::getID)(var(self.symbolDB_),arg1)]
						>> eps_p[bind(&CDataGuiDefRemain::setMaxNum)(var(pRemain_),remain_.val,CDataGuiDefRemain::SYMBOL)]
						>> !(str_p("x=\"") >> int_p[bind(&CDataGuiDefRemain::setMaxNum)(var(pRemain_),arg1,CDataGuiDefRemain::X)] >> '"')
						>> !(str_p("y=\"") >> int_p[bind(&CDataGuiDefRemain::setMaxNum)(var(pRemain_),arg1,CDataGuiDefRemain::Y)] >> '"')
						>> str_p("/>"))
					>> !(str_p("<slash")
						>> name_[remain_.val=bind(&Movie::CSymbolDB::getID)(var(self.symbolDB_),arg1)]
						>> eps_p[bind(&CDataGuiDefRemain::setSlash)(var(pRemain_),remain_.val,CDataGuiDefRemain::SYMBOL)]
						>> !(str_p("x=\"") >> int_p[bind(&CDataGuiDefRemain::setSlash)(var(pRemain_),arg1,CDataGuiDefRemain::X)]) >> '"'
						>> !(str_p("y=\"") >> int_p[bind(&CDataGuiDefRemain::setSlash)(var(pRemain_),arg1,CDataGuiDefRemain::Y)]) >> '"' 
						>> str_p("/>"))
				>> str_p("</remain>")
				;
			
			// <gage name="">
			//	!<left value="" />
			//	<current_gage name="" />
			//	<remain name="" x="" y="" />
			// </gage>
			gage_	= str_p("<gage") >> name_[gage_.str=arg1] >> '>'
						>> eps_p[bind(&definition::newGage)(var(*this),var(self.db_),gage_.str)]
						>> !(str_p("<left") 
							>> str_p("value=\"") >> boolSymbol_[bind(&CDataGuiDefGage::left)(var(pGage_),arg1)] >> '"' 
						>> str_p("/>"))
						>> str_p("<current_gage") >> name_[gage_.val=bind(&Movie::CSymbolDB::getID)(var(self.symbolDB_),arg1)] >> str_p("/>")
						>> eps_p[bind(&CDataGuiDefGage::setCurrentGage)(var(pGage_),gage_.val)]
						>> str_p("<remain") 
							>> name_[bind(&CDataGuiDefGage::setRemain)(var(pGage_),arg1)]
							>> !(str_p("x=\"") >> int_p[bind(&CDataGuiDefGage::setRemainX)(var(pGage_),arg1)] >> '"')
							>> !(str_p("y=\"") >> int_p[bind(&CDataGuiDefGage::setRemainY)(var(pGage_),arg1)] >> '"')
						>> str_p("/>")
					>> str_p("</gage>")
					;

			// <circle name="">
			//	!<pos x="" y="" />
			//	<r value="" />
			//	!<frame intro="" exit="" />
			//	*<widget />
			// </circle>
			circle_	= str_p("<circle") >> name_[circle_.val = arg1] >> '>'
						>> eps_p[bind(&definition::newCircle)(var(*this),var(self.db_),circle_.val)]
						>> !pos_[bind(&CDataGuiDefCircle::setPos)(var(pCircle_),arg1)]
						>> str_p("<r")
							>> str_p("value=\"") >> int_p[bind(&CDataGuiDefCircle::setR)(var(pCircle_),arg1)] >> '"'
						>> str_p("/>")
						>> !(str_p("<frame")
							>> str_p("intro=\"") >> int_p[bind(&CDataGuiDefCircle::setIntro)(var(pCircle_),arg1)] >> '"'
							>> str_p("exit=\"") >> int_p[bind(&CDataGuiDefCircle::setExit)(var(pCircle_),arg1)] >> '"'
						>> str_p("/>"))
						>> *widget_
					>> str_p("</circle>")
					;

			//	<popup>TEXT</popup>
			popup_ = str_p("<popup>") 
							>> (*(anychar_p - "</popup>"))[popup_.val = construct_<string>(arg1,arg2)]
					>> str_p("</popup>")
					;

			// <button name="">
			//	<symbol name="" />
			//	!<popup>TEXT</popup>
			// </button>
			button_	= str_p("<button") >> name_[button_.str = arg1] >> str_p(">")
						>> eps_p[bind(&definition::newButton)(var(*this),var(self.db_),button_.str)]
						>> str_p("<symbol")
							>> name_[button_.val=bind(&Movie::CSymbolDB::getID)(var(self.symbolDB_),arg1)]
							>> eps_p[bind(&CDataGuiDefButton::setSymbolID)(var(pButton_),button_.val)]
						>> str_p("/>")
						>> !popup_[bind(&CDataGuiDefButton::setPopUp)(var(pButton_),arg1)]
					>> str_p("</button>")
					;

			// <text name="" !font="" !size="" !side="" !color="" !type="">
			// !<popup>TEXT</popup>
			// TEXT
			// </text>
			text_g_	= str_p("<text")
					>> name_[bind(&definition::newTextGui)(var(*this),var(self.db_),arg1)]
					>> !(str_p("font=\"") >> fontSymbol_[bind(&CDataGuiDefTextGui::setFont)(var(pTextGui_),arg1)] >> '"')
					>> !(str_p("size=\"") >> int_p[bind(&CDataGuiDefTextGui::setSize)(var(pTextGui_),arg1)] >> '"')
					>> !(str_p("side=\"") >> sidefSymbol_[bind(&CDataGuiDefTextGui::setSide)(var(pTextGui_),arg1)] >> '"')
					>> !(str_p("color=\"") 
							>> (*(anychar_p - '"'))[text_g_.val=construct_<string>(arg1,arg2),bind(&CDataGuiDefTextGui::setColorRGB)(var(pTextGui_),text_g_.val)] 
						>> '"')
					>> !(str_p("type=\"") >> texttypeSymbol_[bind(&CDataGuiDefTextGui::setType)(var(pTextGui_),arg1)] >> '"')
					>> '>' 
					>> !popup_[bind(&CDataGuiDefTextGui::setPopUp)(var(pTextGui_),arg1)]
					>> (*(anychar_p - "</text>"))[text_g_.val = construct_<string>(arg1,arg2)]
					>> eps_p[bind(&CDataGuiDefTextGui::setText)(var(pTextGui_),text_g_.val)]
					>> str_p("</text>")
					;

			// <graphic name="">
			//	<symbol name="" />
			//	!<popup>TEXT</popup>
			// </graphic>
			graphic_	= str_p("<graphic") >> name_[graphic_.str = arg1] >> str_p(">")
						>> eps_p[bind(&definition::newGraphic)(var(*this),var(self.db_),graphic_.str)]
						>> str_p("<symbol") 
							>> name_[graphic_.val=bind(&Movie::CSymbolDB::getID)(var(self.symbolDB_),arg1)]
							>> eps_p[bind(&CDataGuiDefGraphic::setSymbolID)(var(pGraphic_),graphic_.val)]
						>> str_p("/>")
						>> !popup_[bind(&CDataGuiDefGraphic::setPopUp)(var(pGraphic_),arg1)]
					>> str_p("</graphic>")
					;

		};

		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>										rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>	rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>		rule_i;
		typedef boost::spirit::rule<S, str_int_closure::context_t>			rule_g;
		typedef boost::spirit::rule<S, Draw::point_closure::context_t>		rule_p;
		typedef boost::spirit::rule<S, Movie::draw_closure::context_t>		rule_d;

		// rule
		rule	start_,xml_;
		rule	interface_,include_,symboldef_;
		rule_s	src_,name_;
		rule	panel_;
		rule_s	widget_,circle_;
		rule_i	symbol_,num_,panel_w_,obj_,gage_w_,face_;
		rule_g	button_w_;
		rule	remain_w_,graphic_w_;
		rule_s	text_,name_w_;
		rule_p	pos_;
		rule_s	action_,panel_a_;
		rule	state_,tween_;
		rule_d	draw_;
		rule_g	gage_,remain_,button_,graphic_;
		rule_s	text_g_,popup_;

		// シンボル
		Parser::boolsym		boolSymbol_;
		font_symbol			fontSymbol_;
		side_symbol			sideSymbol_;
		side_f_symbol		sidefSymbol_;
		text_type_symbol	texttypeSymbol_;
		button_symbol		buttonactSymbol_;
		panel_symbol		panelType_;
		obj_symbol			objType_;

		// 一時データ
		CDataGuiDefPanel*		pDefPanel_;

		IDataGuiDefWidget*			pWidget_; // 現在処理中のWidget
		CDataGuiDefSymbol*			pSymbol_;
		CDataGuiDefNum*				pNum_;
		CDataGuiDefText*			pText_;
		CDataGuiDefWidgetPanel*		pWidgetPanel_;
		CDataGuiDefWidgetRemain*	pWidgetRemain_;
		CDataGuiDefWidgetGage*		pWidgetGage_;
		CDataGuiDefWidgetButton*	pWidgetButton_;
		CDataGuiDefWidgetGraphic*	pWidgetGraphic_;
		CDataGuiDefFace*			pFace_;
		CDataGuiDefName*			pName_;
		CDataGuiDefObj*				pObj_;

		CDataGuiDefAction*		pAction_;
		CDataGuiDefActionState*	pState_;
		CDataGuiDefActionTween*	pTween_;

		CDataGuiDefRemain*		pRemain_;
		CDataGuiDefGage*		pGage_;
		CDataGuiDefCircle*		pCircle_;
		CDataGuiDefButton*		pButton_;
		CDataGuiDefTextGui*		pTextGui_;
		CDataGuiDefGraphic*		pGraphic_;

		void resetWidget()
		{
			/*#ifdef BMW_DEBUG_INTER
				CDbg().Out("WIDGET %s",pWidget_->getID().c_str());
			#endif*/
			pWidget_=NULL;
		}
		// データ生成
		void newPanel(CGuiDefDB& db, const string& name)
		{
			/*#ifdef BMW_DEBUG_INTER
				CDbg().Out("PANEL %s",name.c_str());
			#endif*/
			pDefPanel_ = new CDataGuiDefPanel();
			pDefPanel_->setID(name);
			db.setGuiDefData(name,pDefPanel_);
		}
		
		void newSymbol()
		{
			pSymbol_ = new CDataGuiDefSymbol();
			pDefPanel_->setWidget(pSymbol_);
			pWidget_ = pSymbol_;
		}

		void newWidgetGraphic()
		{
			pWidgetGraphic_ = new CDataGuiDefWidgetGraphic();
			pDefPanel_->setWidget(pWidgetGraphic_);
			pWidget_ = pWidgetGraphic_;
		}

		void newWidgetButton()
		{
			pWidgetButton_ = new CDataGuiDefWidgetButton();
			pDefPanel_->setWidget(pWidgetButton_);
			pWidget_ = pWidgetButton_;
		}

		void newNum()
		{
			pNum_ = new CDataGuiDefNum();
			pDefPanel_->setWidget(pNum_);
			pWidget_ = pNum_;
		}

		void newText()
		{
			pText_ = new CDataGuiDefText();
			pDefPanel_->setWidget(pText_);
			pWidget_ = pText_;
		}

		void newWidgetPanel()
		{
			pWidgetPanel_ = new CDataGuiDefWidgetPanel();
			pDefPanel_->setWidget(pWidgetPanel_);
			pWidget_ = pWidgetPanel_;
		}

		void newObj()
		{
			pObj_ = new CDataGuiDefObj();
			pDefPanel_->setWidget(pObj_);
			pWidget_ = pObj_;
		}

		void newWidgetRemain()
		{
			pWidgetRemain_ = new CDataGuiDefWidgetRemain();
			pDefPanel_->setWidget(pWidgetRemain_);
			pWidget_ = pWidgetRemain_;
		}

		void newWidgetGage()
		{
			pWidgetGage_ = new CDataGuiDefWidgetGage();
			pDefPanel_->setWidget(pWidgetGage_);
			pWidget_ = pWidgetGage_;
		}

		void newFace()
		{
			pFace_ = new CDataGuiDefFace();
			pDefPanel_->setWidget(pFace_);
			pWidget_ = pFace_;
		}

		void newName()
		{
			pName_ = new CDataGuiDefName();
			pDefPanel_->setWidget(pName_);
			pWidget_ = pName_;
		}

		void newAction(CGuiDefDB& db, const string& name)
		{
			pAction_ = new CDataGuiDefAction();
			db.setGuiDefData(name,pAction_);
		}

		void newState()
		{
			pState_ = new CDataGuiDefActionState();
			pAction_->setAction(pState_);
		}

		void newTween()
		{
			pTween_ = new CDataGuiDefActionTween();
			pAction_->setAction(pTween_);
		}

		void newRemain(CGuiDefDB& db, const string& name)
		{
			pRemain_ = new CDataGuiDefRemain();
			db.setGuiDefData(name,pRemain_);
		}

		void newGage(CGuiDefDB& db, const string& name)
		{
			pGage_ = new CDataGuiDefGage();
			db.setGuiDefData(name,pGage_);
		}

		void newCircle(CGuiDefDB& db, const string& name)
		{
			pCircle_ = new CDataGuiDefCircle();
			pDefPanel_ = pCircle_;
			db.setGuiDefData(name,pCircle_);
		}

		void newButton(CGuiDefDB& db, const string& name)
		{
			pButton_ = new CDataGuiDefButton();
			db.setGuiDefData(name,pButton_);
		}

		void newTextGui(CGuiDefDB& db, const string& name)
		{
			pTextGui_ = new CDataGuiDefTextGui();
			db.setGuiDefData(name,pTextGui_);
		}

		void newGraphic(CGuiDefDB& db, const string& name)
		{
			pGraphic_ = new CDataGuiDefGraphic();
			db.setGuiDefData(name,pGraphic_);
		}
	};
};

} // namespace GUI end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね