#include "stdafx.h"

#ifdef BMW_DEBUG

#include "../Input/CTaskInput.h"

#include "../Face/DB/CFaceDB.h"

#include "../Scene/GUI/CGraphicFace.h"

#include "../Scene/IScene.h"

#include "CDictCharaScene.h"
#include "CDictCharaFace.h"

namespace BMW{
namespace Dict{

CDictCharaFace::~CDictCharaFace()
{
	DELETE_SAFE(pBack_);

	// タスクリストは空にして二重deleteを回避
	listTask_.clear();

	// 本体は顔リストの方
	for_each(listFace_.begin(), listFace_.end(), DeleteObj());
	listFace_.clear();
}

void CDictCharaFace::Task(Task::CTaskContext* pContext)
{
	// 背景描画
	if(!pContext->IsAction())
		(*pContext->getDrawPlane())->BltNatural(pBack_,0,0);

	// それ以外は通常動作
	Task::CTaskList::Task(pContext);
}

void CDictCharaFace::OnInit(Task::CTaskContext* pContext)
{
	// インターフェイス
	gui_.setGuiDef(Config::Const::configDB_.getConfigFileStr("FACE_VIEWER"));
	// ウインドウサイズ取得
	pContext->getApp()->getWindowSize(nWidth_, nHeight_);
	// キーボード
	pKey_ = &(static_cast<Input::CTaskInput*>(pContext->getInput())->getKeyBoard());
	// インターフェイス生成
	// 背景だけ作る
	pBack_ = new CFastPlane();
	pBack_->CreateSurface(nWidth_,nHeight_,false);
	pBack_->SetFillColor(RGB(255,255,255));
	pBack_->Clear();
}

void CDictCharaFace::OnReset(Task::CTaskContext* pContext)
{
	// 顔リストリセット
	nToward_=0;
	bBattle_=false;
	listTask_.clear();
	for_each(listFace_.begin(), listFace_.end(), DeleteObj());
	listFace_.clear();

	// 現在のキャラデータを取得
	pCurrentItem_ = static_cast<CDictCharaContext*>(pContext)->getCurrentCharaItem();

	// タイトル設定
	pContext->getApp()->setWindowTitle(pCurrentItem_->getName());

	// そのキャラの全顔情報からグラフィックを生成

	// 顔IDリストゲット
	int nFaceID = pCurrentItem_->getCharaData().getBattle().getFaceID();
	Face::CFaceDB* pFaceDB = pContext->getApp()->getFaceMap().getFaceDB(nFaceID);
	const map<string,int>& faceIDMap = pFaceDB->getFaceStringIDMap().getMap();

	map<string,int>::const_iterator it;
	// マップをぶんまわす
	for(it=faceIDMap.begin(); it!=faceIDMap.end(); ++it)
	{
		// 顔と名前のパネル
		GUI::CPanel* pFacePanel = gui_.createInterfaceCast<GUI::CPanel>("FACE_PANEL");
		listFace_.push_back(pFacePanel);

		// 各パネル取得
		GUI::CText* pFaceName=NULL;
		GUI::CGraphicFace* pFace=NULL;

		// 顔名設定
		pFaceName = pFacePanel->getWidgetCast<GUI::CText>("FACE_NAME");
		pFaceName->setText(it->first);
		pFaceName->UpdateTextAA();
		// 顔生成
		pFace = pFacePanel->getWidgetCast<GUI::CGraphicFace>("FACE_FACE");
		pFace->setFace(nFaceID, it->first, pContext);
		// GraphicFaceは下端にそろえられてしまうので、その分足しておく
		LONG nFaceWidth=0,nFaceHeight=0;
		pFace->getSize(nFaceWidth,nFaceHeight);
		if(bBattle_ || nToward_==0)	pFace->setX(0);
		else						pFace->setX(nFaceWidth);
		pFace->setY(nFaceHeight+15);
	}

	// 描画する顔ページを設定
	listPagePairIterator_.clear();
	list_face::iterator it_begin,it_end;
	pair<list_face::iterator,list_face::iterator> pair_it;

	it_begin = listFace_.begin();
	while(it_begin!=listFace_.end())
	{
		it_end = createFacePage(it_begin,pContext);
		pair_it.first = it_begin;
		pair_it.second = it_end;
		listPagePairIterator_.push_back(pair_it);
		it_begin = it_end;
	}

	// 初期ページ
	it_page_pair_ = listPagePairIterator_.begin();
	setFacePage();

	pContext->getInput()->cursolVisible(true);
	pContext->getInput()->guard(false);
}

void CDictCharaFace::OnAction(Task::CTaskContext* pContext)
{
	// キャンセルしたら、キャラセレへ
	if(Input::releaseCancel(pContext)
	|| pKey_->IsKeyPush(DIK_BACK))
	{
		pContext->getInput()->cursolVisible(false);
		pContext->getInput()->guard(true);
		pContext->getApp()->getFoward()->clearPopUp();
		getTaskListCtrl()->returnTaskList();

		// タイトル設定戻す
		pContext->getApp()->setWindowTitle("BattleMoonWars銀");
	}
	// キー入力に合わせて顔の状態を変化
	else if(pKey_->IsKeyPush(DIK_B))
	{// バトルフラグ反転
		bBattle_ = !bBattle_;
		refreshFaceList(pContext);
	}
	else if(pKey_->IsKeyPush(DIK_T))
	{// 方向反転
		nToward_ = 1-nToward_;
		refreshFaceList(pContext);
	}
	else if(pKey_->IsKeyPush(DIK_RIGHT)
		 || Input::releaseOK(pContext))
	{// 次の顔リストへ
		++it_page_pair_;

		if(it_page_pair_==listPagePairIterator_.end())
			it_page_pair_=listPagePairIterator_.begin();

		setFacePage();
	}
	else if(pKey_->IsKeyPush(DIK_LEFT))
	{// 前の顔リストへ
		if(it_page_pair_==listPagePairIterator_.begin())
			it_page_pair_=listPagePairIterator_.end();

		--it_page_pair_;
		setFacePage();
	}
}

CDictCharaFace::list_face::iterator CDictCharaFace::createFacePage(CDictCharaFace::list_face::iterator it_face_begin, Task::CTaskContext* pContext)
{
	// スタートiterator
	list_face::iterator it = it_face_begin;

	// 画面に収まる範囲で設定していく
	// 横・縦・横・縦と入れていく
	// 座標
	int nX=0,nY=0;
	// 各種必要ポインタ
	GUI::CText* pFaceName=NULL;
	GUI::CGraphicFace* pFace=NULL;
	LONG nFaceWidth=0,nFaceHeight=0;
	LONG nFaceNameWidth=0,nFaceNameHeight=0;
	LONG nFaceMaxHeight=0;

	// Faceを取り出す
	pFaceName = (*it)->getWidgetCast<GUI::CText>("FACE_NAME");
	pFaceName->getSize(nFaceNameWidth,nFaceNameHeight);
	pFace = (*it)->getWidgetCast<GUI::CGraphicFace>("FACE_FACE");
	pFace->getSize(nFaceWidth,nFaceHeight);
	
	for(;;)
	{
		// 最大高さ更新
		if(nFaceMaxHeight<nFaceHeight) nFaceMaxHeight=nFaceHeight;

		// 顔位置
		(*it)->setX(nX);
		(*it)->setY(nY);

		// 横方向へ進む
		nX += (std::max(nFaceWidth,nFaceNameWidth)+5);
		++it;

		// リストの最後に来ても終了
		if(it==listFace_.end()) break;

		// Faceを取り出す
		pFaceName = (*it)->getWidgetCast<GUI::CText>("FACE_NAME");
		pFaceName->getSize(nFaceNameWidth,nFaceNameHeight);
		pFace = (*it)->getWidgetCast<GUI::CGraphicFace>("FACE_FACE");
		pFace->getSize(nFaceWidth,nFaceHeight);

		// 横方向に進めなくなったら一つ下に降りる
		if((nWidth_-nX) < std::max(nFaceWidth,nFaceNameWidth))
		{
			nX = 0;
			nY += (nFaceMaxHeight+40);
			nFaceMaxHeight=0;

			// 縦方向に進めなくなったら終了
			if((nHeight_-nY)<nFaceHeight) break;
		}
	}
	
	// 現在のiteratorがラスト
	return it;
}

void CDictCharaFace::setFacePage()
{
	// 一端クリア
	listTask_.clear();
	// タスクリストへ追加
	int i=0;
	for(list_face::iterator it = it_page_pair_->first; it!=it_page_pair_->second; ++it)
		addTask(*it,i++);
}

// 現在の状態に併せて顔をリフレッシュ
void CDictCharaFace::refreshFaceList(Task::CTaskContext* pContext)
{
	// スタートiterator
	list_face::iterator it;
	GUI::CText* pFaceName=NULL;
	GUI::CGraphicFace* pFace=NULL;
	
	for(it=it_page_pair_->first; it!=it_page_pair_->second; ++it)
	{
		pFaceName = (*it)->getWidgetCast<GUI::CText>("FACE_NAME");
		pFace = (*it)->getWidgetCast<GUI::CGraphicFace>("FACE_FACE");
		pFace->setToward(nToward_);
		pFace->battle(bBattle_);
		pFace->setFace(pCurrentItem_->getCharaData().getBattle().getFaceID(),
					   pFaceName->getText(), pContext);
		LONG nFaceWidth=0,nFaceHeight=0;
		pFace->getSize(nFaceWidth,nFaceHeight);
		if(bBattle_ || nToward_==0)	pFace->setX(0);
		else						pFace->setX(nFaceWidth);
		pFace->setY(nFaceHeight+15);
	}
}

} // namespace Dict end
} // namespace BMW end

#endif // BMW_DEBUG
