#include "TransactionBlock.h"

bool isBlockFull(const TransactionBlock block)
{
    return block.validTransactions == MAX_TRANSACTIONS_IN_BLOCK;
}

void createBlock(TransactionBlock &block, unsigned id, unsigned prevBlockId, unsigned prevBlockHash)
{
    block.id = id;
    block.prevBlockId = prevBlockId;
    block.prevBlockHash = prevBlockHash;
    block.validTransactions = 0;
}

void addTransaction(const Transaction transaction, TransactionBlock *&blockList, unsigned &blockCount)
{
    if (blockCount == 0)
    {
        blockList = new(std::nothrow) TransactionBlock;
        if(!blockList)
        {
            std::cerr << "Error allocating memory for new block list!" << std::endl;
            return;
        }
        createBlock(*blockList, 0, 0, 0);
        blockCount++;
    }

    if (isBlockFull(blockList[blockCount - 1]))
    {
        TransactionBlock *newBlockList = new(std::nothrow) TransactionBlock[blockCount + 1];
        if(!newBlockList)
        {
            std::cerr << "Error allocating memory for new block list!" << std::endl;
            return;
        }

        for(int i = 0; i < blockCount; i++)
        {
            newBlockList[i] = blockList[i];
        }

        delete[] blockList;
        blockList = newBlockList;

        unsigned prevBlockHash = computeHash((unsigned char *)&blockList[blockCount - 1], sizeof(TransactionBlock));
        createBlock(blockList[blockCount], blockCount, blockCount - 1, prevBlockHash);
        blockCount++;
    }

    blockList[blockCount - 1].transactions[blockList[blockCount - 1].validTransactions] = transaction;
    blockList[blockCount - 1].validTransactions++;
}

unsigned computeHash(const unsigned char *memory, int length)
{
    unsigned hash = 0xbeaf;

    for (int c = 0; c < length; c++) {
        hash += memory[c];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

bool isBlockValid(const TransactionBlock block, const unsigned blockHash)
{
    return blockHash == block.prevBlockHash;
}

bool VerifyTrans(const TransactionBlock *currentBlock, unsigned blockHash)
{
    if(currentBlock->id == 0)
    {
        return true;
    }

    if(!isBlockValid(*currentBlock, blockHash))
    {
        return false;
    }

    currentBlock--;
    blockHash = currentBlock->prevBlockHash;

    return VerifyTrans(currentBlock, blockHash);

}

unsigned getBlockCoins(const TransactionBlock block)
{
    unsigned coins = 0;
    for(int i = 0; i < block.validTransactions; i++)
    {
        coins += block.transactions[i].coins;
    }

    return coins;
}

TransactionBlock *sortBlocksBySize(const TransactionBlock *blockList, const unsigned blockCount)
{
    TransactionBlock *sortedList = new(std::nothrow) TransactionBlock[blockCount];
    if(!sortedList)
    {
        return nullptr;
    }

    for(int i = 0; i < blockCount; i++)
    {
        sortedList[i] = blockList[i];
    }

    for(int i = 0; i < blockCount - 1; i++)
    {
        for(int j = i + 1; j < blockCount; j++)
        {
            unsigned coinsI = getBlockCoins(sortedList[i]);
            unsigned coinsJ = getBlockCoins(sortedList[j]);
            if(coinsI < coinsJ)
            {
                std::swap(sortedList[i], sortedList[j]);
            }
        }
    }

    return sortedList;
}

void clearCin()
{
    std::cin.clear();
    std::cin.ignore(MAX_COMMAND_SIZE, '\n');
}
