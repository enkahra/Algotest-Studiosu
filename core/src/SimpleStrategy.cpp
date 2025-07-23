#include "SimpleStrategy.h"
#include "CommonTypes.h"  // Add this include
#include <iostream>

SimpleStrategy::SimpleStrategy() {
    std::cout << "SimpleStrategy instance created." << std::endl;
}

void SimpleStrategy::onData(const StockData& data) {
    const double BUY_THRESHOLD = 100.0;  // Buy when price is below this
    const double SELL_THRESHOLD = 150.0; // Sell when price is above this
    const int TRADE_QUANTITY = 10;       // Standard trade quantity
    
    double currentPrice = data.close;
    
    // Simple moving average strategy simulation
    // In a real strategy, you'd calculate actual moving averages
    static double previousPrice = currentPrice; // Remember previous price
    double priceChange = currentPrice - previousPrice;
    
    // Buy signal: Price is low and trending up
    if (currentPrice < BUY_THRESHOLD && priceChange > 0) {
        std::cout << "BUY Signal: Price " << currentPrice << " is below threshold " 
                  << BUY_THRESHOLD << " and trending up on " << data.date << std::endl;
        
        if (m_sendSignal) {
            Trade buyTrade = {TradeType::BUY, "AAPL", TRADE_QUANTITY, currentPrice};
            m_sendSignal(buyTrade);
        }
    }
    // Sell signal: Price is high and we can make profit
    else if (currentPrice > SELL_THRESHOLD) {
        std::cout << "SELL Signal: Price " << currentPrice << " is above threshold " 
                  << SELL_THRESHOLD << " - taking profit on " << data.date << std::endl;
        
        if (m_sendSignal) {
            Trade sellTrade = {TradeType::SELL, "AAPL", TRADE_QUANTITY, currentPrice};
            m_sendSignal(sellTrade);
        }
    }
    // Hold signal: No clear trend or price in middle range
    else {
        std::cout << "HOLD Signal: Price " << currentPrice << " - waiting for better opportunity on " 
                  << data.date << std::endl;
    }
    
    previousPrice = currentPrice; // Update for next iteration
}

void SimpleStrategy::setSignalSender(std::function<void(const Trade&)> sender) {
    m_sendSignal = sender;
}
