/*
	katze 05/12/27
	PlaneLoader拡張
	読み込みファイルのXML化や、追記可能に
*/
#pragma once

namespace BMW{
namespace Draw{

class CPlaneLoader2 : public CPlaneLoader
{/**
	PlaneLoader拡張
	読み込みファイルのXML化や、追記可能に
	これで、mapとPlaneが一本化！
 */
public:
	// 設定データのXML化
	// bUseIDは、ここでは追記フラグ
	LRESULT	Set(const string& filename,bool bUseID=false);
	LRESULT	SetPre(const string& filename, const string& sPre, bool bUseID=false);
	// リリースしたら、読み込み済みフラグを倒し、IDもクリア
	LRESULT	ReleaseAll();

	// ID
	int			getPlaneID(const string& sID) const { return mapID_.getValue(sID); }
	void		setPlaneID(const string& sID, int nID){ mapID_.writeMap(sID,nID); }
	void		setPlaneID(const string& sID){ mapID_.writeMap(sID,mapID_.getMapSize()); }

	// 読み込み済みか？
	bool IsReadEnd()const{ return !m_bCanReloadAgain; }

protected:
	katzeSDK::Misc::CStringMap	mapID_;
};

} // namespace Draw end
} // namespace BMW end