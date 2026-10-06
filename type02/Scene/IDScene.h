/*
	katze 05/03/12
	update 08/06/21
	BMWのシーンID
*/

namespace BMW{
namespace Scene{

namespace ID{
enum eSceneID
{
	INIT_LOAD,	// 起動時ロード
	LOGO,		// ロゴシーン
	TITLE,		// タイトル
	HERO,		// 主人公選択
	DATA,		// データ
	TUTORIAL,	// チュートリアル
	GAME,		// ゲームモード選択
	ADV,		// ADV
	SLG,		// SLG
	INTER,		// Intermission
	DEMO,		// 戦闘デモ
	ED,			// エンディング
	MOVIE,		// ムービー再生シーン
	END,		// クリア後の処理をするシーン
	DICT_CHARA,	// キャラ辞典
	DICT_SOUND,	// サウンド辞典
	HANDOVER,	// クリアデータ引き継ぎ
};

} // namespace ID end

namespace Mode{
// ゲームモード選択シーンが呼ばれた時の状況
enum eMode{
	NEW,		// NEWゲーム
	LOAD,		// データロード
	CONTINUE,	// コンティニュー
};
} // namespace Mode end

} // namespace Scene end
} // namespace BMW end