/**
	katze 08/08/19
	キャラ辞典のキャラ表示
*/
#pragma once

namespace BMW{

namespace Weapon{
class CDataWeaponBattle;
} // namespace Weapon end

namespace Dict{

class CDictCharaItem;

class CDictCharaView : public Task::ITaskList
{
public:
	enum eState{
		NORMAL,
		DEMO
	};
	enum eButton{
		WEAPON1,
		WEAPON2,
		WEAPON3,
		WEAPON4,
		CHANGE,
		LEFT,
		RIGHT,
		ORIGINAL,
		BMW,
		COMMENT,
	};
	enum ePlot{
		PLOT_ORIJINAL,
		PLOT_BMW,
		PLOT_COMMENT,
	};
	// コンストラクタ・デストラクタ
	CDictCharaView();
	~CDictCharaView();

	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// イベントリスナー
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);

private:
	// 現在、表示中のキャラデータ
	CDictCharaItem* pCurrentItem_;

	// 対応する武器ID
	int nWeaponID_[4];
	// 再生する武器データの実体
	Weapon::CDataWeaponBattle* pWeaponData_;
	// 戦闘背景ファイル名一覧
	katzeSDK::Misc::CIntMap battleBackMap_;

	// 現在、選択中の説明とページ
	int nCurrentPlot_, nCurrentPlotPage_;
	// 各説明の最大ページ
	int nPlotMaxPage_[3];
	// 現在のページ
	list<string>::iterator page_it_;
	// 各ページ
	list<string> pageProfile_;
	list<string> pageIntro_;
	list<string> pageComment_;

	// インターフェイス
	GUI::CPanel*		pPanel_;
	// 各種操作対象
	GUI::CText*			pText_;
	GUI::INum*			pTextPage_;
	GUI::CPanel*		pChange_;
	GUI::CText*			pCurrentNum_;
	GUI::CPanelCtrl*	apWeapon_[4];
	GUI::CButton*		pPlotButton_[3];

	// ヘルパ
	// 武器アイコンの設定
	void setWeaponIcon(Task::CTaskContext* pContext);
	// 説明表示、新しく表示する説明とそのページを渡す
	void setPlot(int nPlot);
	// 現在のページを生成する
	void createPage();
	// 武器データを生成し、CBattleDataを設定する
	void setDemoData(int nWeaponSelect, Task::CTaskContext*);
};

} // namespace Dict end
} // namespace BMW end
