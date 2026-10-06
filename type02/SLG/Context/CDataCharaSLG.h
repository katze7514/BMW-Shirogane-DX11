/*
	katze 05/03/26
	SLGシーンで使うキャラデータクラス
*/
#pragma once

#include "../../Chara/CDataCharaBattle.h"
#include "CMapSymbolDB.h"
#include "CCharaState.h"

namespace BMW{
namespace SLG{
class CSLGContext;

namespace Action{
class IAction;
} // namespace Action end

class CDataCharaSLG : public IArchive
{/**
	SLGシーンで使うキャラデータクラス
 */
public:
	// コンストラクタ
	CDataCharaSLG();
	// デストラクタ
	~CDataCharaSLG();

	// シリアライズ
	void Serialize(ISerialize& s);

	// 設定・取得
	int								getID() const { return nID_; }
	void							setID(int nID){ nID_=nID; }
	int								getPhase() const { return nPhase_; }
	void							setPhase(int nPhase){ nPhase_=nPhase; }
	Chara::CDataCharaBattle&		getBattle(){ return battle_; }
	void							setBattle(const Chara::CDataCharaBattle& battle){ battle_=battle; }
	int								getTrain() const { return nTrain_; }
	void							setTrain(int nTrain){ nTrain_=nTrain; }

	const CCharaState&				getState()const{ return state_; }
	CCharaState&					getState(){ return state_; }
	void							setState(const CCharaState& state){ state_=state; }

	bool							IsWeaponLoad()const{ return bWeapon_; }
	void							weaponLoad(bool bWeapon){ bWeapon_=bWeapon; }

	// const版
	const Chara::CDataCharaBattle&	getBattle() const { return battle_; }

	Action::IAction*				getAction(){ return pAction_; }
	const Action::IAction*			getAction()const{ return pAction_; }
	void							setAction(Action::IAction* action){ pAction_=action; }
	void							createMapSymbol(const string& sFile);
	smart_ptr<CMapSymbolDB>&		getMapSymbol(){ return pMapSymbol_; }
	void							setMapSymbol(const smart_ptr<CMapSymbolDB>& pDB){ pMapSymbol_=pDB; }

	// 操作
	bool							IsExist()const; // マップ上に居るか？
	bool							IsLive()const; // 生きているか？
	CCharaState*					getStatePtr(){ return &state_; }
	Chara::CDataCharaBattle*		getBattlePtr(){ return &battle_; }

	int								getCharaID() const { return battle_.getID(); }

	// 武器IDから、武器SLGIDを取得する
	int								getWeaponSlgID(int nID, CSLGContext& context);
	int								getWeaponSlgID(const string& sID, CSLGContext& context);

	int								getIndex() const { return state_.getIndex(); }
	void							setIndex(int nIndex){ state_.setIndex(nIndex); }

	bool							IsItem(CSLGContext*);

	list<int>&						getPersList(){ return listPers_; }
	void							setPersList(list<int>& listPers){ listPers_=listPers; }
	bool							IsPers(int nID, const Chara::CCharaDB& db)
									{
										list<int>::iterator it;
										for(it=listPers_.begin(); it!=listPers_.end(); it++)
											if(db.IsChild(nID,*it)) return true;

										return false;
									}
	bool							IsPers(int nID)
									{
										list<int>::iterator it;
										for(it=listPers_.begin(); it!=listPers_.end(); it++)
											if(nID==*it) return true;

										return false;
									}
	void							addPers(int nID)
									{
										listPers_.push_back(nID);
									}
	void							delPers(int nID)
									{
										list<int>::iterator it;
										for(it=listPers_.begin(); it!=listPers_.end(); it++)
											if(*it == nID) 
											{// あえて、始めに見つかったのだけ消すことで、
											 // 説得回数を表現してみる
												listPers_.erase(it);
											}
									}
	void							popPers()
									{// とりあえず、ポップするだけということで
										listPers_.pop_front();
									}
	void							refill(CSLGContext* p, bool bRefill=true); // 補給動作
	void							calcHP(int nHP);
	
	// アクション
	int  actionCounter(int nDist, int nRealDist, int nHeight, CSLGContext& p, int nHP=0, bool bBackUp=false);
	void actionPhasePer(CSLGContext& p);
	void actionPhaseStart(CSLGContext& p,bool bIntro=false, bool bReset=false);
	void actionMental(int nID);
	
private:
	// SLG基本情報
	int							nID_;		// SLG ID
	int							nPhase_;	// 所属フェーズ
	Chara::CDataCharaBattle		battle_;	// キャラ戦闘データ
	int							nTrain_;	// ↑に対応する養成データID
											// 負の時は、セーブデータが対象
	// SLG状態
	CCharaState					state_;		// このキャラの状態(CMapChipCharaに共有される)
	// 武器設定は終わってる？
	bool						bWeapon_;	
	// 説得可能対象キャラID
	list<int>					listPers_;

	// 思考ルーチンなどを担うアクション
	Action::IAction*			pAction_;
	// デモOFFなどの時のMovie
	smart_ptr<CMapSymbolDB>		pMapSymbol_;
};

} // namespace SLG end
} // namespace BMW end