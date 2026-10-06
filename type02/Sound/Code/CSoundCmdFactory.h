/*
	katze 05/05/21
	コードファクトリ
*/
#pragma once

namespace BMW{
namespace Sound{
namespace Code{
class CCmdSound;

class CSoundCmdFactory
{/**
 	コードファクトリ
 */
public:
	static void createBgm(int nCtrl, int nID, int nFade, VM::CScript* script);
	static void createBgm(CCmdSound& cmd, Task::CTaskContext* context, VM::CScript* script);
	static void createSe(int nCtrl, int nID, VM::CScript* script);
	static void createSe(CCmdSound& cmd, Task::CTaskContext* context, VM::CScript* script);
};

} // namespace Code end
} // namespace Sound end
} // namespace BMW end