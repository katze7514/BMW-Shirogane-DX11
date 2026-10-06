#include "stdafx.h"

#include "../Task/CTaskContext.h"

#include "../Sound/CBgm.h"
#include "../Sound/CSeDB.h"
#include "../SLG/Map/CMapChipState.h"

#include "CGlobalData.h"

namespace BMW{
namespace Save{

void CGlobalData::setData(Task::CTaskContext* pContext)
{// 現在のデータの反映 

	// BGMに反映
	pContext->getBgmSound()->setVolume(percentVol2dB(getBGM()));
	// SEに反映
	pContext->getApp()->getSeDB().setSeVolume(percentVol2dB(getSE()));
	// グリッド表示
	SLG::Map::CMapChipState::grid(IsGrid());
}


void CGlobalData::Serialize(ISerialize& s)
{
	s << nVer_ << bFull_ << eColor_ << nBGM_ << nSE_ << bSkip_ << bGrid_;
	int nSize;
	if(s.IsStoring())
	{// save
		// まずは、サイズをsave
		nSize = (int)visitChara_.size();
		s << nSize;
		for(it=visitChara_.begin(); it!=visitChara_.end(); ++it)
			s << (int)*it;
	}
	else
	{// load
		visitChara_.clear();
		s << nSize;
		int nChara;
		for(int i=0; i<nSize; ++i)
		{
			s << nChara;
			visitChara_.insert(nChara);
		}
	}

	// バージョンによる追加
	if(nVer_>1)
	{// バージョン2以降

		// バージョン2で追加
		s << bDemoOff_ << bHelp_ << nDataPanel_ << bInit_;
		if(nVer_>2)
		{// バージョン3以降

			// バージョン3で追加
			s << bInit2_;

			if(nVer_>3)
			{// バージョン4以降

				// バージョン4で追加
				s << bInit3_ << bTakumi_ << bHaruna_ << nDifficult_;

				if(nVer_>4)
				{// バージョン5
					// バージョン5で追加
					s << bAmber_;
				}
				ef(!s.IsStoring())
				{// バージョン4だったら、バージョン5になるように設定
					nVer_=5;
					bAmber_=false;
				}
			}
			ef(!s.IsStoring())
			{// バージョン3だったら、バージョン5になるように設定
				nVer_=5;
				bInit3_=false;
				bTakumi_=false; bHaruna_=false; nDifficult_=-1;
				bAmber_=false;
			}
		}
		ef(!s.IsStoring())
		{// バージョン2だったら、バージョン5になるように設定 
			nVer_=5; 
			bInit2_=false;
			bInit3_=false; bTakumi_=false; bHaruna_=false; nDifficult_=-1;
			bAmber_=false;
		}
	}
	ef(!s.IsStoring())
	{// バージョン1だったら、バージョン5になるように設定
		nVer_=5;
		bDemoOff_=false; bHelp_=true; nDataPanel_=0; bInit_=false; 
		bInit2_=false;
		bInit3_=false; bTakumi_=false; bHaruna_=false; nDifficult_=-1;
		bAmber_=false;
	}
}

} // namespace Save end
} // namespace BMW end
