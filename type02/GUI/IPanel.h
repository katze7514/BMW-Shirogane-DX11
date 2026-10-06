/*
	katze 06/02/04
	パネル基底クラス
*/
#pragma once

namespace BMW{
namespace GUI{

class IPanel : public Task::CTaskBase
{/**
	パネル基底クラス
 */
public:
	// デストラクタ
	virtual ~IPanel(){}

	// 設定・取得
	virtual Task::ITaskBase*	getWidget(int nID){ return NULL; }
	virtual Task::ITaskBase*	getWidget(const string& sID){ return NULL; }
	// /区切りで、IDを指定することでフォルダからファイルを取り出すのか
	// ごとくタスクを取り出すことができる
	virtual Task::ITaskBase*	getWidgetRec(const string& sID);
	virtual void				addWidget(Task::ITaskBase* pBase, const string& sID){}
	virtual void				delWidget(const string& sID){}
	virtual Task::ITaskBase*	removeWidget(const string& sID){ return NULL; }

	int		getID(const string& sID)const{ return priorityID_.getValue(sID); }
	void	setID(const string& sID);

	// Widget取得のヘルパ
	template<class T>
	T* getWidgetCast(int nID){ return static_cast<T*>(getWidget(nID)); }
	template<class T>
	T* getWidgetCast(const string& sID){ return static_cast<T*>(getWidget(sID)); }
	template<class T>
	T* getWidgetRecCast(const string& sID){ return static_cast<T*>(getWidgetRec(sID)); }

protected:
	// 名前とpriorityのマップ
	katzeSDK::Misc::CStringMap priorityID_;
};

} // namespace GUI end
} // namespace BMW end