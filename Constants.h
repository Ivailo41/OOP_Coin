#ifndef HOMEWORK1_CONSTANTS_H
#define HOMEWORK1_CONSTANTS_H

const unsigned short MAX_COMMAND_SIZE = 128;
const double OOP_COIN_RATE = 420;
const unsigned short MAX_NAME_SIZE = 128;
const unsigned short MAX_TRANSACTIONS_IN_BLOCK = 16;

const char commandList[][MAX_COMMAND_SIZE] = {"create-user","remove-user","send-coins", "verify-transactions",
                                              "wealthiest-users", "biggest-blocks", "exit"};

#endif //HOMEWORK1_CONSTANTS_H
