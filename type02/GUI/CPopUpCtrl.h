/*
	katze 06/02/01
	ポップアップコントローラ
*/
#pragma once

#include "CPopUpBase.h"

namespace BMW{
namespace Draw{

class CPopUpCtrl
{/**
	ポップアップコントローラ

	キャッシュ動作を行う
	こいつは、あくまでPopUpインスタンス管理だけを行う
 */
public:
	typedef list<CPopUpBase*> popup_list;
	
	// キャッシュ数
	enum eCache{
		CACHE=10,
	};

	// コンストラクタ・デストラクタ
	CPopUpCtrl();
	~CPopUpCtrl();

	// ポップアップの生成とキャッシュ動作
	CPopUpBase* createPopUp(GUI::IButton* pButton, Task::CTaskContext* pContext);

	// キャッシュからの特定のポップアップ削除
	void delPopUp(const string& sID);

	// ポップアップクリア
	void clearPopUp();

private:
	// ポップアップベースリスト
	// キャッシュ数を越えると末尾から消えていく
	// もちろん、LRU
	popup_list listPopUp_;
};

} // namespace Draw end
} // namespace BMW end