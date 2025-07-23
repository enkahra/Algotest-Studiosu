#include <iostream>
#include "DataHandler.h"
#include "SimpleStrategy.h"
#include "Portfolio.h"
#include "Backtester.h"

int main() {
    DataHandler dataHandler;
    
    // Load market data
    if (!dataHandler.loadDataFromCSV("C:\\Users\\musae\\Algotest-Studiosu\\data\\sample_data.csv")) {
        std::cerr << "Data loading failed. Terminating program." << std::endl;
        return -1;
    }

    std::cout << "Data loading successful." << std::endl;
    
    // Initialize trading components
    SimpleStrategy strategy;
    Portfolio portfolio(10000.0); // Initial balance 10,000 TL
    
    // Connect strategy signals to portfolio
    strategy.setSignalSender([&portfolio](const Trade& trade) {
        portfolio.processTrade(trade);
    });

    // Get market data and show stats
    const auto& allData = dataHandler.getStockData();
    std::cout << "Total record count: " << allData.size() << std::endl;

    // Show initial portfolio state
    std::cout << "\nInitial Portfolio State:" << std::endl;
    portfolio.displayHoldings();

    // Create backtester and run simulation
    Backtester backtester(strategy, portfolio, dataHandler);
    backtester.startSimulation();

    std::cout << "\nProgram finished. Press Enter to close..." << std::endl;
    std::cin.get();

    return 0;
}