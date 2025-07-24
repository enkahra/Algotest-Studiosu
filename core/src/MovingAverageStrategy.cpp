#include "MovingAverageStrategy.h"
#include "CommonTypes.h"
#include "Portfolio.h"
#include <iostream>

MovingAverageStrategy::MovingAverageStrategy(int longPeriod, int shortPeriod, double positionSizePercent)
    : m_longPeriod(longPeriod), m_shortPeriod(shortPeriod), m_positionSizePercent(positionSizePercent) {
    std::cout << "MovingAverageStrategy created with " << shortPeriod 
              << "-day short SMA and " << longPeriod << "-day long SMA" << std::endl;
    std::cout << "Position size: " << (positionSizePercent * 100) << "% of available balance" << std::endl;
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
        std::cout << "Not enough data yet: " << m_historicalData.size() 
                  << "/" << m_longPeriod << " required" << std::endl;
        return; // Not enough data to calculate SMA
    }
    
    double shortSMA = calculateSMA(m_shortPeriod);
    double longSMA = calculateSMA(m_longPeriod);
    
    // Calculate previous SMAs for crossover detection
    static double prevShortSMA = 0.0;
    static double prevLongSMA = 0.0;
    static bool firstRun = true;
    
    // Only print every 100th data point to avoid spam
    static int counter = 0;
    counter++;
    if (counter % 100 == 0 || firstRun) {
        std::cout << "Data point " << counter << " - Date: " << data.date 
                  << ", Price: " << data.close 
                  << ", Short SMA: " << shortSMA << ", Long SMA: " << longSMA 
                  << ", Position: " << (m_positonOpen ? "OPEN" : "CLOSED") << std::endl;
    }
    
    // Skip first run since we don't have previous values
    if (!firstRun) {
        // BUY Condition: Short SMA crosses ABOVE long SMA (golden cross) AND no position open
        if(prevShortSMA <= prevLongSMA && shortSMA > longSMA && !m_positonOpen) {
            static int buySignals = 0;
            buySignals++;
            std::cout << "*** GOLDEN CROSS #" << buySignals << " - BUY Signal at " << data.date << " ***" << std::endl;
            std::cout << "Prev: Short=" << prevShortSMA << ", Long=" << prevLongSMA << std::endl;
            std::cout << "Now:  Short=" << shortSMA << ", Long=" << longSMA << std::endl;
            
            if(m_sendSignal && m_portfolio) {
                // Calculate shares based on percentage of available balance
                double availableBalance = m_portfolio->getBalance();
                double investmentAmount = availableBalance * m_positionSizePercent;
                long sharesToBuy = static_cast<long>(investmentAmount / data.close);
                
                if(sharesToBuy > 0) {
                    m_currentShares = sharesToBuy;
                    Trade buyTrade = {TradeType::BUY, "AAPL", sharesToBuy, data.close};
                    std::cout << "Investing " << investmentAmount << " TL (" << (m_positionSizePercent*100) 
                              << "% of " << availableBalance << " TL) = " << sharesToBuy << " shares" << std::endl;
                    m_sendSignal(buyTrade);
                    m_positonOpen = true; // Mark position as open
                } else {
                    std::cout << "Insufficient balance for meaningful trade - skipping" << std::endl;
                }
            }
        }
        // SELL Condition: Short SMA crosses BELOW long SMA (death cross) AND position is open
        else if(prevShortSMA >= prevLongSMA && shortSMA < longSMA && m_positonOpen) {
            static int sellSignals = 0;
            sellSignals++;
            std::cout << "*** DEATH CROSS #" << sellSignals << " - SELL Signal at " << data.date << " ***" << std::endl;
            std::cout << "Prev: Short=" << prevShortSMA << ", Long=" << prevLongSMA << std::endl;
            std::cout << "Now:  Short=" << shortSMA << ", Long=" << longSMA << std::endl;
            
            if(m_sendSignal && m_currentShares > 0) {
                Trade sellTrade = {TradeType::SELL, "AAPL", m_currentShares, data.close};
                std::cout << "Selling all " << m_currentShares << " shares" << std::endl;
                m_sendSignal(sellTrade);
                m_positonOpen = false; // Mark position as closed
                m_currentShares = 0;   // Reset share count
            }
        }
    }
    
    // Update previous values for next iteration
    prevShortSMA = shortSMA;
    prevLongSMA = longSMA;
    firstRun = false;
}

void MovingAverageStrategy::setSignalSender(std::function<void(const Trade&)> sender) {
    m_sendSignal = sender;
}