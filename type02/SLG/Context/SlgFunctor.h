/*
	katze 06/06/06
	SLGÇ≈ÅAfor_eachÇ∆Ç©ÇÊÇ§Functor
*/
#pragma once

#include "../Action/IAction.h"

namespace BMW{
namespace SLG{

#pragma warning(disable:4512) // ë„ì¸ââéZéqçÏÇÍÇÀ
////////////////////////////////////
// ñ¢çsìÆÉLÉÉÉâ
////////////////////////////////////
struct IsNoAction : public unary_function<int,bool>
{
	bool operator() (int nID)
	{
		return p_.getCharaData(nID)!=NULL && p_.getCharaData(nID)->getState().getAct()==Act::BEFORE;
	}

	IsNoAction(CSLGContext& p):p_(p){}
	CSLGContext& p_;
};

////////////////////////////////////
// É}ÉbÉvÇ…Ç¢ÇÈÅH
////////////////////////////////////
struct IsMapExist : public unary_function<int,bool>
{
	bool operator() (int nID)
	{
		CDataCharaSLG* pChara = p_.getCharaData(nID);
		return pChara!=NULL 
			&& pChara->IsExist()
			&& pChara->getIndex()>=0;
	}

	IsMapExist(CSLGContext& p):p_(p){}
	CSLGContext& p_;
};

////////////////////////////////////
// É}ÉbÉvÇ…Ç¢Ç»Ç¢ÅH
////////////////////////////////////
struct IsMapNoExist : public unary_function<int,bool>
{
	bool operator() (int nID)
	{
		CDataCharaSLG* pChara = p_.getCharaData(nID);
		return pChara==NULL 
			|| !pChara->IsExist()
			|| pChara->getIndex()<0;
	}

	IsMapNoExist(CSLGContext& p):p_(p){}
	CSLGContext& p_;
};

////////////////////////////////////
// ëÄçÏÇ≈Ç´Ç»Ç¢
////////////////////////////////////
struct IsNoCtrl : public unary_function<int,bool>
{
	bool operator() (int nID)
	{
		CDataCharaSLG* pChara = p_.getCharaData(nID);
		return pChara==NULL 
			|| !pChara->IsExist()
			|| pChara->getIndex()<0
			|| pChara->getAction()->IsNonPlayer();
	}

	IsNoCtrl(CSLGContext& p):p_(p){}
	CSLGContext& p_;
};

////////////////////////////////////
// IDè∏èá
////////////////////////////////////
struct sort_IdUp : public binary_function<pair<int,int>, pair<int,int>, bool>
{
	bool operator()(const pair<int,int>& first, const pair<int,int>& second)
	{
		return first.first < second.first;
	}
};

////////////////////////////////////
// IDç~èá
////////////////////////////////////
struct sort_IdDown : public binary_function<pair<int,int>, pair<int,int>, bool>
{
	bool operator()(const pair<int,int>& first, const pair<int,int>& second)
	{
		return first.first > second.first;
	}
};

////////////////////////////////////
// LVè∏èá
////////////////////////////////////
struct sort_LvUp : public binary_function<pair<int,int>, pair<int,int>, bool>
{
	sort_LvUp(CSLGContext::chara_map& m):mapChara(m){}

	bool operator()(const pair<int,int>& first, const pair<int,int>& second)
	{
		return mapChara.find(first.second)->second->getBattle().getLv() < mapChara.find(second.second)->second->getBattle().getLv();
	}

	CSLGContext::chara_map& mapChara;
};

////////////////////////////////////
// LVç~èá
////////////////////////////////////
struct sort_LvDown : public binary_function<pair<int,int>, pair<int,int>, bool>
{
	sort_LvDown(CSLGContext::chara_map& m):mapChara(m){}

	bool operator()(const pair<int,int>& first, const pair<int,int>& second)
	{
		return mapChara.find(first.second)->second->getBattle().getLv() > mapChara.find(second.second)->second->getBattle().getLv();
	}

	CSLGContext::chara_map& mapChara;
};

////////////////////////////////////
// HPè∏èá
////////////////////////////////////
struct sort_HpUp : public binary_function<pair<int,int>, pair<int,int>, bool>
{
	sort_HpUp(CSLGContext::chara_map& m):mapChara(m){}

	bool operator()(const pair<int,int>& first, const pair<int,int>& second)
	{
		return mapChara.find(first.second)->second->getBattle().getHP() < mapChara.find(second.second)->second->getBattle().getHP();
	}

	CSLGContext::chara_map& mapChara;
};

////////////////////////////////////
// HPç~èá
////////////////////////////////////
struct sort_HpDown : public binary_function<pair<int,int>, pair<int,int>, bool>
{
	sort_HpDown(CSLGContext::chara_map& m):mapChara(m){}

	bool operator()(const pair<int,int>& first, const pair<int,int>& second)
	{
		return mapChara.find(first.second)->second->getBattle().getHP() > mapChara.find(second.second)->second->getBattle().getHP();
	}

	CSLGContext::chara_map& mapChara;
};

////////////////////////////////////
// ENè∏èá
////////////////////////////////////
struct sort_EnUp : public binary_function<pair<int,int>, pair<int,int>, bool>
{
	sort_EnUp(CSLGContext::chara_map& m):mapChara(m){}

	bool operator()(const pair<int,int>& first, const pair<int,int>& second)
	{
		return mapChara.find(first.second)->second->getBattle().getEN() < mapChara.find(second.second)->second->getBattle().getEN();
	}

	CSLGContext::chara_map& mapChara;
};

////////////////////////////////////
// ENç~èá
////////////////////////////////////
struct sort_EnDown : public binary_function<pair<int,int>, pair<int,int>, bool>
{
	sort_EnDown(CSLGContext::chara_map& m):mapChara(m){}

	bool operator()(const pair<int,int>& first, const pair<int,int>& second)
	{
		return mapChara.find(first.second)->second->getBattle().getEN() > mapChara.find(second.second)->second->getBattle().getEN();
	}

	CSLGContext::chara_map& mapChara;
};

////////////////////////////////////
// SPè∏èá
////////////////////////////////////
struct sort_SpUp : public binary_function<pair<int,int>, pair<int,int>, bool>
{
	sort_SpUp(CSLGContext::chara_map& m):mapChara(m){}

	bool operator()(const pair<int,int>& first, const pair<int,int>& second)
	{
		return mapChara.find(first.second)->second->getBattle().getSP() < mapChara.find(second.second)->second->getBattle().getSP();
	}

	CSLGContext::chara_map& mapChara;
};

////////////////////////////////////
// SPç~èá
////////////////////////////////////
struct sort_SpDown : public binary_function<pair<int,int>, pair<int,int>, bool>
{
	sort_SpDown(CSLGContext::chara_map& m):mapChara(m){}

	bool operator()(const pair<int,int>& first, const pair<int,int>& second)
	{
		return mapChara.find(first.second)->second->getBattle().getSP() > mapChara.find(second.second)->second->getBattle().getSP();
	}

	CSLGContext::chara_map& mapChara;
};

////////////////////////////////////
// NEXTè∏èá
////////////////////////////////////
struct sort_NextUp : public binary_function<pair<int,int>, pair<int,int>, bool>
{
	sort_NextUp(CSLGContext::chara_map& m):mapChara(m){}

	bool operator()(const pair<int,int>& first, const pair<int,int>& second)
	{
		return mapChara.find(first.second)->second->getBattle().getExp() < mapChara.find(second.second)->second->getBattle().getExp();
	}

	CSLGContext::chara_map& mapChara;
};

////////////////////////////////////
// NEXTç~èá
////////////////////////////////////
struct sort_NextDown : public binary_function<pair<int,int>, pair<int,int>, bool>
{
	sort_NextDown(CSLGContext::chara_map& m):mapChara(m){}

	bool operator()(const pair<int,int>& first, const pair<int,int>& second)
	{
		return mapChara.find(first.second)->second->getBattle().getExp() > mapChara.find(second.second)->second->getBattle().getExp();
	}

	CSLGContext::chara_map& mapChara;
};

////////////////////////////////////
// MENTALè∏èá
////////////////////////////////////
struct sort_MentalUp : public binary_function<pair<int,int>, pair<int,int>, bool>
{
	sort_MentalUp(CSLGContext::chara_map& m):mapChara(m){}

	bool operator()(const pair<int,int>& first, const pair<int,int>& second)
	{
		return mapChara.find(first.second)->second->getBattle().getMental() < mapChara.find(second.second)->second->getBattle().getMental();
	}

	CSLGContext::chara_map& mapChara;
};

////////////////////////////////////
// MENTALç~èá
////////////////////////////////////
struct sort_MentalDown : public binary_function<pair<int,int>, pair<int,int>, bool>
{
	sort_MentalDown(CSLGContext::chara_map& m):mapChara(m){}

	bool operator()(const pair<int,int>& first, const pair<int,int>& second)
	{
		return mapChara.find(first.second)->second->getBattle().getMental() > mapChara.find(second.second)->second->getBattle().getMental();
	}

	CSLGContext::chara_map& mapChara;
};
#pragma warning(default:4512) // ë„ì¸ââéZéqçÏÇÍÇÀ

} // namespace SLG end
} // namespace BMW end