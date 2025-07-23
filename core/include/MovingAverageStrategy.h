#pragma once 

#include "IStrategy.h"
#include <vector>


class MovingAverageStrategy : public IStrategy {
public:
    MovingAverageStrategy(int longPeriod, int shortPeriod);

    void onData(const StockData& data) override;

    void setSignalSender(std::function<void(const Trade&)> sender) override;

private:
    int m_longPeriod;
    int m_shortPeriod;

    std::vector<StockData> m_historicalData;

    bool m_positonOpen = false; // Indicates if a position is currently open
    
    double calculateSMA(int period) const;
};

