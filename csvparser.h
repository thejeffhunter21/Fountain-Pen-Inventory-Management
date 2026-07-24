#ifndef csvparser_h
#define csvparser_h

#include <string>
#include<vector>
#include "inventory.h"

Inventory loadInventoryFromCSV(const std::string& filename);

void saveInventoryToCSV(const Inventory& inv, const std::string& filename);


#endif