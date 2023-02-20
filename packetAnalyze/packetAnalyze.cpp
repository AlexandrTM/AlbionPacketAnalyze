#include "pch.h"


using namespace Tins;

void printPacket(RawPDU pdu) {
    for (size_t i = 0; i < pdu.payload_size(); i++)
    {
        if ((int)pdu.payload()[i] < 16)
            std::cout << 0;
        std::cout << std::hex << (int)pdu.payload()[i];
        if (i % 4 == 3)
            std::cout << " ";
        if (i % 16 == 15 and i != pdu.payload_size())
            std::cout << "\n";
    }
}
void printPacket(RawPDU pdu, size_t regionStart, size_t regionEnd) {
    for (size_t i = regionStart; i < regionEnd; i++)
    {
        if ((int)pdu.payload()[i] < 16)
            std::cout << 0;
        std::cout << std::hex << (int)pdu.payload()[i];
        if (i % 4 == 3)
            std::cout << " ";
        if (i % 16 == 15 and i != pdu.payload_size())
            std::cout << "\n";
    }
}
void readPacket(RawPDU pdu, size_t regionStart, size_t regionEnd, std::stringstream &ss)
{
    for (size_t i = regionStart; i < regionEnd; i++)
    {
        uint8_t x = pdu.payload()[i];
        if (x < 16)
            ss << 0;
        ss << std::hex << (int)x;
    }
}
std::stringstream readPacket(RawPDU pdu, size_t regionStart, size_t regionEnd)
{
    std::stringstream ss;
    for (size_t i = regionStart; i < regionEnd; i++)
    {
        uint8_t x = pdu.payload()[i];
        if (x < 16)
            ss << 0;
        ss << std::hex << (int)x;
    }
    return ss;
}

bool callback(const PDU& pdu) {
    const IP& ip = pdu.rfind_pdu<IP>();// Find the IP layer
    const UDP& udp = pdu.rfind_pdu<UDP>();// Find the UDP layer
    IPv4Range albionIPRange = IPv4Range::from_mask("5.188.125.0", "5.188.125.255");
    RawPDU rawPDU = pdu.rfind_pdu<RawPDU>();
    if (albionIPRange.contains(ip.src_addr()) and udp.sport() == 5056)
    { 
        if (rawPDU.payload_size() > 23)
        {
            // 0-4 num of commnads in packet, 
            // 4-8 time, not 20-22, eventcandidate 36-40
            // 8-12 location id randomly generates each time
            // 12-16 only 5 types exists 
            // 07000000 06000100 01000000 01ff0000 08000100
            // 16-20 event code ?
            // one byte is 2
            std::vector<std::string> compareStrings = 
            {"01000000", "01ff0000", "06000100", "07000000", "08000100"};
            std::stringstream ss;
            std::stringstream compareString;
            std::vector<size_t> eventParts;

            if (ss.str() != "0fff000")
            {
                size_t counter = 0;
                for (size_t i = 12; i < (rawPDU.payload_size() - 4); i++)
                { 
                    readPacket(rawPDU, i, i + 4, compareString);
                    for (size_t j = 0; j < compareStrings.size(); j++)
                    {
                        if (compareString.str() == compareStrings[j])
                        {
                            eventParts.push_back(i);
                            i += 7;
                        }
                    }
                    compareString.str(std::string());
                }
                eventParts.push_back(rawPDU.payload_size());
                std::cout << "event parts: \n";
                for (size_t i = 0; i < eventParts.size() - 1; i++)
                {
                    printPacket(rawPDU, eventParts[i], eventParts[i + 1]);
                    std::cout << "\n";
                }
                //std::cout << "counter: " << counter << "\n";
                /*std::cout << "packet: \n";
                printPacket(rawPDU);
                std::cout << "\n";*/
                //std::cout << stoi(ss.str());
            }
            
        }
    }
    
    return true;
}

int main() {
    // Get the default interface (where the default gateway route is)
    NetworkInterface iface = NetworkInterface::default_interface();
    SnifferConfiguration albionConfig;
    albionConfig.set_filter("ip dst 192.168.1.71");
    //albionConfig.set_filter("");
    Sniffer(iface.name(), albionConfig).sniff_loop(callback);
   
}