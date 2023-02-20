#include "pch.h"


using namespace Tins;

bool callback(const PDU& pdu) {
    const IP& ip = pdu.rfind_pdu<IP>();// Find the IP layer
    const UDP& udp = pdu.rfind_pdu<UDP>();// Find the UDP layer
    IPv4Range albionIPRange = IPv4Range::from_mask("5.188.125.0", "5.188.125.255");
    if (albionIPRange.contains(ip.src_addr()) and udp.sport() == 5056)
    {   
        std::vector<uint8_t> packetData;
        std::string packet;
        if (100 < pdu.rfind_pdu<RawPDU>().payload_size())
        {
            for (size_t i = 16; i < 20; i++)
            //for (size_t i = 0; i < pdu.rfind_pdu<RawPDU>().payload_size(); i++)
            {
                /*uint8_t x(pdu.rfind_pdu<RawPDU>().payload()[i]);
                if (x < 16)
                    std::cout << "0";
                std::cout << std::hex << (int)x;
                if (i % 4 == 3)
                    std::cout << " ";
                if (i % 16 == 15 and i != pdu.rfind_pdu<RawPDU>().payload_size())
                    std::cout << "\n";*/
                
                uint8_t x = pdu.rfind_pdu<RawPDU>().payload()[i];
                if (x < 16)
                    packetData.push_back(0);
                    packet += "0";
                packetData.push_back(x);
                packet += x;
            }
            //std::cout << "\n\n" << std::dec << pdu.rfind_pdu<RawPDU>().payload_size() << "\n\n";
            //std::cout << packet.size();
            
            std::stringstream ss;
            for (size_t i = 0; i < packetData.size(); i++)
                ss << (int)packetData[i];
            packet = ss.str();
            if (packet == "00000067")
                std::cout << packet;
            else
                std::cout << "no";
                //std::cout << packetData.data();
                //std::cout << std::hex << (int)packetData[6];
                for (size_t i = 0; i < packet.size(); i++)
                {
                    std::cout << packet[i] << " ";
                }
            std::cout << "\n";
            /*for (size_t i = 0; i < packetData.size(); i++)
            {
                std::cout << (int)packetData[i];
                if (i % 8 == 7)
                    std::cout << " ";
                if (i % 16 == 15 and i != pdu.rfind_pdu<RawPDU>().payload_size())
                    std::cout << "\n";                                              
            }           
            std::cout << "\n";*/
        }
    }
    
    return true;
}

int main() {
    // Get the default interface (where the default gateway route is)
    NetworkInterface iface = NetworkInterface::default_interface();
    SnifferConfiguration albionConfig;
    albionConfig.set_filter("ip dst 192.168.1.71");
    albionConfig.set_filter("");
    Sniffer(iface.name(), albionConfig).sniff_loop(callback);
   
}