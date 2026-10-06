/**
	辞書シーンで共通の関数
	katze 08/08/25
*/
#pragma once

namespace BMW{
namespace Dict{

__inline void createPageHelper(const string& str, list<string>& listString)
{
	// 7行ごとに分割してlistにいれる
	// 与えられた文字列を改行ごとに分割する
	string page;
	int nLine=0;
	string::size_type preindex = 0, index;
	index = str.find("\\n",preindex);
	while(index!=string::npos)
	{
		string sub = str.substr(preindex, index-preindex);
		page += (sub + "\n");

		#ifdef BMW_DEBUG
		//	CDbg().Out("Line1 %d %d %d %s",nLine,preindex,index,sub.c_str());
		#endif

		if(++nLine>=7)
		{
			listString.push_back(page);
			page.clear();
			nLine=0;
		}
		preindex = index+2; // \nの分ずらす
		index = str.find("\\n",preindex);
	}
	// 最後に見つかった改行位置と文字数の差が1以上だったら、まだ文字がある
	if(str.length()-preindex > 1)
	{
		string sub = str.substr(preindex, str.length()-preindex);
		page += sub;

		#ifdef BMW_DEBUG
		//	CDbg().Out("Line2 %d %d %s",preindex,index,sub.c_str());
		#endif
	}
	// 7行未満だということも考えて最後にpush
	listString.push_back(page);
}

} // namespace Dict end
} // namespace BMW end
