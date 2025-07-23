#pragma once
#include <string>

enum class TradeType {
    BUY,
    SELL
};

struct Trade 
{
    TradeType type;
    std::string stockSymbol;
    long quantity;
    double price;
};

