#pragma once 

#include "IStrategy.h"
#include <vector>


class MovingAverageStrategy : public IStrategy {
public:
    MovingAverageStrategy(int longPeriod, int shortPeriod, double positionSizePercent = 0.95);

    void onData(const StockData& data) override;

    void setSignalSender(std::function<void(const Trade&)> sender) override;
    
    // Set reference to portfolio to get balance
    void setPortfolio(const class Portfolio* portfolio) { m_portfolio = portfolio; }

private:
    int m_longPeriod;
    int m_shortPeriod;
    double m_positionSizePercent;  // Percentage of balance to use per trade (default 95%)
    const class Portfolio* m_portfolio = nullptr;  // Reference to portfolio for balance

    std::vector<StockData> m_historicalData;

    bool m_positonOpen = false; // Indicates if a position is currently open
    long m_currentShares = 0;   // Track current shares held
    
    double calculateSMA(int period) const;
};

