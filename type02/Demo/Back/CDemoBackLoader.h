/*
	katze 06/03/23
	デモ背景ローダ
*/
#pragma once

namespace BMW{
namespace Demo{
class CBackSymbolDB;
class CDemoBack;

class CDemoBackLoader
{/**
	デモ背景ローダ
 */
public:
	// コンストラクタ・デストラクタ
	CDemoBackLoader();
	~CDemoBackLoader();

	// 設定
	CDemoBack* createDemoBack(const string& sFile);

private:
	CBackSymbolDB* pSymbolDB_;
}; 

} // namespace Demo end
} // namespace BMW end