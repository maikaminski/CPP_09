#include <iostream>
#include <string>
#include "BitcoinExchange.hpp"
#include <exception>

int main(int argc, char **argv){
    if (argc != 2) {
        std::cout << "Wrong number of arguments. Provide valid file" << std::endl;
        return 1;
    }
    BitcoinExchange btc;
    try {
        btc.parseRates();
        btc.parseInput(argv[1]);
    }
    catch(std::exception &e) {std::cerr << e.what() << std::endl;}
}