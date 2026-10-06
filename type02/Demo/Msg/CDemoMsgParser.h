/*
	katze 05/07/05
	デモメッセージパーサ
*/
#pragma once

#include "../../Face/DB/CFaceDB.h"

#include "CDemoMsg.h"
#include "CDemoMsgList.h"

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

#include "../../ADV/adv_closure.h"
#include "../../ADV/adv_symbol.h"

#include "CDemoMsgDB.h"
namespace BMW{
namespace Demo{

class CDemoMsgDB;

struct CDemoMsgParser : public boost::spirit::grammar<CDemoMsgParser>
{
	CDemoMsgParser(CDemoMsgDB& db, CDemoContext* p):db_(db),p_(p){}
	CDemoMsgDB&		db_;
	CDemoContext*	p_;
	
	template<typename S>
	struct definition
	{
		definition(const CDemoMsgParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// データ設定
			p = self.p_;

			// スタート
			start_ = xml_ >> demo_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <demo_msg>
			//	*<msglist></msglist>
			// </demo_msg>
			demo_	= str_p("<demo_msg>")
						>> eps_p[var(nCount_)=0]
						>> *msglist_
					>> str_p("</demo_msg>")
					;

			//	<msglist name="" size="" !cond="">
			//	*<msg></msg>
			//	</msglist>
			msglist_ = str_p("<msglist")
						>> name_[bind(&definition::newMsgList)(var(*this),var(self.db_),arg1)]
						>> str_p("size=\"") >> int_p[bind(&Demo::CDemoMsgList::resizeMsgList)(var(pMsgList_),arg1)] >> '"'
						>> !(str_p("cond=\"") >> int_p/*[bind(&Demo::CDemoMsgList::setCond)(var(pMsgList_),arg1)]*/ >> '"')
						>> ch_p('>')
						>> eps_p[var(nCount2_)=0]
						>> *msg_
					>> str_p("</msglist>")
					;

			// <msg side="" chara="" face="" !mask="">
			//	TEXT
			// </msg>
			msg_	= str_p("<msg")
						>> str_p("side=\"") >> sideID_[bind(&ADV::CCmdMsg::setSide)(msg_.val,arg1)] >> '"'
						>> str_p("chara=\"") >> (*(anychar_p - '"'))[msg_.s = construct_<string>(arg1,arg2)] >> '"'
						>> eps_p[bind(&ADV::CCmdMsg::setChara)(msg_.val,msg_.s)]
						>> str_p("face=\"") >> (*(anychar_p - '"'))[msg_.s = construct_<string>(arg1,arg2)] >> '"'
						>> eps_p[bind(&ADV::CCmdMsg::setFace)(msg_.val,msg_.s)]
						>> !(str_p("mask=\"") >> boolSymbol_[bind(&ADV::CCmdMsg::mask)(msg_.val,arg1)] >> '"')
					>> '>'
					>> (*(anychar_p - "</msg>"))[msg_.s = construct_<string>(arg1,arg2)]
					>> eps_p[bind(&ADV::CCmdMsg::setText)(msg_.val,msg_.s)]
					>> str_p("</msg>")
					>> eps_p[bind(&definition::newMsg)(var(*this),msg_.val)]
					;

			// name属性
			name_		= str_p("name=\"") >> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';
		}

		
		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>											rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>		rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>			rule_i;
		typedef boost::spirit::rule<S, ADV::msg_closure::context_t>				rule_m;
		
		// ルール
		rule	start_,xml_,demo_,msglist_;
		rule_m	msg_;
		rule_s	name_;

		// シンボル
		Parser::boolsym		boolSymbol_;
		ADV::side_symbol	sideID_;

		// 一時データ
		CDemoContext*	p;
		int				nCount_;
		CDemoMsgList*	pMsgList_;
		int				nCount2_;
		CDemoMsg*		pMsg_;

		// 生成
		void newMsgList(CDemoMsgDB& db,const string& sID)
		{
			pMsgList_ = new CDemoMsgList();
			db.addDemoMsgList(nCount_, pMsgList_);
			db.setDemoMsgID(sID,nCount_++);
		}

		void newMsg(ADV::CCmdMsg& cmd)
		{
			pMsg_		= new CDemoMsg();
			pMsgList_->setMsg(nCount2_++,pMsg_);

			pMsg_->setSide(cmd.getSide());
			pMsg_->setChara(Face::Const::faceID_.getValue(cmd.getChara()));
			pMsg_->setFace(p->getApp()->getFaceMap().getFaceDB(pMsg_->getChara())->getFaceID(cmd.getFace()));
			pMsg_->setMsg(cmd.getText());
			pMsg_->mask(cmd.IsMask());
		}
	};

};


} // namespace Demo end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね