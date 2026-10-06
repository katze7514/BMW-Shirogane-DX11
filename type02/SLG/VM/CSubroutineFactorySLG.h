/*
	katze 05/04/26
	サブルーチンファクトリ
*/
#pragma once

namespace BMW{
namespace SLG{
class CSLGContext;
class CSLGDef;

class CSubroutineFactorySLG : public Task::ITaskListFactory
{/**
	サブルーチンファクトリ for SLG

	こいつを上手く作ることで、サブルーチン呼び出しだけで、
	SLGシーンが操作できる。
	つまり、スクリプト部分と、API部分を透過的に扱える。
 */
public:
	typedef map<int, smart_ptr<Task::ITaskList> > api_map;

	// デストラタクタ
	virtual ~CSubroutineFactorySLG(){}
	// 生成子
	virtual smart_ptr<Task::ITaskList> createTaskList(int nID);
	virtual smart_ptr<Task::ITaskList> createTaskListUser(int nID);

	// 規定の初期化
	virtual void OnInit(CSLGContext*);
	// ↓こいつをオーバーライドすることで、Storyが作れらるとおもいねえ
	virtual void setScript(CSLGContext*);
	// ストーリーごとに存在するフラグのクリア
	virtual void clearFlag(CSLGContext*){}

	// 設定
	void setSlgDef(const smart_ptr<CSLGDef>& pSlgDef){ pSlgDef_=pSlgDef; }
	smart_ptr<CSLGDef>& getSlgDef(){ return pSlgDef_; }

protected:
	// 定義済みサブルーチン
	// また、setScriptでユーザー定義サブルーチンも設定されるかも
	api_map		mapApi_;

	smart_ptr<CSLGDef> pSlgDef_;
};

} // namespace SLG end
} // namespace BMW end