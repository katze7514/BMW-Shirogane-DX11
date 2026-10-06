/*
	katze 05/05/03

	現在値 / 最大値

	という形式のGUI
*/
#pragma once

namespace BMW{
namespace GUI{
class CGraphic;
class CNumCtrl;
class CNum;

class CNumRemain : public Task::CTaskBase
{/**
 	現在値 / 最大値

	という形式のGUI

	なお、0以下になると色替えをするので、
	必ず、現在地は、WHITEとREDの数字を設定しておくこと
	実体は作ってはおく
 */
public:
	enum eNum{
		WHITE,
		RED,
	};

	// コンストラクタ・デストラクタ
	CNumRemain();
	virtual ~CNumRemain();

	// タスク
	virtual void OnDraw(Task::CTaskContext*);

	// 設定・取得
	CNumCtrl*	getCurrentNumGui(){ return pCurrent_; }
	CNum*		getMaxNumGui(){ return pMax_; }
	CGraphic*	getSlashGui(){ return pSlash_; }

	LONG		getCurrentNum() const;
	void		setCurrentNum(LONG lNum);
	LONG		getMaxNum() const { return pMax_->getNum(); }
	void		setMaxNum(LONG lNum){ pMax_->setNum(lNum); }

	int			getTurn()const{ return nTurn_; }
	void		setTurn(int nTurn){ nTurn_=nTurn; }

	// 現在値と最大値の比率
	double getRate();

protected:
	CNumCtrl*		pCurrent_;
	CNum*			pMax_;
	CGraphic*		pSlash_;

	int	nTurn_; // 切り替え％
};

} // namespace GUI end
} // namespace BMW end