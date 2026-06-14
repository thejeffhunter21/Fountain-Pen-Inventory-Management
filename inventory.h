#ifndef INVENTORY_H   // "if INVENTORY_H is NOT defined"
#define INVENTORY_H   // define it now
#include <string>
#include <vector>
class Pen{
    protected:
        std::string brand;
        std::string name;
        std::string nibSize;
        double price;
        std::vector<std::string> colors;
        int quantity;
    public:
        Pen(); //constructor
        Pen(std::string brand, std::string name, std::string nibSize,
            double price, std::vector<std::string> colors, int quantity); //constructor overload
        virtual ~Pen(); //virtual to prevent memory leaks

        // Getters
        std::string getBrand() const;
        std::string getName() const;
        std::string getNibSize() const;
        double getPrice() const;
        std::vector<std::string> getColors() const;
        int getQuantity() const;
 
        // Setters
        void setPrice(double newPrice);
        void setQuantity(int newQuantity);

        // Stock operations
        void addStock(int amount);
        bool removeStock(int amount);

        // Display
        virtual void display() const;
        // Serialization helpers (for csvparser to use)
        virtual std::string toCSV() const;
};

class LimitedEditionPen : public Pen {
    private:
        std::string editionNumber;
        int totalProduced;

    public:
        LimitedEditionPen();
        LimitedEditionPen(std::string brand, std::string name, std::string nibSize,
            double price, std::vector<std::string> colors, int quantity,
        std::string editionNumber, int totalProduced);

        std::string getEditionNumber() const;
        int getTotalProduced() const;

        void display() const override;
        std::string toCSV() const override;
};

class Inventory {
    private:
        std::vector<Pen*> pens;

    public:
        Inventory();
        ~Inventory();
        void addPen(Pen* pen);
        bool removePen(const std::string& brand, const std::string& name, const std::string& nibSize);
        Pen* findPen(const std::string& brand, const std::string& name, const std::string& nibSize);
        std::vector<Pen*> getAllPens() const;
        int size() const;
};

            
            




#endif