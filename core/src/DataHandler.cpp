#include "DataHandler.h"

#include <fstream>
#include <sstream>  
#include <iostream>

DataHandler::DataHandler() {
    // Kurucu metodun içeriği, başlangıçta herhangi bir işlem yapmıyor
    std::cout << "DataHandler instance created." << std::endl;
    }


bool DataHandler::loadDataFromCSV(const std::string& filePath) {
    std::cout << "Attempting to load file: " << filePath << std::endl;
    
    std::ifstream file(filePath);   
    if (!file.is_open()) {          
        std::cerr << "Error opening file: " << filePath << std::endl;
        return false;
    }
    
    std::cout << "File opened successfully!" << std::endl;
    
    std::string line;
    std::getline(file, line); // Başlık satırını atla
    std::cout << "Header line: " << line << std::endl;

    int lineNumber = 1;
    int successfulLines = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string cell;
        StockData dailyData;

        try {
            // Her bir hücreyi okurken debug bilgisi ekle
            if (!std::getline(ss, cell, ',')) {
                std::cerr << "Error reading date on line " << lineNumber << std::endl;
                continue;
            }
            dailyData.date = cell;

            if (!std::getline(ss, cell, ',')) {
                std::cerr << "Error reading open price on line " << lineNumber << std::endl;
                continue;
            }
            dailyData.open = std::stod(cell);
            
            if (!std::getline(ss, cell, ',')) {
                std::cerr << "Error reading high price on line " << lineNumber << std::endl;
                continue;
            }
            dailyData.high = std::stod(cell);

            if (!std::getline(ss, cell, ',')) {
                std::cerr << "Error reading low price on line " << lineNumber << std::endl;
                continue;
            }
            dailyData.low = std::stod(cell);

            if (!std::getline(ss, cell, ',')) {
                std::cerr << "Error reading close price on line " << lineNumber << std::endl;
                continue;
            }
            dailyData.close = std::stod(cell);

            if (!std::getline(ss, cell, ',')) {
                std::cerr << "Error reading volume on line " << lineNumber << std::endl;
                continue;
            }
            dailyData.volume = std::stoll(cell);

            m_stockData.push_back(dailyData);
            successfulLines++;
            
        } catch (const std::invalid_argument& e) {
            std::cerr << "Warning: Invalid data format on line " << lineNumber << ". Skipping line. Content: " << line << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Warning: Data out of range on line " << lineNumber << ". Skipping line. Content: " << line << std::endl;
        }
    }

    file.close();
    std::cout << "Successfully loaded " << successfulLines << " lines from " << filePath << std::endl;
    std::cout << "Total data points in memory: " << m_stockData.size() << std::endl;
    
    return successfulLines > 0; // En az bir satır başarıyla yüklendiyse true döndür
}

const std::vector<StockData>& DataHandler::getStockData() const {
    return m_stockData; // Yüklenen veriyi döndür;
}