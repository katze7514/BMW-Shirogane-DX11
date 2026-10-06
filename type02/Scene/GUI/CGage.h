/*
	katze 05/03/21
	ゲージを表現するクラス
*/
#pragma once

namespace BMW{
namespace GUI{

class CGraphic;
class CNumRemain;
class CNum;
class CNumCtrl;

class CGage : public Task::CTaskBase
{/**
	ゲージを表現するクラス

	基本的にゲージの位置が基本
 */
public:
	enum eNum{
		WHITE,
		RED,
	};
	// コンストラクタ・デストラクタ
	CGage();
	virtual ~CGage();

	// タスク
	virtual void OnDraw(Task::CTaskContext*);

	// 設定・取得
	CNumRemain* getRemain(){ return pRemain_; }
	void		setRemain(CNumRemain* pRemain);
	CGraphic*	getGageGui(){ return pGage_; }
	bool		IsLeft() const { return bLeft_; }
	void		left(bool bLeft){ bLeft_=bLeft; }

	// 操作
	LONG		getCurrentNum() const;
	void		setCurrentNum(LONG lNum);
	LONG		getMaxNum() const;
	void		setMaxNum(LONG lNum);
	void		visibleNum(bool bV);

	// アクション
	void actionChangeNum();
	void actionChangeNum(int nCurrent, int nMax);

	// ヘルパ
	CNumCtrl*	getCurrentNumGui();
	CNum*		getMaxNumGui();
	CGraphic*	getSlashGui();

protected:
	CNumRemain*		pRemain_;
	CGraphic*		pGage_;

	// 減る方向フラグ
	// 左から右の時にfalse
	bool bLeft_;
};

} // namespace GUI end
} // namespace BMW end