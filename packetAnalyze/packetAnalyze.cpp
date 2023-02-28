#include "pch.h"


using namespace Tins;

// 0-3 num of commnads in packet, 
// 4-7 time
// 8-11 session id randomly generates each time
// 12-15 only 5 types exists 
// 07000000 06000100 01000000 01ff0000 08000100
// 16-19 event code
// one byte is 2
// word width is 4
// wisperingmarch locaiton with low players
// 
// command structure 
// 0-3 command category ?
// 4-7 event code
// 8-11 time since launch/location change ?
// 12-15 some id
// 16-18 changes rarely
// data
// last 4 command connector ?

// ap, disap - appearing, disappearing
// CD - corrupt dungeon
// SD - solo dungeon
// GD - group dungeon
// RM - resource mob
// ERM - empty resource mob
// DRM - dead resource mob


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

            std::vector<size_t> commandBorders;
            std::vector<std::string> desiredEventCodes = {               
                // resource mob               
                // 19 ERM, DRM disap
                // 1b ERM, RM, DRM disap
                // 4b ERM, DRM ap/disap
                // 61 ERM ap
                // 82 RM ap
                
                // player appearing
                // 41, 9b, 1a2, 3e, 19 player ap
                // 65, 6d, 3b, 1b player disap


                //mob spawn
                /*"-00000019","-0000001b","-0000001d","-00000025","-0000002b","-0000003b","-00000041",
                "-00000053","-00000058","-00000006f","+-00000082","-000000c6","-00000104",
                "-000001a6"*/
                /*"-00000027","+0000004b","-00000051","-00000065",
                "-0000007f","+00000080","-000000e8","-000000fc"*/
                // 3b mob ap
                // 80 mob spot ap
                // 3b some disap
                // 7e resource ap ?
                // resource mob spawn
                /*"00000019","0000001b","0000001d","00000025","-0000003b","+0000003c","-0000003e","+00000040",
                "-00000058","00000082"*/

                // corrupt dungeon
                // 82, 19 CD disap
                // 3b, 3e, 1b CD ap/disap
                // 7e CD ap
                
            };
            std::vector<std::string> DesiredEventCodes = {
                // something
                "000001e8","00000200","0000036a","00000433","00000443","00000474","00000879","0000225a",
                "00003300","000096da","00010000","00010001","00010100","00010101","00010200","0001ff00",
                "0001fffc","00030000","00030804","00040301","00077800","000c0000","01000000","01000001",
                "01000100","01000700","01010000","01010100","01010101","03646400","0c000000","1103ff01",
                "2003ff01","2103ff01","2203ff01","2a000101","2b000700","3803ff01","94060001","a1060001",
                "a6060001","ae060001","c8060001","0000003e","00000046","0000006e",

                //market
                "000004a4",

                // resources
                "00000019","0000001c","00000060",

                // mobs
                "0000003f",

                // opening tabs
                "00000001","00000048","00000077","00000353","00000356","00034f52","0004524f","00054649",
                "01001762",

                // staying in market
                "00000000","00000013","00000014","00000018","0000001b","0000001e","00000020","00000021",
                "00000024","00000025","00000027","00000028","0000002a","0000002b","0000002d","0000002e",
                "0000002f","00000030","00000031","00000032","00000033","00000034","00000035","00000037",
                "00000038","00000039","0000003a","0000003b","0000003c","0000003d","00000040","00000041",
                "00000042","00000043","00000045","00000047","00000049","0000004a","0000004b","0000004c",
                "0000004d","0000004e","0000004f","00000050","00000051","00000052","00000053","00000054",
                "00000055","00000056","00000058","0000005b","0000005d","0000005e","00000061","00000062",
                "00000063","00000064","00000065","00000068","00000069","0000006a","0000006b","0000006c",
                "0000006d","0000006f","00000070","00000071","00000075","00000076","0000007a","0000007b",
                "0000007c","0000007d","0000007e","00000080","00000081","00000082","00000083","00000084",
                "00000085","00000086","00000087","00000089","0000008a","0000008c","0000008d","0000008e",
                "0000008f","00000090","00000091","00000092","00000093","00000094","00000095","00000096",
                "00000097","00000098","00000099","0000009a","0000009b","0000009c","0000009d","0000009e",
                "0000009f","000000a0","000000a1","000000a2","000000a3","000000a4","000000a5","000000a6",
                "000000a7","000000a8","000000a9","000000aa","000000ab","000000ac","000000ad","000000ae",
                "000000af","000000b0","000000b1","000000b2","000000b3","000000b4","000000b5","000000b6",
                "000000b7","000000b8","000000b9","000000ba","000000bb","000000bc","000000bd","000000be",
                "000000bf","000000c0","000000c1","000000c2","000000c3","000000c4","000000c5","000000c6",
                "000000c7","000000c8","000000c9","000000ca","000000cb","000000cc","000000cd","000000ce",
                "000000cf","000000d0","000000d1","000000d2","000000d3","000000d4","000000d5","000000d6",
                "000000d7","000000d8","000000d9","000000da","000000db","000000dc","000000dd","000000de",
                "000000df","000000e0","000000e1","000000e2","000000e3","000000e4","000000e5","000000e6",
                "000000e7","000000e8","000000e9","000000ea","000000eb","000000ec","000000ed","000000ee",
                "000000ef","000000f0","000000f1","000000f2","000000f3","000000f4","000000f5","000000f6",
                "000000f7","000000f8","000000f9","000000fa","000000fb","000000fc","000000fd","000000fe",
                "000000ff","00000100","00000101","00000102","00000103","00000104","00000105","00000106",
                "00000107","00000108","00000109","0000010a","0000010b","0000010c","0000010d","0000010e",
                "0000010f","00000110","00000111","00000112","00000113","00000114","00000115","00000116",
                "00000117","00000118","00000119","0000011a","0000011b","0000011c","0000011d","0000011e",
                "0000011f","00000120","00000121","00000122","00000123","00000124","00000125","00000126",
                "00000127","00000128","00000129","0000012a","0000012b","0000012c","0000012d","0000012e",
                "0000012f","00000130","00000131","00000132","00000133","00000134","00000135","00000136",
                "00000137","00000138","00000139","0000013a","0000013b","0000013c","0000013d","0000013e",
                "0000013f","00000140","00000141","00000142","00000143","00000144","00000145","00000146",
                "00000147","00000148","00000149","0000014a","0000014b","0000014c","0000014d","0000014e",
                "0000014f","00000150","00000151","00000152","00000153","00000154","00000155","00000156",
                "00000158","00000159","0000015a","0000015b","0000015c","0000015d","0000015e","0000015f",
                "00000160","00000161","00000162","00000163","00000164","00000165","00000166","00000167",
                "00000168","00000169","0000016a","0000016b","0000016c","0000016d","0000016e","0000016f",
                "00000170","00000171","00000172","00000173","00000174","00000175","00000176","00000177",
                "00000178","00000179","0000017a","0000017b","0000017c","0000017d","0000017e","0000017f",
                "00000180","00000181","00000182","00000183","00000184","00000185","00000186","00000187",
                "00000188","00000189","0000018a","0000018b","0000018c","0000018d","0000018e","0000018f",
                "00000190","00000191","00000192","00000193","00000194","00000195","00000196","00000197",
                "00000198","00000199","0000019a","0000019b","0000019c","0000019d","0000019e","0000019f",
                "000001a0","000001a1","000001a2","000001a3","000001a4","000001a5","000001a6","000001a7",
                "000001a8","000001a9","000001aa","000001ab","000001ac","000001ad","000001ae","000001af",
                "000001b0","000001b1","000001b2","000001b5","000001b6","000001b7","000001b9","00067800",
                "0009a500","01067800","06067800","07780000"
            };

            commandBorders = findCommandBordersInPacket(rawPDU);
            for (size_t i = 0; i < commandBorders.size() - 1; i++)
            {
                std::string eventCode = readPacket(rawPDU, commandBorders[i] + 4,
                                                           commandBorders[i] + 8).str();
                //std::string timeOfCommand = readPacket(rawPDU, commandBorders[i] + 8,
                //    commandBorders[i] + 12).str();
                //std::string someIds = readPacket(rawPDU, commandBorders[i] + 12,
                //    commandBorders[i] + 16).str();
                if ((std::find(std::begin(desiredEventCodes), std::end(desiredEventCodes),
                    eventCode) != std::end(desiredEventCodes)))
                {
                    std::cout << "\"" << eventCode << "\"" << " " /*<< commandBorders[i + 1]
                       - (commandBorders[i] + 4)*/ << "\n";
                    //std::cout << "packet:\n";
                    //std::cout << readPacket(rawPDU, 0, 4).str() << " " << commandBorders.size() - 1;
                    //printPacket(rawPDU, commandBorders[i] + 16, commandBorders[i + 1] - 4);
                    //for (size_t j = commandBorders[i] + 16; j < commandBorders[i + 1]; j++)
                    //    std::cout << rawPDU.payload()[j];
                    //std::cout << "\n";
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