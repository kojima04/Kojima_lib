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

//�^�X�N�̃��Z�b�g
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


//�^�X�N�̒ǉ�
void CTaskManager::AddTask(CTask *pTask)
{
	m_TaskList.push_back(pTask);
	pTask->SetIsUsed(false);
}

//�^�X�N�̍폜
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

//�^�X�N�̕`�揈��
void CTaskManager::TaskDraw()
{
	//�`��D��x�Ń^�X�N���\�[�g����
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

//�^�X�N�̍X�V����
void CTaskManager::TaskUpdate()
{
	//�X�V�D��x�Ń^�X�N���\�[�g����
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

//�X�V�D��x���ׂ�
bool CTaskManager::ComparePriUpdate(CTask *task1, CTask *task2)
{
	if(task1 != 0&& task2 != 0)
	{
		return (task1->m_PriUpdate > task2->m_PriUpdate);
	}
	else
		return false;
}

//�`��D��x���ׂ�
bool CTaskManager::ComparePriDraw(CTask *task1, CTask *task2)
{
	if(task1 != 0&& task2 != 0)
	{
		return (task1->m_PriDraw > task2->m_PriDraw);
	}
	else
		return false;
}

//�D��x�Ń\�[�g����
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
		//1.3�Ŋ���
		gap = (gap * 10) / 13;
		//Gap��9��10�Ȃ��������悤��11�ɂ���
                //����ɂ�荂�������}���
		if(gap == 9 || gap == 10)
		{
			gap = 11;
		}
		IsSwap = 0;
		//�ԊuGap�Ńo�u���\�[�g

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
