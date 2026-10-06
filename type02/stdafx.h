#pragma once

#include "cpp17_compat.h"
#pragma warning(disable:4702)
#define _SECURE_SCL 0
#include <functional>
#pragma warning(default:4702)

// yaneSDK 読み込み
#include "yaneSDK/stdafx.h"
#include "yaneSDK/yaneSDK.h"

using std::multiset;

// misc
#include "Misc/CIntMap.h"
#include "Misc/CStringMap.h"
#include "Misc/CIntBiStringMap.h"
#include "Misc/CFixedNum.h"
#include "Misc/delete.h"
#include "Misc/misc_fun.h"

namespace katzeSDK{
	namespace Misc{}
} // namespace katzeSDK end
// using 宣言
using namespace katzeSDK::Misc;

// ↓ちょっとしたデバッグモード
// いらんときは、コメントアウト
// プロジェクトレベルで設定することにする
//#define BMW_DEBUG

// BMWトライアルモード
// これもプロジェクトレベル設定
//#define BMW_TRIAL

// BMW関係
namespace BMW{
const int WINDOW_WIDTH=640;
const int WINDOW_HEIGHT=480;

// トライアルかそうじゃないかで切り分け
#ifndef BMW_TRIAL

// 最大レベル
const int LV_MAX=99;
// 撃墜数 MAX
const int KILL_MAX=999;
// BP・FP MAX
const int BP_MAX=9999999;
const int FP_MAX=999999;

#else // #ifndef BMW_TRIAL

// 最大レベル
const int LV_MAX=15;
// 撃墜数 MAX
const int KILL_MAX=25;
// BP・FP MAX
const int BP_MAX=50000;
const int FP_MAX=500;

#endif // #ifndef BMW_TRIAL

const int EXP_MAX=500;
// 気力定数
const int MENTAL_MAX=150;
const int MENTAL_NORMAL=100;
const int MENTAL_MIN=50;
// 熟練度MAX
const int EXPERT_MAX=50;
// 週目表記上MAX
const int HANDOVER_MAX=99;
// セーブデータフォルダ名
const string sSaveFolder="savedata";
} // namespace BMW end

// 警告Lv4で発生するSpiritの警告、Spiritでは問題ないらしいので抑制してしまう
#pragma warning(disable:4511) // コピーコンストラクタ作れね
#pragma warning(disable:4512) // 代入演算子作れね
#pragma warning(disable:4709) // []の中でカンマ使うとアブね

// Spiritを読み込んでしまう
#include <boost/spirit.hpp>
#include <boost/spirit/dynamic/for.hpp>
#include <boost/spirit/dynamic/if.hpp>
#include <boost/spirit/actor/push_back_actor.hpp>
// phoenixも読み込む
#include <boost/spirit/phoenix.hpp>
// 解析系はコンパイルが遅いのでプリコンパイルをする
#include "Parser/debug_functor.h"
#include "Parser/skip_comment.h"
#include "Parser/parse_closure.h"
#include "Parser/parse_symbol.h"
#include "Draw/DB/sprite_closure.h"
#include "Face/DB/face_closure.h"
#include "Chara/DB/chara_symbol.h"
#include "Chara/DB/chara_closure.h"
#include "Chara/fund_grammar.h"
#include "Weapon/DB/weapon_symbol.h"
#include "Weapon/DB/weapon_closure.h"

#include "Config/DB/CConfigParser.h"
#include "Draw/DB/CSpriteParser.h"
#include "Face/DB/CFaceParser.h"
#include "Chara/DB/CCharaParser.h"
#include "Chara/DB/CStatusParser.h"
#include "Weapon/DB/CWeaponParser.h"

// 読み込み終了したので抑制解除
#pragma warning(default:4511) // コピーコンストラクタ作れね
#pragma warning(default:4512) // 代入演算子作れね
#pragma warning(default:4709) // []の中でカンマ使うとアブね

#include "Config/ConstConfig.h"

// Primitiveなクラスの読み込み
#include "Task/Task.h"
#include "Draw/Draw.h"
#include "GUI/GUI.h"
#include "Rule/Rule.h"
#include "Movie/Movie.h"
#include "Input/IInput.h"
#include "Sound/Sound.h"
// VM
#include "VM/VM.h"
// セーブデータ
#include "Save/CGlobalData.h"
#include "Save/CExecData.h"
// Foward
#include "Scene/Draw/CFoward.h"
// CApp読み込み
#include "CApp.h"

// よく使うやつのtypedef
namespace BMW{
namespace GUI{
typedef Task::CTaskCtrl<CGraphic>	CGraphicCtrl;
typedef Task::CTaskCtrl<CText>		CTextCtrl;
typedef Task::CTaskCtrl<CButton>	CButtonCtrl;
} // namespace GUI end
} // namespace BMW