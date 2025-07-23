#pragma once 

#include "DataHandler.h"
#include "IStrategy.h"
#include "Portfolio.h"

class Backtester {
public:
    Backtester(IStrategy& strategy, Portfolio& portfolio, DataHandler& dataHandler);

    void startSimulation();
private:
    IStrategy& m_strategy;
    Portfolio& m_portfolio;
    DataHandler& m_dataHandler;
};
