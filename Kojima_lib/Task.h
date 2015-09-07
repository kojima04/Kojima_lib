#pragma once

#include <algorithm>

class CTaskManager;

//�^�X�N�̃N���X
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

	int		m_PriDraw;		// �`��D��x
	int		m_PriUpdate;	// �X�V�D��x
	bool	m_IsUsed;		// ���ݎg�p�����ǂ���
};

//�^�X�N�̃e���v���[�g�N���X
template <class T> class TTask :public CTask
{
protected:
	void	(T::*m_pFuncUpdate)();		//�X�V�֐��|�C���^
	void	(T::*m_pFuncDraw)();		//�`��֐��|�C���^

public:

	//�X�V�֐��|�C���^�̃Z�b�g
	void	SetpFuncUpdate(void (T::*func)())
	{
		m_pFuncUpdate = func;
	}

	//�`��֐��|�C���^�̃Z�b�g
	void	SetpFuncDraw(void (T::*func)())
	{
		m_pFuncDraw = func;
	}

	//�X�V
	void	DoUpdate()
	{
		if(m_pFuncUpdate != 0)
		{
			(((T*)this)->*m_pFuncUpdate)();
		}
	}

	//�`��
	void	DoDraw()
	{
		if(m_pFuncDraw != 0)
		{
			(((T*)this)->*m_pFuncDraw)();
		}
	}

};

//�^�X�N�}�l�[�W��
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
	std::list<CTask *>		m_TaskList;		// �^�X�N�̃��X�g
	bool (CTaskManager::*TaskCompare)(CTask *, CTask *); 

};

