/*
	katze 05/03/24
	SLGシーンのコンテキスト
*/
#pragma once

#include "../Effect/CEffectDB.h"
#include "../Event/CSLGMsgLog.h"

#include "../IDSLG.h"
#include "CDataBattleMap.h"

namespace BMW{

namespace Chara{
class CDataCharaTrain;
} // namespace Chara end

namespace Weapon{
class CDataWeaponBattle;
} // namespace Weapon end

namespace SLG{
class CSLGScene;
class CSlgVM;
class CDataCharaSLG;
class CSLGDef;

namespace Map{
class CMap;
class CMapChip;
} // namespace Map end

namespace Event{
class CEvent;
} // namespace Event end

struct CWeaponRange
{// 武器範囲計算時に使われる構造体
	int nID_;		// 対応するキャラID
	int nIndex_;	// MapIndex
	bool bAttack_;	// Attackとして有効
	bool bField_;	// Fieldとして有効
	
	CWeaponRange(int nID=-1, int nIndex=-1, bool bAttack=false, bool bField=false):nID_(nID),nIndex_(nIndex),bAttack_(bAttack),bField_(bField){}
};

class CSLGContext : public Task::CTaskContext
{/**
	SLGシーンのコンテキスト
 */
public:
	typedef map<int, CDataCharaSLG*>				chara_map;
	typedef map<int, Weapon::CDataWeaponBattle*>	weapon_map;
	typedef	list<CWeaponRange>						range_list;
	typedef vector<range_list>						weapon_range;
	// コンストラクタ・デストラクタ
	CSLGContext();
	virtual ~CSLGContext();

	// シリアライズ
	void Serialize(ISerialize& s);
	void SerializeSave(ISerialize& s);
	void SerializeLoad(ISerialize& s);
	void SerializeList(list<int>& List, ISerialize& s);
	void SerializeSet(set<int>& Set, ISerialize& s);
	// データの完全化
	void completeData();
	void completeChara(CDataCharaSLG& chara);
	void readyTrain(Chara::CDataCharaTrain& train, CDataCharaSLG& chara);
	void completeWeapon(Weapon::CDataWeaponBattle* pWeapon, CDataCharaSLG& chara);

	// 設定・取得
	CSLGDef&				getSLGDef(){ return *slgDef_; }
	CSLGDef*				getSLGDefPtr(){ return slgDef_; }
	void					initValue(int nValue, int nID);
	Chara::CDataCharaTrain& getTrain(int nID);
	void					setTrain(int nID, const Chara::CDataCharaTrain& pTrain);

	// 各種フラグへのアクセッサ
	void		clearFlag();
	int			getID(){ return mapValue_[Flag::ID]; }
	void		setID(int nID){  mapValue_[Flag::ID]=nID; }
	int			getTurn(){ return mapValue_[Flag::TURN]; }
	void		setTurn(int nTurn){  mapValue_[Flag::TURN]=nTurn; }
	int			getBP(){ return mapValue_[Flag::BP]; }
	void		setBP(int nBP){  mapValue_[Flag::BP]=nBP; }
	int			getFP(){ return mapValue_[Flag::FP]; }
	void		setFP(int nFP){  mapValue_[Flag::FP]=nFP; }
	int			getExpert(){ return mapValue_[Flag::EXPERT]; }
	void		setExpert(int nExpert){  mapValue_[Flag::EXPERT]=nExpert; }
	int			getBgm(){ return nBgm_; }
	void		setBgm(int nBgm){ nBgm_=nBgm; }

	list<int>&	getGetItemList(){ return listGetItem_;}
	void		addGetItem(int nItemID){ listGetItem_.push_back(nItemID); }
	
	int			getPhase(){ return mapValue_[Flag::PHASE]; }
	void		setPhase(int nPhase){ mapValue_[Flag::PHASE]=nPhase; }
	int			getPhaseChange(){ return mapValue_[Flag::PHASE_CHANGE]; }
	void		setPhaseChange(int nPhaseChange){ mapValue_[Flag::PHASE_CHANGE]=nPhaseChange; }

	int			getEnemyReset(){ return mapValue_[Flag::ENEMY_RESET]; }
	void		setEnemyReset(int nEnemyReset){ mapValue_[Flag::ENEMY_RESET]=nEnemyReset; }

	int			getVictory(){ return mapValue_[Flag::VICTORY]; }
	void		setVictory(int nVictory){ mapValue_[Flag::VICTORY]=nVictory; }
	void		initVictory(int nVictory){ if(getValue(Flag::CONTINUE)) mapValue_[Flag::VICTORY]=nVictory; }
	void		initVictoryHard(int nVictory){	if(getValue(Flag::CONTINUE)) initHardVic(nVictory, Flag::VICTORY);	}
	int			getLose(){ return mapValue_[Flag::LOSE]; }
	void		setLose(int nLose){ mapValue_[Flag::LOSE]=nLose; }
	void		initLose(int nLose){ if(getValue(Flag::CONTINUE)) mapValue_[Flag::LOSE]=nLose; }
	void		initLoseHard(int nLose){ if(getValue(Flag::CONTINUE)) initHardVic(nLose, Flag::LOSE); }
	int			getExpertCond(){ return mapValue_[Flag::EXPERT_C]; }
	void		setExpertCond(int nExpertCond){ mapValue_[Flag::EXPERT_C]=nExpertCond; }
	void		initExpertCond(int nExpertCond){ if(getValue(Flag::CONTINUE) && getExpertCond()>=0) mapValue_[Flag::EXPERT_C]=nExpertCond; }
	void		initExpertHard(int nExpert){ if(getValue(Flag::CONTINUE)) initHardVic(nExpert, Flag::EXPERT); }
	void		initHardVic(int nInit, int nFlag)
	{// ハードモードのみ設定
		if(getScenarioData()->getExpertRank(getApp()->getExec().getExpert())==Expert::HARD)
			mapValue_[nFlag]=nInit;
	}

	int			getDemo(){ return mapValue_[Flag::DEMO]; }
	void		setDemo(int nDemo){ mapValue_[Flag::DEMO]=nDemo; }

	int			getNextWeapon(){ return mapValue_[Flag::NEXT_WEAPON_ID]; }
	void		setNextWeapon(int nID){ mapValue_[Flag::NEXT_WEAPON_ID]=nID; }
	int			getNextSally(){ return mapValue_[Flag::NEXT_SALLY_ID]; }
	void		setNextSally(int nID){ mapValue_[Flag::NEXT_SALLY_ID]=nID; }

	int			getTargetMap(){ return mapValue_[Flag::TARGET_MAP]; }
	void		setTargetMap(int nTargetMap){ mapValue_[Flag::TARGET_MAP]=nTargetMap; }
	int			getTargetWeapon(){ return mapValue_[Flag::TARGET_WEAPON]; }
	void		setTargetWeapon(int nTargetWeapon){ mapValue_[Flag::TARGET_MAP]=nTargetWeapon; }
	int			getCtrlWeapon(){ return mapValue_[Flag::CTRL_WEAPON]; }
	void		setCtrlWeapon(int nCtrlWeapon){ mapValue_[Flag::CTRL_WEAPON]=nCtrlWeapon; }
	int			getTargetChara(){ return mapValue_[Flag::TARGET_CHARA]; }
	void		setTargetChara(int nTargetChara){ mapValue_[Flag::TARGET_CHARA]=nTargetChara; }
	int			getCtrlChara(){ return mapValue_[Flag::CTRL_CHARA]; }
	void		setCtrlChara(int nCtrlChara){ mapValue_[Flag::CTRL_CHARA]=nCtrlChara; }
	int			getTargetAbility(){ return mapValue_[Flag::TARGET_ABILITY]; }
	void		setTargetAbility(int nTargetAbility){ mapValue_[Flag::TARGET_ABILITY]=nTargetAbility; }

	// マップインデックスセット
	set<int>&	getIndexSet(){ return setIndex_; }
	void		clearMove();
	void		clearDist();
	void		clearAttack();

	// サークルメニュー座標
	int			getCircleX()const{ return nCircleX_; }
	int			getCircleY()const{ return nCircleY_; }
	void		setCirclePos(int nID=Pos::CHIP);
	void		setCirclePos(int nID, int& nX, int& nY);
	void		setCirclePos(int nX, int nY){ nCircleX_=nX; nCircleY_=nY; }

	// フェーズ
	list<int>&	getPhaseList(int nPhase)
	{
		if(nPhase==Phase::PLAYER)	return getPlayerPhaseList();
		ef(nPhase==Phase::ENEMY)	return getEnemyPhaseList();
		else						return getNeutralPhaseList();
	}
	list<int>&	getPlayerPhaseList(){ return listPlayer_; }
	list<int>&	getEnemyPhaseList(){ return listEnemy_; }
	list<int>&	getNeutralPhaseList(){ return listNeutral_; }
	list<int>&	getNonPlayerPhaseList(){ return listNonPlayer_; }
	set<int>&	getDeathSet(){ return setDeath_; }
	// 未行動キャラ数取得
	int			getNoActionChara();
	// マップにいるキャラ数
	int			getMapExistChara();
	// キャラデータ検索
	int			searchPhase(int nSlgID);
	bool		searchPhase(int nSlgID,int nPhase);
	int			searchSlg(int nCharaID);
	int			searchSlgChild(int nCharaID, int nPhase);
	int			searchFace2Slg(int nFaceID);
	int			searchMapSymbol(const string& sMapID, int nPhase);
	// 削除した次のイテレータが返る
	list<int>::iterator	delPhase(int nSlgID, int nPhase=-1, bool bNonPlayer=false);
	// マップに存在してるキャラのSLG IDリストを取得する
	void		getMapExistList(list<int>& listChara);
	// 全キャラのactionPhasePerを呼び出す
	void		phasePer();
	// ↑の下請け
	void		phasePerList(list<int>& List);
	// 指定したフェーズのキャラのPhaseStartを呼び出す
	void		phaseStartPhase(int nPhase);
	// ↑の下請け
	void		phaseStartList(list<int>& List);
	// 死んでいない全キャラのActをBeforeにする
	void		allActBefore();
	// 指定したフェイズの全キャラのActをBeforeにする
	void		actBefore(int nPhase);
	// 指定したフェイズの全キャラのActをAfterにする
	void		actAfter(int nPhase,set<int>& setOut);
	// 指定したフェイズの全キャラの指定したIDでMentalを呼び出す
	// nSlgIDに指定したのは除かれる
	void		allMental(int nPhase, int nID, int nSlg);
	
	// エフェクトDB
	Effect::CEffectDB&	getEffectDB(){ return effectDB_; }
	// マップチップ設定
	void createMapChip();
	
	// キャラ系
	chara_map&		getCharaMap(){ return mapChara_; }
	CDataCharaSLG*	getCharaData(int nID);
	CDataCharaSLG*	getCharaData(const string& sID);
	CDataCharaSLG*	getTargetCharaData(){ return getCharaData(getTargetChara()); }
	CDataCharaSLG*	getCtrlCharaData(){ return getCharaData(getCtrlChara()); }
	void			setCharaData(int nID, CDataCharaSLG* pData, bool bPhase=true);
	void			delCharaData(int nID);
	CDataCharaSLG*	delMapCharaData(int nID, bool bPhase=true);
	void			clearCharaData();

	// 武器系
	Weapon::CDataWeaponBattle*	getWeaponData(int nID);
	Weapon::CDataWeaponBattle*	getTargetWeaponData(){ return getWeaponData(getTargetWeapon()); }
	Weapon::CDataWeaponBattle*	getCtrlWeaponData(){ return getWeaponData(getCtrlWeapon()); }
	void						setWeaponData(int nID, Weapon::CDataWeaponBattle* pData);
	void						delWeaponData(int nID, CDataCharaSLG* pChara=NULL);
	void						delCharaWeaponData(CDataCharaSLG* pChara);
	void						clearWeaponData();

	// 武器射程系
	void			resizeRange(int nMax){ rangeWeapon_.resize(nMax); }
	range_list&		getRangeList(int nRange){ return rangeWeapon_[nRange]; }
	void			addRange(int nRange, int nID, int nIndex){ rangeWeapon_[nRange].push_back(CWeaponRange(nID,nIndex,true)); }
	void			delRange(int nRange, int nIndex);
	void			clearRange();

	bool			IsRangeChara(int nHeight, bool bF=false);
	bool			IsRangeCharaInner(int nMin, int nHeight, int nPhase);
	bool			IsRangeChara(int nRange, int nPhase, bool bFriend, int nHeight, bool bF=false, bool bSmart=false);
	int				hasRangeChara(int nRange, int nID);
	int				hasRangeIndex(int nIndex, bool bF=false);
	// 指定したフェーズ（bFirend次第で外にななる）の射程範囲内にいるキャラIDリストを取得する
	void			getRangeChara(int nPhase, bool bFriend, list<int>& listChara);
	// フィールド向け
	void			addRangeField(int nRange, int nID, int nIndex){ rangeWeapon_[nRange].push_back(CWeaponRange(nID,nIndex,false,true)); }
	void			delRangeField(int nRange, int nIndex);
	void			getRangeFieldChara(int nFieldType, int nPhase, int nMin, int nMax, int nFileToward, set<int>& setChara);
	void			getRangeFieldToward(int nFieldType, int nPhase, int nMin, int nMax, int anToward[], int nSnipe, bool bSnipe[]);
	bool			IsRangeField(int nFieldToward, int nMin, int nMax, int nPhase);
	bool			IsRangeFieldThrow(int nPhase);

	// MAP兵器用攻撃データ
	CDataBattleMap&	getBattleMap(){ return battleMap_; }

	// バックログ
	list<CSLGMsgLog>&		getBackLogList(){ return listBackLog_; }
	void					addBackLog(int nSide, int nSlg, int nChara, int nFace, int nString, int nMask){	listBackLog_.push_front(CSLGMsgLog(nSide,nSlg,nChara,nFace,nString,nMask));	}
	void					clearBackLog(){ listBackLog_.clear(); }

	// クイックロード
	bool					IsQuickLoad()const{ return bQuickLoad_; }
	void					quickLoad(bool bQuickLoad){ bQuickLoad_=bQuickLoad; }

	// シーン
	//const smart_ptr<CSLGScene>& getSLGScene(){ return pSLGScene_; }
	//void						setSLGScene(const smart_ptr<CSLGScene>& scene){ pSLGScene_=scene; }

	// マップ系
	const smart_ptr<Map::CMap>& getMap(){ return pMap_; }
	void						setMap(const smart_ptr<Map::CMap>& map){ pMap_=map; }
	Map::CMapChip*				getMapChip(int nIndex);
	Map::CMapChip*				getTargetMapChip();

	// イベント系
	const smart_ptr<Event::CEvent>& getEvent(){ return pEvent_; }
	void							setEvent(const smart_ptr<Event::CEvent>& pEvent){ pEvent_=pEvent; }

	// VM
	const smart_ptr<CSlgVM>&	getVM(){ return pVM_; }
	void						setVM(const smart_ptr<CSlgVM>& pVM){ pVM_=pVM; }

private:
	// 内部利用関数
	bool IsList(int nSlgID, list<int>& list);
	int	 IsChara(int nCharaID, list<int>& list);
	int	 IsFace(int nFaceID, list<int>& list);
	int	 IsMapSymbol(const string& sMapID, list<int>& List);

	// SLG定義データ
	CSLGDef* slgDef_;
	// スクリプトデータ
	// 養成データも↑の中に
	// map<int, Chara::CDataCharaTrain> mapTrain_;

	// indexセット
	set<int>	setIndex_;
	// サークルメニュー表示中心
	int			nCircleX_,nCircleY_;
	
	// ゲーム進行データ
	// 各フェーズに属するキャラのSLG IDリスト
	list<int>	listPlayer_;
	list<int>	listEnemy_;
	list<int>	listNeutral_;
	// PlayerNPCリスト
	list<int>	listNonPlayer_;
	// 死亡セット
	set<int>	setDeath_;

	// エフェクトDB
	Effect::CEffectDB	effectDB_;

	// マップマス
	GUI::CGraphic*	pMapChip_[5];

	// MSGバックログ用
	list<CSLGMsgLog> listBackLog_;

	// クイックロード可能フラグ
	bool bQuickLoad_;

	// 各種データ
	// キャラ
	chara_map	mapChara_;
	// 武器
	weapon_map		mapWeapon_;
	weapon_range	rangeWeapon_;
	// MAP兵器用攻撃データ
	CDataBattleMap	battleMap_;
	
	// 唯の保存用
	int nBgm_; // 中断時に鳴っていたBGM
	// このマップで手に入れたアイテムリスト
	// data_refでこのデータにあわせてアイテムが追加
	list<int> listGetItem_;

	// Scene
	//smart_ptr<CSLGScene>		pSLGScene_;
	// Map
	smart_ptr<Map::CMap>		pMap_;
	// Event
	smart_ptr<Event::CEvent>	pEvent_;
	// VM
	smart_ptr<CSlgVM>			pVM_;
};

// ヘルパ
// 攻撃可能な武器を持っている？
bool IsAtk(CDataCharaSLG* pChara, CSLGContext* p, bool bP);

} // namespace SLG end
} // namespace BMW end