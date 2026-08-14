#include "transaction.h"

#include "string"
#include "algorithm"
#include "iostream"
#include "ctime"

//helper function for timestamping
std::string getTimeStamp() {
    //data type = current time
    std::time_t t = std::time(nullptr);\
    //tm is a struct for prenamed fields; pointer to point to one of those structs = converts address of t to appropriate struct
    std::tm* brokenDown = std::localtime(&t);
    char buffer[32];
    //arugment goes: destination, size limit as max, format, source of the data
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", brokenDown);
    return buffer;
}



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

// --- Serialization ---
std::string Transaction::ToCSV() const{
    std::string result = std::to_string(id) + "," + transactionTypeToString(type) + "," + brand + "," +
        name + "," + nibSize + "," + std::to_string(quantity) + "," + std::to_string(price) + "," +
        std::to_string(oldPrice) + "," + timestamp;
    return result;
}

int Transaction::getID() const {
    return id;
}
TransactionType Transaction::getType() const{
    return type;
}
std::string Transaction::getBrand() const {
    return brand;
}
std::string Transaction::getName() const{
    return name;
}
std::string Transaction::getNibSize() const{
    return nibSize;
}
int Transaction::getQuantity() const{
    return quantity;
}
double Transaction::getPrice() const{
    return price;
}
double Transaction::getOldPrice() const{
    return oldPrice;
}


//transactionlog functions
TransactionLog::~TransactionLog(){
    for (Transaction* p : transactions){
        delete p;
    }
}

bool TransactionLog::sellPen(Inventory& inv, const std::string& brand, const std::string& name, const std::string& nibSize, int quantity){
        Pen* found = inv.findPen(brand, name, nibSize);
        if (found == nullptr){
            std::cout << "Pen not found.";
            return false;
        }
        else{
            if (found->getQuantity() < quantity){
                std::cout << "Not enough stock. Can not sell \n";
                return false;
            }
            else{
                std::cout << "Sale for " << brand << " " << name << " " << nibSize <<" successful! \n"
                << "Total is: " << found->getPrice() << ".";
                //increment to get unique ID for each transaction type
                nextId += 1;
                found->removeStock(quantity);
                //instantiate object assigned to pointer data type because normally "new" requires a pointer, and transaction expects pointer 
                Transaction* newTransaction = new Transaction(nextId, TransactionType::SALE, brand, name, nibSize, quantity, found->getPrice(), found->getPrice(), getTimeStamp()); 
                transactions.push_back(newTransaction);
                return true;
            }
        }
    }

bool TransactionLog::restockPen(Inventory& inv, const std::string& brand, const std::string& name,const std::string& nibSize, int quantity){
    Pen* found = inv.findPen(brand,name,nibSize);
    if (found == nullptr){
        std::cout << "Pen not found.";
        return false;
    }
    else{
        //restocked X units of this pen on this date
        nextId += 1;
        found->addStock(quantity);
        std::cout << "Restocked " << quantity << " units of this pen on " << getTimeStamp() << ". \n";
        std::cout << "Stock for " << brand << " " << name << " " << nibSize << ": " << found->getQuantity();
        Transaction* newTransaction = new Transaction(nextId, TransactionType::RESTOCK, brand, name, nibSize, quantity, found->getPrice(), found->getPrice(), getTimeStamp()); 
        transactions.push_back(newTransaction);
        return true;
    }
}

bool TransactionLog::returnPen(Inventory& inv, int transactionID, int quantity){
    char choice;
    auto it = std::find_if(transactions.begin(), transactions.end(),
        //capture clause -- takes in variables from outside of scope; parameter that holds a transacation
        [transactionID](Transaction* t){
            //lambda function to return a bool
            return t->getID() == transactionID && t->getType() == TransactionType::SALE;
        });

    if (it == transactions.end()){
        std::cout << "Transaction not found. Return failed.\n";
        return false;
    }
    else {
        while (true){
        std::cout << "Transaction found. Proceed with return? Y/N \n";
        std::cin >> choice;
        if (choice == 'N' || choice == 'n'){
            std::cout << "Return cancelled.\n";
            return false;
        } 
        else if (choice == 'Y'  || choice == 'y'){
            Transaction* originalSale = *it; //dereference, actually assign the address. original sale now holds the pen's information
            Pen* found = inv.findPen(originalSale->getBrand(), originalSale->getName(), originalSale->getNibSize());
            if (found == nullptr){
                std::cout << "No pen found.\n";
                return false;
            }
            else if(quantity != originalSale->getQuantity()){
                std::cout << "Invalid quantity to return\n";
                return false;
            }
            else{
                nextId+=1;
                found->addStock(quantity); //whenever have time, work on partial returns 
                Transaction* newTransaction = new Transaction(nextId, TransactionType::RETURN, originalSale->getBrand(), originalSale->getName(), originalSale->getNibSize(), quantity, originalSale->getPrice(), originalSale->getPrice(), getTimeStamp()); 
                transactions.push_back(newTransaction);
                std::cout << "Return for " << originalSale->getBrand() << " " << originalSale->getName() << " " << originalSale->getNibSize() << " completed.\n";
                std::cout << "Total refunded: " << originalSale->getOldPrice() * quantity; 
                return true;
            }
        }
        else{
            std::cout << "Invalid choice. Please enter Y or N.\n";
        }
        }
    }
}

bool TransactionLog::changePrice(Inventory& inv, const std::string& brand, const std::string& name,const std::string& nibSize, double newPrice){
    Pen* found = inv.findPen(brand,name,nibSize);
    if (found == nullptr){
        std::cout << "Pen not found\n";
        return false; 
    }
    else{
        if (found->getPrice() == newPrice){
            std::cout << "Same price. No update made";
        }
        else{
            nextId+=1;
            double oldPrice = found->getPrice();
            found->setPrice(newPrice);
            Transaction* newTransaction = new Transaction(nextId,TransactionType::PRICE_CHANGE, brand, name, nibSize, found->getQuantity(), newPrice, oldPrice, getTimeStamp());
            transactions.push_back(newTransaction);
            return true;
        }
    }
}