#pragma once

#include <functional>
#include "DataHandler.h"
#include "CommonTypes.h"  // Add this include for Trade type



class IStrategy {
    public:
        virtual  ~IStrategy() {};

        virtual void onData(const StockData& data) = 0;

        virtual void setSignalSender(std::function<void(const Trade&)> sender) = 0;
    protected:
        std::function<void(const Trade&)> m_sendSignal; // Function used to send trade signals
};