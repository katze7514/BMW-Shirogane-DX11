/*
	katze 05/04/30
	思考ルーチンファクトリ
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Action{

class IAction;

class CActionFactory : public IArchive
{/**
	思考ルーチンファクトリ
 */
public:
	IAction* createAction(int nID, list<int>& listParam);
	// シリアライズ情報に合わせて生成する
	IAction* createAction();

	void Serialize(ISerialize& s);

private:
	// シリアライズによって得られるデータ
	int nID_;
	list<int> listParam_;
};

} // namespace Action end
} // namespace SLG end
} // namespace BMW end