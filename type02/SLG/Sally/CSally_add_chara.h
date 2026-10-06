/*
	katze 05/04/30
	update 06/06/12
	キャラデータの追加
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{
namespace Sally{

class CSally_add_chara : public Task::ITaskList
{/**
	キャラデータを、Contextにロードする

	スタックに、

	思考ルーチンパラメタ数
	思考ルーチンパラメタ*n
	思考ルーチンID
	所属フェーズ
	追加したいキャラID
	養成データID（-1の時はセーブデータから）
	SLG ID（-1の時は自動的に確保される）

	と積んでおく

	SLGIDに-1を指定した時は、終了すると、
	確保されたSLG IDがスタックトップに積まれる
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);

	// データ生成
	static void initCharaData(int nChara, int nSlg, int nPhase, int nTrain, int nCpu, list<int>& listParam,
							  CDataCharaSLG* pData, CSLGContext* p);

//protected:
	// 次に対応するSLG ID
//	int nID_;
};

} // namespaec Sally end
} // namespace SLG end
} // namespace BMW end