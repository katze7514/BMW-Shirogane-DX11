/*
	katze 05/02/16
	タスクコンテキストの基底クラス
*/
#pragma once

#include "../SLG/Context/CDataBattle.h"
#include "../Scenario/CDataScenario.h"

class CApp;

namespace BMW{

namespace Sound{
class CBgm;
} // namespace Sound end

namespace Input{
class IInput;
}// namepsace Input end

namespace Scene{
class IScene;
} // namespace Scene end
namespace Task{

class ITaskList;

class CTaskContext : public IArchive
{/**
	タスクコンテキスト
 */
public:
	// コンストラクタ・デストラクタ
	virtual ~CTaskContext(){}

	// シリアライズ
	virtual void Serialize(ISerialize& s){}

	// 設定・取得
	bool						IsAction() const { return bAction_; }
	void						action(bool bA){ bAction_=bA; }

	ITaskList*					getTaskList(){ return pTaskList_; }
	void						setTaskList(ITaskList* pTaskList){ pTaskList_=pTaskList; }

	Input::IInput*				getInput(){ return pInput_; }
	void						setInput(Input::IInput* pInput){ pInput_=pInput; }
	smart_ptr<CApp>&			getApp(){ return pApp_; }
	void						setApp(const smart_ptr<CApp>& pApp){ pApp_=pApp; }
	CPlane*						getDrawPlane(){ return pPlane_; }
	void						setDrawPlane(CPlane* pPlane){ pPlane_=pPlane; }
	Sound::CBgm*				getBgmSound(){ return pBgm_; }
	void						setBgmSound(Sound::CBgm* pSound){ pBgm_=pSound; }
	smart_ptr<Scene::IScene>&	getScene(){ return pScene_; }
	void						setScene(const smart_ptr<Scene::IScene>& pScene){ pScene_=pScene; }

	// シナリオデータ
	const smart_ptr<Scenario::CDataScenario>&	getScenarioData()const{ return pScenario_; }
	smart_ptr<Scenario::CDataScenario>&			getScenarioData(){ return pScenario_; }
	void										setScenarioData(const smart_ptr<Scenario::CDataScenario>& pData){ pScenario_=pData; }

	// 戦闘データ
	smart_ptr<SLG::CDataBattle>&	getBattleData(){ return pBattleData_; }
	void							setBattleData(const smart_ptr<SLG::CDataBattle>& data){ pBattleData_=data; }

	// スタック操作
	void						push(int n){ calcStack_.push(n); }
	void						pop(){ calcStack_.pop(); }
	int							top() const { return calcStack_.top(); }
	bool						empty() const { return calcStack_.empty(); }

	// 一時データ操作
	int				getValue(int nIndex){ return mapValue_[nIndex]; }
	void			setValue(int nValue, int nIndex){ mapValue_[nIndex]=nValue; }

	// 文字列プール操作
	const string&	getString(int nID) const { return mapString_.getValue(nID); }
	int				setString(const string& s){ mapString_.writeMap(mapString_.getMapSize(), s); return mapString_.getMapSize()-1; }

protected:
	// アクションフェーズかどうか
	bool bAction_;
	// タスクListポインタ
	ITaskList* pTaskList_;

	// 入力タスク
	// 入れ替えて入力をユーザーから切り離すことがあるのだが、
	// smart_ptrだと、大元のクラスをチェックしていないので、
	// 参照カウンタが上がりっぱなしになる（と思う）
	Input::IInput*						pInput_;
	// アプリクラス
	smart_ptr<CApp>						pApp_;
	// 描画対象
	CPlane*								pPlane_;
	// BGM
	Sound::CBgm*						pBgm_;
	// シーン
	smart_ptr<Scene::IScene>			pScene_;

	// シナリオデータ
	smart_ptr<Scenario::CDataScenario>	pScenario_;
	// 戦闘データ
	smart_ptr<SLG::CDataBattle>			pBattleData_;

	// VMとしての実行環境
	// 計算スタック
	stack<int>		calcStack_;
	// 一時データ
	map<int,int>	mapValue_;
	// 文字列プール
	CIntMap			mapString_;
};

} // namespace Task
} // namespace BMW