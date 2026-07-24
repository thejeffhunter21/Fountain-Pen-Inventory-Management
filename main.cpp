#include "csvparser.h"
#include <iostream>

int main(){
    Inventory inv = loadInventoryFromCSV("inventory.csv");

    std::cout << "Loaded " << inv.size() << " pens from inventory.csv\n";
    std::cout << "----------------------------------------\n";

    for (Pen* pen : inv.getAllPens()) {
        pen->display();
    }

    return 0;
}