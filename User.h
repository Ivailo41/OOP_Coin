#ifndef HOMEWORK1_USER_H
#define HOMEWORK1_USER_H

#include <cstring>
#include "TransactionBlock.h"
#include "Constants.h"

struct User
{
    unsigned id;
    char name[MAX_NAME_SIZE];
};

struct DataHolder
{
    User *usersList;
    unsigned usersCount;
    TransactionBlock *transBlockList;
    unsigned blocksCount;
};

void createUser(DataHolder &dataHolder);

void addUser(DataHolder &dataHolder, User user);

void removeUser(DataHolder &dataHolder, const char name[MAX_NAME_SIZE]);

unsigned getUserPos(const DataHolder dataHolder, const char name[MAX_NAME_SIZE]);

unsigned generateUserID(const DataHolder dataHolder);

unsigned doesUserExist(const DataHolder dataHolder, const char name[MAX_NAME_SIZE]);

double getUserBalance(const DataHolder dataHolder, const unsigned userId);

User *sortUsersByBalance(const DataHolder dataHolder, User *sortedUserList);

#endif //HOMEWORK1_USER_H
