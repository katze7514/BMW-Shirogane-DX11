/*
	katze 05/05/21
	ADVのクロージャ
*/
#pragma once

#include "AdvCmd.h"

namespace BMW{
namespace ADV{

// メッセージ変更
struct msg_closure : public boost::spirit::closure<msg_closure, CCmdMsg, string>
{
	member1 val;
	member2 s;
};

// 背景変更
struct back_closure : public boost::spirit::closure<back_closure, CCmdBack>
{
	member1 val;
};

// メッセージ状態変更
struct msg_state_closure : public boost::spirit::closure<msg_state_closure, CCmdMsgState>
{
	member1 val;
};

// フェードコントロール
struct fade_closure : public boost::spirit::closure<fade_closure, CCmdFade>
{
	member1 val;
};

struct valid_closure : public boost::spirit::closure<valid_closure, CCmdValid>
{
	member1 val;
};

struct train_closure : public boost::spirit::closure<train_closure, CCmdTrain>
{
	member1 val;
};

struct train_all_closure : public boost::spirit::closure<train_all_closure, CCmdTrainAll>
{
	member1 val;
};

struct item_ctrl_closure : public boost::spirit::closure<item_ctrl_closure, CCmdItemCtrl>
{
	member1 val;
};

struct item_closure : public boost::spirit::closure<item_closure, pair<int,int> >
{
	member1 val;
};

} // namespace ADV end
} // namespace BMW end