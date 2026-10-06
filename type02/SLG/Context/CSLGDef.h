/*
	katze 05/06/28
	update 06/04/09
	update 06/04/19
	SLGの定義クラス
*/
#pragma once

#include "../../Chara/CDataCharaTrain.h"
#include "../Effect/CEffectDB.h"

namespace BMW{
namespace SLG{
class CSLGContext;
class ISlgCond;

namespace Event{
class CBattleEventData;
}// namespace Event end

class CSLGDef
{/**
	SLGの定義クラス

	正確には、SLGの定義スクリプトの読み込み、
	定義情報を保持するクラス
 */
public:
	typedef map<int, smart_ptr<VM::CScript> >	script_map;
	typedef vector<ISlgCond*>					vic_vec;
	typedef	map<int, Event::CBattleEventData*>	battle_map;

	// コンストラクタ・デストラクタ
	CSLGDef();
	~CSLGDef();

	// 操作
	void setSLGDef(const string& sFile,CSLGContext* pContext);

	// 設定・取得
	const string&	getMap()const{ return sMap_; }
	void			setMap(const string& sMap){ sMap_=sMap; }
	int				getFactory()const{ return nFactory_; }
	void			setFactory(int nFactory){ nFactory_=nFactory; }

	// スクリプト
	smart_ptr<VM::CScript>& getScript(int nID);
	smart_ptr<VM::CScript>& getScript(const string& sID){ return getScript(scriptID_.getValue(sID)); }
	void					setScript(const string& sID, VM::CScript* pScript);
	int						getScriptID(const string& sID)const{ return scriptID_.getValue(sID); }
	void					setScriptID(const string& sID);

	// イベント戦闘
	Event::CBattleEventData*	getBattleEvent(int nID);
	Event::CBattleEventData*	getBattleEvent(const string& sID);
	void						setBattleEvent(const string& sID, Event::CBattleEventData* pBattle);
	int							getBattleEventID(const string& sID){ return battleID_.getValue(sID); }

	// 勝利条件判定
	bool					IsVictory(CSLGContext*);
	void					setVictory(ISlgCond* pCond){ victoryCond_.push_back(pCond); }
	bool					IsLose(CSLGContext*);
	void					setLose(ISlgCond* pCond){ loseCond_.push_back(pCond); }
	bool					IsExpert(CSLGContext*);
	void					setExpert(ISlgCond* pCond){ expertCond_.push_back(pCond); }

	// 養成
	Chara::CDataCharaTrain& getTrain(int nID){ return mapTrain_[nID]; }
	void					setTrain(int nID, const Chara::CDataCharaTrain& pTrain)
							{
								mapTrain_.insert(pair<int, Chara::CDataCharaTrain>(nID, pTrain));
							}
	
	// エフェクト
	Effect::CEffectDB&		getEffect(){ return effectDB_; }
	void					setEffect(const string& sFile){ effectDB_.setSymbol(sFile); }

	// SLG IDマップ
	int						getSlgID(const string& sID){ return slgID_.getValue(sID); }
	void					setSlgID(const string& sID, int nID){ slgID_.writeMap(sID,nID); }
	int						getSlgMapSize()const{ return slgID_.getMapSize(); }

	// FLAG IDマップ
	int						getFlagID(const string& sID){ return flagID_.getValue(sID); }
	void					setFlagID(const string& sID, int nID){ flagID_.writeMap(sID,nID); }

private:
	// マップ定義ID
	string								sMap_;
	// このSLGに対応するSLGファクトリID
	int									nFactory_;
	// このSLGで使用するスクリプトマップ
	script_map							mapScript_;
	// 関数IDとその文字列表現の変換
	katzeSDK::Misc::CStringMap			scriptID_;
	// 戦闘イベント
	battle_map							mapBattle_;
	katzeSDK::Misc::CStringMap			battleID_;
	// 勝利条件判定
	vic_vec								victoryCond_;
	vic_vec								loseCond_;
	vic_vec								expertCond_;
	// このSLGで使用する一時養成データ
	map<int, Chara::CDataCharaTrain>	mapTrain_;
	// このSLGだけで使うEffect
	Effect::CEffectDB					effectDB_;
	// このSLGでの文字列IDとSLG IDのマップ
	katzeSDK::Misc::CStringMap			slgID_;
	// このSLGでの文字列IDとFLAG IDのマップ
	katzeSDK::Misc::CStringMap			flagID_;

	// 判断
	bool IsCond(vic_vec& vec, int nID, CSLGContext*);
};

} // namespace SLG end
} // namespace BMW end