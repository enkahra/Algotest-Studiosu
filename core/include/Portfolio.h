#pragma once

#include "CommonTypes.h"
#include <map>
#include <string>
#include <iostream>

class Portfolio {
private:
    double m_balance; // Portföy bakiyesi
    std::map<std::string, long> m_stocks; // Hisse senedi sembolü ve miktarını tutan harita

public:
    Portfolio(double initialBalance = 0.0) : m_balance(initialBalance) {}
    
    bool processTrade(const Trade& trade); // void yerine bool döndürsün
    double getBalance() const { return m_balance; }
    void displayHoldings() const; // Portföy durumunu göstermek için
    long getStockQuantity(const std::string& symbol) const; // Belirli bir hisse miktarını almak için
};