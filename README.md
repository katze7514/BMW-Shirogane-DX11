# BattleMoonWars銀 DX11

同人ゲーム「BattleMoonWars銀」(Werk) のソースコードを、Direct3D 11 で動作するように移植したものです。

## ビルド環境

- Visual Studio (C++20、Win32)
- boost 1.34_1 (`boost/` に配置。リポジトリには含みません)
- ゲームデータ (`GameData/`) はリポジトリには含みません

## コントリビューションについて

fork や改変は自由に行ってください。ただし、Pull Request や Issue は受け付けていません。

## ライセンス

### 本リポジトリのソースコード

katze (Werk) が著作権を持つソースコード（主に `type02/`）は [MIT License](LICENSE) で公開しています。
改変・商用利用・ゲーム制作の参考など、ライセンスの条件（著作権表示とライセンス文の同梱）を守る範囲で自由に利用できます。

### yaneSDK (`yaneSDK/`)

`yaneSDK/` は、やねうらお氏 (M.Isozaki) による **yaneuraoGameSDK 3rd** に、DX11 対応などの改変を加えたものです。
著作権は原作者に帰属し、MIT License の対象外です。利用条件は原作者の配布条件に従ってください。

- 配布元: http://bm98.yaneu.com/yaneSDK.html

### 原作について

本作は TYPE-MOON 作品の二次創作です。作品名・キャラクター名などの権利は、TYPE-MOON および各権利者に帰属し、MIT License の対象外です。
