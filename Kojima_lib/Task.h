#pragma once

#include <algorithm>

class CTaskManager;

//タスクのクラス
class CTask
{
public:
	friend	CTaskManager;

	CTask();
	virtual ~CTask();

	virtual void	DoUpdate() = 0;
	virtual void	DoDraw() = 0;

	void SetIsUsed(bool tmp){m_IsUsed = tmp;}
	bool GetIsUsed(){return m_IsUsed;}

protected:

	int		m_PriDraw;		// 描画優先度
	int		m_PriUpdate;	// 更新優先度
	bool	m_IsUsed;		// 現在使用中かどうか
};

//タスクのテンプレートクラス
template <class T> class TTask :public CTask
{
protected:
	void	(T::*m_pFuncUpdate)();		//更新関数ポインタ
	void	(T::*m_pFuncDraw)();		//描画関数ポインタ

public:

	//更新関数ポインタのセット
	void	SetpFuncUpdate(void (T::*func)())
	{
		m_pFuncUpdate = func;
	}

	//描画関数ポインタのセット
	void	SetpFuncDraw(void (T::*func)())
	{
		m_pFuncDraw = func;
	}

	//更新
	void	DoUpdate()
	{
		if(m_pFuncUpdate != 0)
		{
			(((T*)this)->*m_pFuncUpdate)();
		}
	}

	//描画
	void	DoDraw()
	{
		if(m_pFuncDraw != 0)
		{
			(((T*)this)->*m_pFuncDraw)();
		}
	}

};

//タスクマネージャ
class CTaskManager
{
public:
	void			AddTask(CTask *);
	void			DeleteTask(CTask *);
	void			TaskDraw();
	void			TaskUpdate();
	void			ResetTask();

	bool ComparePriUpdate(CTask *task1, CTask *task2);
	bool ComparePriDraw(CTask *task1, CTask *task2);

	void SortbyPriority(bool (CTaskManager::*Compare)(CTask *, CTask *));

	CTaskManager();
	~CTaskManager();
protected:
	std::list<CTask *>		m_TaskList;		// タスクのリスト
	bool (CTaskManager::*TaskCompare)(CTask *, CTask *); 

};

