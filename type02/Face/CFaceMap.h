/*
	katze 05/05/01
	FaceIDに対応するCFaceDBを管理する
*/
#pragma once

namespace BMW{
namespace Face{

class CFaceDB;
class CFaceMap
{/**
 	FaceIDに対応するCFaceDBを管理する
	また、こいつの設定ファイルから、FaceIDが生成される
 */
public:
	typedef map<int, CFaceDB*> facedb_map;
	typedef map<int, set<int> > faceeq_map;
	// デストラクタ
	~CFaceMap();

	// 設定
	void setFaceMap(const string& sFile);

	void setFaceDB(CFaceDB* pDB, int nID)
	{
		mapFace_.insert(pair<int, CFaceDB*>(nID,pDB));
	}
	CFaceDB* getFaceDB(int nID){ return mapFace_[nID]; }

	// 名前の取得
	const string& getName(int nID);

	// グラフィック設定
	void setNameGraphic(GUI::CGraphic* pGraphic, int nID, int nX=0, int nY=0);
	void setFaceGraphic(GUI::CGraphic* pGraphic, int nID, int nFaceID, int nToward, int nX=0, int nY=0, bool bBattle=false);
	void setFaceGraphic(GUI::CGraphic* pGraphic, int nID, const string& sFaceID, int nToward, int nX=0, int nY=0, bool bBattle=false);

	// 等価マップ設定
	faceeq_map& getEqFaceMap(){ return mapEqFace_; }
	void		addEqFaceMap(int nFaceID, int nEQ);
	// nFaceIDを基準に、nEQが等価かどうかをチェック
	bool		IsFace(int nFaceID, int nEQ);

private:
	// フェイスマップ
	facedb_map mapFace_;

	// フェイスの等価マップ
	faceeq_map mapEqFace_;
};

} // namespace Face end
} // namespace BMW end