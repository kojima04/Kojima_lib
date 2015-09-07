#include "Common.h"
#include "BlockManager.h"

CBlockManager::CBlockManager()
{
	BlockNum = 30;
	for(int i = 0;i < BlockNum;++i)
		m_Blocks[i].SetUp();
	SetUp();
}

CBlockManager::~CBlockManager()
{
}

void CBlockManager::SetUp()
{
	for(int i = 0;i < BlockNum - 1;++i)
	{
		m_Blocks[i + 1].SetJumpPort(m_Blocks[i].GetJumpStand());
	}
}

void CBlockManager::Draw()
{
	for(int i = 0;i < BlockNum;++i)
		m_Blocks[i].Draw();
}

void CBlockManager::CalVeloHit(CChara * pChar)
{
	for(int i = 0;i < BlockNum;++i)
	{
		m_Blocks[i].CalVeloHit(pChar);
		m_Blocks[i].Acceleration();
		m_Blocks[i].Move();
	}
}
