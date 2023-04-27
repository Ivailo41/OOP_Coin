#include "Transaction.h"

void createTransaction(Transaction &transaction, const unsigned senderId, const unsigned receiverId, const double coins, const long long timeSent)
{
    transaction.sender = senderId;
    transaction.receiver = receiverId;
    transaction.coins = coins;
    transaction.time = timeSent;
}