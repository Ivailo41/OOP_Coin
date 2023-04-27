#ifndef HOMEWORK1_TRANSACTIONBLOCK_H
#define HOMEWORK1_TRANSACTIONBLOCK_H

#include "Transaction.h"
#include <iostream>
#include "Constants.h"

struct TransactionBlock
{
    unsigned id;
    unsigned prevBlockId;
    unsigned prevBlockHash;
    int validTransactions;
    Transaction transactions[MAX_TRANSACTIONS_IN_BLOCK];
};

void createBlock(TransactionBlock &block, const unsigned id, const unsigned prevBlockId, const unsigned prevBlockHash);

bool isBlockFull(const TransactionBlock block);

void addTransaction(const Transaction transaction, TransactionBlock *&blockList, unsigned &blockCount);

unsigned computeHash(const unsigned char *memory, int length);

bool isBlockValid(const TransactionBlock block, const unsigned blockHash);

bool VerifyTrans(const TransactionBlock *currentBlock, unsigned blockHash);

unsigned getBlockCoins(const TransactionBlock block);

TransactionBlock *sortBlocksBySize(const TransactionBlock *blockList, const unsigned blockCount);

void clearCin();

#endif //HOMEWORK1_TRANSACTIONBLOCK_H
