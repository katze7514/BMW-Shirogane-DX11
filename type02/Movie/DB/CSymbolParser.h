/*
	katze 05/05/10
	シンボル解析
*/
#pragma once

#include "../../Sound/IDSound.h"

#include "CDataSymbolGui.h"
#include "CDataSymbolMovieClip.h"

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

#include "../../Sound/Code/sound_symbol.h"

#include "symbol_closure.h"
#include "symbol_symbol.h"

#include "CSymbolDB.h"
namespace BMW{
namespace Movie{

class CSymbolDB;
struct CSymbolParser : public boost::spirit::grammar<CSymbolParser>
{
	CSymbolParser(CSymbolDB& db,CSpriteDB& spriteDB,const string& sPrefix):db_(db),spriteDB_(spriteDB),sPrefix_(sPrefix){}

	CSymbolDB& db_;
	CSpriteDB& spriteDB_;
	string sPrefix_;

	template<typename S>
	struct definition
	{
		definition(const CSymbolParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;
			
			// スタート
			start_ = !xml_
					>> eps_p[bind(&definition::setPrefix)(var(*this),self.sPrefix_)]
					>> symbol_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <symbol>
			//  <spritedef />
			//	<graphic></graphic>
			//	<button></button>
			//	<movieclip></movieclip>
			// </symbol>
			symbol_	= str_p("<symbol>")
						>> *(spritedef_ | include_)
					//>> eps_p[var(nCount_)=0]
						>> *(graphic_ | movieclip_ | button_ | num_ | include_)
					>> str_p("</symbol>")
					;

			// <include src="" !pre="" />
			include_	= str_p("<include")
						>> src_[include_.val=arg1]
						>> eps_p[bind(static_cast<void(string::*)()>(&string::clear))(include_.pre)]
						>> !pre_[include_.pre=arg1] 
						>> str_p("/>")
						>> eps_p[bind(&Movie::CSymbolDB::setSymbolPre)(var(self.db_),include_.val,var(sPrefix_)+include_.pre)]
						;

			// <spritedef src="" />
			spritedef_	= str_p("<spritedef")
						>> src_[spritedef_.val=arg1]
						>> str_p("/>")
						>> eps_p[bind(&Draw::CSpriteDB::setSpriteDBPre)(var(self.spriteDB_),spritedef_.val,var(sPrefix_))]
						;

			// src属性
			src_	= str_p("src=\"")	>> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';

			// pre属性
			pre_	= str_p("pre=\"")	>> (*(anychar_p - '"'))[pre_.val = construct_<string>(arg1,arg2)] >> '"';

			// <graphic name="" !type="">
			//  <sprite name="" />
			// </graphic>
			graphic_	= str_p("<graphic")
						>> name_[bind(&definition::newGraphic)(var(*this),var(self.db_),arg1)]
						>> !(str_p("type=\"") >> graphicType_[bind(&Movie::CDataSymbolGraphic::setGuiKind)(var(pGraphic_),arg1)] >> '"')
						>> '>'
						>> sprite_[bind(&Movie::CDataSymbolGraphic::setID)(var(pGraphic_),arg1,0)]
						>> str_p("</graphic>")
						;

			// <sprite name="" />
			sprite_	= str_p("<sprite") >> name_[sprite_.val = bind(&Draw::CSpriteDB::getSpriteID)(var(self.spriteDB_),arg1)] >> str_p("/>");

			// 名前属性
			name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';

			// <button name="" !type="">
			//	<range left="" top="" right="" bottom="" />
			//	<symbol name="" /> <sprite name="" /> * 3
			// </button>
			button_	= str_p("<button") >> name_[bind(&definition::newButton)(var(*this),var(self.db_),arg1)]
					>> !(str_p("type=\"") >> buttonType_[bind(&Movie::CDataSymbolButton::setGuiKind)(var(pButton_),arg1)] >> '"')	
					>> '>'
					>> range_[bind(&CDataSymbolButton::setRect)(var(pButton_),arg1)]
					>> 
					if_p(bind(&Movie::CDataSymbolButton::getGuiKind)(var(pButton_))!=Movie::CDataSymbolButton::NONE)
					[
						//if_p(bind(&Movie::CDataSymbolButton::getGuiKind)(var(pButton_))==Movie::CDataSymbolButton::SPRITE)
						//[
							(for_p(button_.val=0, button_.val<3, button_.val++)
							[sprite_[bind(&Movie::CDataSymbolButton::setID)(var(pButton_),arg1,button_.val)]]
							>> eps_p[bind(&Movie::CDataSymbolButton::setGuiKind)(var(pButton_),Movie::CDataSymbolButton::SPRITE)]
							)
						//].else_p[
							|
							(for_p(button_.val=0, button_.val<3, button_.val++)
							[symbol_a_[bind(&Movie::CDataSymbolButton::setID)(var(pButton_),arg1,button_.val)]]
							>> eps_p[bind(&Movie::CDataSymbolButton::setGuiKind)(var(pButton_),Movie::CDataSymbolButton::SYMBOL)])
						//]
					]
					>> str_p("</button>")
							;

			// <range left="" top="" right="" bottom="" />
			range_	=  str_p("<range")
					>> left_[bind(&RECT::left)(range_.val)=arg1]
					>> top_[bind(&RECT::top)(range_.val)=arg1]
					>> right_[bind(&RECT::right)(range_.val)=arg1]
					>> bottom_[bind(&RECT::bottom)(range_.val)=arg1]
					>> str_p("/>")
					;

			left_	= str_p("left=\"")	>> int_p[left_.val = arg1] >> '"';
			top_	= str_p("top=\"")	>> int_p[top_.val = arg1] >> '"';
			right_	= str_p("right=\"")	>> int_p[right_.val = arg1] >> '"';
			bottom_	= str_p("bottom=\"")>> int_p[bottom_.val = arg1] >> '"';

			// <num name="" !type="">
			//	<symbol name="" /> <sprite name="" /> * 12
			// </num>
			num_	= str_p("<num") >> name_[bind(&definition::newNum)(var(*this),var(self.db_),arg1)]
					>> eps_p[bind(&Movie::CDataSymbolNum::setGuiKind)(var(pNum_),Movie::CDataSymbolNum::SPRITE)]
					>> !(str_p("type=\"") >> numType_[bind(&Movie::CDataSymbolNum::setGuiKind)(var(pNum_),arg1)] >> '"')
					>> '>'
					>> for_p(num_.val=0, num_.val<12, num_.val++)
							[
						//		if_p(bind(&Movie::CDataSymbolNum::getGuiKind)(var(pNum_))==Movie::CDataSymbolNum::SPRITE)
						//		[
									sprite_[bind(&Movie::CDataSymbolNum::setID)(var(pNum_),arg1,num_.val)]
						//		].else_p[
						//			symbol_a_[bind(&Movie::CDataSymbolNum::setID)(var(pNum_),arg1,num_.val)]
						//		]
							]
					>> str_p("</num>")
							;

			// <movieclip name="" !type="">
			//	+<layer></layer>
			// </movieclip>
			movieclip_ = str_p("<movieclip") 
							>> name_[bind(&definition::newMovieClip)(var(*this),var(self.db_),arg1)] 
							>> !(str_p("type=\"") >> movieType_[bind(&Movie::CDataSymbolMovieClip::setType)(var(pClip_),arg1)] >> '"')
							>> '>'
						>> +layer_
						>> str_p("</movieclip>")
						;

			// <layer name="" >
			//	<keyframe></keyframe> | <tween />
			// </layer>
			layer_	= str_p("<layer") >> !name_ >> '>'
					>> eps_p[bind(&definition::newLayer)(var(*this))]
					>> *( keyframe_ | tween_ )
					>> str_p("</layer>")
					;

			// <keyframe !name="">
			//	(<symbol name="" /> | code系
			//	!<change value="" />
			//	!<draw />
			// </keyframe>
			keyframe_ = str_p("<keyframe") >> !name_ >> '>'
						>> eps_p[bind(&definition::newKeyFrame)(var(*this))]
						>> (
							(
								symbol_a_[bind(&Movie::CDataKeyFrame::setID)(var(pFrame_),arg1)]
								>> eps_p[bind(&Movie::CDataKeyFrame::setSymbolKind)(var(pFrame_),CDataKeyFrame::SYMBOL)]
							)
							| 
							(
								code_
								>> eps_p[bind(&Movie::CDataKeyFrame::setSymbolKind)(var(pFrame_),CDataKeyFrame::CODE)]
							)
						   )
						>> !(
							str_p("<change") 
							>> str_p("value=\"") 
							>> int_p[bind(&Movie::CDataKeyFrame::setFrame)(var(pFrame_),arg1)]
							>> '"' 
							>> str_p("/>")
							)
						>> !draw_[bind(&Movie::CDataKeyFrame::setDrawInfo)(var(pFrame_),arg1)]
						>> str_p("</keyframe>")
						;

			// <tween>
			//  <symbol name="" />
			//	!<change value="" />
			//	<draw />
			//	<draw />
			//	<edging />
			// </tween>
			tween_ = str_p("<tween>")
					>> eps_p[bind(&definition::newTween)(var(*this))]
					>> symbol_a_[bind(&Movie::CDataTween::setID)(var(pTween_),arg1)]
					>> !(str_p("<change") 
						>> str_p("value=\"") 
						>> int_p[bind(&Movie::CDataTween::setFrame)(var(pTween_),arg1)]
						>> '"' 
						>> str_p("/>"))
					>> draw_[bind(&Movie::CDataTween::setDrawInfo)(var(pTween_),arg1)]
					>> draw_[bind(&Movie::CDataTween::setEnd)(var(pTween_),arg1)]
					>> !(
						str_p("<edging") 
						>> str_p("value=\"") 
						>> int_p[bind(&Movie::CDataTween::setEdging)(var(pTween_),arg1)]
						>> '"' 
						>> str_p("/>")
						)
					>> str_p("</tween>")
					;

			// <symbol name="" />
			symbol_a_	= str_p("<symbol")
							>>( 
								(
									str_p("name=\"*\"")
									>>eps_p[symbol_a_.val=INT_MAX]
								)
								| 
									name_[symbol_a_.val=bind(&Movie::CSymbolDB::getID)(var(self.db_),arg1)]
								)
							>> str_p("/>")
							;

			// <no /> | <end /> |<se !ctrl="" id="" /> | <sewait id="" />|<back vel=""|visible="" /> | <msg no="" /> | <stop /> | <gage type="" />
			code_	= no_ | end_ | se_ | se_wait_ | back_ | msg_ | stop_ | gage_;

			// <no />
			no_		= str_p("<no") >> str_p("/>")
					>> eps_p[bind(&Movie::CDataKeyFrame::setID)(var(pFrame_),Movie::Code::NO)]
					;

			// <stop />
			stop_	= str_p("<stop") >> str_p("/>")
					>> eps_p[bind(&Movie::CDataKeyFrame::setID)(var(pFrame_),Movie::Code::STOP_MOVIE)]
					;

			// <end />
			end_	= str_p("<end") >> str_p("/>")
					>> eps_p[bind(&Movie::CDataKeyFrame::setID)(var(pFrame_),Movie::Code::END)]
					;

			// <se !ctrl="" id="" />
			se_		= str_p("<se")
					>> eps_p[se_.val=Sound::Ctrl::PLAY]
					>> !(str_p("ctrl=\"") >> ctrlID_[se_.val=arg1] >> '"')
					>> id_[se_.str=arg1]
					>> str_p("/>")
					>> eps_p[bind(&definition::newSe)(var(*this),se_.val,se_.str)]
					;

			// <sewait id="" />
			se_wait_	= str_p("<sewait") >> id_[se_wait_.val=arg1]
						>> str_p("/>")
						>> eps_p[bind(&definition::newSeWait)(var(*this),se_wait_.val)]
						;

			// <back vel=""|(type="" visible="")/>
			back_		= str_p("<back") 
						>>( (
								str_p("vel=\"") >> velID_[bind(&Movie::CDataKeyFrame::setParam)(var(pFrame_),arg1)] >> '"'
								>> eps_p[bind(&Movie::CDataKeyFrame::setID)(var(pFrame_),Code::BACK)]
							)
							|
							(
								eps_p[bind(&Movie::CDataKeyFrame::setParam2)(var(pFrame_),Demo::Code::CCode_back_visible::ALL)]
								>> !(str_p("type=\"") >> visibletypeID_[bind(&Movie::CDataKeyFrame::setParam2)(var(pFrame_),arg1)] >> '"')
								>> str_p("visible=\"") >> visibleID_[bind(&Movie::CDataKeyFrame::setParam)(var(pFrame_),arg1)] >> '"'
								>> eps_p[bind(&Movie::CDataKeyFrame::setID)(var(pFrame_),Code::BACK_VISIBLE)]
							)
						  )
						>> str_p("/>")
					;
			// <msg no="" />
			msg_		= str_p("<msg")
							>> eps_p[bind(&Movie::CDataKeyFrame::setID)(var(pFrame_),Code::MES)]
							>> str_p("no=\"") >> int_p[bind(&Movie::CDataKeyFrame::setParam)(var(pFrame_),arg1)] >> '"'
						>> str_p("/>")
						;

			// <gage type="" />
			gage_		= str_p("<gage")
							>> eps_p[bind(&Movie::CDataKeyFrame::setID)(var(pFrame_),Code::GAGE)]
							>> str_p("type=\"") >> gageType_[bind(&Movie::CDataKeyFrame::setParam)(var(pFrame_),arg1)] >> '"'
						>> str_p("/>")
						;

			// <bgm ctrl="" id="" />
			bgm_	= str_p("<bgm")
					>> (str_p("ctrl=\"") >> ctrlID_[bgm_.val=arg1] >> '"')
					>> id_[bgm_.str=arg1]
					>> str_p("/>")
					>> eps_p[bind(&definition::newBgm)(var(*this),bgm_.val,bgm_.str)]
					;

			// id属性
			id_	= str_p("id=\"") >> (*(anychar_p - '"'))[id_.val = construct_<string>(arg1,arg2)] >> '"';

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
		};

		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>										rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>	rule_s;
		typedef boost::spirit::rule<S, Parser::ss_closure::context_t>		rule_ss;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>		rule_i;
		typedef boost::spirit::rule<S, Parser::int_str_closure::context_t>	rule_n;
		typedef boost::spirit::rule<S, Draw::rect_closure::context_t>		rule_r;
		typedef boost::spirit::rule<S, draw_closure::context_t>				rule_d;

		// rule
		rule	start_,xml_,symbol_;
		rule_ss	include_;
		rule_s	spritedef_;
		rule_s	src_,name_,id_,pre_;
		rule	graphic_,movieclip_;
		rule_i	button_,num_;
		rule_i	sprite_,symbol_a_,code_;
		rule_r	range_;
		rule_i	left_,top_,right_,bottom_;
		rule	layer_,keyframe_,tween_;
		rule	no_,stop_,end_,back_,msg_,gage_;
		rule_n	se_,bgm_;
		rule_s	se_wait_;
		rule_d	draw_;

		// シンボル
		Parser::boolsym					boolSymbol_;
		Sound::Code::sound_ctrl_symbol	ctrlID_;
		graphic_type_symbol				graphicType_;
		button_type_symbol				buttonType_;
		num_type_symbol					numType_;
		movie_code_symbol				codeSymbol_;
		back_vel_symbol					velID_;
		back_visible_symbol				visibleID_;
		back_visible_type_symbol		visibletypeID_;
		gage_symbol						gageType_;
		movie_type_symbol				movieType_;

		// カウンタ
		//int nCount_;
		// 名前のprefix
		string sPrefix_;

		// 一時データ
		CDataSymbolGraphic*		pGraphic_;
		CDataSymbolButton*		pButton_;
		CDataSymbolNum*			pNum_;
		CDataSymbolMovieClip*	pClip_;
		CDataLayer*				pLayer_;
		CDataKeyFrame*			pFrame_;
		CDataTween*				pTween_;

		void setPrefix(const string& sPrefix)
		{
			sPrefix_=sPrefix;
		}

		void newGraphic(CSymbolDB& db, const string& sID)
		{
			pGraphic_ = new CDataSymbolGraphic();
			db.setSymbolData(pGraphic_,sPrefix_+sID);
		}

		void newButton(CSymbolDB& db, const string& sID)
		{
			pButton_ = new CDataSymbolButton();
			db.setSymbolData(pButton_,sPrefix_+sID);
		}

		void newNum(CSymbolDB& db, const string& sID)
		{
			pNum_ = new CDataSymbolNum();
			db.setSymbolData(pNum_,sID);
		}

		void newMovieClip(CSymbolDB& db, const string& sID)
		{
			pClip_ = new CDataSymbolMovieClip();
			db.setSymbolData(pClip_,sPrefix_+sID);
		}

		void newLayer()
		{
			pLayer_ = new CDataLayer();
			pClip_->addLayer(pLayer_);
		}

		void newKeyFrame()
		{
			pFrame_ = new CDataKeyFrame();
			pLayer_->addKeyFrame(pFrame_);
		}

		void newTween()
		{
			pTween_ = new CDataTween();
			pLayer_->addKeyFrame(pTween_);
		}

		void newSe(int nCtrl,const string& sID)
		{
			pFrame_->setID(Code::SE);
			pFrame_->setParam(nCtrl);
			pFrame_->setParam2(Sound::Const::seID_.getValue(sID));
		//	CDbg().Out("SE %s %d",sID.c_str(),Sound::Const::seID_.getValue(sID));
		}

		void newBgm(int nCtrl,const string& sID)
		{
			pFrame_->setID(Code::BGM);
			pFrame_->setParam(nCtrl);
			pFrame_->setParam2(Sound::Const::bgmID_.getValue(sID));
		//	CDbg().Out("SE %s %d",sID.c_str(),Sound::Const::seID_.getValue(sID));
		}

		void newSeWait(const string& sID)
		{
			pFrame_->setID(Code::SE_WAIT);
			pFrame_->setParam(Sound::Const::seID_.getValue(sID));
		}
	};
};

} // namespace Movie end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね