#include "Portfolio.h"

bool Portfolio::processTrade(const Trade& trade) {
    if (trade.type == TradeType::BUY) {
        double totalCost = trade.quantity * trade.price;
        if (m_balance >= totalCost) {
            m_balance -= totalCost;
            m_stocks[trade.stockSymbol] += trade.quantity;
            std::cout << "BUY: " << trade.quantity << " " << trade.stockSymbol 
                      << " @ " << trade.price << " TL. Total cost: " << totalCost << " TL" << std::endl;
            return true;
        } else {
            std::cout << "Insufficient balance! Required: " << totalCost 
                      << " TL, Available: " << m_balance << " TL" << std::endl;
            return false;
        }
    } 
    else if (trade.type == TradeType::SELL) {
        if (m_stocks[trade.stockSymbol] >= trade.quantity) {
            double totalRevenue = trade.quantity * trade.price;
            m_balance += totalRevenue;
            m_stocks[trade.stockSymbol] -= trade.quantity;
            std::cout << "SELL: " << trade.quantity << " " << trade.stockSymbol 
                      << " @ " << trade.price << " TL. Total revenue: " << totalRevenue << " TL" << std::endl;
            return true;
        } else {
            std::cout << "Insufficient stock! " << trade.stockSymbol << " available: " 
                      << m_stocks[trade.stockSymbol] << ", requested: " << trade.quantity << std::endl;
            return false;
        }
    }
    return false;
}

void Portfolio::displayHoldings() const {
    std::cout << "\n=== PORTFOLIO STATUS ===" << std::endl;
    std::cout << "Cash balance: " << m_balance << " TL" << std::endl;
    std::cout << "Stock holdings:" << std::endl;
    for (const auto& stock : m_stocks) {
        if (stock.second > 0) {
            std::cout << "  " << stock.first << ": " << stock.second << " shares" << std::endl;
        }
    }
    std::cout << "========================" << std::endl;
}

long Portfolio::getStockQuantity(const std::string& stockSymbol) const {
    auto it = m_stocks.find(stockSymbol);
    return (it != m_stocks.end()) ? it->second : 0;
}