#pragma once // Bu başlık dosyasının birden fazla kez include edilmesini önler

#include <string>
#include <vector>

// Tek bir zaman dilimine ait hisse senedi verisini temsil eden yapı (struct)
struct StockData {
    std::string date;
    double open;
    double high;
    double low;
    double close;
    long long volume;
};

// Veri işlemlerini yönetecek olan sınıf
class DataHandler {
public:
    // Kurucu metod (Constructor)
    DataHandler();

    // Verilen yoldan CSV verisini yükler
    bool loadDataFromCSV(const std::string& filePath);

    // Yüklenmiş olan veriyi dışarıya verir
    const std::vector<StockData>& getStockData() const;

private:
    // Hisse senedi verilerini saklayacağımız vektör
    std::vector<StockData> m_stockData;
};