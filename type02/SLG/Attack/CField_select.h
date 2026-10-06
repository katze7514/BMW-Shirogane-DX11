/*
	katze 06/06/11
	MAP兵器用攻撃対象選択Ver.2
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Attack{

class CField_select : public BMW::Rule::CRuleList
{/*
	フィールド武器用攻撃対象選択

	選択されると0が積まれ次へ
	キャンセルの場合は、-1を積む
 */
public:
	enum eState{
		NORMAL,
		OK,
		CANCEL,
	};
	enum ePriority{
		OK_T,
		CANCEL_T,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// アクション
	void actionEnd(Task::CTaskContext*);

	// アクセッサ
	int		getFieldType()const{ return nFieldType_; }
	void	setFieldType(int nFieldType){ nFieldType_=nFieldType; }

private:
	// 現在表示しているキャラID
	int nChara_;

	// 範囲内に攻撃対象が存在するか？
	bool IsTargetAttack();
	// フィールド兵器のタイプ
	int nFieldType_;
	// 投げ込み範囲計算
	// データ的には、移動範囲の部分を流用
	void calcThrow(Map::CMapChip* pMap, int nMove);
	int nSize_;
	int nHeight_;
	int nReach_;
	int nIndex_; // 現在表示中の中心

	// SLGコンテキスト
	CSLGContext* p;
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end