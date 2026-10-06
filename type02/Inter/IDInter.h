/*
	katze 05/06/16
	インターミッションID
*/
#pragma once

namespace BMW{
namespace Inter{

namespace Scene{
// インターミッションのサブシーン
enum eScene{
	SELECT,
	CHARA,
};

namespace Chara{
enum eChara{
	STATUS,
	TRAIN,
	ITEM,
};
} // namespace Chara end

} // namespace Scene end

namespace Flag{
enum eFlag{
	TARGET_CHARA,	// 現在対象のキャラID
	EXCHANGE_CHARA,	// 交換対象のキャラID
	TARGET_ABILITY,	// 取得対象のアビリティID
	CTRL_ABILITY,	// 保持対象のアビリティID
	TARGET_ITEM,	// 取得対象のアイテムホルダ位置
	TARGET_ITEM_ID,	// 取得対象のアイテムID
	CTRL_ITEM,		// 保持対象のアイテムホルダ位置
	INITIALIZE,		// 一度、初期化した？
	EXCHANGE,		// 交換が一度でも実行されている？
	LOAD,			// ロード？
};
} // namespace Flag end

} // namespace Inter end
} // namespace BMW end