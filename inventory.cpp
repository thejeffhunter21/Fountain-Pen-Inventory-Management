#include "inventory.h"
#include <iostream>
#include <algorithm>

// ---------------------------------------------------------
// Pen
// ---------------------------------------------------------

//default constructor for pens w/o any values
Pen::Pen() : brand(""), name(""), nibSize(""), price(0.0), quantity(0) {}


Pen::Pen(std::string brand, std::string name, std::string nibSize,
         double price, std::vector<std::string> colors, int quantity)
    : brand(brand), name(name), nibSize(nibSize), price(price),
      colors(colors), quantity(quantity) {}

Pen::~Pen() {}

// --- Getters ---
std::string Pen::getBrand() const { return brand; }
std::string Pen::getName() const { return name; }
std::string Pen::getNibSize() const { return nibSize; }
double Pen::getPrice() const { return price; }
std::vector<std::string> Pen::getColors() const { return colors; }
int Pen::getQuantity() const { return quantity; }

// --- Setters ---
void Pen::setPrice(double newPrice) { price = newPrice; }
void Pen::setQuantity(int newQuantity) { quantity = newQuantity; }

// --- Stock operations ---
//no necessary for bool
void Pen::addStock(int amount) {
    //error bound checking
    if (amount < 0){
        std::cout << "Can't be a negative amount. \n";
        return;
    }

    //update
    quantity += amount;
    std::cout << amount << " pens added. Total number is now: " << quantity <<". \n";
}

//return bool to verify succes/failure
bool Pen::removeStock(int amount) {
    //bounds checking
    if (amount < 0){
        std::cout << "Can't be a negative amount. \n";
        return false;
    }

    if (amount > quantity){
        std::cout << "Not enough pens in stock. \n";
        return false;
    }

    //update
    if (amount <= quantity  ){
        quantity -= amount;
        std::cout << amount << " pens sold. Total number is now: " << quantity <<". \n";
        return true;
    }
}

// --- Display ---
void Pen::display() const {
    std::cout << brand << " " << name << " " << nibSize << " || " << "$" << price << " || Stock: " << quantity << " || " << "Color: ";
    for (const std::string& color : colors) {
        std::cout << color;
    if (&color != &colors.back()) {
        std::cout << ", ";
    }
    }  

    std::cout << "\n";
    //Pilot 823 Custom <M> || $10.99 || Stock: 12 || Color: Blue, Grey, Green, ...
}

// --- Serialization ---
std::string Pen::toCSV() const {
    std::string colorsCSV;
    for(const std::string& color : colors){
        colorsCSV += color;
        if (&color != &colors.back()){
            colorsCSV += "|";
        }
    }
    //-------------------------
    //FIX THE FORMATTING OF "std::to_string()" LATER!!!!!!! 
    //-------------------------
    std::string result = brand + "," + name + "," + nibSize + "," + std::to_string(price) + "," + colorsCSV + "," + std::to_string(quantity);
    return result;
}

// ---------------------------------------------------------
// LimitedEditionPen
// ---------------------------------------------------------

LimitedEditionPen::LimitedEditionPen() : Pen(), editionNumber(""), totalProduced(0) {}

LimitedEditionPen::LimitedEditionPen(std::string brand, std::string name, std::string nibSize,
    double price, std::vector<std::string> colors, int quantity,
    std::string editionNumber, int totalProduced)
    : Pen(brand, name, nibSize, price, colors, quantity),
      editionNumber(editionNumber), totalProduced(totalProduced) {}

std::string LimitedEditionPen::getEditionNumber() const { return editionNumber; }
int LimitedEditionPen::getTotalProduced() const { return totalProduced; }

void LimitedEditionPen::display() const {
    std::cout << brand << " " << name << " " << nibSize << " || "
              << "$" << price << " || Stock: " << quantity << " || Color: ";

    for (const std::string& color : colors) {
        std::cout << color;
    if (&color != &colors.back()) {
        std::cout << ", ";
    }
    }  

    std::cout << " || Edition: " << editionNumber << "/" << totalProduced << "\n";
}

std::string LimitedEditionPen::toCSV() const {
    // TODO: same idea — you could call Pen::toCSV() and append
    // ",editionNumber,totalProduced", or write it out fully.
    // Keep in mind: if your parser only expects 6 fields per row today,
    // you'll eventually need a way to distinguish LimitedEditionPen rows
    // when saving/loading. Worth thinking about now, even if you don't
    // solve it yet.
    return "";
}

// ---------------------------------------------------------
// Inventory
// ---------------------------------------------------------

Inventory::Inventory() {}

Inventory::~Inventory() {
    // TODO: `pens` holds raw Pen* pointers that Inventory owns.
    // Loop through and `delete` each one, or you'll leak memory every
    // time an Inventory object is destroyed.
}

void Inventory::addPen(Pen* pen) {
    pens.push_back(pen);
}

Pen* Inventory::findPen(const std::string& brand, const std::string& name, const std::string& nibSize) {
    // TODO: loop through `pens`, compare each pen's brand/name/nibSize
    // against the arguments (use the getters you already wrote).
    // Return the matching Pen* if found, or nullptr if not found.
    return nullptr;
}

bool Inventory::removePen(const std::string& brand, const std::string& name, const std::string& nibSize) {
    // TODO:
    // 1. Find the matching pen (you can reuse findPen, or search directly).
    // 2. If found: delete the object, THEN erase its entry from the
    //    `pens` vector (deleting doesn't remove it from the vector —
    //    that leaves a dangling pointer sitting in `pens` until erased).
    // 3. Return true if something was removed, false otherwise.
    return false;
}

std::vector<Pen*> Inventory::getAllPens() const {
    return pens;
}

int Inventory::size() const {
    return pens.size();
}