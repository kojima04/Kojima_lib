#include "Common.h"
#include "Task.h"

//-------------------------------------------------------------
//						CTask
//-------------------------------------------------------------

CTask::CTask()
{
	m_IsUsed = false;
	m_PriDraw = 0;
	m_PriUpdate = 0;
}


CTask::~CTask()
{
}


//-----------------------------------------------------
//					CTaskManager
//-----------------------------------------------------

CTaskManager::CTaskManager()
{
}

CTaskManager::~CTaskManager()
{
	ResetTask();
}

//タスクのリセット
void CTaskManager::ResetTask()
{
	std::list<CTask *>::iterator iter = m_TaskList.begin();

	while(iter != m_TaskList.end())
	{
		if(*iter != 0)
		{
			SAFE_DELETE(*iter);
		}
		++iter;
	}
	m_TaskList.clear();	
}


//タスクの追加
void CTaskManager::AddTask(CTask *pTask)
{
	m_TaskList.push_back(pTask);
	pTask->SetIsUsed(false);
}

//タスクの削除
void CTaskManager::DeleteTask(CTask *task)
{
	std::list<CTask *>::iterator iter = m_TaskList.begin();

	while(iter != m_TaskList.end())
	{
		if((*iter) == task)
		{
			SAFE_DELETE(*iter);
			m_TaskList.erase(iter);
			break;
		}
		++iter;
	}
}

//タスクの描画処理
void CTaskManager::TaskDraw()
{
	//描画優先度でタスクをソートする
	this->SortbyPriority(&CTaskManager::ComparePriDraw);

	std::list<CTask *>::iterator iter = m_TaskList.begin();

	while(iter != m_TaskList.end())
	{
		if(*iter != 0)
		{
			if((*iter)->m_IsUsed == true)
			{
				(*iter)->DoDraw();
			}
		}
		++iter;
	}
}

//タスクの更新処理
void CTaskManager::TaskUpdate()
{
	//更新優先度でタスクをソートする
	this->SortbyPriority(&CTaskManager::ComparePriUpdate);

	std::list<CTask *>::iterator iter = m_TaskList.begin();

	while(iter != m_TaskList.end())
	{
		if(*iter != 0)
		{
			if((*iter)->m_IsUsed == true)
			{
				(*iter)->DoUpdate();
			}
		}
		++iter;
	}
}

//更新優先度を比べる
bool CTaskManager::ComparePriUpdate(CTask *task1, CTask *task2)
{
	if(task1 != 0&& task2 != 0)
	{
		return (task1->m_PriUpdate > task2->m_PriUpdate);
	}
	else
		return false;
}

//描画優先度を比べる
bool CTaskManager::ComparePriDraw(CTask *task1, CTask *task2)
{
	if(task1 != 0&& task2 != 0)
	{
		return (task1->m_PriDraw > task2->m_PriDraw);
	}
	else
		return false;
}

//優先度でソートする
void CTaskManager::SortbyPriority(bool (CTaskManager::*Compare)(CTask *, CTask *))
{
	//TaskCompare = Compare;
	std::list<CTask *>::iterator Top = m_TaskList.begin();
	std::list<CTask *>::iterator Last = m_TaskList.end();

	int gap = std::distance(Top,Last);
	int dis = gap;
	bool IsSwap = 1;

	while(gap > 1 && IsSwap)
	{
		//1.3で割る
		gap = (gap * 10) / 13;
		//Gapが9か10なら収束するように11にする
                //これにより高速化が図れる
		if(gap == 9 || gap == 10)
		{
			gap = 11;
		}
		IsSwap = 0;
		//間隔Gapでバブルソート

		std::list<CTask *>::iterator Half = Top;
		std::advance(Half, dis - gap);

		for(std::list<CTask *>::iterator iter = Top;iter != Half;++iter)
		{
			if((this->*Compare)(*iter,*Half))
			{
				std::list<CTask *>::iterator tmp1 = iter;
				std::list<CTask *>::iterator tmp2 = Half;

				std::iter_swap(tmp1,tmp2);
				IsSwap = 1;
			}
		}

	}
	return;

}
