#include "csvparser.h"
#include <fstream>
#include <sstream>
#include <iostream>

// splits a string on a delimiter into a vector of tokens
static std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> tokens; //store entire string
    std::stringstream ss(s);
    std::string token; //holds each data member
    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

Inventory loadInventoryFromCSV(const std::string& filename) {
    Inventory inv;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return inv;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::vector<std::string> fields = split(line, ',');
        if (fields.size() != 6) continue; // malformed row, skip it

        std::string brand = fields[0];
        std::string name = fields[1];
        std::string nibSize = fields[2];
        double price = std::stod(fields[3]);
        std::vector<std::string> colors = split(fields[4], '|');
        int quantity = std::stoi(fields[5]);

        Pen* pen = new Pen(brand, name, nibSize, price, colors, quantity);
        inv.addPen(pen);
    }

    return inv;
}

void saveInventoryToCSV(const Inventory& inv, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }

    for (Pen* pen : inv.getAllPens()) {
        file << pen->toCSV() << "\n";
    }
}