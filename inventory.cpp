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
// Nothing to manually clean up here — no raw pointers as members.
// It's still virtual so that `delete somePenPtr;` calls the RIGHT
// destructor even when somePenPtr is a Pen* pointing at a LimitedEditionPen.

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

bool Pen::removeStock(int amount) {
    if (amount < 0){
        std::cout << "Can't be a negative amount. \n";
        return false;
    }

    if (amount > quantity){
        std::cout << "Not enough pens in stock. \n";
        return false;
    }

    if (amount <= quantity  ){
        quantity -= amount;
        std::cout << amount << " pens sold. Total number is now: " << quantity <<". \n";
        return true;
    }
}

// --- Display ---
void Pen::display() const {
    // TODO: print brand, name, nibSize, price, quantity, and colors
    // (colors is a vector<string> — you'll want a small loop or
    // std::accumulate-style join to print them nicely, e.g. "Black, Blue, Green")
}

// --- Serialization ---
std::string Pen::toCSV() const {
    // TODO: this is the reverse of what loadInventoryFromCSV does in
    // csvparser.cpp. Build a comma-separated line:
    //   brand,name,nibSize,price,colors,quantity
    // where colors need to be re-joined with '|' (they were split() on '|'
    // when loaded — write a small helper to do the opposite of split()).
    return "";
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
    // TODO: consider calling Pen::display() first to reuse the base
    // printing logic, then print editionNumber / totalProduced on top.
    // (This is a common inheritance pattern — extend, don't duplicate.)
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