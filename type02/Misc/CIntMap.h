/*
	katze 04/11/10
	rewritten 05/02/22
	地味に使うIDから文字列変換クラス
*/
#pragma once

namespace katzeSDK{
namespace Misc{

class CIntMap
{/**
	int2string だと思いねえ
	ようは、IDからそれの名称文字列を手に入れるのに使うのさ
 */
public:
	typedef map<int,string> intmap;

	/// intをキーとして、stringを値とする
	void writeMap(const int,const string&);
	/// 引数をキーとして値を取得する。無い場合は空文字列が返る
	const string& getValue(const int nKey, const string& s="") const;
	/// 文字列\n というフォーマットで書かれたファイルを読み込んでマップを作る
	/// 上から0番目、1番目と番号が自動的に振られていく
	void readMapFile(const string&);
	/// 現在のマップを書き出す
	void writeMapFile(const string&);
	/// 現在のマップのサイズを取得する
	int getMapSize() const { return map_.size(); }

private:
	intmap map_;
};

__inline void CIntMap::writeMap(const int nKey, const string& sValue)
{
	map_[nKey]=sValue;
}

__inline const string& CIntMap::getValue(const int nKey,const string& s) const
{
	intmap::const_iterator it = map_.find(nKey);
	if(it!=map_.end())
		return it->second;
	else 
		return s;
}

__inline void CIntMap::readMapFile(const string& sFile)
{
	int n=0;
	string s;
	CFile file;

	file.Read(sFile);
	while(file.ReadLine(s)==0)	writeMap(n++,s);
	file.Close();
}

__inline void CIntMap::writeMapFile(const string& sFile)
{
	intmap::size_type n;
	CFile file;
	file.Open(sFile,"w");

	for(n=0; n<map_.size(); n++)
		file.Write(map_[n]+"\n");
	
	file.Close();
}

} // namespace mics end
} // namespace katzeSDK end