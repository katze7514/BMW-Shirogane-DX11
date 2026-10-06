#include "stdafx.h"

#include "../mode.h"

#include "../Scene/ConstScene.h"
#include "../Chara/CDataCharaTrain.h"
#include "CExecData.h"

namespace BMW{
namespace Save{

map<int,int> CExecData::trainMap_;
//map<int,int> CExecData::hideMap_;

CExecData::~CExecData()
{
	clear();
}

int	CExecData::getExpert() const
{
#ifndef EXPERT_NORMAL
	return head_.getExpert();
#else
	// NORMALモード
	return 0;
#endif
}

void CExecData::Serialize(ISerialize& s)
{
	int nSize;
	int nFirst,nSecond;
	// ヘッダのシリアライズ
	s << head_;

	// 次のシナリオID
	s << nNextScenario_;

	// 養成マップのシリアライズ
	if(s.IsStoring())
	{// save

		// 有効なキャラセットサイズを書き出す
		nSize = (int)setValid_.size();
		s << nSize;
		// 有効なキャラIDをシリアライズ
		for(it_s=setValid_.begin(); it_s!=setValid_.end(); ++it_s)
			s << (int)*it_s;

		// 養成データ
		// マップサイズを書き出す
		nSize = (int)mapTrain_.size();
		s << nSize;
		// TrainDataをシリアライズ
		train_map::iterator it;
		for(it=mapTrain_.begin(); it!=mapTrain_.end(); ++it)
			s << *it->second;

		// アイテム
		// マップサイズを書き出す
		nSize = (int)mapItem_.size();
		s << nSize;
		item_map::iterator it_i;
		for(it_i=mapItem_.begin(); it_i!=mapItem_.end(); ++it_i)
		{
			nFirst = it_i->first; 
			s << nFirst;
			s << it_i->second;
		}

		// フラグ
		// マップサイズを書き出す
		nSize = (int)mapFlag_.size();
		s << nSize;
		flag_map::iterator it_f;
		for(it_f=mapFlag_.begin(); it_f!=mapFlag_.end(); ++it_f)
		{
			nFirst = it_f->first; 
			s << nFirst;
			s << it_f->second;
		}
	}
	else
	{// load
		int j;

		// とりあえず、setをクリア
		setValid_.clear();
		// 格納されてるサイズを取り出す
		s << nSize;
		// 有効なキャラをシリアライズ
		for(int i=0; i<nSize; ++i)
		{
			s << j;
			setValid_.insert(j);
		}

		// とりあえず、mapをクリア
		clearTrainData();
		// 格納されているサイズを取り出す
		s << nSize;
		// サイズ分TrainDataを生成
		for(int i=0; i<nSize; ++i)
		{	// TrainDataを生成
			Chara::CDataCharaTrain* pTrain = new Chara::CDataCharaTrain();
			// シリアライズ
			s << *pTrain;
			// マップに格納
			mapTrain_.insert(pair<int, Chara::CDataCharaTrain*>(pTrain->getID(),pTrain));
		}

		// とりあえず、mapをクリア
		mapItem_.clear();
		// 格納されているサイズを取り出す
		s << nSize;
		// サイズ分ItemDataを生成
		for(int i=0; i<nSize; ++i)
		{	// TrainDataを生成
			// シリアライズ
			s << nFirst;
			s << nSecond;
			// マップに格納
			mapItem_.insert(pair<int, int>(nFirst, nSecond));
		}

		// とりあえず、mapをクリア
		mapFlag_.clear();
		// 格納されているサイズを取り出す
		s << nSize;
		// サイズ分ItemDataを生成
		for(int i=0; i<nSize; ++i)
		{	// TrainDataを生成
			// シリアライズ
			s << nFirst;
			s << nSecond;
			// マップに格納
			mapFlag_.insert(pair<int, int>(nFirst, nSecond));
		}
	}
}

bool CExecData::getFlag(const string& sFlag, int nValue)
{
	flag_map::iterator it = mapFlag_.find(Scene::Const::flagID_.getValue(sFlag));
	if(it==mapFlag_.end()) return nValue==-1;
	return nValue==it->second;
}

bool CExecData::getFlag(int nFlag, int& nValue)
{
	flag_map::iterator it = mapFlag_.find(nFlag);
	if(it==mapFlag_.end())
	{
		nValue=-1;
		return false; 
	}
	else
	{
		nValue=it->second;
		return true;
	}
}

void CExecData::setFlag(int nFlag, int nValue)
{
	flag_map::iterator it = mapFlag_.find(nFlag);
	if(it==mapFlag_.end())
		mapFlag_.insert(pair<int ,int>(nFlag,nValue));
	else
		it->second = nValue;
}

void CExecData::setFlag(const string& sFlag, int nValue)
{
	setFlag(Scene::Const::flagID_.getValue(sFlag),nValue);
}

void CExecData::calcFlag(const string& sFlag, int nValue)
{
	calcFlag(Scene::Const::flagID_.getValue(sFlag),nValue);
}

void CExecData::calcFlag(int nFlag, int nValue)
{
	flag_map::iterator it = mapFlag_.find(nFlag);
	if(it==mapFlag_.end())
		mapFlag_.insert(pair<int ,int>(nFlag,nValue));
	else
		it->second += nValue;
}

Chara::CDataCharaTrain* CExecData::getTrainData(int nID,bool bNew)
{
	// IDの取得
	nID = getTrainID(nID);

	// 養成データゲット
	train_map::iterator it;
	it=mapTrain_.find(nID);
	if(it!=mapTrain_.end())
	{
		return it->second;
	}
	else
	{// mapに無かったら
		if(bNew)
		{// 新規作成フラグが立ってたら、
		 // 新しく生成して、mapに追加
			Chara::CDataCharaTrain* pTrain = new Chara::CDataCharaTrain();
			pTrain->setID(nID); // IDの設定
			mapTrain_.insert(pair<int, Chara::CDataCharaTrain*>(nID,pTrain));
			return pTrain;
		}
		else
		{
			return NULL;
		}
	}
}

void CExecData::delTrainData(int nID)
{
	nID = getTrainID(nID);
	train_map::iterator it = mapTrain_.find(nID);
	if(it==mapTrain_.end()) return;
	DELETE_SAFE(it->second);
	mapTrain_.erase(it);
}

void CExecData::clearTrainData()
{
	train_map::iterator it;
	for(it=mapTrain_.begin(); it!=mapTrain_.end(); it++)
		DELETE_SAFE(it->second);

	mapTrain_.clear();
}

void CExecData::incItem(int nID)
{
	item_map::iterator it = mapItem_.find(nID);
	if(it!=mapItem_.end())
	{
		++it->second;
		// 回復系アイテム以外は9個制限
		// 回復系は99個まで溜められます
		/*switch(it->first)
		{
		case Item::MEDI:
		case Item::CURRY:
		case Item::SHIRO:
		case Item::MAGAZINE:
		case Item::MABO:
			if(it->second>99) it->second=99;
		break;

		default:*/
			if(it->second>9) it->second=9;
		//break;
		//}
	}
	else
	{// 持ってなかったら新たに追加
		mapItem_.insert(pair<int,int>(nID,1));
	}
}

void CExecData::decItem(int nID)
{
	item_map::iterator it = mapItem_.find(nID);
	if(it!=mapItem_.end())
	{
		--it->second;
		if(it->second<=0) mapItem_.erase(it);
	}
}

void CExecData::clear()
{
	head_.clear();
	nNextScenario_=-1;
	setValid_.clear();
	clearTrainData();
	mapItem_.clear();
	mapFlag_.clear();
}

void CExecData::clearHandover()
{
	head_.clearHandover();
	nNextScenario_=-1;
	setValid_.clear();
	clearTrainData();
}

} // namespace Save end
} // namespace BMW end