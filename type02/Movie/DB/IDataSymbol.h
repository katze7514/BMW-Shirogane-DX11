/*
	katze 05/05/10
	シンボルデータクラスの基底
*/
#pragma once

namespace BMW{
namespace Movie{

class IDataSymbol
{/**
	シンボルデータクラスの基底
 */
public:
	enum eKind{
		GRAPHIC,
		BUTTON,
		NUM,
		MOVIE_CLIP,
	};

	// コンストラクタ・デストラクタ
	IDataSymbol(int nKind):nKind_(nKind){}
	virtual ~IDataSymbol(){}

	// 設定・取得
	int		getKind() const { return nKind_; }
	void	setKind(int nKind){ nKind_=nKind; }

protected:
	int nKind_;
};

} // namespace Movie end
} // namespace BMW end
