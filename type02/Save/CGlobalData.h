/*
	katze 05/03/13
	update 06/02/21 Ver.2へ
	グローバルフラグクラス
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace Save{

const string sGlobal="config.ini";

class CGlobalData : public IArchive
{/**
	グローバルフラグを管理するクラス
 */
public:
	enum COLOR{
		COLOR_32=32,
		COLOR_16=16
	};
	// コンストラクタ
	CGlobalData():nVer_(5),
				#ifdef BMW_DEBUG
					bFull_(false)
				#else
					bFull_(true)
				#endif
				,nBGM_(80),nSE_(80),eColor_(COLOR_32),bSkip_(true),bGrid_(false),bDemoOff_(false),bHelp_(true),nDataPanel_(0)
				,bInit_(false),bInit2_(false),bInit3_(false),bTakumi_(false),bHaruna_(false),nDifficult_(-1),bAmber_(false)
	{}

	// セーブ対象なのでシリアライズを実装する
	void Serialize(ISerialize& s);

	// 設定・取得
	int		getVer() const { return nVer_; }
	void	setVer(int nVer) { nVer_=nVer; }
	bool	IsFull()const{ return bFull_; }
	void	full(bool bFull){ bFull_=bFull; }
	int		getColor()const{ return eColor_;}
	void	setColor(COLOR eColor){ eColor_=eColor; }
	int		getBGM() const { return nBGM_; }
	void	setBGM(int nBGM){ nBGM_=nBGM; }
	int		getSE() const { return nSE_; }
	void	setSE(int nSE){ nSE_=nSE; }
	bool	IsSkip() const { return bSkip_; }
	void	skip(bool bSkip){ bSkip_=bSkip; }
	bool	IsGrid() const { return bGrid_; }
	void	grid(bool bGrid){ bGrid_=bGrid; }
	bool	IsDemoOff() const { return bDemoOff_; }
	void	demoOff(bool bDemoOff){ bDemoOff_=bDemoOff; }
	bool	IsHelp() const { return bHelp_; }
	void	help(bool bHelp){ bHelp_=bHelp; }
	int		getDataPanel()const{ return nDataPanel_; }
	void	setDataPanel(int nData){ nDataPanel_=nData; }
	bool	IsInit() const { return bInit_; }
	void	init(bool bInit){ bInit_=bInit; }
	bool	IsInit2() const { return bInit2_; }
	void	init2(bool bInit2){ bInit2_=bInit2; }
	bool	IsInit3() const { return bInit3_; }
	void	init3(bool bInit3){ bInit3_=bInit3; }
	bool	IsTakumiClear()const{ return bTakumi_; }
	void	takumiClear(bool bTakumi){ bTakumi_=bTakumi; }
	bool	IsHarunaClear()const{ return bHaruna_; }
	void	harunaClear(bool bHaruna){ bHaruna_=bHaruna; }
	int		getDifficult()const{ return nDifficult_; }
	void	setDifficult(int nDifficult){ nDifficult_=nDifficult; }
	bool	IsAmberClear()const{ return bAmber_; }
	void	amberClear(bool bAmber){ bAmber_=bAmber; }

	// ヘルパ
	bool	IsClear()const{ return IsTakumiClear() || IsHarunaClear(); }

	// 操作
	set<int>::iterator	beginChara() const 
	{
		CGlobalData* pData = const_cast<CGlobalData*>(this);
		pData->it = pData->visitChara_.begin();
		return it;
	}
	bool				endChara() const
	{
		CGlobalData* pData = const_cast<CGlobalData*>(this);
		return pData->it==pData->visitChara_.end();
	}
	set<int>::iterator	nextChara() const
	{
		CGlobalData* pData = const_cast<CGlobalData*>(this);
		return pData->it++;
	}
	void				addChara(int nChara){ visitChara_.insert(nChara); }

	// 対応するグローバルファイルに設定する
	void setData(Task::CTaskContext* pContext);

private:
	// セーブデータのバージョン
	int		nVer_;
	// 初期起動モード
	bool	bFull_;
	// 色深度
	int		eColor_;
	// BGM音量(0～100)
	int		nBGM_;
	// SE音量(0～100)
	int		nSE_;
	// アニメ時のフレームスキップフラグ
	bool	bSkip_;
	// グリッド表示フラグ
	bool	bGrid_;

	// 出会ったキャラID
	set<int>	visitChara_;
	set<int>::iterator it;

	// デモOFF時の曲切り替え
	bool	bDemoOff_;
	// 簡易ヘルプ
	bool	bHelp_;
	// 最後にセーブを行ったパネルNo
	int		nDataPanel_;
	// 初回起動（第一部から第二部への変更）
	bool	bInit_;
	// 初回起動２（第二部から第三部への変更）
	bool	bInit2_;
	// 初回起動３（第三部から第四部への変更）
	bool	bInit3_;

	// クリア情報
	// 主人公
	bool bTakumi_;
	bool bHaruna_;
	// クリア難易度
	int nDifficult_;
	// アンバー戦クリア？
	bool bAmber_;
};

} // namespace Save end
} // namespace BMW end