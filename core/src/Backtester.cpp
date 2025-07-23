#include "Backtester.h"
#include <iostream>

Backtester::Backtester(IStrategy& strategy, Portfolio& portfolio, DataHandler& dataHandler)
    : m_strategy(strategy), m_portfolio(portfolio), m_dataHandler(dataHandler) {}

void Backtester::startSimulation() {
    const auto& allHistoricalData = m_dataHandler.getStockData();

    std::cout << "Starting backtest simulation..." << std::endl;
    for(const auto& data: allHistoricalData) {
        m_strategy.onData(data);
    }
    
    std::cout << "\nFinal Portfolio State:" << std::endl;
    m_portfolio.displayHoldings(); // Display portfolio status
    std::cout << "Backtest simulation completed." << std::endl;
}
