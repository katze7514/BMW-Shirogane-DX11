/*
	katze 06/05/11
	キャラデータ操作コード
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Code{

class CCode_chara : public BMW::Rule::IRuleTask
{/**
	キャラデータ操作コード

	大したコストじゃないからいいと思うけど
	もしかしたら、ファクトリ用意しても良いかもね
 */
public:
	enum eChara
	{
		PLAYER=-3,
		ENEMY=-2,
		NEUTORAL=-1,
	};
	enum eKind{
		HP,
		EN,
		MENTAL,
		STATE,
		ACT,
		WAY,
		APPER,
		VALID,
		ACTION,
		LOVE,
		SP,
	};
	enum eValueType{
		ABS,
		RATIO_MAX,
		RATIO_CURRENT,
	};
	enum eValidType{
		MOVE,
		ATTACK,
		SPIRIT,
		ITEM,
		CURE,
		REFILL,
		PERS,
	};
	enum eKindType{
		MAX,
		CURRENT
	};
	// アクセッサ
	int		getChara()const{ return nChara_; }
	void	setChara(int nChara){ nChara_=nChara; }
	int		getKind()const{ return nKind_; }
	void	setKind(int nKind){ nKind_=nKind; }
	int		getType()const{ return nType_; }
	void	setType(int nType){ nType_=nType; }
	int		getValue()const{ return listValue_.front(); }
	void	setValue(int nValue){ listValue_.push_back(nValue); }
	void	setValueList(const list<int>& listValue){ listValue_=listValue; }

	// 動作
	void OnAction(Task::CTaskContext*);

private:
	int nChara_;
	int nKind_;
	int nType_;
	list<int> listValue_;

	int getRatioValue(int nAbs);
	void allChara(CSLGContext* p);
	void oneChara(CSLGContext* p);

	void for_each(list<int>& phaseList, CSLGContext* p, void (CCode_chara::*apply)(CDataCharaSLG* pChara));
	void applyHP(CDataCharaSLG*);
	void applyEN(CDataCharaSLG*);
	void applySP(CDataCharaSLG* pChara);
	void applyMental(CDataCharaSLG*);
	void applyAct(CDataCharaSLG*);
	void applyValid(CDataCharaSLG*);
	void applyAction(CDataCharaSLG*);
};

} // namespace Code end
} // namepsace SLG end
} // namespace BMW end