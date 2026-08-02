#include "inventory.h"
#include "csvparser.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>

static int testsRun = 0;
static int testsPassed = 0;

void check(bool condition, const std::string& testName) {
    testsRun++;
    if (condition) {
        testsPassed++;
        std::cout << "[PASS] " << testName << "\n";
    } else {
        std::cout << "[FAIL] " << testName << "\n";
    }
}

// captures whatever a Pen's display() prints to stdout
std::string captureDisplay(const Pen& pen) {
    std::ostringstream capture;
    std::streambuf* oldBuf = std::cout.rdbuf(capture.rdbuf());
    pen.display();
    std::cout.rdbuf(oldBuf);
    return capture.str();
}

void testPenConstruction() {
    std::cout << "\n-- Pen construction --\n";

    Pen blank;
    check(blank.getBrand() == "", "default Pen has empty brand");
    check(blank.getQuantity() == 0, "default Pen has 0 quantity");

    Pen p("Lamy", "Safari", "M", 29.00, {"Black", "Red", "Blue"}, 10);
    check(p.getBrand() == "Lamy", "Pen ctor sets brand");
    check(p.getName() == "Safari", "Pen ctor sets name");
    check(p.getNibSize() == "M", "Pen ctor sets nibSize");
    check(p.getPrice() == 29.00, "Pen ctor sets price");
    check(p.getQuantity() == 10, "Pen ctor sets quantity");
    check(p.getColors().size() == 3, "Pen ctor sets colors");

    p.setPrice(35.00);
    p.setQuantity(4);
    check(p.getPrice() == 35.00, "setPrice updates price");
    check(p.getQuantity() == 4, "setQuantity updates quantity");
}

void testPenStock() {
    std::cout << "\n-- Pen stock operations --\n";

    Pen p("Lamy", "Safari", "M", 29.00, {"Black"}, 10);

    p.addStock(5);
    check(p.getQuantity() == 15, "addStock increases quantity");

    p.addStock(-3);
    check(p.getQuantity() == 15, "addStock rejects negative amount");

    bool sold = p.removeStock(5);
    check(sold == true, "removeStock returns true on success");
    check(p.getQuantity() == 10, "removeStock decreases quantity");

    bool overSold = p.removeStock(999);
    check(overSold == false, "removeStock rejects amount exceeding stock");
    check(p.getQuantity() == 10, "removeStock leaves quantity unchanged on failure");

    bool negSold = p.removeStock(-1);
    check(negSold == false, "removeStock rejects negative amount");
}

void testPenSerialization() {
    std::cout << "\n-- Pen display / toCSV --\n";

    Pen p("Pilot", "Custom 74", "EF", 120.00, {"Black", "Blue"}, 3);

    std::string shown = captureDisplay(p);
    check(shown.find("Pilot") != std::string::npos, "display() includes brand");
    check(shown.find("Custom 74") != std::string::npos, "display() includes name");

    std::string csv = p.toCSV();
    std::vector<std::string> fields;
    std::stringstream ss(csv);
    std::string field;
    while (std::getline(ss, field, ',')) fields.push_back(field);

    check(fields.size() == 6, "toCSV produces 6 comma-separated fields");
    if (fields.size() == 6) {
        check(fields[0] == "Pilot", "toCSV field 0 is brand");
        check(fields[1] == "Custom 74", "toCSV field 1 is name");
        check(fields[2] == "EF", "toCSV field 2 is nibSize");
        check(fields[4] == "Black|Blue", "toCSV joins colors with |");
        check(fields[5] == "3", "toCSV field 5 is quantity");
    }
}

void testLimitedEditionPen() {
    std::cout << "\n-- LimitedEditionPen --\n";

    LimitedEditionPen lep("Montblanc", "Meisterstuck 149", "M", 1050.00,
                           {"Black"}, 2, "045/500", 500);

    check(lep.getEditionNumber() == "045/500", "LimitedEditionPen ctor sets editionNumber");
    check(lep.getTotalProduced() == 500, "LimitedEditionPen ctor sets totalProduced");
    check(lep.getBrand() == "Montblanc", "LimitedEditionPen inherits base fields");

    // polymorphism check: calling display() through a Pen* should use the override
    Pen* asBase = &lep;
    std::string shown = captureDisplay(*asBase);
    check(shown.find("045/500") != std::string::npos,
          "display() through Pen* still uses LimitedEditionPen override");

    std::string csv = lep.toCSV();
    check(csv.find("045/500") != std::string::npos, "toCSV includes edition fields");
}

void testInventoryBasics() {
    std::cout << "\n-- Inventory add / find / size --\n";

    Inventory inv;
    check(inv.size() == 0, "new Inventory starts empty");

    inv.addPen(new Pen("Lamy", "Safari", "M", 29.00, {"Black"}, 10));
    inv.addPen(new Pen("Pilot", "Custom 74", "F", 120.00, {"Blue"}, 5));
    check(inv.size() == 2, "size() reflects added pens");
    check(inv.getAllPens().size() == 2, "getAllPens() returns all added pens");

    Pen* found = inv.findPen("Pilot", "Custom 74", "F");
    check(found != nullptr, "findPen locates an existing pen");
    if (found) check(found->getPrice() == 120.00, "findPen returns the correct pen");

    Pen* missing = inv.findPen("Nope", "Nothing", "X");
    check(missing == nullptr, "findPen returns nullptr for unknown pen");
}

void testInventoryRemove() {
    std::cout << "\n-- Inventory removePen --\n";

    Inventory inv;
    inv.addPen(new Pen("Lamy", "Safari", "M", 29.00, {"Black"}, 10));
    inv.addPen(new Pen("Pilot", "Custom 74", "F", 120.00, {"Blue"}, 5));

    bool removed = inv.removePen("Lamy", "Safari", "M");
    check(removed == true, "removePen returns true when pen exists");
    check(inv.size() == 1, "removePen shrinks inventory size");
    check(inv.findPen("Lamy", "Safari", "M") == nullptr, "removed pen can no longer be found");

    bool removedAgain = inv.removePen("Lamy", "Safari", "M");
    check(removedAgain == false, "removePen returns false for already-removed pen");
}

void testCsvLoaderHappyPath() {
    std::cout << "\n-- loadInventoryFromCSV(inventory.csv) --\n";

    Inventory inv = loadInventoryFromCSV("inventory.csv");
    check(inv.size() == 96, "loads all 96 rows from inventory.csv");

    std::vector<Pen*> pens = inv.getAllPens();
    check(!pens.empty(), "getAllPens() is non-empty after load");
    if (!pens.empty()) {
        Pen* first = pens[0];
        check(first->getBrand() == "Pilot", "first row brand parsed correctly");
        check(first->getName() == "Custom 74", "first row name parsed correctly");
        check(first->getNibSize() == "EF", "first row nibSize parsed correctly");
        check(first->getPrice() == 120.00, "first row price parsed correctly");
        check(first->getQuantity() == 3, "first row quantity parsed correctly");
        check(first->getColors().size() == 5, "first row colors split correctly");
    }
}

void testCsvLoaderEdgeCases() {
    std::cout << "\n-- loadInventoryFromCSV edge cases --\n";

    Inventory missing = loadInventoryFromCSV("does_not_exist.csv");
    check(missing.size() == 0, "missing file returns an empty Inventory instead of crashing");

    const std::string tempFile = "test_malformed_temp.csv";
    {
        std::ofstream out(tempFile);
        out << "Lamy,Safari,M,29.00,Black|Blue,10\n";
        out << "\n";                          // blank line, should be skipped
        out << "TooFewFields,29.00,10\n";      // malformed, should be skipped
        out << "Pilot,Custom 74,F,120.00,Black,5\n";
    }

    Inventory inv = loadInventoryFromCSV(tempFile);
    check(inv.size() == 2, "blank and malformed lines are skipped, valid rows still load");

    std::remove(tempFile.c_str());
}

int main() {
    testPenConstruction();
    testPenStock();
    testPenSerialization();
    testLimitedEditionPen();
    testInventoryBasics();
    testInventoryRemove();
    testCsvLoaderHappyPath();
    testCsvLoaderEdgeCases();

    std::cout << "\n========================================\n";
    std::cout << testsPassed << " / " << testsRun << " tests passed\n";

    return (testsPassed == testsRun) ? 0 : 1;
}
