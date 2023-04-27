#include <fstream>
#include <cstring>
#include "main.h"

int main()
{
    DataHolder dataHolder;

    char commandLine[MAX_COMMAND_SIZE];

    if(!callLoadData(dataHolder))
    {
        return -1;
    }

    if(!doesUserExist(dataHolder, "admin"))
    {
        User admin;

        strcpy(admin.name, "admin");
        admin.id = 0;

        addUser(dataHolder, admin);
    }

    listenForCommands(dataHolder, commandLine);

    delete[] dataHolder.usersList;
    delete[] dataHolder.transBlockList;

    return 0;
}

int getObjectsCount(std::fstream &usersFile, size_t objectSize)
{
    int usersCount = 0;
    usersFile.seekg(0, std::ios::end);
    size_t fileSize = usersFile.tellg();
    usersFile.seekg(0, std::ios::beg);

    if(fileSize % objectSize == 0)
    {
        usersCount = fileSize / objectSize;
        return usersCount;
    }
    return -1;

}

char loadData(DataHolder &dataHolder) //returns 0 if data is loaded successfully.
{
    std::fstream usersFile("users.dat", std::ios::in | std::ios::binary);

    if(!usersFile.is_open())
    {
        usersFile.open("users.dat", std::ios::out | std::ios::binary);
        if(!usersFile.is_open())
        {
            return 1;
        }
        usersFile.close();

        usersFile.open("users.dat", std::ios::in | std::ios::binary);
        if(!usersFile.is_open())
        {
            return 1;
        }
    }

    dataHolder.usersCount = getObjectsCount(usersFile, sizeof(User));

    if(dataHolder.usersCount == -1)
    {
        usersFile.close();
        return 2;
    }

    dataHolder.usersList = new User[dataHolder.usersCount];

    for (int i = 0; i < dataHolder.usersCount; i++)
    {
        usersFile.read(reinterpret_cast<char*>(&dataHolder.usersList[i]), sizeof(User));
        if(!usersFile.good())
        {
            delete[] dataHolder.usersList;
            usersFile.close();
            return 3;
        }
    }
    usersFile.close();
    //CLOSE USERS FILE AND PROCEED WITH THE BLOCKS FILE

    std::fstream transBlocksFile("blocks.dat", std::ios::in | std::ios::binary);

    if(!transBlocksFile.is_open())
    {
        transBlocksFile.open("blocks.dat", std::ios::out | std::ios::binary);
        if(!transBlocksFile.is_open())
        {
            delete[] dataHolder.usersList;
            return 4;
        }
        transBlocksFile.close();

        transBlocksFile.open("blocks.dat", std::ios::in | std::ios::binary);
        if(!transBlocksFile.is_open())
        {
            delete[] dataHolder.usersList;
            return 4;
        }
    }

    dataHolder.blocksCount = getObjectsCount(transBlocksFile, sizeof(TransactionBlock));
    dataHolder.transBlockList = new TransactionBlock[dataHolder.blocksCount];

    for (int i = 0; i < dataHolder.blocksCount; i++)
    {
        transBlocksFile.read(reinterpret_cast<char*>(&dataHolder.transBlockList[i]), sizeof(TransactionBlock));
        if(!transBlocksFile.good())
        {
            delete[] dataHolder.transBlockList;
            transBlocksFile.close();
            return 5;
        }
    }
    transBlocksFile.close();

    return 0;
}

void saveData(const DataHolder &dataHolder)
{
    std::ofstream usersFile("users.dat", std::ios::binary);
    if(!usersFile.is_open())
    {
        std::cerr << "Can't open Users file for writing, changes won't save!";
        usersFile.close();
        return;
    }

    for (int i = 0; i < dataHolder.usersCount; i++)
    {
        usersFile.write(reinterpret_cast<const char*>(&dataHolder.usersList[i]), sizeof(User));
        if(!usersFile.good())
        {
            std::cerr << "Can't write to the Users file, changes won't save!";
            usersFile.close();
            return;
        }
    }
    usersFile.close();
    //CLOSE USERS FILE AND PROCEED WITH THE BLOCKS FILE

    std::ofstream blocksFile("blocks.dat", std::ios::binary);
    if(!blocksFile.is_open())
    {
        std::cerr << "Can't open Blocks file for writing, changes won't save!";
        blocksFile.close();
        return;
    }

    for (int i = 0; i < dataHolder.blocksCount; i++)
    {
        blocksFile.write(reinterpret_cast<const char*>(&dataHolder.transBlockList[i]), sizeof(TransactionBlock));
        if(!blocksFile.good())
        {
            std::cerr << "Can't write to the Blocks file, changes won't save!";
            blocksFile.close();
            return;
        }
    }
    blocksFile.close();
}

void listenForCommands(DataHolder &dataHolder, char commandLine[MAX_COMMAND_SIZE])
{
    while (true)
    {
        std::cin.getline(commandLine, MAX_COMMAND_SIZE);

        if(!strcmp(commandLine,commandList[(int)Commands::CREATE_USER]))
        {
            callCreateUser(dataHolder);
        }
        else if(!strcmp(commandLine,commandList[(int)Commands::REMOVE_USER]))
        {
            callRemoveUser(dataHolder);
        }
        else if(!strcmp(commandLine,commandList[(int)Commands::SEND_COINS]))
        {
            callSendCoins(dataHolder);
        }
        else if(!strcmp(commandLine,commandList[(int)Commands::VERIFY_TRANS]))
        {
            callVerifyTrans(dataHolder);
            printAllUsers(dataHolder);
        }
        else if(!strcmp(commandLine,commandList[(int)Commands::WEALTHIEST_USER]))
        {
            callWealthiestPeople(dataHolder);
        }
        else if(!strcmp(commandLine,commandList[(int)Commands::BIGGEST_BLOCKS]))
        {
            callBiggestBlocks(dataHolder);
            //printAllBlocks(dataHolder);
        }
        else if(!strcmp(commandLine,commandList[(int)Commands::EXIT]))
        {
            saveData(dataHolder);
            break;
        }
        else
        {
            std::cout << "Invalid command!" << '\n';
        }
    }
}

void printAllUsers(const DataHolder dataHolder)
{
    for(int i = 0; i < dataHolder.usersCount; i++)
    {
        std::cout << dataHolder.usersList[i].name << " : " << dataHolder.usersList[i].id << " : " << getUserBalance(dataHolder, dataHolder.usersList[i].id) <<std::endl;
    }
} //DEBUG FUNCTION

void printAllBlocks(const DataHolder dataHolder)
{
    for(int i = 0; i < dataHolder.blocksCount; i++)
    {
        std::cout << dataHolder.transBlockList[i].id << " : " << dataHolder.transBlockList[i].prevBlockId << " : " << dataHolder.transBlockList[i].prevBlockHash << " : " << dataHolder.transBlockList[i].validTransactions << std::endl;
    }
} //DEBUG FUNCTION

bool callLoadData(DataHolder &dataHolder) //returns true if data is loaded successfully
{
    switch (loadData(dataHolder))
    {
        case 0:
            return true;
        case 1:
            std::cerr << "Cant open Users file for reading" << std::endl;
            return false;
        case 2:
            std::cerr << "Users file is corrupted!" << std::endl;
            return false;
        case 3:
            std::cerr << "Reading operation in Users file failed!" << std::endl;
            return false;
        case 4:
            std::cerr << "Cant open Blocks file for reading" << std::endl;
            return false;
        case 5:
            std::cerr << "Reading operation in Blocks file failed!" << std::endl;
            return false;
        default:
            std::cerr << "Unknown error!" << std::endl;
            return false;
    }
}

void callCreateUser(DataHolder &dataHolder)
{
    createUser(dataHolder);
}

void callRemoveUser(DataHolder &dataHolder)
{
    char name[MAX_NAME_SIZE];

    std::cout << "Enter name: ";
    std::cin.getline(name, MAX_NAME_SIZE);

    removeUser(dataHolder, name);
}

void callSendCoins(DataHolder &dataHolder)
{
    char senderName[MAX_NAME_SIZE];

    std::cout << "Sender name: ";
    std::cin >> senderName;

    unsigned senderId = doesUserExist(dataHolder, senderName);

    if(!senderId)
    {
        std::cerr << "User doesn't exist! Terminating the transaction!" << std::endl;
        clearCin();
        return;
    }

    char receiverName[MAX_NAME_SIZE];

    std::cout << "Receiver name: ";
    std::cin >> receiverName;

    unsigned receiverId = doesUserExist(dataHolder, receiverName);

    if(!receiverId)
    {
        std::cerr << "User doesn't exist! Terminating the transaction!" << std::endl;
        clearCin();
        return;
    }

    double amount;
    std::cout << "Amount to send: ";
    std::cin >> amount;

    if(getUserBalance(dataHolder, senderId) < amount)
    {
        std::cerr << "The sender has not enough money! Terminating the transaction!" << std::endl;
        clearCin();
        return;
    }

    Transaction transaction;
    createTransaction(transaction, senderId, receiverId, amount, time(nullptr));
    addTransaction(transaction, dataHolder.transBlockList, dataHolder.blocksCount);

    clearCin();
}

void callVerifyTrans(const DataHolder dataHolder)
{
    if(dataHolder.blocksCount <= 0)
    {
        std::cout << "The blockchain is empty!" << std::endl;
        return;
    }

    TransactionBlock *lastBlock = dataHolder.transBlockList + dataHolder.blocksCount - 1;
    unsigned lastBlockHash = lastBlock->prevBlockHash;

    if(!VerifyTrans(lastBlock, lastBlockHash))
    {
        std::cout << "The blockchain is invalid!" << std::endl;

    }
    std::cout << "The blockchain is valid!" << std::endl;
}

void callWealthiestPeople(const DataHolder dataHolder)
{
    unsigned peopleCount;
    std::cout << "How many people to show: ";
    std::cin >> peopleCount;

    if(peopleCount > dataHolder.usersCount || std::cin.fail())
    {
        std::cerr << "Invalid number of people!" << std::endl;
        clearCin();
        return;
    }

    User *sortedUsers = new(std::nothrow) User[dataHolder.usersCount];
    if(!sortedUsers)
    {
        std::cerr << "Memory allocation failed!" << std::endl;
        return;
    }

    sortedUsers = sortUsersByBalance(dataHolder, sortedUsers);

    //Print top n wealthiest users
    for (int i = 0; i < peopleCount && i < dataHolder.usersCount; i++)
    {
        std::cout << sortedUsers[i].name << " : " << getUserBalance(dataHolder, sortedUsers[i].id) << std::endl;
    }

    saveWealthiestUsers(dataHolder, sortedUsers, peopleCount);

    clearCin();
    delete[] sortedUsers;
}

void saveWealthiestUsers(const DataHolder dataHolder, const User *sortedUsers, const unsigned peopleCount)
{
    std::ofstream file;
    unsigned fileNameSize = strlen(commandList[(int)Commands::WEALTHIEST_USER]) + digitCount(time(nullptr)) + strlen(".txt") + 1;

    char *fileName = new(std::nothrow) char[fileNameSize];
    if(!fileName)
    {
        std::cerr << "Memory allocation failed!" << std::endl;
        return;
    }

    char* timeString = toString(time(nullptr));
    if(!timeString)
    {
        std::cerr << "Memory allocation failed!" << std::endl;
        delete[] fileName;
        return;
    }

    strcpy(fileName, commandList[(int)Commands::WEALTHIEST_USER]);
    strcat(fileName, timeString);
    strcat(fileName, ".txt");

    file.open(fileName, std::ios::out);

    if(!file.is_open())
    {
        std::cerr << "Cant open file for writing!" << std::endl;
        return;
    }

    for (int i = 0; i < peopleCount; i++)
    {
        file << sortedUsers[i].name << " : " << getUserBalance(dataHolder, sortedUsers[i].id) << std::endl;
    }

    file.close();
    delete[] fileName;
    delete[] timeString;
}

void callBiggestBlocks(const DataHolder dataHolder)
{
    unsigned blocksToShow;
    std::cout << "How many blocks to show: ";
    std::cin >> blocksToShow;

    if(blocksToShow > dataHolder.blocksCount || std::cin.fail())
    {
        std::cerr << "Invalid number of blocks!" << std::endl;
        clearCin();
        return;
    }

    TransactionBlock *sortedBlocks = sortBlocksBySize(dataHolder.transBlockList, dataHolder.blocksCount);
    if(!sortedBlocks)
    {
        std::cerr << "Memory allocation failed!" << std::endl;
        return;
    }

    //Print top n biggest blocks
    for (int i = 0; i < blocksToShow; i++)
    {
        std::cout << "id " << sortedBlocks[i].id << " : Coins " << getBlockCoins(sortedBlocks[i]) << std::endl;
    }

    SaveBiggestBlocks(sortedBlocks, blocksToShow);

    clearCin();
    delete[] sortedBlocks;
}

void SaveBiggestBlocks(const TransactionBlock *sortedBlocks, const unsigned blocksToShow)
{
    std::ofstream file;
    unsigned fileNameSize = strlen(commandList[(int)Commands::BIGGEST_BLOCKS]) + digitCount(time(nullptr)) + strlen(".txt") + 1;

    char *fileName = new(std::nothrow) char[fileNameSize];
    if(!fileName)
    {
        std::cerr << "Memory allocation failed!" << std::endl;
        return;
    }

    char* timeString = toString(time(nullptr));
    if(!timeString)
    {
        std::cerr << "Memory allocation failed!" << std::endl;
        delete[] fileName;
        return;
    }

    strcpy(fileName, commandList[(int)Commands::BIGGEST_BLOCKS]);
    strcat(fileName, timeString);
    strcat(fileName, ".txt");

    file.open(fileName, std::ios::out);

    if(!file.is_open())
    {
        std::cerr << "Cant open file for writing!" << std::endl;
        return;
    }

    for (int i = 0; i < blocksToShow; i++)
    {
        file << "id " << sortedBlocks[i].id << " : Coins " << getBlockCoins(sortedBlocks[i]) << std::endl;
    }

    file.close();
    delete[] fileName;
    delete[] timeString;
}

unsigned digitCount(unsigned number)
{
    unsigned count = 0;
    while(number)
    {
        number /= 10;
        count++;
    }
    return count;
}

char* toString(unsigned number)
{
    unsigned count = digitCount(number);
    char *str = new(std::nothrow) char[count + 1];
    if(!str)
    {
        std::cerr << "Memory allocation failed!" << std::endl;
        return nullptr;
    }

    for (int i = count - 1; i >= 0; i--)
    {
        str[i] = number % 10 + '0';
        number /= 10;
    }
    str[count] = '\0';

    return str;
}