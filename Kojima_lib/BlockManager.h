#pragma once
#include "Chara.h"
#include "Block.h"

class CBlockManager
{
public:
	CBlockManager();
	~CBlockManager();
	void SetUp();
	void Draw();
	CBlock m_Blocks[30];
	int BlockNum;
	void CalVeloHit(CChara * pChar);
};
