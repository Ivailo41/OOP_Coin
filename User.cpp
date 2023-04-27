#include "User.h"

void createUser(DataHolder &dataHolder)
{
    User newUser;

    std::cout << "Username: ";
    std::cin.getline(newUser.name, MAX_NAME_SIZE);

    if(std::cin.fail())
    {
        std::cerr << "Username can be max " << MAX_NAME_SIZE <<" characters! Terminating user creation!" << std::endl;
        return;
    }

    if(doesUserExist(dataHolder, newUser.name))
    {
        std::cerr << "User already exists! Terminating user creation!" << std::endl;
        return;
    }

    double moneyInvested;

    std::cout << "Money invested: ";
    std::cin >> moneyInvested;

    if(moneyInvested < 0 || std::cin.fail())
    {
        std::cerr << "Money invested must be a positive number! Terminating user creation!" << std::endl;
        clearCin();
        return;
    }

    double coins = moneyInvested * OOP_COIN_RATE;

    newUser.id = generateUserID(dataHolder);
    addUser(dataHolder, newUser);

    Transaction startTransaction;
    createTransaction(startTransaction, 0, newUser.id, coins, time(nullptr));
    addTransaction(startTransaction, dataHolder.transBlockList, dataHolder.blocksCount);

    clearCin();
}

void addUser(DataHolder &dataHolder, User user)
{
    User *newUserList = new(std::nothrow) User[dataHolder.usersCount + 1];
    if(!newUserList)
    {
        std::cerr << "Error allocating memory for new user list!" << std::endl;
        return;
    }

    for (int i = 0; i < dataHolder.usersCount; i++)
    {
        newUserList[i] = dataHolder.usersList[i];
    }

    newUserList[dataHolder.usersCount] = user;

    delete[] dataHolder.usersList;
    dataHolder.usersList = newUserList;
    dataHolder.usersCount++;
}

void removeUser(DataHolder &dataHolder, const char name[MAX_NAME_SIZE])
{
    if(!strcmp(name, "admin"))
    {
        std::cerr << "Cannot remove admin!" << std::endl;
        return;
    }

    unsigned pos = getUserPos(dataHolder, name);

    if(pos == -1)
    {
        std::cerr << "User not found!" << std::endl;
        return;
    }

    User *newUserList = new(std::nothrow) User[dataHolder.usersCount - 1];
    if(!newUserList)
    {
        std::cerr << "Error allocating memory for the function needs!" << std::endl;
        return;
    }

    double coins = getUserBalance(dataHolder, dataHolder.usersList[pos].id);

    Transaction endTransaction;
    createTransaction(endTransaction, dataHolder.usersList[pos].id, 0, coins, time(0));
    addTransaction(endTransaction, dataHolder.transBlockList, dataHolder.blocksCount);

    for(int i = 0; i < pos; i++)
    {
        newUserList[i] = dataHolder.usersList[i];
    }

    for(int i = pos; i < dataHolder.usersCount - 1; i++)
    {
        newUserList[i] = dataHolder.usersList[i + 1];
    }

    delete[] dataHolder.usersList;
    dataHolder.usersList = newUserList;
    dataHolder.usersCount--;
}

unsigned getUserPos(const DataHolder dataHolder, const char name[MAX_NAME_SIZE])
{
    for(int i = 0; i < dataHolder.usersCount; i++)
    {
        if(!strcmp(dataHolder.usersList[i].name, name))
        {
            return i;
        }
    }
    return -1;
}

unsigned generateUserID(const DataHolder dataHolder)
{
    unsigned id = dataHolder.usersList[dataHolder.usersCount - 1].id + 1;
    return id;
}

unsigned doesUserExist(const DataHolder dataHolder, const char name[MAX_NAME_SIZE]) //returns user id if user exists, 0 otherwise
{
    for(int i = 0; i < dataHolder.usersCount; i++)
    {
        if(!strcmp(dataHolder.usersList[i].name, name))
        {
            if(dataHolder.usersList[i].id == 0)
            {
                return 1;
            }
            return dataHolder.usersList[i].id;
        }
    }
    return 0;
}

double getUserBalance(const DataHolder dataHolder, const unsigned userId)
{
    double coins = 0;

    if(userId == 0)
    {
        return coins;
    }

    for(int i = 0; i < dataHolder.blocksCount; i++)
    {
        for(int j = 0; j < MAX_TRANSACTIONS_IN_BLOCK; j++)
        {
            if(dataHolder.transBlockList[i].transactions[j].sender == userId)
            {
                coins -= dataHolder.transBlockList[i].transactions[j].coins;
            }
            else if(dataHolder.transBlockList[i].transactions[j].receiver == userId)
            {
                coins += dataHolder.transBlockList[i].transactions[j].coins;
            }
        }
    }

    return coins;
}

User *sortUsersByBalance(const DataHolder dataHolder, User *sortedUserList)
{
    for (int i = 0; i < dataHolder.usersCount; i++)
    {
        sortedUserList[i] = dataHolder.usersList[i];
    }


    // Sort sortedUsers based on account balance
    for (int i = 0; i < dataHolder.usersCount - 1; i++)
    {
        for (int j = i + 1; j < dataHolder.usersCount; j++)
        {
            double balanceI = getUserBalance(dataHolder, sortedUserList[i].id);
            double balanceJ = getUserBalance(dataHolder, sortedUserList[j].id);
            if (balanceI < balanceJ)
            {
                std::swap(sortedUserList[i], sortedUserList[j]);
            }
        }
    }

    return sortedUserList;
}