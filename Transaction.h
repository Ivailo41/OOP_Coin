#ifndef HOMEWORK1_TRANSACTION_H
#define HOMEWORK1_TRANSACTION_H

struct Transaction
{
    unsigned sender;
    unsigned receiver;
    double coins;
    long long time;
};

void createTransaction(Transaction &transaction, const unsigned senderId, const unsigned receiverId, const double coins, const long long timeSent);


#endif //HOMEWORK1_TRANSACTION_H
