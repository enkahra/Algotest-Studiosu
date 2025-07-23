#include <iostream>
#include "DataHandler.h"
#include "SimpleStrategy.h"
#include "Portfolio.h"
#include "Backtester.h"
#include "MovingAverageStrategy.h"

int main() {
    DataHandler dataHandler;
    
    // Load market data
    if (!dataHandler.loadDataFromCSV("C:\\Users\\musae\\Algotest-Studiosu\\data\\sample_data.csv")) {
        std::cerr << "Data loading failed. Terminating program." << std::endl;
        return -1;
    }

    std::cout << "Data loading successful." << std::endl;
    
    // Let user choose strategy
    std::cout << "\nChoose your trading strategy:" << std::endl;
    std::cout << "1. Simple Strategy (threshold-based)" << std::endl;
    std::cout << "2. Moving Average Strategy (crossover-based)" << std::endl;
    std::cout << "Enter your choice (1 or 2): ";
    
    int choice;
    std::cin >> choice;
    
    // Initialize trading components
    SimpleStrategy simpleStrategy;
    MovingAverageStrategy movingAvgStrategy(10, 30); 
    Portfolio portfolio(10000.0); // Initial balance 10,000 Dollars
    
    IStrategy* selectedStrategy = nullptr;
    
    if (choice == 1) {
        std::cout << "\nYou selected: Simple Strategy" << std::endl;
        selectedStrategy = &simpleStrategy;
        simpleStrategy.setSignalSender([&portfolio](const Trade& trade) {
            portfolio.processTrade(trade);
        });
    } else if (choice == 2) {
        std::cout << "\nYou selected: Moving Average Strategy" << std::endl;
        selectedStrategy = &movingAvgStrategy;
        movingAvgStrategy.setSignalSender([&portfolio](const Trade& trade) {
            portfolio.processTrade(trade);
        });
    } else {
        std::cerr << "Invalid choice! Using Simple Strategy as default." << std::endl;
        selectedStrategy = &simpleStrategy;
        simpleStrategy.setSignalSender([&portfolio](const Trade& trade) {
            portfolio.processTrade(trade);
        });
    }

    // Get market data and show stats
    const auto& allData = dataHandler.getStockData();
    std::cout << "Total record count: " << allData.size() << std::endl;

    // Show initial portfolio state
    std::cout << "\nInitial Portfolio State:" << std::endl;
    portfolio.displayHoldings();

    // Create backtester and run simulation with selected strategy
    Backtester backtester(*selectedStrategy, portfolio, dataHandler);
    backtester.startSimulation();

    std::cout << "\nProgram finished. Press Enter to close..." << std::endl;
    std::cin.ignore(); // Clear any leftover newline from previous input
    std::cin.get();    // Wait for user to press Enter

    return 0;
}