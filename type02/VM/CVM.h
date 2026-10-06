/*
	katze 05/04/19
	VMの実行制御を行うクラス
*/
#pragma once

namespace BMW{
namespace VM{

// ファクトリとITaskListを上手く使えば、CTaskCtrlで代用可
typedef Task::CTaskListCtrl CVM;

} // namespace VM end
} // namespace BMW end