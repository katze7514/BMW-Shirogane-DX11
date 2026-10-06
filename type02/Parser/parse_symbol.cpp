#include "stdafx.h"

namespace BMW{
namespace Parser{

void planesym::setSymbol(const string& sFile)
{// ファイルからシンボル追加
	CFile file;
	file.Read(sFile);
	string s;
	int n=0;
	while(file.ReadLine(s)==0) add(s.c_str(),n++);
}

} // namespace Symbol end
} // namespace BMW end