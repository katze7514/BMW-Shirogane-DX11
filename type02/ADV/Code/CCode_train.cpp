#include "stdafx.h"

#include "../../Chara/ConstChara.h"
#include "../../Weapon/ConstWeapon.h"
#include "../../Ability/IDAbility.h"

#include "CCode_train.h"

namespace BMW{
namespace ADV{
namespace Code{

void CCode_train::ctrlTrain(int nCharaID, int nKind, int nType, int nValue, int nMax, Task::CTaskContext* pContext)
{
#ifdef BMW_DEBUG
	CDbg().Out("TRAIN %d %d %d",nKind,nCharaID,nValue);
#endif
	Chara::CDataCharaTrain* pTrain = pContext->getApp()->getExec().getTrainData(nCharaID,nKind==CHANGE||nKind==COPY);
	if(pTrain==NULL)
	{
	#ifdef BMW_DEBUG
		CDbg().Out("TRAIN %dが存在しません",nCharaID);
	#endif
		return;
	}

	switch(nKind)
	{
	case EXP:// 経験値操作
	{// つまり、LvUP
		int nLv=0;
		int nExp = nType==ADD ? pTrain->getExp()+nValue : nValue;
		while(nExp>500)
		{
			if(nMax>=0 && pTrain->getLv()+nLv >= nMax)
			{// 規定のレベルを越えたら 
				if(nExp>500) nExp=500;
				break;
			}
			++nLv;
			nExp-=500;
		}
		pTrain->setLv(pTrain->getLv()+nLv);
		pTrain->setExp(nExp);
	}
	break;


	case KILL: // 撃墜数操作
		nType==CCode_train::ADD ? pTrain->setKill(pTrain->getKill()+nValue) : pTrain->setKill(nValue);
	break;

	case CHANGE: // キャラ変更
	case COPY:	 // 養成データコピー
	{
		if(nValue>=0)
		{// 変更先が設定さてるなら変更 ← じゃない時は単にデータ生成という扱いで
			// コピー元生成
			Chara::CDataCharaTrain* pChangeTarget = pContext->getApp()->getExec().getTrainData(nValue);
			// データコピー
			*pChangeTarget = *pTrain;
			// IDが上書きされてしまうので改めて設定
			pChangeTarget->setID(Save::CExecData::getTrainID(nValue));
			// キャラ変更だったら
			// 元の養成データはいらなくなるので削除
			if(nKind==CHANGE)
			{	pContext->getApp()->getExec().delTrainData(nCharaID); }
			else
			{// アイテムとか技能はコピーしない
				pChangeTarget->getSkillList().clear();
				pChangeTarget->getItemList().clear();
			}

		}
	}
	break;

	case BACK:
	{// BP・FPが絡む養成データをBP・FPに還元する
		int nFP=0;
		int nBP=0;
		// 基礎能力：FP
		nFP =
			( pTrain->getStrength()
			+ pTrain->getMagic()
			+ pTrain->getHit()
			+ pTrain->getAvoid()
			+ pTrain->getDefence()
			+ pTrain->getSkill()
			)
			* Chara::Const::FUND_TRAINING_FP;
		// 戦闘能力：BP
		// HP
		for(int i=pTrain->getHP(); i>=1; --i) 
			nBP += Chara::Const::HP_TRAINING_BP[i];
		// EN
		for(int i=pTrain->getEN(); i>=1; --i) 
			nBP += Chara::Const::EN_TRAINING_BP[i];
		// Tough
		for(int i=pTrain->getTough(); i>=1; --i) 
			nBP += Chara::Const::TOUGH_TRAINING_BP[i];
		// Quick
		for(int i=pTrain->getQuick(); i>=1; --i) 
			nBP += Chara::Const::QUICK_TRAINING_BP[i];

		// 武器：BP
		// 武器コスト取得
		Chara::CDataCharaData* pCharaData = const_cast<Chara::CCharaDB&>(pContext->getApp()->getChara()).getCharaData(nCharaID);
		int nCost = pCharaData->getInit().getWeaponCost();
		while(nCost<0)
		{// 設定されてないってのは継承されてるキャラ
			pCharaData = pCharaData->getParentData().getPointer();
			nCost = pCharaData->getInit().getWeaponCost();
		}
		// 武器段階
		for(int i=pTrain->getWeapon(); i>=1; --i) 
			nBP += Weapon::Const::WEAPON_TRAINING_COST[nCost][i];

		// 技能：FP
		Chara::skill_list::iterator it;
		pTrain->beginSkill();
		while(!pTrain->endSkill())
		{
			it = pTrain->nextSkill();
			// LV制は、LV分還元
			// 底力・カウンター・SPアップ・移動力アップ
			// 援護攻撃・援護防御
			switch(it->getID())
			{
			case Ability::FUNDPOWER:
			case Ability::COUNTER:
			case Ability::MOVE_UP:
			case Ability::BACKUPATTACK:
			case Ability::BACKUPDEFENCE:
			case Ability::SPUP:
				for(int i=it->getAttr(); i>=1; --i)
					nFP += pContext->getApp()->getAbility().getGetFP(it->getID(),i);
			break;

			default:
				nFP += pContext->getApp()->getAbility().getGetFP(it->getID(),it->getAttr());
			break;
			}
		}

	#ifdef BMW_DEBUG
		CDbg().Out("BACK %d %d",nBP,nFP);
	#endif
		// セーブデータに還元
		Save::CExecData& save = pContext->getApp()->getExec();
		save.calcBP(nBP);
		save.calcFP(nFP);
	}
	break;

	case CLEAR:
		// BP・FPが絡む養成データクリア
		pTrain->clearFund();
		pTrain->clearBattle();
		pTrain->clearWeapon();
		pTrain->clearSkill();
	break;

	case DEL:
		// 養成データ削除
		pContext->getApp()->getExec().delTrainData(nCharaID);
	break;
	}
}

void CCode_train::OnAction(Task::CTaskContext* pContext)
{
	// 養成データ操作
	// キャラ変更時は、養成データ生成という可能性がある
	ctrlTrain(getCharaID(),getKind(), getType(), getValue(), getMax(), pContext);
}

} // namespace Code end
} // namespace ADV end
} // namespace BMW end