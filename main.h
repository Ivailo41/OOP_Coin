#ifndef HOMEWORK1_MAIN_H
#define HOMEWORK1_MAIN_H

#include "User.h"

enum class Commands
{
    UNKNOWN = -1,
    CREATE_USER,
    REMOVE_USER,
    SEND_COINS,
    VERIFY_TRANS,
    WEALTHIEST_USER,
    BIGGEST_BLOCKS,
    EXIT
};

int getObjectsCount(std::ifstream &usersFile, size_t objectSize);

void listenForCommands(DataHolder &dataHolder, char commandLine[MAX_COMMAND_SIZE]);

char loadData(DataHolder &dataHolder);

bool callLoadData(DataHolder &dataHolder);

void saveData(const DataHolder &dataHolder);

void printAllUsers(const DataHolder dataHolder);

void printAllBlocks(const DataHolder dataHolder);

void callCreateUser(DataHolder &dataHolder);

void callRemoveUser(DataHolder &dataHolder);

void callSendCoins(DataHolder &dataHolder);

void callVerifyTrans(const DataHolder dataHolder);

void callWealthiestPeople(const DataHolder dataHolder);

void saveWealthiestUsers(const DataHolder dataHolder, const User *sortedUsers, const unsigned peopleCount);

void callBiggestBlocks(const DataHolder dataHolder);

void SaveBiggestBlocks(const TransactionBlock *sortedBlocks, const unsigned blocksToShow);

unsigned digitCount(unsigned number);

char* toString(unsigned number);

#endif //HOMEWORK1_MAIN_H
