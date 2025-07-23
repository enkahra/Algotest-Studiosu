#pragma once

#include "IStrategy.h"
#include <functional> // Add this include for std::function

class SimpleStrategy : public IStrategy {
    public:
        SimpleStrategy();
            
        void onData(const StockData& data) override;
        
        void setSignalSender(std::function<void(const Trade&)> sender) override;
        
    private:
        std::function<void(const Trade&)> m_sendSignal;
};
