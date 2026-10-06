/*
	katze 06/03/15
	É\Å[Égé¿ëïÇÃÇΩÇﬂÇÃfunctoråQ
*/
#pragma once

namespace BMW{
namespace Inter{

#pragma warning(disable:4512) // ë„ì¸ââéZéqçÏÇÍÇÀ
////////////////////////////////////
// IDè∏èá
////////////////////////////////////
struct sort_IdUp : public binary_function<int, int, bool>
{
	sort_IdUp(CInterContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst,int nSecond)
	{
		return mapChara.find(nFirst)->second->getID() < mapChara.find(nSecond)->second->getID();
	}

	CInterContext::chara_map& mapChara;
};

////////////////////////////////////
// IDç~èá
////////////////////////////////////
struct sort_IdDown : public binary_function<int, int, bool>
{
	sort_IdDown(CInterContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst,int nSecond)
	{
		return mapChara.find(nFirst)->second->getID() > mapChara.find(nSecond)->second->getID();
	}

	CInterContext::chara_map& mapChara;
};

////////////////////////////////////
// LVè∏èá
////////////////////////////////////
struct sort_LvUp : public binary_function<int, int, bool>
{
	sort_LvUp(CInterContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst,int nSecond)
	{
		return mapChara.find(nFirst)->second->getData()->getLv() < mapChara.find(nSecond)->second->getData()->getLv();
	}

	CInterContext::chara_map& mapChara;
};

////////////////////////////////////
// LVç~èá
////////////////////////////////////
struct sort_LvDown : public binary_function<int, int, bool>
{
	sort_LvDown(CInterContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst,int nSecond)
	{
		return mapChara.find(nFirst)->second->getData()->getLv() > mapChara.find(nSecond)->second->getData()->getLv();
	}

	CInterContext::chara_map& mapChara;
};

////////////////////////////////////
// HPè∏èá
////////////////////////////////////
struct sort_HpUp : public binary_function<int, int, bool>
{
	sort_HpUp(CInterContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst,int nSecond)
	{
		return mapChara.find(nFirst)->second->getData()->getHP() < mapChara.find(nSecond)->second->getData()->getHP();
	}

	CInterContext::chara_map& mapChara;
};

////////////////////////////////////
// HPç~èá
////////////////////////////////////
struct sort_HpDown : public binary_function<int, int, bool>
{
	sort_HpDown(CInterContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst,int nSecond)
	{
		return mapChara.find(nFirst)->second->getData()->getHP() > mapChara.find(nSecond)->second->getData()->getHP();
	}

	CInterContext::chara_map& mapChara;
};

////////////////////////////////////
// ENè∏èá
////////////////////////////////////
struct sort_EnUp : public binary_function<int, int, bool>
{
	sort_EnUp(CInterContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst,int nSecond)
	{
		return mapChara.find(nFirst)->second->getData()->getEN() < mapChara.find(nSecond)->second->getData()->getEN();
	}

	CInterContext::chara_map& mapChara;
};

////////////////////////////////////
// ENç~èá
////////////////////////////////////
struct sort_EnDown : public binary_function<int, int, bool>
{
	sort_EnDown(CInterContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst,int nSecond)
	{
		return mapChara.find(nFirst)->second->getData()->getEN() > mapChara.find(nSecond)->second->getData()->getEN();
	}

	CInterContext::chara_map& mapChara;
};

////////////////////////////////////
// SPè∏èá
////////////////////////////////////
struct sort_SpUp : public binary_function<int, int, bool>
{
	sort_SpUp(CInterContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst,int nSecond)
	{
		return mapChara.find(nFirst)->second->getData()->getSP() < mapChara.find(nSecond)->second->getData()->getSP();
	}

	CInterContext::chara_map& mapChara;
};

////////////////////////////////////
// SPç~èá
////////////////////////////////////
struct sort_SpDown : public binary_function<int, int, bool>
{
	sort_SpDown(CInterContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst,int nSecond)
	{
		return mapChara.find(nFirst)->second->getData()->getSP() > mapChara.find(nSecond)->second->getData()->getSP();
	}

	CInterContext::chara_map& mapChara;
};

////////////////////////////////////
// NEXTè∏èá
////////////////////////////////////
struct sort_NextUp : public binary_function<int, int, bool>
{
	sort_NextUp(CInterContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst,int nSecond)
	{
		return mapChara.find(nFirst)->second->getData()->getExp() < mapChara.find(nSecond)->second->getData()->getExp();
	}

	CInterContext::chara_map& mapChara;
};

////////////////////////////////////
// NEXTç~èá
////////////////////////////////////
struct sort_NextDown : public binary_function<int, int, bool>
{
	sort_NextDown(CInterContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst,int nSecond)
	{
		return mapChara.find(nFirst)->second->getData()->getExp() > mapChara.find(nSecond)->second->getData()->getExp();
	}

	CInterContext::chara_map& mapChara;
};

////////////////////////////////////
// MENTALè∏èá
////////////////////////////////////
struct sort_MentalUp : public binary_function<int, int, bool>
{
	sort_MentalUp(CInterContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst,int nSecond)
	{
		return mapChara.find(nFirst)->second->getData()->getMental() < mapChara.find(nSecond)->second->getData()->getMental();
	}

	CInterContext::chara_map& mapChara;
};

////////////////////////////////////
// MENTALç~èá
////////////////////////////////////
struct sort_MentalDown : public binary_function<int, int, bool>
{
	sort_MentalDown(CInterContext::chara_map& m):mapChara(m){}

	bool operator()(int nFirst,int nSecond)
	{
		return mapChara.find(nFirst)->second->getData()->getMental() > mapChara.find(nSecond)->second->getData()->getMental();
	}

	CInterContext::chara_map& mapChara;
};
#pragma warning(default:4512) // ë„ì¸ââéZéqçÏÇÍÇÀ

} // namespace Inter end
} // namespace BMW end