/* GNU General Public License v3.0 */
/******************************************************************************
*
* Name: NetWatcher.h
* NetWatcher header file for Homework2_2 project
*
* Copyright (C) 2022, binary_cat, v01z@mail.ru
*
*****************************************************************************/


#ifndef SNIFFER_NETWATCHER_H
#define SNIFFER_NETWATCHER_H

#include <iostream>
#include <vector>
#include <pcapplusplus/PcapLiveDeviceList.h>

//----------------------------------------------------------------------------

struct PacketInfo{
    std::string srcAddr;
    std::string destAddr;
    std::string protocol;
};

//----------------------------------------------------------------------------

class NetWatcher {
private:
    pcpp::PcapLiveDevice* _device;
    std::vector<PacketInfo> _packets;

    const std::string getProtocolAsString(const pcpp::Packet&) const;
    void consumePacket(const pcpp::Packet&);
    static bool onPacketArrivesBlockingMode(pcpp::RawPacket*, pcpp::PcapLiveDevice*, void*);

public:
    NetWatcher();
    void init(const std::string &ip = "127.0.0.1");
    void startCapture(const size_t);
    ~NetWatcher();
    void printInterfaceInfo() const;
    void printPacketsInfo() const;
};

//----------------------------------------------------------------------------

#endif //SNIFFER_NETWATCHER_H
