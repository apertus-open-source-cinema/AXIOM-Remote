#ifndef ICSPROGRAMMER_H
#define ICSPROGRAMMER_H

#include <stdint.h>

// Added 0x20 to get PDAT change at the end of command transfer
enum class ICSPCommand : uint8_t
{
    LoadConfiguration = 0x0,
    LoadDataForProgMem = 0x2,
    ReadDataFromProgMem = 0x4,
    IncrementAddress = 0x6,
    BeginInternallyTimedProg = 0x8,
    BulkEraseProgMem = 0x9,
    ResetAddress = 0x16
};

enum class KeyManager
{
    East,
    West
};

class ICSProgrammer
{
    uint32_t MCLR_PIN;
    uint32_t PCLK_PIN;
    uint32_t PDAT_OUT_PIN;
    uint32_t PDAT_IN_PIN;

    void TransferBit(uint8_t value);
    bool ReceiveBit();

    void Send(uint32_t data, uint8_t length);
    uint16_t Read();
    
    void SetupEastKM();
    void SetupWestKM();    

public:
    ICSProgrammer();
    ~ICSProgrammer();

    void SelectKM(KeyManager device);

    // LVP -> Low Voltage Programming
    void EnterLVP();
    void ExitLVP();

    void SendCommand(ICSPCommand command, uint16_t value = 0);
    uint16_t Receive();
};

#endif //ICSPROGRAMMER_H
