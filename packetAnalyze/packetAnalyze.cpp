#include "pch.h"

using namespace Tins;

std::vector<std::string> ListOfEventCodes = {
    "00000000","00000001","00000013","00000014","00000018","00000019","0000001b","0000001c",
    "0000001e","00000020","00000021","00000024","00000025","00000027","00000028","0000002a",
    "0000002b","0000002d","0000002e","0000002f","00000030","00000031","00000032","00000033",
    "00000034","00000035","00000037","00000038","00000039","0000003a","0000003b","0000003c",
    "0000003d","0000003e","0000003f","00000040","00000041","00000042","00000043","00000045",
    "00000046","00000047","00000048","00000049","0000004a","0000004b","0000004c","0000004d",
    "0000004e","0000004f","00000050","00000051","00000052","00000053","00000054","00000055",
    "00000056","00000058","0000005b","0000005d","0000005e","00000060","00000061","00000062",
    "00000063","00000064","00000065","00000068","00000069","0000006a","0000006b","0000006c",
    "0000006d","0000006e","0000006f","00000070","00000071","00000075","00000076","00000077",
    "0000007a","0000007b","0000007c","0000007d","0000007e","00000080","00000081","00000082",
    "00000083","00000084","00000085","00000086","00000087","00000089","0000008a","0000008c",
    "0000008d","0000008e","0000008f","00000090","00000091","00000092","00000093","00000094",
    "00000095","00000096","00000097","00000098","00000099","0000009a","0000009b","0000009c",
    "0000009d","0000009e","0000009f","000000a0","000000a1","000000a2","000000a3","000000a4",
    "000000a5","000000a6","000000a7","000000a8","000000a9","000000aa","000000ab","000000ac",
    "000000ad","000000ae","000000af","000000b0","000000b1","000000b2","000000b3","000000b4",
    "000000b5","000000b6","000000b7","000000b8","000000b9","000000ba","000000bb","000000bc",
    "000000bd","000000be","000000bf","000000c0","000000c1","000000c2","000000c3","000000c4",
    "000000c5","000000c6","000000c7","000000c8","000000c9","000000ca","000000cb","000000cc",
    "000000cd","000000ce","000000cf","000000d0","000000d1","000000d2","000000d3","000000d4",
    "000000d5","000000d6","000000d7","000000d8","000000d9","000000da","000000db","000000dc",
    "000000dd","000000de","000000df","000000e0","000000e1","000000e2","000000e3","000000e4",
    "000000e5","000000e6","000000e7","000000e8","000000e9","000000ea","000000eb","000000ec",
    "000000ed","000000ee","000000ef","000000f0","000000f1","000000f2","000000f3","000000f4",
    "000000f5","000000f6","000000f7","000000f8","000000f9","000000fa","000000fb","000000fc",
    "000000fd","000000fe","000000ff","00000100","00000101","00000102","00000103","00000104",
    "00000105","00000106","00000107","00000108","00000109","0000010a","0000010b","0000010c",
    "0000010d","0000010e","0000010f","00000110","00000111","00000112","00000113","00000114",
    "00000115","00000116","00000117","00000118","00000119","0000011a","0000011b","0000011c",
    "0000011d","0000011e","0000011f","00000120","00000121","00000122","00000123","00000124",
    "00000125","00000126","00000127","00000128","00000129","0000012a","0000012b","0000012c",
    "0000012d","0000012e","0000012f","00000130","00000131","00000132","00000133","00000134",
    "00000135","00000136","00000137","00000138","00000139","0000013a","0000013b","0000013c",
    "0000013d","0000013e","0000013f","00000140","00000141","00000142","00000143","00000144",
    "00000145","00000146","00000147","00000148","00000149","0000014a","0000014b","0000014c",
    "0000014d","0000014e","0000014f","00000150","00000151","00000152","00000153","00000154",
    "00000155","00000156","00000158","00000159","0000015a","0000015b","0000015c","0000015d",
    "0000015e","0000015f","00000160","00000161","00000162","00000163","00000164","00000165",
    "00000166","00000167","00000168","00000169","0000016a","0000016b","0000016c","0000016d",
    "0000016e","0000016f","00000170","00000171","00000172","00000173","00000174","00000175",
    "00000176","00000177","00000178","00000179","0000017a","0000017b","0000017c","0000017d",
    "0000017e","0000017f","00000180","00000181","00000182","00000183","00000184","00000185",
    "00000186","00000187","00000188","00000189","0000018a","0000018b","0000018c","0000018d",
    "0000018e","0000018f","00000190","00000191","00000192","00000193","00000194","00000195",
    "00000196","00000197","00000198","00000199","0000019a","0000019b","0000019c","0000019d",
    "0000019e","0000019f","000001a0","000001a1","000001a2","000001a3","000001a4","000001a5",
    "000001a6","000001a7","000001a8","000001a9","000001aa","000001ab","000001ac","000001ad",
    "000001ae","000001af","000001b0","000001b1","000001b2","000001b5","000001b6","000001b7",
    "000001b9","000001e8","00000200","00000353","00000356","0000036a","00000433","00000443",
    "00000474","000004a4","00000879","0000225a","00003300","000096da","00010000","00010001",
    "00010100","00010101","00010200","0001ff00","0001fffc","00030000","00030804","00034f52",
    "00040301","0004524f","00054649","00067800","00077800","0009a500","000c0000","01000000",
    "01000001","01000100","01000700","01001762","01010000","01010100","01010101","01067800",
    "03646400","06067800","07780000","0c000000","1103ff01","2003ff01","2103ff01","2203ff01",
    "2a000101","2b000700","3803ff01","94060001","a1060001","a6060001","ae060001","c8060001"
};
std::vector<std::string> Bytes16_19 = {
    /*0003de8e,0003e317,0003e785,0003ec13,0003f0a2,0003f511,0003f998,0003fe0e,
    000401db,0004023d,000402a7,00040312,0004037a,000403ec,00040454,000404bf,
    0004053d,000405a2,00040625,000406af,00040720,000407aa,00040820,00040896,
    00040911,0004097c,000409f7,00040a63,00040aec,00040b64,00040bca,00040c40,
    00040cb4,00040d1b,00040d81,00040de9,00040e5a,00040edf,00040f56,00040fca,
    00041036,000410a7,0004110c,00041173,000411ec,00041258,000412c8,00041333,
    000413a3,00041409,00041417,000415a5,00041604,00041676,000416db,000416fd,
    0004190d,00041977,000419e3,00041a59,00041ad0,00041b44,00041bbc,00041c27,
    00041c95,00041d1c,00041d85,0004220f,002a0002,01010401,02006900,02006b07,
    02006b0c,02006b0d,02006b0e,02006b0f,02006b10,04006b0f,05007900,06006900,
    06007900,08006900,09006900,09007800,0a006900,0b006900,0c006900,10006900,
    11006900,13006900,f3040100,f3040300*/
};
std::vector<std::string> Bytes12_15 = {  };

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
// 4-7 command lenght
// 8-11 time since launch/location change ?
// 12-15 some id
// 16-18 changes rarely
// data
// last 4 command connector ?

// ap, disap - appearing, disappearing
// CD - corrupt dungeon
// SD - solo dungeon
// GD, CGD - group dungeon, crystal group dungeon
// RM - resource mob
// HG - hell gate
// ERM - empty resource mob
// DRM - dead resource mob
// distance of resource view is ~45-46.75 meters

// pen garn, brons hill - high mountain for testing
int counter;
size_t packetsNum;
size_t packetsFilteredNum;
std::vector<std::string> borderStrings = {"01000000","01ff0000","06000100","07000000","08000100"};

std::vector<std::string> eventCodes = {
    // resource mob  
    // 80, 82 RM ap 
    // 58(resources), 60(resources), 61(Tier < 4), 63(leather), 64(other resources) DRM ap, RM hitting, last 4 bytes determine difference  
    // 60(leather), 61 ERM ap          
    // 19, 6b ERM, DRM disap
    // 1b ERM, DRM, RM disap
    // 4b ERM, DRM ap/disap
    //"00000063","00000064"
    //"00000080","00000082"

    // player appearing
    // 41, 9b, 1a2, 3e, 19(not clean) player ap
    // 65, 6d, 3b, 1b player disap

    //mob spawn
    // 7a, 80, 81, 82 mob ap
    // 19, 6f mob disap
    // 1b, 3b(not clean), 3e mob ap/disap

    // solo dungeon 
    // 73, 71(with butts) SD ap
    // 19 SD disap
    // 1b, 3b(not clean), 82, 58(with butts) SD ap/disap
    //"0000003e","00000040","0000004b","0000006d","0000006f","0000007c"

    // corrupt dungeon
    // 7e, 76 CD ap
    // 19, 58, 82 CD disap
    // 3b, 3e, 1b CD ap/disap             

    // group dungeon
    // 61(with butts), 63(box), 65, 67, 7a, 7f GD ap
    // 1b, 72(clean), 4b(box), 60, 61, 3e, 18 GD disap
    // 19, 3b GD ap/disap
    //"00000061","00000063","00000065","00000067"

    // crystal group dungeon
    // 64, 66, 72, 82(entered players ?) CGD ap
    // 71, d5 disap
    // 58 CGD ap/disap

    // hell gate
    // 6c, 58 HG ap
    // 6d, 1b(not clean) HG disap
    // 3b(not clean),19(not clean) HG ap/disap

    // mist
    //"0000001f","00000024","0000002b","00000034","00000035","0000003b","00000041","00000048","0000004b","00000059","00000065","0000006f","00000082","00000096","000000f0","00000103"
    // 96 mist portal ap

    // fish node
    // 3c fish node ap
    // 3e fish node ap/disap

    // resource node
    //"00000066","0000006d","00000072","000000e6"
    // 40, 46(resources T4+ enchanted), 47(mob hitting), 58, 59, 5c, 5f, 60, 61, 63, 66(alive leather) ?
    // 6d,6f,81,82 empty resources
    //"00000066"
    //"00000025","00000034","00000037","00000039","0000003b","0000003f","00000043","00000047","00000054","00000059","0000005a","00000069","00000072","00000082","000000e6"
    /*"00000051","00000046","00000058","00000060","00000065","0000006a","00000072","00000073","0000007f","0000008c",
    /*"000000a2","000000c8","000000e3","0000014e"*/
    //"0000005c","0000005f"
    //"00000028","0000002b","00000037","00000039","0000003b","0000003d","0000003f","00000040","00000043",*/
    //"00000047","00000048","0000004b","0000005a","00000061","0000006c"
    //"0000002b","00000040","00000043","00000052","00000068","0000006d","00000071"
    /*"0000005c","0000005f","00000061","00000066"*/

    //"00000014","00000019","0000001b","0000001c","0000001e","0000001f","00000041",
    //"00000043","00000060","000c0000"
};
std::vector<std::vector<std::size_t>> commandLenghts;
std::vector<std::size_t> amountOfSameCommands;
std::vector<std::vector<std::string>> text;

class PacketAnalyze {
public:
    void run() 
    {
        initWindow();
        initSniffer();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;
    bool framebufferResized;

    IPv4Range albionIPRange = IPv4Range::from_mask("5.188.125.0", "5.188.125.255");
    NetworkInterface iface = NetworkInterface::default_interface();
    Sniffer sniffer = Sniffer(iface.name());

private:
    void mainLoop() 
    {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            sniffPacket();
        }
    }

    void cleanup() 
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void initWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);

        window = glfwCreateWindow(100, 100, "Packet Analyze", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        glfwSetKeyCallback(window, keyCallback);
        glfwSetWindowPos(window, 0, 40);
    }

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<PacketAnalyze*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
            if (counter < eventCodes.size() - 1) {
                counter += 1;
            }
            else if (counter == eventCodes.size() - 1) {
                counter = 0;
            }
            std::cout << "\n" << "<<" << eventCodes[counter] << ">>" << "\n";
        }
        if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
            if (counter > 0) {
                counter -= 1;
            }
            else if (counter == 0) {
                counter = eventCodes.size() - 1;
            }
            std::cout << "\n" << "<<" << eventCodes[counter] << ">>" << "\n";
        }
        if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    void initSniffer() 
    {
        SnifferConfiguration albionConfig;
        albionConfig.set_filter("ip dst 192.168.1.71");
        sniffer = Sniffer(iface.name(), albionConfig);
    }
    void sniffPacket()
    {
        try {
            PDU* sniffedPacket = sniffer.next_packet();
            if (sniffedPacket) {
                const IP& ip = sniffedPacket->rfind_pdu<IP>();
                const UDP& udp = sniffedPacket->rfind_pdu<UDP>();

                if (albionIPRange.contains(ip.src_addr()) and udp.sport() == 5056) {
                    RawPDU rawPacket = sniffedPacket->rfind_pdu<RawPDU>();
                    std::string packet;
                    readPacket(rawPacket, packet);

                    if (packet.length() > 0) {
                        analyzePacket(packet);
                    }
                }
            }
        }
        catch (std::exception& e) {
        }
    }

    void printPacket(std::string packet)
    {
        for (size_t i = 0; i < packet.length(); i++)
        {
            std::cout << std::hex << packet[i];
            if (i % 8 == 7)
                std::cout << " ";
            if (i % 32 == 31 and i != packet.length())
                std::cout << "\n";
        }
    }
    void printPacket(std::string packet, size_t regionStart, size_t regionEnd)
    {
        for (size_t i = regionStart; i < regionEnd; i++)
        {
            std::cout << std::hex << packet[i];
            if ((i - regionStart) % 8 == 7)
                std::cout << " ";
            if ((i - regionStart) % 32 == 31 and i != (packet.length() - 1))
                std::cout << "\n";
        }
    }
    void printPacketInOneString(std::string packet, size_t regionStart, size_t regionEnd)
    {
        for (size_t i = regionStart; i < regionEnd; i++) {
            std::cout << std::hex << packet[i];
        }
    }
    void readPacket(RawPDU pdu, std::string& string)
    {
        std::stringstream ss;

        for (size_t i = 0; i < pdu.payload_size(); i++) {
            uint8_t x = pdu.payload()[i];
            if (x < 16)
                ss << 0;
            ss << std::hex << (int)x;
        }

        string = ss.str();
    }
    void readPacket(RawPDU pdu, size_t regionStart, size_t regionEnd, std::string string)
    {
        std::stringstream ss;

        for (size_t i = regionStart; i < regionEnd; i++) {
            uint8_t x = pdu.payload()[i];
            if (x < 16) {
                ss << 0;
            }
            ss << std::hex << (int)x;
        }

        string = ss.str();
    }
    std::string readPacket(RawPDU pdu, size_t regionStart, size_t regionEnd)
    {
        std::stringstream ss;

        for (size_t i = regionStart; i < regionEnd; i++)
        {
            uint8_t x = pdu.payload()[i];
            if (x < 16)
                ss << 0;
            ss << std::hex << (int)x;
        }

        return ss.str();
    }
    std::vector<size_t> findCommandBordersInPacket(std::string packet, std::vector<std::string> borderStrings)
    {
        std::vector<size_t> commandBorders;

        for (size_t i = 24; i < (packet.length() - 16); i += 2)
        {
            for (size_t j = 0; j < borderStrings.size(); j++)
            {
                if (packet.substr(i, 8) == borderStrings[j])
                {
                    commandBorders.push_back(i);
                    i += 14;
                }
            }
        }
        commandBorders.push_back(packet.length());

        return commandBorders;
    }
    bool findStringInPacket(std::string packet, size_t regionStart, size_t regionEnd, std::string string)
    {
        for (size_t i = regionStart; i < regionEnd - string.length() + 1; i += 2) {
            if (packet.substr(i, string.length()) == string) {
                return true;
            }
        }

        return false;
    }
    bool findStringInPacket(std::string packet, size_t regionStart, size_t regionEnd, std::string string, 
        size_t &stringPosition)
    {
        for (size_t i = regionStart; i < regionEnd - string.length() + 1; i += 2) {
            if (packet.substr(i, string.length()) == string) {
                stringPosition = i;
                return true;
            }
        }

        return false;
    }

    std::vector<size_t> commandBorders;
    size_t stringPos = 0;
    bool analyzePacket(std::string packet) 
    {
        commandBorders = findCommandBordersInPacket(packet, borderStrings);

        for (size_t i = 0; i < commandBorders.size() - 1; i++) 
        {
            packetsNum += 1;
            //std::string eventCode = packet.substr(commandBorders[i] + 16, 8);
            //size_t commandLenght = commandBorders[i + 1] - commandBorders[i];

            //if (eventCode == eventCodes[counter]) 
            {
                //if ((commandBorders[i + 1] - commandBorders[i]) > 72) 
                {
                    if(findStringInPacket(packet, commandBorders[i], commandBorders[i + 1], "f3040300"))
                    {
                        packetsFilteredNum += 1;
                        ////std::string eventCode = packet.substr(commandBorders[i] + stringPos / 2 + 28, 8);

                        //if (!(std::find(std::begin(eventCodes), std::end(eventCodes),
                        //eventCode) != std::end(eventCodes)))
                        //{
                        //    eventCodes.push_back(eventCode);
                        //    text.push_back({});
                        //    amountOfSameCommands.push_back({});
                        //}
                        ////printPacket(packet, commandBorders[i] + 32, commandBorders[i + 1] - 8), std::cout << "\n";
                        //ptrdiff_t eventCodeIndex = std::distance(eventCodes.begin(),
                        //    std::find(eventCodes.begin(), eventCodes.end(), eventCode));
                        //amountOfSameCommands[eventCodeIndex] += 1;
                        ////if (text[eventCodeIndex].size() < 10)
                        //{
                        //    text[eventCodeIndex].push_back(packet.substr(commandBorders[i],
                        //        commandBorders[i + 1] - commandBorders[i]));
                        //}
                    }
                }
            }

            /*if (!(std::find(std::begin(eventCodes), std::end(eventCodes),
                eventCode) != std::end(eventCodes)))
            {   
                eventCodes.push_back(eventCode);
                commandLenghts.push_back({});
            }

            ptrdiff_t eventCodeIndex = std::distance(eventCodes.begin(),
                    std::find(eventCodes.begin(), eventCodes.end(), eventCode));

            if (!(std::find(commandLenghts[eventCodeIndex].begin(), commandLenghts[eventCodeIndex].end(),
                commandLenght) != commandLenghts[eventCodeIndex].end()))
            {
                commandLenghts[eventCodeIndex].push_back(commandLenght);
            }*/

        return true;
        }
    }
};

std::vector<bool> findSameSymbolsInText(std::vector<std::string> text)
{
    std::vector<bool> sameSymbols = {};

    for (size_t i = 0; i < text[0].length(); i++) {
        int symbolSimilarity = 1;

        for (size_t j = 1; j < text.size(); j++) {
            if (text[j][i] == text[0][i]) {
                symbolSimilarity += 1;
            }
        }
        sameSymbols.push_back(floor((float)symbolSimilarity / text.size()));
    }

    return sameSymbols;
}

void colorizeSameText(std::vector<std::string> text, HANDLE consoleHandle)
{
    std::vector<bool> sameSymbols = findSameSymbolsInText(text);

    for (size_t i = 0; i < sameSymbols.size(); i++) {
        if (sameSymbols[i] == 1) {
            SetConsoleTextAttribute(consoleHandle, 2);
        }

        std::cout << sameSymbols[i];
        SetConsoleTextAttribute(consoleHandle, 7);
        if (i % 8 == 7) {
            std::cout << " ";
        }
        if (i % 32 == 31 and i != (sameSymbols.size() - 1)) {
            std::cout << "\n";
        }
    }
    std::cout << "\n";

    for (size_t i = 0; i < text.size(); i++) {
        for (size_t j = 0; j < text[i].length(); j++) {
            if (sameSymbols[j] == 1) {
                SetConsoleTextAttribute(consoleHandle, 2);
            }

            std::cout << text[i][j];
            SetConsoleTextAttribute(consoleHandle, 7);
            if (j % 8 == 7) {
                std::cout << " ";
            }
            if (j % 32 == 31 and j != (text[i].length() - 1)) {
                std::cout << "\n";
            }
        }
        std::cout << "\n";
    }
}

void outputColorizedPackets() 
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    for (size_t i = 0; i < text.size(); i++) {
        SetConsoleTextAttribute(consoleHandle, 6);
        std::cout << "\"" << eventCodes[i] << "\"" << " " << amountOfSameCommands[i] << "\n";
        SetConsoleTextAttribute(consoleHandle, 7);

        if (text[i].size() > 1) {
            colorizeSameText(text[i], consoleHandle);
        }
        
    }
}

int main() {
    PacketAnalyze packetAnalyze;
    packetAnalyze.run();

    std::cout << (float)packetsNum / packetsFilteredNum;
    //outputColorizedPackets();

    /*for (size_t i = 0; i < commandLenghts.size(); i++) {
        std::cout << eventCodes[i] << " ";
        for (size_t j = 0; j < commandLenghts[i].size(); j++){
            std::cout << std::dec << commandLenghts[i][j] / 2 << " ";
        }
        std::cout << "\n";
    }*/


    return 0;
}