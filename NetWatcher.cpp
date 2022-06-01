/* GNU General Public License v3.0 */
/******************************************************************************
*
* Name: NetWatcher.cpp
* NetWatcher source code for Homework2_2 project
*
* Copyright (C) 2022, binary_cat, v01z@mail.ru
*
*****************************************************************************/

#include "NetWatcher.h"
#include <pcapplusplus/EthLayer.h>
#include <pcapplusplus/IPv4Layer.h>

//----------------------------------------------------------------------------

const std::string NetWatcher::getProtocolAsString(const pcpp::Packet &packet) const {
    std::string tempStr;
    if (packet.isPacketOfType(pcpp::Ethernet))
        tempStr.append("Ethernet.");
    if (packet.isPacketOfType(pcpp::IPv4))
        tempStr.append("IPv4.");
    if (packet.isPacketOfType(pcpp::IPv6))
        tempStr.append("IPv6.");
    if (packet.isPacketOfType(pcpp::TCP))
        tempStr.append("TCP.");
    if (packet.isPacketOfType(pcpp::UDP))
        tempStr.append("UDP.");
    if (packet.isPacketOfType(pcpp::DNS))
        tempStr.append("DNS.");
    if (packet.isPacketOfType(pcpp::HTTP))
        tempStr.append("HTTP.");
    if (packet.isPacketOfType(pcpp::SSL))
        tempStr.append("SSL.");

    if (tempStr.empty())
        tempStr.append("Unknown.");

    return tempStr;
}

//----------------------------------------------------------------------------

void NetWatcher::consumePacket(const pcpp::Packet &packet) {
    PacketInfo packetInfo{};

    pcpp::EthLayer* ethernetLayer = packet.getLayerOfType<pcpp::EthLayer>();

    if (nullptr != ethernetLayer)
    {
        packetInfo.srcAddr.append(ethernetLayer->getSourceMac().toString());
        packetInfo.destAddr.append(ethernetLayer->getDestMac().toString());
    }

    pcpp::IPv4Layer* ipLayer = packet.getLayerOfType<pcpp::IPv4Layer>();

    if (nullptr != ipLayer)
    {
        packetInfo.srcAddr.append("\n\t\t      " + ipLayer->getSrcIPAddress().toString());
        packetInfo.destAddr.append("\n\t\t      " + ipLayer->getDstIPAddress().toString());
    }

    packetInfo.protocol = getProtocolAsString(packet);

    _packets.push_back(packetInfo);
}

//----------------------------------------------------------------------------

bool NetWatcher::onPacketArrivesBlockingMode(pcpp::RawPacket* packet,
        pcpp::PcapLiveDevice* dev, void* cookie) {

    NetWatcher* netWatcher = (NetWatcher*) cookie;

    pcpp::Packet parsedPacked(packet);

    netWatcher->consumePacket(parsedPacked);

    return false;
}

//----------------------------------------------------------------------------

NetWatcher::NetWatcher():
    _device{ nullptr },
    _packets{}
{}

//----------------------------------------------------------------------------

void NetWatcher::init(const std::string &ipOrIntrfc) {
    if (nullptr != _device)
    {
        if (_device->isOpened())
            _device->close();
        _packets.clear();
    }

    _device = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIpOrName(ipOrIntrfc);

    if (nullptr == _device)
    {
        std::cerr << "Cannot find interface with IPv4 address of '" << ipOrIntrfc << "'" << std::endl;
        exit(4); // "1, 2, 3" are occupied in main()
    }

    if (!_device->open())
    {
        std::cerr << "Cannot open device" << std::endl;
        //return 1;
        exit(5);
    }
}

//----------------------------------------------------------------------------

void NetWatcher::startCapture(const size_t howLong) {
   if (!howLong)
       return;

   if ((nullptr == _device) || (!_device->isOpened()))
   {
       std::cerr << "Device hasn't initialized yet.\n" <<
            "Call init() first.\n";
       return;
   }

   std::cout << std::endl << "Starting capture in blocking mode..." << std::endl;

   _device->startCaptureBlockingMode(onPacketArrivesBlockingMode,
    this, howLong);

   std::cout << "\nCapturing complete.\n";
}

//----------------------------------------------------------------------------

NetWatcher::~NetWatcher() {
    if (nullptr != _device && _device->isOpened()) {
        _device->close();
    }
}

//----------------------------------------------------------------------------

void NetWatcher::printInterfaceInfo() const {
    if (nullptr == _device)
        return;

    std::cout
            << "Interface info:" << std::endl
            << "   Interface name:        " << _device->getName() << std::endl
            << "   Interface description: " << _device->getDesc() << std::endl
            << "   MAC address:           " << _device->getMacAddress() << std::endl
            << "   IP address:            " << _device->getIPv4Address() << std::endl
            << "   Default gateway:       " << _device->getDefaultGateway() << std::endl
            << "   Interface MTU:         " << _device->getMtu() << std::endl;

    if (_device->getDnsServers().size() > 0)
        std::cout << "   DNS server:            " << _device->getDnsServers().at(0) << std::endl;
}

//----------------------------------------------------------------------------

void NetWatcher::printPacketsInfo() const {

    for (size_t i{}; i < _packets.size(); ++i)
    {
        std::cout << "\nPacket number:  " << i + 1;
        std::cout << "\n\t\tFrom: " << _packets.at(i).srcAddr;
        std::cout << "\n\t\tTo:   " << _packets.at(i).destAddr;
        std::cout << "\n\tProtocol: " << _packets.at(i).protocol;
        std::cout << std::endl;
    }
}

//----------------------------------------------------------------------------
