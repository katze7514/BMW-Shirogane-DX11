/*
	katze 05/06/25
	データのID
*/
#pragma once

namespace BMW{
namespace Data{

namespace Flag{
enum eFlag{
	DATA_FLAG,		// Modeフラグ
	TARGET_DATA,	// 対象のデータ
	NEXT,
};
} // namespace Flag end

namespace Mode{
enum eMode{
	LOAD,
	SAVE_LOAD,
	SAVE,
	HANDOVER, // 引き継ぎ時ロード
};
} // namespace Mode end

namespace Result{
// 確認ダイアログの結果
enum eResult{
	CANCEL,
	SAVE,
	LOAD,
};
} // namespace Result end

} // namespace Data end
} // namepsace BMW end