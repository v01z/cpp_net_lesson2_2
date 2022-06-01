/* GNU General Public License v3.0 */
/******************************************************************************
*
* Name: main.cpp
* Main source code for Homework2_2 project
*
* Copyright (C) 2022, binary_cat, v01z@mail.ru
*
*****************************************************************************/

#include "NetWatcher.h"
#include <sstream>

//----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    std::string ipOrInterface;
    int howLong{};

    switch(argc)
    {
        case(1):
        {
            ipOrInterface = "127.0.0.1";
            howLong = 1;
            break;
        }
        case(2):
        {
            ipOrInterface = argv[1];
            howLong = 1;
            break;
        }
        case(3):
        {
            ipOrInterface = argv[1];
            std::istringstream ss(argv[2]);
            if (!(ss >> howLong)) {
                std::cerr << "Invalid value for seconds: " << argv[2] << '\n';
                return 1;
            } else if (!ss.eof()) {
                std::cerr << "Trailing characters after number: " << argv[2] << '\n';
                return 2;
            }
            break;
        }
        default:
        {
            std::cerr << "Usage: " << argv[0] <<
                      " <IP-address or interface name> <capture time (seconds)>\n";
            return 3;
        }
    }

    NetWatcher netWatcher;
    netWatcher.init(ipOrInterface);
    netWatcher.printInterfaceInfo();
    netWatcher.startCapture(howLong);
    netWatcher.printPacketsInfo();

    return 0;
}

//----------------------------------------------------------------------------
