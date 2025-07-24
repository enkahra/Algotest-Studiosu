#include "Portfolio.h"

bool Portfolio::processTrade(const Trade& trade) {
    static int tradeCount = 0;
    tradeCount++;
    
    std::cout << "\n--- TRADE #" << tradeCount << " ---" << std::endl;
    
    if (trade.type == TradeType::BUY) {
        double totalCost = trade.quantity * trade.price;
        if (m_balance >= totalCost) {
            m_balance -= totalCost;
            std::cout.precision(10);
            m_stocks[trade.stockSymbol] += trade.quantity;
            m_lastPrices[trade.stockSymbol] = trade.price; // Store latest price
            std::cout << "BUY: " << trade.quantity << " " << trade.stockSymbol 
                      << " @ " << trade.price << " TL. Total cost: " << totalCost << " TL" << std::endl;
            std::cout << "New balance: " << m_balance << " TL, " 
                      << trade.stockSymbol << " holdings: " << m_stocks[trade.stockSymbol] << " shares" << std::endl;
            return true;
        } else {
            std::cout << "BUY FAILED - Insufficient balance! Required: " << totalCost 
                      << " TL, Available: " << m_balance << " TL" << std::endl;
            return false;
        }
    } 
    else if (trade.type == TradeType::SELL) {
        if (m_stocks[trade.stockSymbol] >= trade.quantity) {
            double totalRevenue = trade.quantity * trade.price;
            m_balance += totalRevenue;
            m_stocks[trade.stockSymbol] -= trade.quantity;
            m_lastPrices[trade.stockSymbol] = trade.price; // Store latest price
            std::cout << "SELL: " << trade.quantity << " " << trade.stockSymbol 
                      << " @ " << trade.price << " TL. Total revenue: " << totalRevenue << " TL" << std::endl;
            std::cout << "New balance: " << m_balance << " TL, " 
                      << trade.stockSymbol << " holdings: " << m_stocks[trade.stockSymbol] << " shares" << std::endl;
            return true;
        } else {
            std::cout << "SELL FAILED - Insufficient stock! " << trade.stockSymbol << " available: " 
                      << m_stocks[trade.stockSymbol] << ", requested: " << trade.quantity << std::endl;
            return false;
        }
    }
    return false;
}

void Portfolio::displayHoldings() const {
    std::cout << "\n=== PORTFOLIO STATUS ===" << std::endl;
    std::cout << "Cash balance: " << m_balance << " TL" << std::endl;
    
    // Calculate total portfolio value
    double totalStockValue = 0.0;
    std::cout << "Stock holdings:" << std::endl;
    
    bool hasStocks = false;
    for (const auto& stock : m_stocks) {
        std::cout << "  " << stock.first << ": " << stock.second << " shares";
        if (stock.second > 0) {
            hasStocks = true;
            std::cout << " (ACTIVE)";
            // Calculate market value: shares × last trade price
            double marketValue = stock.second * m_lastPrices.at(stock.first);
            totalStockValue += marketValue;
            std::cout << " Market value: " << marketValue << " TL";
        } else {
            std::cout << " (SOLD OUT)";
        }
        std::cout << std::endl;
    }
    
    if (!hasStocks && m_stocks.empty()) {
        std::cout << "  No trades executed yet" << std::endl;
    } else if (!hasStocks) {
        std::cout << "  All positions have been closed" << std::endl;
    }
    
    std::cout << "Total stock symbols traded: " << m_stocks.size() << std::endl;
    std::cout << "Total stock value: " << totalStockValue << " TL" << std::endl;
    std::cout << "Total portfolio value: " << (m_balance + totalStockValue) << " TL" << std::endl;
    std::cout << "========================" << std::endl;
}

long Portfolio::getStockQuantity(const std::string& stockSymbol) const {
    auto it = m_stocks.find(stockSymbol);
    return (it != m_stocks.end()) ? it->second : 0;
}