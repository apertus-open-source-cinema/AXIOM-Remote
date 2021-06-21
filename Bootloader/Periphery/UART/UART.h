#ifndef UART_H
#define UART_H

#include <stdint.h>

// NOTE: Using UART2 only at the moment
// NOTE: Removed destructor, as it's not required

using uartCallbackFunc = void (*)(volatile uint8_t* command, uint8_t seqNo, uint8_t chip);

class UART
{
  public:
    UART();

    void Initialize(uint32_t baudRate);

    void SendText(const char* message) const;

    void SendHex(uint8_t value) const;

    void SendByte(uint8_t value) const;

    void SetCallback(uartCallbackFunc callback);

    void Process();

    bool IsDataAvailable();

    unsigned char* GetData();
};

#endif // UART_H
