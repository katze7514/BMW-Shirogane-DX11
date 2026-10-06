/*
	katze 05/06/28
	ストーリーごとのファクトリを生成するファクトリ
*/
#pragma once

namespace BMW{
namespace SLG{
class CSubroutineFactorySLG;

class CSLGFactory
{/**
	ストーリーごとのファクトリを生成するファクトリ
 */
public:
	CSubroutineFactorySLG* createFactory(int nID);
};

} // namespace SLG end
} // namespace BMW end