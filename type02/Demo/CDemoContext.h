/*
	katze 05/05/23
	デモコンテキスト
*/
#pragma once

namespace BMW{
namespace Demo{
class CDemoScene;
class CDemoMsgBoard;
class CDemoEasyStatus;
class CDemoDamage;

class CDemoContext : public Task::CTaskContext
{/**
	デモコンテキスト
 */
public:
	// 設定・取得
	smart_ptr<CDemoScene>&		getDemoScene(){ return pDemoScene_;}
	void						setDemoScene(const smart_ptr<CDemoScene>& pScene){ pDemoScene_=pScene; }
	smart_ptr<CDemoMsgBoard>&	getMsgBoard(){ return pBoard_;}
	void						setMsgBoard(const smart_ptr<CDemoMsgBoard>& pBoard){ pBoard_=pBoard; }
	smart_ptr<CDemoEasyStatus>& getEasyStatus(int nSide){ return pStatus_[nSide]; }
	void						setEasyStatus(const smart_ptr<CDemoEasyStatus>& status, int nSide){ pStatus_[nSide]=status; }
	smart_ptr<CDemoDamage>&		getDamage(){ return pDamage_; }
	void						setDamage(const smart_ptr<CDemoDamage>& pDamage){ pDamage_=pDamage; }

	// フラグデータ
	void setFlag(int nFlag, int nHP, int nDamageEN, int nEN, int nSkillEN, int nSkillDefEN)
	{
		setValue(nHP,nFlag);
		setValue(nDamageEN,nFlag+1);
		setValue(nEN,nFlag+2);
		setValue(nSkillEN,nFlag+3);
		setValue(nSkillDefEN,nFlag+4);
	}

private:
	// シーン
	smart_ptr<CDemoScene>		pDemoScene_;
	// MSGボード
	smart_ptr<CDemoMsgBoard>	pBoard_;
	// EasyStatus
	smart_ptr<CDemoEasyStatus>	pStatus_[2];
	// ダメージ
	smart_ptr<CDemoDamage>		pDamage_;
};

} // namespace Demo end
} // namespace BMW end