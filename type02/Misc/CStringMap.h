/*
	katze 05/03/13
	katze版CStringMap
*/
#pragma once

namespace katzeSDK{
namespace Misc{

class CStringMap
{/**
	ようはyaneSDKのCStringMapほど大がかりなのはいらないし、
	インターフェイスが気にくわなくなった（爆
	CIngMapと整合性を取るためでもある
 */
public:
	typedef map<string,int> stringmap;
	// デストラクタ
	virtual ~CStringMap(){}

	/// stringをキーとして、intを値とする
	void writeMap(const string&,int nValue);
	/// Valueが無い場合は、自動的にsizeが設定される
	/// そして設定されたValueを返す
	int writeMapAuto(const string&);
	/// 引数をキーとして値を取得する。無い場合は-1が返る
	int	 getValue(const string& sKey) const;
	/// 文字列\n というフォーマットで書かれたファイルを読み込んでマップを作る
	/// 上から0番目、1番目と番号が自動的に振られていく
	void readMapFile(const string&);
	/// 現在のマップを書き出す
	void writeMapFile(const string&){}
	/// 現在のマップサイズを返す
	int getMapSize()const{ return (int)map_.size(); }
	/// 現在のマップをクリアする
	void clearMap(){ map_.clear(); }
	/// マップを直接ゲット
	const stringmap& getMap()const{ return map_; }

protected:
	stringmap map_;
};

__inline void CStringMap::writeMap(const string& sKey,int nValue)
{
	map_.insert(pair<string, int>(sKey,nValue));
}

__inline int CStringMap::writeMapAuto(const string& sKey)
{
	int nValue = getMapSize();
	map_.insert(pair<string, int>(sKey,nValue));
	return nValue;
}

__inline int CStringMap::getValue(const string& sKey) const
{
	stringmap::const_iterator it = map_.find(sKey);
	if(it!=map_.end())
		return it->second;
	else 
		return -1;
}

__inline void CStringMap::readMapFile(const string& sFile)
{
	int n=0;
	string s;
	CFile file;

	file.Read(sFile);
	while(file.ReadLine(s)==0)	writeMap(s,n++);
	file.Close();
}

} // namespace Misc end
} // namespace katzeSDK end