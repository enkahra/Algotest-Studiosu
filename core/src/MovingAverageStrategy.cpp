#include "MovingAverageStrategy.h"
#include "CommonTypes.h"
#include <iostream>

MovingAverageStrategy::MovingAverageStrategy(int longPeriod, int shortPeriod)
    : m_longPeriod(longPeriod), m_shortPeriod(shortPeriod) {
    std::cout << "MovingAverageStrategy created with " << shortPeriod 
              << "-day short SMA and " << longPeriod << "-day long SMA" << std::endl;
}

double MovingAverageStrategy::calculateSMA(int period) const {
    if(m_historicalData.size() < period) {
        return 0.0; // Not enough data to calculate SMA
    }
    
    double totalPrice = 0.0;
    for(int i = 0; i < period; ++i) {  // Fixed: was period - 1
        totalPrice += m_historicalData[m_historicalData.size() - 1 - i].close;
    }
    return totalPrice / period; // Return the average price over the period
}

void MovingAverageStrategy::onData(const StockData& data) {
    m_historicalData.push_back(data);
    
    // Need enough data to calculate both SMAs
    if(m_historicalData.size() < m_longPeriod) {
        return; // Not enough data to calculate SMA
    }
    
    double shortSMA = calculateSMA(m_shortPeriod);
    double longSMA = calculateSMA(m_longPeriod);
    
    // Calculate previous SMAs for crossover detection (if we have enough data)
    static double prevShortSMA = shortSMA;
    static double prevLongSMA = longSMA;
    
    std::cout << "Date: " << data.date << ", Price: " << data.close 
              << ", Short SMA: " << shortSMA << ", Long SMA: " << longSMA 
              << ", Position: " << (m_positonOpen ? "OPEN" : "CLOSED") << std::endl;
    
    // BUY Condition: Short SMA crosses ABOVE long SMA (golden cross) AND no position open
    if(prevShortSMA <= prevLongSMA && shortSMA > longSMA && !m_positonOpen) {
        std::cout << "*** GOLDEN CROSS - BUY Signal ***" << std::endl;
        if(m_sendSignal) {
            Trade buyTrade = {TradeType::BUY, "AAPL", 10, data.close};
            m_sendSignal(buyTrade);
            m_positonOpen = true; // Mark position as open
        }
    }
    // SELL Condition: Short SMA crosses BELOW long SMA (death cross) AND position is open
    else if(prevShortSMA >= prevLongSMA && shortSMA < longSMA && m_positonOpen) {
        std::cout << "*** DEATH CROSS - SELL Signal ***" << std::endl;
        if(m_sendSignal) {
            Trade sellTrade = {TradeType::SELL, "AAPL", 10, data.close};
            m_sendSignal(sellTrade);
            m_positonOpen = false; // Mark position as closed
        }
    }
    
    // Update previous values for next iteration
    prevShortSMA = shortSMA;
    prevLongSMA = longSMA;
}

void MovingAverageStrategy::setSignalSender(std::function<void(const Trade&)> sender) {
    m_sendSignal = sender;
}