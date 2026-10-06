//	yaneThread.h :
//		programmed by yaneurao	'00/02/26
//		同期オブジェクトの導入 '01/11/19

#ifndef __yaneThread_h__
#define __yaneThread_h__


#include "../Thread/yaneEvent.h"
#include "../Thread/yaneLockObject.h"

namespace yaneuraoGameSDK3rd {
namespace Thread {

class IThread {
public:
	virtual LRESULT CreateThread()=0;
	virtual LRESULT CreateThread(const smart_ptr<function_callback>& fn)=0;
	virtual void SetThreadFunction(const smart_ptr<function_callback>& fn)=0;
	virtual LRESULT StopThread()=0;
	virtual LRESULT JumpToThread()=0;
	virtual bool IsThreadExecute()const=0;
	virtual bool IsThreadValid()const=0;
	virtual void InvalidateThread()=0;
	virtual DWORD	GetThreadId() const=0;

	//	java互換スレッド機能
	virtual LRESULT	wait(int nTimeOut=-1)
		#ifdef USE_EXCEPTION
			noexcept(false)	// throws CInterruptedException
		#endif
			=0;
	virtual	LRESULT sleep(int nTime)
		#ifdef USE_EXCEPTION
			noexcept(false)	// throws CInterruptedException
		#endif
			=0;
	virtual void	notify()=0;
	virtual void	notifyAll()=0;
	virtual void	interrupt()=0;
	virtual bool	isInterrupted() const=0;
	virtual bool	interrupted()=0;

	virtual ~IThread(){}

	virtual CEvent* GetExceptionEvent()=0;
	//	例外発生時にsignal状態になるイベント

	//	そのスレッドに対応するスレッドクラスを得る
	//	(NULLであることもありうる)
	static IThread* getThread() { return pThread; }
	static void setThread(IThread*p) { pThread = p; }
		//	↑これはCThreadがCreateThreadしたときに勝手にやりよる
	static void unsetThread() { pThread.Clear(); }

protected:
	static ThreadLocal<IThread*> pThread;
};

}}

//	他のnamespaceにあるものを前方宣言する
namespace yaneuraoGameSDK3rd { namespace AppFrame { class IAppBase; }}

namespace yaneuraoGameSDK3rd {
namespace Thread {

class CThread : public IThread {
/**
	スレッドを生成（管理）します。

	スレッド生成に関しては、
	class CThreadManager のほうも参照してください。

	//	スレッドクラスから派生させる場合は、
	//	必ず派生クラス側でStopThreadを行なう必要があるので注意
	(→やね本２を参照のこと)
*/
public:
	virtual void ThreadProc() {}
	/**
		ワーカースレッドの処理を記述するために、
		この関数をオーバーライドします。
		（このクラスを派生させて使うとき）
	*/

	virtual LRESULT CreateThread();
	virtual LRESULT CreateThread(const smart_ptr<function_callback>& fn);
	virtual void SetThreadFunction(const smart_ptr<function_callback>& fn);
	/**
		CreateThreadは、スレッドを作成し、ThreadProcを実行しはじめます。
		CreateThreadの引数として、コールバックする関数を指定しない場合は、
		ThreadProcが呼び出されるのでThreadProcをオーバーライドしておくか、
		SetThreadFunctionで、スレッド関数としてコールバックする関数を
		設定しておくこと。
	*/

	virtual LRESULT StopThread();
	/**
		スレッドのストップ。停止するまで待ちます
		返し値：
			０：正常終了
			１：すでにスレッドが存在していなかった（正常終了）
	*/

	virtual LRESULT JumpToThread();	///	スレッドを作らずにジャンプ

	virtual bool IsThreadExecute()const; /// スレッドは実行中か？

	virtual bool IsThreadValid()const;
	/**
		スレッドのStopはかかっていないか
		この関数がfalseになったときに、ThreadProcから脱出するように
		コーディングします。
	*/

	virtual void InvalidateThread();
	/**
		スレッドに停止信号を送る
		（その停止を待つわけではない）
		ワーカースレッドは次にIsThreadValidをチェックしたときに
		falseが返ってきますので、そのときにThreadProcから抜け出るでしょう。
		あるいはThreadSleepしたときに非０が返るので、そこでスレッドを
		抜けるコードを書いても良いです。
	*/

	/// ----- Javaのスレッド機能をいくつか実装
	/**
		それぞれの説明について詳しくは class CLockObject の同名の関数と
		JavaのThreadの同名の関数を参照のこと。
	*/

	virtual LRESULT	wait(int nTimeOut=-1)
		#ifdef USE_EXCEPTION
			noexcept(false)	// throws CInterruptedException
		#endif
			;
	///		wait キューに並ぶ

	virtual void	notify();
	/// waitで待っているスレッドを叩き起こす(1つのみ)

	virtual void	notifyAll();
	/// waitで待っているすべてのスレッドを叩き起こす

	virtual void	interrupt();
	/**
		割り込みをかける。この関数は、他スレッドから呼び出す。
		そうすると、このスレッドは、次のwaitかsleepで
			CInterruptedException例外を発生させる。
	*/

	virtual bool	isInterrupted() const;
	///	割り込み状態になっているか

	virtual bool	interrupted();
	///	割り込み状態になっているかを返すが、その後、
	///	割り込み状態はリセットする。

	virtual	LRESULT sleep(int nTime)
		#ifdef USE_EXCEPTION
			noexcept(false)	// throws CInterruptedException
		#endif
			;
	/**
		nTime [ms]待つ。
	返し値：
		0：正常終了　
		1 : 待っている最中に、別スレッドからStopThreadが呼び出されれば
		同期オブジェクトがシグナル状態（スレッドを終了させなさい、
		という状態）になり、ただちに関数を抜ける。その場合、非0が返る。
		他スレッドから、このスレッドのinterruptメソッドが呼び出された
		場合、sleepでの待機を即座に終了し、CInterruptedException例外を
		発生させる。

	通例、ワーカースレッドは、
　　while ( IsThreadValid( ) ) {
　　　	処理 ;
　　　	Sleep( 20 );
　　} 
	というようなコーディングによって、何かの処理を行なう処理を書きますが、
	このときにSleep(20)があるため、スレッドがInvalidateされて（終了の合図）
	から、この分だけレスポンスが低下します。そこで、ここをSleepではなく、
	if (thread.sleep( 20 )) break; などとすれば、この分のレスポンスを
	向上させることが出来ます。thread.sleepの実装は、同期オブジェクトを待つ
	(::WaitForSingleObject)ようになっているので、この部分においてCPUパワーを、
	あまり使用せず、効率的にSleepすることが出来ます。
	*/

	/// ----- property

	virtual DWORD	GetThreadId() const { return m_dwRunningThreadID; }
	/**
		起動しているスレッドの、GetCurrentThreadIdで返される値を返す。
		スレッドが起動していない場合は、-1が返る
	*/
	virtual HANDLE	GetThreadHandle() const { return m_dwThreadHandle; }
	/**
		起動しているスレッドのハンドルを返す
	*/

	//	スレッドのスタックサイズの指定
	void	SetStackSize(size_t size) { m_nStackSize = size; }
	size_t	GetStackSize() const { return m_nStackSize; }

	CThread();
	virtual ~CThread();

protected:
	volatile HANDLE	m_dwThreadHandle;		//	スレッドハンドル
	volatile DWORD	m_dwRunningThreadID;	//	スレッドのID(上記と同じだと思うがOS依存の可能性もあるので一応別扱いにする)

//	static void ThreadCallBack(LPVOID lpVoid);
	static unsigned __stdcall ThreadCallBack( void * lpVoid);

	void	InitEvent();
	//	以下の２つのスレッドのステータスを初期化する

	CEvent	m_vExistEvent;
	//	これは、スレッドが生きている間は、ノンシグナル状態
	//	シグナル状態になったときは、スレッドを終了させなさいという合図
	CEvent* GetExistEvent() { return &m_vExistEvent; }
	//	⇒シグナル状態になれば、IsThreadValidがfalseを返す

	CEvent m_vThreadExecute;
	///	スレッドが生成され、スレッドコールバック関数を開始したか？
	CEvent* GetThreadExecute() { return &m_vThreadExecute; }
	//	↑実行中ならばシグナル状態になる

	//	sleepするときなどに待つべき例外イベント
	CEvent	m_vExceptionEvent;
	virtual CEvent* GetExceptionEvent() { return &m_vExceptionEvent; }

	//	threadがstartするまでの状態
	CEvent	m_vThreadStarting;
	CEvent*	GetThreadStarting() { return &m_vThreadStarting; }
	//	↑開始するまでの待ち状態＝ノンシグナル状態

	//	CreateとStop動作を排他するためのクリティカルセクション
	CCriticalSection m_vCriticalSection;
	CCriticalSection* GetCriticalSection() { return& m_vCriticalSection;}

	smart_ptr<function_callback> m_fn;
	smart_ptr<function_callback> GetCallBack() const { return m_fn; }

	CLockObject	m_vLockObject;
	/*volatile*/ CLockObject* GetLockObject() { return &m_vLockObject;}
	//	これスレッドのためのLock Object

	bool	m_bCreatedThread;
	/*
		CreateThreadで生成されたスレッドなのかを示すフラグ
		JumpThreadで生成されたスレッドは、終了時にWaitForSingleObjectしては
		いけないので、フラグを立てておく
	*/

	size_t	m_nStackSize;
};

class IThreadManager {
public:
	///	スレッドの追加と削除
	virtual void	AddThread(IThread* pThread)=0;
	virtual void	DelThread(IThread* pThread)=0;
	virtual bool	StopAllThread()=0;
	virtual void	WaitAllThreadEnded()=0;
	virtual ~IThreadManager(){}
};

class CThreadManager : public IThreadManager {
/**
	Threadを統括するThreadManager
	これは主にclass CThread から内部的に呼び出される

	ユーザーが使うとしたら、runメソッドぐらい
	これは、
		CThreadManager::RunThread(new CThreadXXX);
	というように使う。あとは、所有権はこのスレッドクラスが
	面倒を見てくれる。
	
*/
public:
	static	CThreadManager* GetObj() { return m_obj.get(); }
	///	↑こいつで、singletonな奴を得る

	///	スレッドの追加と削除
	virtual void	AddThread(IThread* pThread);
	virtual void	DelThread(IThread* pThread);

	///	すべてのスレッドを終了させる（終了を待つ）
	virtual bool	StopAllThread();
	/*
		すでに他スレッドがStopAllThreadを呼び出し、終了処理中で
		あれば、balkして、そのときはfasleが返る
	*/

	///	すべてのスレッドが終了するのを待つ（終了させるわけではない）
	virtual void	WaitAllThreadEnded();

	/**
		スレッドを保持する。所有権は、このクラスが面倒を見る
		スレッドを走らせる前に、こいつに所有権を持たせておけば、
		そのスレッドが終了するときにインスタンスが自動的に削除される
	*/
	static void	RegistThread(IThread* pThread)
		{ GetObj()->RegistThread(shared_ptr<IThread>(pThread)); }
//	static void RegistThread(const smart_ptr<IThread>& pThread)
//		{ GetObj()->_RegistThread(pThread); }
	static void RegistThread(const shared_ptr<IThread>& pThread)
		{ GetObj()->_RegistThread(pThread); }

	/**
		RegistThreadと同じく所有権までこのクラスが面倒を見るが、さらに
		IAppBaseのRunを呼び出すことを保証する
	*/
	static LRESULT RunThread(IAppBase*pAppBase);
//	static LRESULT RunThread(const smart_ptr<IAppBase>& pAppBase);
	static LRESULT RunThread(const shared_ptr<IAppBase>& pAppBase);

	/**	
		RegistThreadと同じく所有権までこのクラスが面倒を見るが、さらに
		IThreadのCreateThreadを呼び出すことを保証する
	*/
	static LRESULT CreateThread(IThread*pThread);
//	static LRESULT CreateThread(const smart_ptr<IThread>& pThread);
	static LRESULT CreateThread(const shared_ptr<IThread>& pThread);

protected:
	//	↑ジャンプ台にして↓にジャンプする
	virtual void _RegistThread(
		const shared_ptr<IThread>& pThread,
			//	このスレッドが停止するときに、
		const shared_obj& obj=shared_obj()
			//	このオブジェクトを削除する
		//	↑これ渡すときは、参照カウントが狂わないように
		//	smart_ptr::inc_refしてから、そのポインタを渡してやる必要あり
	);
public:

	CThreadManager():m_bStopAllThread(false) {}

	typedef vector<IThread*> threadlist; 
protected:

	/**
		管理しているスレッド一覧
	*/

	threadlist m_apThreadList;
	threadlist* GetThreadList() { return& m_apThreadList; }
		//	こいつら本当はvolatileなのだが、VC++は、
		//	メンバ変数へのアクセスはレジスタ割付しない

	/**
		これは所有しているスレッド一覧
	*/
	struct CThreadList {
		shared_ptr<IThread> pThread;	//	このスレッドが停止するときに、
		shared_obj obj;		//	このオブジェクトを削除する
		bool	bValid;					//	この↑２つのデータメンバが適正かを示す
			//	別スレッドから削除しないと、自分で自分のスレッドを削除することは出来ないので．．
		CThreadList():bValid(true){}
	};
	smart_list_ptr<CThreadList> m_apMyThreadList;
	smart_list_ptr<CThreadList>* GetMyThreadList()
		{ return& m_apMyThreadList; }
	void	GarbageThread();
	//	deleteマーク(CThreadList::bValid==false)のついている
	//	スレッドを削除する(ThreadListに対するCriticalLockは、
	//	掛けた状態で呼び出すこと)

	CLockObject m_lockobject;
	CLockObject* GetLockObject() { return &m_lockobject; }

	/////////////////////////////////////////////////////////////
	///	全スレッドの停止のための制御機構
	volatile bool	m_bStopAllThread;
	CCriticalSection m_csStopAllThread;
	CCriticalSection* GetStopAllThreadLock() { return &m_csStopAllThread;}
	/**
		全スレッドの停止勧告が出ているかどうかのフラグと
		それに対するクリティカルセクション
		(latch)
	*/
	bool	isStopAllThread() {
	///	このメソッドは、一度呼び出すと、以降はtrueを返す
		CCriticalLock cl(GetStopAllThreadLock());
		bool b = m_bStopAllThread;
		m_bStopAllThread = true;
		return b;
	}

	/////////////////////////////////////////////////////////////

	///	singletonパターン
	static	singleton<CThreadManager> m_obj;
};

} // end of namespace Thread
} // end of namespace yaneuraoGameSDK3rd

#endif
