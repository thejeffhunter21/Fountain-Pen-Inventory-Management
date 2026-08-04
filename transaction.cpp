#include "transaction.h"

#include "string"
#include "algorithm"
#include "iostream"


//transaction class
Transaction::Transaction() : id(0), type(TransactionType::NONE), brand(""), name(""), nibSize(""), quantity(0), price(0.0), oldPrice(0), timestamp("") {}

Transaction::Transaction(int id, TransactionType type, std::string brand, std::string name, std::string nibSize, int quantity, double price, double oldPrice, std::string timestamp)
            : id(id), type(type), brand(brand), name(name), nibSize(nibSize), quantity(quantity), price(price), oldPrice(oldPrice), timestamp(timestamp) {}

Transaction::~Transaction(){}

//enum to string
std::string transactionTypeToString(TransactionType type){
    switch(type){
        case TransactionType::NONE:
            return "None";
        case TransactionType::SALE:
            return "Sale";
        case TransactionType::RESTOCK:
            return "Restock";
        case TransactionType::RETURN:
            return "Return";
        case TransactionType::PRICE_CHANGE:
            return "Price change";
    }
    return "Invalid";
}