/*
	katze 05/04/10
	マップパーサ
*/
#pragma once

#include "../IDSLG.h"
#include "CMapChip.h"

#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

#include "map_closure.h"
#include "map_symbol.h"

namespace BMW{
namespace SLG{
namespace Map{

struct CMapParser : public boost::spirit::grammar<CMapParser, Parser::int_closure::context_t>
{
	CMapParser(CMap* pMap, Draw::CSpriteDB& spriteDB):pMap_(pMap),spriteDB_(spriteDB){}
	CMap* pMap_;
	Draw::CSpriteDB& spriteDB_;

	template<typename S>
	struct definition
	{
		definition(const CMapParser& self)
		{
			using namespace boost::spirit;
			using namespace phoenix;
			using phoenix::bind;

			// スタート
			start_ = xml_ >> map_;

			// <?xml ・・・ ?>の認識
			xml_	= str_p("<?xml") >> *(anychar_p - "?>") >> str_p("?>");

			// <map size="">
			//	<spritedef />
			//	<back></back>
			//  <rect />
			//	<start />
			//	<chip></chip>*n
			// </map>
			map_	= str_p("<map") 
						>> str_p("size=\"") >> int_p[bind(&Map::CMap::setMapChipSize)(var(*self.pMap_),arg1)] 
						>> '"' >> ch_p('>')
					>> spritedef_
					>> back_
					>> rect_
					>> start_i_
					>> *chip_
					>> str_p("</map>")
					;

			// <spritedef src="" />
			spritedef_	= str_p("<spritedef") 
						>> src_[spritedef_.val=arg1]
						>> str_p("/>")
						>> eps_p[bind(&Draw::CSpriteDB::setSpriteDB)(var(self.spriteDB_),spritedef_.val)]
						;

			// src属性
			src_	= str_p("src=\"")	>> (*(anychar_p - '"'))[src_.val = construct_<string>(arg1,arg2)] >> '"';
			
			// <back>
			//	<demo src="" />
			//	(<grahic> | <anime>)*n
			// </back>
			back_	= str_p("<back>")
					>> str_p("<demo") >> src_[bind(&Map::CMap::setDemoBack)(var(*self.pMap_),arg1)] >> str_p("/>")
					>> eps_p[var(pList_) = bind(&Map::CMap::getBack)(var(*self.pMap_))]
					>> gui_
					>> str_p("</back>")
					;

			// <rect left="" top="" right="" bottom="" />
			// 矩形
			rect_	=  str_p("<rect")
					>> left_[bind(&RECT::left)(rect_.val)=arg1]
					>> top_[bind(&RECT::top)(rect_.val)=arg1]
					>> right_[bind(&RECT::right)(rect_.val)=arg1]
					>> bottom_[bind(&RECT::bottom)(rect_.val)=arg1]
					>> str_p("/>")
					>> eps_p[bind(&Map::CMap::setRect)(var(*self.pMap_),rect_.val)]
					;

			// <start index="" />
			start_i_ = str_p("<start")
					>> str_p("index=\"") >> int_p[self.val=arg1] >> '"'
					>> str_p("/>")
					;

			// <chip index="">
			//	!<info></info> 単なるオブジェクトとしてのチップならinfoを省略する
			//	<pos />
			//	<obj></obj>
			// </chip>
			chip_	= str_p("<chip")
						>> str_p("index=\"") >> int_p[chip_.val=arg1] >> '"'
						>> ch_p('>')
					>> eps_p[bind(&definition::newChip)(var(*this),var(self.pMap_),chip_.val)]
					>> !info_
					>> pos_
					>> obj_
					>> str_p("</chip>")
					;

			// <info>
			//	!<need move="" height="" />
			//	!<corect hp="" en="" hit="" defence="" />
			//	<on top="" left="" bottom="" right="" />
			//	!<back id="" />
			//	!<range />
			// </info>
			info_	= str_p("<info>")
					>> !(
						str_p("<need")
						>> str_p("move=\"") >> int_p[bind(&Map::CMapChipInfo::setMove)(info_.val, arg1)] >> '"'
						>> str_p("height=\"") >> int_p[bind(&Map::CMapChipInfo::setHeight)(info_.val, arg1)] >> '"'
						>> str_p("/>")
						)
					>> !(
						str_p("<corect")
						>> str_p("hp=\"") >> int_p[bind(&Map::CMapChipInfo::setHP)(info_.val, arg1)] >> '"'
						>> str_p("en=\"") >> int_p[bind(&Map::CMapChipInfo::setEN)(info_.val, arg1)] >> '"'
						>> str_p("hit=\"") >> int_p[bind(&Map::CMapChipInfo::setHit)(info_.val, arg1)] >> '"'
						>> str_p("defence=\"") >> int_p[bind(&Map::CMapChipInfo::setDefence)(info_.val, arg1)] >> '"'
						>> str_p("/>")
						)
					>> str_p("<on")
						>> str_p("left=\"") >> int_p[bind(&Map::CMapChipInfo::setOnMap)(info_.val, arg1, Way::LEFT)] >> '"'
						>> str_p("top=\"") >> int_p[bind(&Map::CMapChipInfo::setOnMap)(info_.val, arg1, Way::TOP)] >> '"'
						>> str_p("right=\"") >> int_p[bind(&Map::CMapChipInfo::setOnMap)(info_.val, arg1, Way::RIGHT)] >> '"'
						>> str_p("bottom=\"") >> int_p[bind(&Map::CMapChipInfo::setOnMap)(info_.val, arg1, Way::BOTTOM)] >> '"'
						>> str_p("/>")
					>> !(str_p("<back")
						>> str_p("id=\"") >> backID_/*[bind(&Map::CMapChipInfo::setBack)(info_.val,arg1)]*/ >> '"'
						>> str_p("/>"))
					>> eps_p[bind(&Map::CMapChip::createMapChipState)(var(pChip_))]
					>> !range_
					>> str_p("</info>")
					>> eps_p[bind(&Map::CMapChip::setMapInfo)(var(pChip_),info_.val)]
					;

			// <range left="" top="" right="" bottom="" />
			// 矩形
			range_	=  str_p("<range")
					>> left_[bind(&RECT::left)(range_.val)=arg1]
					>> top_[bind(&RECT::top)(range_.val)=arg1]
					>> right_[bind(&RECT::right)(range_.val)=arg1]
					>> bottom_[bind(&RECT::bottom)(range_.val)=arg1]
					>> str_p("/>")
					>> eps_p[bind(&Map::CMapChip::setRange)(var(pChip_),range_.val)]
					;

			left_	= str_p("left=\"")	>> int_p[left_.val = arg1] >> '"';
			top_	= str_p("top=\"")	>> int_p[top_.val = arg1] >> '"';
			right_	= str_p("right=\"")	>> int_p[right_.val = arg1] >> '"';
			bottom_	= str_p("bottom=\"")>> int_p[bottom_.val = arg1] >> '"';

			// <pos x="" y="" />
			pos_	=  str_p("<pos")
					>> str_p("x=\"") >> int_p[bind(&Map::CMapChip::setX)(var(pChip_),arg1)] >> '"'
					>> str_p("y=\"") >> int_p[bind(&Map::CMapChip::setY)(var(pChip_),arg1)] >> '"'
					>> str_p("/>")
					;

			// id属性
			id_	= str_p("id=\"") >> (*(anychar_p - '"'))[id_.val = construct_<string>(arg1,arg2)] >> '"';

			// <obj>
			//	(<graphic>)*n
			// </obj>
			obj_	= str_p("<obj>")
					>> eps_p[bind(&definition::newObj)(var(*this))]
					>> gui_
					>> str_p("</obj>")
					;

			// (<graphic>)*n
			gui_	= eps_p[var(nCount_)=0] 
					>> *graphic_
					;

			// <graphic !name="" >
			//	<sprite />
			//	!<pos />	←　親のposに対する相対値
			// </graphic>
			graphic_	= str_p("<graphic") >> !name_ >> '>'
						>> sprite_[bind(&definition::newGraphic)(var(*this),var(self.spriteDB_),arg1)]
						>> !(
							str_p("<pos")
							>> str_p("x=\"") >> int_p[bind(&GUI::CGraphic::setX)(var(pGraphic_),arg1)] >> '"'
							>> str_p("y=\"") >> int_p[bind(&GUI::CGraphic::setY)(var(pGraphic_),arg1)] >> '"'
							>> str_p("/>")
							)
						>> str_p("</graphic>")
						;

			// <sprite name="" />
			sprite_	= str_p("<sprite") >> name_[sprite_.val = bind(&Draw::CSpriteDB::getSpriteID)(var(self.spriteDB_),arg1)] >> str_p("/>");

			// 名前属性
			name_	= str_p("name=\"")	>> (*(anychar_p - '"'))[name_.val = construct_<string>(arg1,arg2)] >> '"';
		}

		const boost::spirit::rule<S>& start() const { return start_; }

		// typedef
		typedef boost::spirit::rule<S>										rule;
		typedef boost::spirit::rule<S, Parser::string_closure::context_t>	rule_s;
		typedef boost::spirit::rule<S, Parser::int_closure::context_t>		rule_i;
		typedef boost::spirit::rule<S, Draw::rect_closure::context_t>		rule_r;
		typedef boost::spirit::rule<S, Map::mapinfo_closure::context_t>		rule_m;

		// rule
		rule	start_,xml_,map_;
		rule_s	spritedef_;
		rule_s	src_,id_;
		rule	back_;
		rule_r	rect_;
		rule_i	start_i_;
		rule_i	chip_;
		rule	pos_;
		rule_r	range_;
		rule_i	left_,top_,bottom_,right_;
		rule_m	info_;
		rule_i	back_a_;
		rule	obj_;
		rule_i	gui_;
		rule	graphic_;
		rule_i	sprite_;
		rule_s	name_;

		// シンボル
		Map::map_symbol backID_;

		// カウンタ
		int nCount_;
		// 一時データ
		Map::CMapChip*		pChip_;
		Task::CTaskList*	pList_;
		GUI::CGraphic*		pGraphic_;
		// 一時メソッド
		void newChip(CMap* pMap, int nIndex){ pChip_ = new Map::CMapChip(); pMap->setMapChip(pChip_,nIndex); }
		void newObj(){ pList_ = new Task::CTaskListDraw(); pChip_->addTask(pList_, CMapChip::OBJ); }
		void newGraphic(Draw::CSpriteDB& db, int nID)
		{
		//	cout << "Graphic" << nID << endl;
			pGraphic_ = new GUI::CGraphic();
			db.setSprite(const_cast<Draw::CSpriteInfo&>(pGraphic_->getSpriteInfo()),nID);
			pList_->addTask(pGraphic_,nCount_++);
		}
	};
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end

#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね