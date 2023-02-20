#include "pch.h"


using namespace Tins;

bool callback(const PDU& pdu) {
    const IP& ip = pdu.rfind_pdu<IP>();// Find the IP layer
    const UDP& udp = pdu.rfind_pdu<UDP>();// Find the UDP layer
    IPv4Range albionIPRange = IPv4Range::from_mask("5.188.125.0", "5.188.125.255");
    RawPDU rawPDU = pdu.rfind_pdu<RawPDU>();
    if (albionIPRange.contains(ip.src_addr()) and udp.sport() == 5056)
    {   
        std::vector<uint8_t> packetData;
        std::string packet;
        if (rawPDU.payload_size() > 23)
        {
            // 0-4 lenght of file or type of packet, 
            // 4-8 time, not 20-22, eventcandidate 36-40
            // 8-12 location id
            // 16-20 event code ?
            //one byte is 2
            for (size_t i = 16; i < 20; i++)
            //for (size_t i = 0; i < pdu.rfind_pdu<RawPDU>().payload_size(); i++)
            {                
                uint8_t x = rawPDU.payload()[i];
                if (x < 16)
                    packetData.push_back(0);
                    packet += "0";
                packetData.push_back(x);
                packet += x;
            }           
            std::stringstream ss;
            for (size_t i = 0; i < packetData.size(); i++)
                ss << std::hex << (int)packetData[i];
            packet = ss.str();
            if (packet == "0000001c")
            {
                for (size_t i = 0; i < rawPDU.payload_size(); i++)
                {
                    if ((int)rawPDU.payload()[i] < 16)
                        std::cout << 0;
                    std::cout << std::hex << (int)rawPDU.payload()[i];
                    if (i % 4 == 3)
                        std::cout << " ";
                    if (i % 16 == 15 and i != rawPDU.payload_size())
                        std::cout << "\n";
                }
                //std::cout << packet;
                std::cout << "\n";
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