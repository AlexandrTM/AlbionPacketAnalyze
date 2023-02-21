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
        if ((i - regionStart) % 4 == 3)
            std::cout << " ";
        if ((i - regionStart) % 16 == 15 and i != pdu.payload_size())
            std::cout << "\n";
    }
}
void readPacket(RawPDU pdu, size_t regionStart, size_t regionEnd, std::stringstream& ss)
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
std::vector<size_t> findCommandBordersInPacket(RawPDU pdu) {
    std::vector<std::string> compareStrings = 
    { "01000000", "01ff0000", "06000100", "07000000", "08000100" };
    std::vector<size_t> commandBorders;
    std::stringstream compareString;

    for (size_t i = 12; i < (pdu.payload_size() - 8); i++)
    {
        readPacket(pdu, i, i + 4, compareString);
        for (size_t j = 0; j < compareStrings.size(); j++)
        {
            if (compareString.str() == compareStrings[j])
            {
                commandBorders.push_back(i);
                i += 7;
            }
        }
        compareString.str(std::string());
    }
    commandBorders.push_back(pdu.payload_size());

    return commandBorders;
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
            // 8-12 location id/session id randomly generates each time
            // 12-16 only 5 types exists 
            // 07000000 06000100 01000000 01ff0000 08000100
            // 16-20 event code ?
            // one byte is 2
            // word width is 4
            // wisperingmarch locaiton with low players
            std::vector<size_t> commandBorders;
            std::vector<std::string> desiredEventCodes = {
                "00000019"};
            std::vector<std::string> DesiredEventCodes = { 
                "00000014", "00000018", "00000041", "00000043", "00000046",
                "00000048", "0000001c", "0000001e", "0000001b", "000c0000",
                "0000003b", "0000003d", "00000028", "00000072", "0000003e",
                "000004a4", "0000004e", "0000005c", "00000044", "00000049",
                "00000033", "00000036", "00000039", "000000ca", "0000003f",
                "000000A9", "00000032", "00000035", "00000073", "00000059",
                "00000026", "00000025"

            };

            commandBorders = findCommandBordersInPacket(rawPDU);
            for (size_t i = 0; i < commandBorders.size() - 1; i++)
            {
                std::string eventCode = readPacket(rawPDU, commandBorders[i] + 4,
                                                           commandBorders[i] + 8).str();
                if ((std::find(std::begin(desiredEventCodes), std::end(desiredEventCodes),
                    eventCode) != std::end(desiredEventCodes)))
                {
                    std::cout << eventCode << " " << commandBorders[i + 1]
                        - (commandBorders[i] + 4) << "\n";
                    std::cout << "packet:\n";
                    for (size_t j = commandBorders[i] + 8; j < commandBorders[i + 1]; j++)
                    {
                        std::cout << rawPDU.payload()[j];
                    }
                    std::cout << "\n";
                }
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