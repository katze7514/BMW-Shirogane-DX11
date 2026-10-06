#include "stdafx.h"

#include "CGlobalData.h"

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
			s << *it;
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
		s << bDemoOff_ << bHelp_ << nDataPanel_ << bInit_;
		if(nVer_>2)
		{// バージョン3以降
			s << bInit2_;

			if(nVer_>3)
			{// バージョン4以降
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
			{// バージョン3だったら、バージョン4になるように設定
				nVer_=5;
				bInit3_=false;
				bTakumi_=false; bHaruna_=false; nDifficult_=-1;
				bAmber_=false;
			}
		}
		ef(!s.IsStoring())
		{// バージョン2だったら、バージョン4になるように設定 
			nVer_=5; 
			bInit2_=false;
			bInit3_=false; bTakumi_=false; bHaruna_=false; nDifficult_=-1;
			bAmber_=false;
		}
	}
	ef(!s.IsStoring())
	{// バージョン1だったら、バージョン4になるように設定
		nVer_=5;
		bDemoOff_=false; bHelp_=true; nDataPanel_=0; bInit_=false; 
		bInit2_=false;
		bInit3_=false; bTakumi_=false; bHaruna_=false; nDifficult_=-1;
		bAmber_=false;
	}
}
