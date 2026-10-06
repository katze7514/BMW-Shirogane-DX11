/*
	katze 05/12/22
	キャラコマを表現するクラス Ver.2
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // naemsapce Task end

namespace SLG{
class CCharaState;

namespace Map{

class CMapChipCharaSprite2 : public Task::CTaskBase
{/**
	キャラコマを表現するクラス Ver.2

	チップアニメを導入する上での変更バージョン
	チップアニメ再生などの管理は全部こいつがやる
	位置などの動かしそのものは、別の上位クラスで行う

	こいつの状態が、再生中のチップアニメ
 */
public:
	// コンストラクタ・デストラクタ
	CMapChipCharaSprite2();
	~CMapChipCharaSprite2();
	// タスク
	void Task(Task::CTaskContext*);

	// 設定・取得
	Task::ITaskBase*			getChipMovie(int nMovie){ return pChipMovie_[nMovie]; }
	void						setChipMovie(Task::ITaskBase* pMovie, int nMovie)
								{ 
									pChipMovie_[nMovie]=pMovie; 
									pChipMovie_[nMovie]->setParent(smart_ptr<Task::ITaskBase>(this,false));
								}
	void						getSize(LONG& lWidth, LONG& lHeight);
	void						getDrawSize(LONG& lWidth, LONG& lHeight);

private:
	// チップアニメ
	Task::ITaskBase* pChipMovie_[ChipMovie::END];
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end