#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>
#include "inventory.h"   // needed since TransactionLog methods take an Inventory&

enum class TransactionType {
    NONE,
    SALE,
    RESTOCK,
    RETURN,
    PRICE_CHANGE
};

class Transaction {
private:
    int id;
    TransactionType type;
    std::string brand;
    std::string name;
    std::string nibSize;
    int quantity;        // meaningful for SALE/RESTOCK/RETURN; maybe 0 for PRICE_CHANGE
    double price;
    double oldPrice;        // for SALE: price it sold at. for PRICE_CHANGE: think— is one price field enough?
    std::string timestamp;  // human-readable, easy to write straight to CSV

public:
    Transaction();
    Transaction(int id, TransactionType type, std::string brand, std::string name,
                std::string nibSize, int quantity, double price, double oldPrice, std::string timestamp);

    // getters for every field above

    std::string ToCSV() const;  // formats this one record as a CSV line
};

class TransactionLog {
private:
    std::vector<Transaction*> transactions;  // every record ever created, in order
    int nextId;                              // next id to hand out, starts at 1

public:
    TransactionLog();
    ~TransactionLog();  // don't forget what this needs to do, given the vector holds pointers

    bool sellPen(Inventory& inv, const std::string& brand, const std::string& name,
                 const std::string& nibSize, int quantity);
    bool restockPen(Inventory& inv, const std::string& brand, const std::string& name,
                     const std::string& nibSize, int quantity);
    bool returnPen(Inventory& inv, const std::string& brand, const std::string& name,
                    const std::string& nibSize, int quantity);
    bool changePrice(Inventory& inv, const std::string& brand, const std::string& name,
                      const std::string& nibSize, double newPrice);

    std::vector<Transaction*> getAllTransactions() const;
    int size() const;
    void saveInventoryToCSV(const Inventory& inv, const std::string& filename);
};

#endif