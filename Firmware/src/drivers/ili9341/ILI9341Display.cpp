#include "ILI9341Display.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include <hardware/clocks.h>
#include <hardware/dma.h>
#include <hardware/gpio.h>
#include <hardware/pio.h>
#include <pico.h>
#include <pico/stdlib.h>
#include <pico/time.h>

#include "pargen.pio.h" // PIO program for parallel 16bit (8wires) ili9341 communication

PIO pio                                      = pio0;
uint sm                                      = 0;
uint offset                                  = 0;
static constexpr uint32_t TE_WAIT_TIMEOUT_US = 30000;

const uint8_t initcmd[] = {
    // clang-format off
    // ILI9341_SWRESET, 0x80,                // Software Reset, 120ms delay
    ILI9341_PWCTR1, 1, 0x23,             // Power Control 1, VRH ~4.5V for 3.3V
    ILI9341_PWCTR2, 1, 0x10,             // Power Control 2
    ILI9341_VMCTR1, 2, 0x3E, 0x28,       // VCOM Control 1, tuned for 3.3V
    ILI9341_VMCTR2, 1, 0x86,       // VCOM Control 1, tuned for 3.3V
    // ILI9341_FRMCTR1, 2, 0x00, 0x1E,      // Frame Rate Control, ~60 Hz
    ILI9341_MADCTL, 1, 0x48,             // Memory Access Control, Landscape, BGR (use 0x68 for right-to-left if mirrored)
    ILI9341_PIXFMT, 1, 0x55,             // Pixel Format, 16-bit RGB565
    0x35, 1, 0x00,                       // Tearing Effect ON, V-blanking (remove if no TE pin)
    ILI9341_SLPOUT, 0 | 0x80,         // Sleep Out, 0 args + 120ms delay flag
    ILI9341_DISPON, 0 | 0x80,         // Display ON, 0 args + 120ms delay flag
    0x00                                  // End of list
    // clang-format on
};

ILI9341Display::ILI9341Display() :
    pio_initialized(false), currentWidth(ILI9341_TFTWIDTH), currentHeight(ILI9341_TFTHEIGHT),
    tx_dma_chan(dma_claim_unused_channel(true))
{
    if (tx_dma_chan < 0) {
        printf("FATAL: Failed to claim DMA channel!\n");
        return;
    }
}

ILI9341Display::~ILI9341Display()
{
    if (tx_dma_chan >= 0) {
        dma_channel_abort(tx_dma_chan);
        dma_channel_unclaim(tx_dma_chan);
    }

    if (pio_initialized && pio_sm_is_claimed(pio, sm)) {
        pio_sm_set_enabled(pio, sm, false);
        pio_remove_program(pio, &pargen_program, offset);
        pio_sm_unclaim(pio, sm);
    }
}

void ILI9341Display::Init()
{
    if (pio_initialized) {
        return;
    }
    if (tx_dma_chan < 0) {
        printf("Error: Cannot initialize display, DMA channel not available.\n");
        return;
    }

    printf("ILI9341 Parallel PIO Init (Using PIO%d, SM%d)\n", pio_get_index(pio), sm);

    gpio_init(ILI_PIN_RST);
    gpio_set_dir(ILI_PIN_RST, GPIO_OUT);

    gpio_init(ILI_PIN_TE);
    gpio_set_dir(ILI_PIN_TE, GPIO_IN);
    gpio_pull_down(ILI_PIN_TE);

    pio_sm_claim(pio, sm);
    offset = pio_add_program(pio, &pargen_program);
    pargen_program_init(pio, sm, offset, ILI_PIN_DB0, 8, ILI_PIN_RD, 8.0f);
    pio_sm_set_enabled(pio, sm, true);

    tx_dma_cfg = dma_channel_get_default_config(tx_dma_chan);

    channel_config_set_transfer_data_size(&tx_dma_cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&tx_dma_cfg, true);
    channel_config_set_write_increment(&tx_dma_cfg, false);
    channel_config_set_dreq(&tx_dma_cfg, pio_get_dreq(pio, sm, true));

    pio_initialized = true;
    printf("PIO and DMA Initialized.\n");

    uint8_t buf[5] = {0};
    ReadBytes(0x09, buf, 5);
    printf("Read Display Status: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", buf[0], buf[1], buf[2], buf[3], buf[4]);

    ResetDisplay();
    ExecuteInitSequence();

    memset(buf, 0, sizeof(buf));
    ReadBytes(0x09, buf, 5);
    printf("Read Display Status: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", buf[0], buf[1], buf[2], buf[3], buf[4]);

    SetOrientation(Orientation::LANDSCAPE_90);

    printf("ILI9341 Initialization Complete.\n");
}

void ILI9341Display::FillScreenRaw(uint16_t color)
{
    if (!pio_initialized) {
        printf("FillScreenRaw: PIO not initialized!\n");
        return;
    }
    printf("ILI9341Display::FillScreenRaw with color 0x%04X\n", color);

    SetArea(0, 0, currentWidth, currentHeight);

    uint32_t byte_count = (uint32_t)currentWidth * currentHeight * 2;

    if (dma_channel_is_busy(tx_dma_chan)) {
        dma_channel_wait_for_finish_blocking(tx_dma_chan);
    }

    pargen_data_dma(pio, sm, offset, tx_dma_chan, &tx_dma_cfg, &color, byte_count, false);

    printf("ILI9341Display::FillScreenRaw done.\n");
}

void ILI9341Display::ExecuteInitSequence()
{
    uint8_t cmd, num_args_raw;
    const uint8_t* addr = initcmd;
    std::vector<uint16_t> params_for_write_command;

    printf("CMD: SWRESET (0x01)\n");
    WriteCommand(ILI9341_SWRESET);
    sleep_ms(150);

    while (true) {
        cmd = *addr++;
        if (cmd == 0x00) {
            break;
        }

        num_args_raw            = *addr++;
        bool delay_after        = (num_args_raw & 0x80);
        uint8_t num_actual_args = num_args_raw & ~0x80;

        printf("CMD: 0x%02X, NumArgs: %d", cmd, num_actual_args);
        if (delay_after) {
            printf(" (Delay After)");
        }

        params_for_write_command.clear();
        if (num_actual_args > 0) {
            printf(", Params: ");
            params_for_write_command.reserve(num_actual_args);
            for (uint8_t i = 0; i < num_actual_args; ++i) {
                uint8_t param_val = *(addr + i);
                printf("0x%02X ", param_val);
                params_for_write_command.push_back(static_cast<uint16_t>(param_val));

                if (cmd == ILI9341_MADCTL && i == 0) {
                    if (param_val & 0x20) {
                        currentWidth  = ILI9341_TFTHEIGHT;
                        currentHeight = ILI9341_TFTWIDTH;
                    }
                    else { // Portrait
                        currentWidth  = ILI9341_TFTWIDTH;
                        currentHeight = ILI9341_TFTHEIGHT;
                    }
                    printf(" (MADCTL processed, W:%d H:%d)", currentWidth, currentHeight);
                }
            }
            addr += num_actual_args;
            WriteCommand(cmd, params_for_write_command.data(), num_actual_args);
        }
        else {
            printf("(none)");
            WriteCommand(cmd);
        }
        printf("\n");

        if (delay_after) {
            uint16_t delay_ms_val = 5;
            if (cmd == ILI9341_SLPOUT || cmd == ILI9341_DISPON) {
                delay_ms_val = 120;
            }
            sleep_ms(delay_ms_val);
            printf("Delay %dms executed.\n", delay_ms_val);
        }
    }

    printf("Initcmd array processing finished.\n");
    printf("ExecuteInitSequence finished.\n");
}

void ILI9341Display::ResetDisplay()
{
    printf("Hardware Reset...\n");
    gpio_put(ILI_PIN_RST, 0);
    sleep_ms(10);
    gpio_put(ILI_PIN_RST, 1);
    sleep_ms(120);
    printf("Hardware Reset Complete.\n");
}

void ILI9341Display::ReadBytes(uint8_t command, uint8_t* buffer, size_t len)
{
    if (!pio_initialized || len == 0 || !buffer) {
        return;
    }

    size_t num_words = (len + 3) / 4;
    std::vector<uint16_t> read_data(num_words);

    printf("Reading %d bytes for CMD: 0x%02X\n", len, command);
    pargen_cmd_rdat(pio, sm, offset, command, read_data.data(), len);

    for (size_t i = 0; i < len; ++i) {
        buffer[i] = (read_data[i / 4] >> (8 * (i % 4))) & 0xFF;
    }
    printf("Read data: ");
    for (size_t i = 0; i < len; ++i)
        printf("0x%02X ", buffer[i]);
    printf("\n");
}

void ILI9341Display::WriteCommand(uint8_t command, const uint16_t* parameters, size_t len)
{
    if (!pio_initialized) {
        return;
    }
    if (len == 0) {
        ili9341_cmd(pio, sm, offset, command);
    }
    else if (len == 1) {
        ili9341_cmd_arg(pio, sm, offset, command, parameters[0]);
    }
    else {
        pargen_cmd_wdat(pio, sm, offset, command, parameters, len);
    }
}

void ILI9341Display::SetArea(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if (!pio_initialized || width == 0 || height == 0) {
        return;
    }

    x              = std::min(x, (uint16_t)(currentWidth - 1));
    y              = std::min(y, (uint16_t)(currentHeight - 1));
    uint16_t x_end = x + width - 1;
    uint16_t y_end = y + height - 1;
    x_end          = std::min(x_end, (uint16_t)(currentWidth - 1));
    y_end          = std::min(y_end, (uint16_t)(currentHeight - 1));

    ili9341_cmd_caset(pio, sm, offset, x, x_end);
    ili9341_cmd_paset(pio, sm, offset, y, y_end);

    ili9341_cmd(pio, sm, offset, ILI9341_RAMWR);
}

void ILI9341Display::SetOrientation(Orientation orientation)
{
    if (!pio_initialized) {
        return;
    }

    uint16_t data = static_cast<uint16_t>(orientation);
    WriteCommand(ILI9341_MADCTL, &data, 1);

    if (data & 0x28) {
        currentWidth  = ILI9341_TFTHEIGHT;
        currentHeight = ILI9341_TFTWIDTH;
    }
    else {
        currentWidth  = ILI9341_TFTWIDTH;
        currentHeight = ILI9341_TFTHEIGHT;
    }

    printf("Orientation set (MADCTL=0x%02X). Width=%d, Height=%d\n", data, currentWidth, currentHeight);
}

void ILI9341Display::DrawPixmap(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t* pixmap)
{
    uint32_t num_pixels         = (uint32_t)width * height;
    uint32_t dma_transfer_count = num_pixels * 2;

    SetArea(x, y, width, height);

    pargen_data_dma(pio, sm, offset, tx_dma_chan, &tx_dma_cfg, (const uint16_t*)pixmap, dma_transfer_count, true);

    while (!pio_sm_is_tx_fifo_empty(pio, sm)) {
        tight_loop_contents();
    }
}

void ILI9341Display::ReadDisplayID(uint8_t* buffer)
{
    ReadBytes(ILI9341_RDDID, buffer, 4);
}

void ILI9341Display::ReadDisplayStatus(uint8_t* buffer)
{
    ReadBytes(ILI9341_RDDST, buffer, 5);
}

bool wait_for_gpio_state(uint pin, bool target_state, uint32_t timeout_us)
{
    uint32_t start = time_us_32();
    while (gpio_get(pin) != target_state) {
        if (time_us_32() - start > timeout_us) {
            return false;
        }
        sleep_us(5);
    }
    return true;
}

uint32_t ILI9341Display::MeasureTEInterval()
{
    if (!pio_initialized) {
        printf("MeasureTEInterval: PIO not initialized!\n");
        return 0;
    }

    printf("Measuring TE interval...\n");
    uint32_t t1_us = 0, t2_us = 0;

    // 1. Wait for TE to go LOW (sync to end of current pulse)
    if (!wait_for_gpio_state(ILI_PIN_TE, 0, TE_WAIT_TIMEOUT_US * 2)) {
        printf("TE Interval: Timeout waiting for first LOW.\n");
        return 0;
    }

    // 2. Wait for TE to go HIGH (first rising edge)
    if (!wait_for_gpio_state(ILI_PIN_TE, 1, TE_WAIT_TIMEOUT_US)) {
        printf("TE Interval: Timeout waiting for first HIGH.\n");
        return 0;
    }
    t1_us = time_us_32();
    printf("TE Pulse 1 detected at %lu us\n", t1_us);

    // 3. Wait for TE to go LOW again
    if (!wait_for_gpio_state(ILI_PIN_TE, 0, TE_WAIT_TIMEOUT_US)) {
        printf("TE Interval: Timeout waiting for second LOW.\n");
        return 0;
    }

    // 4. Wait for TE to go HIGH again (second rising edge)
    if (!wait_for_gpio_state(ILI_PIN_TE, 1, TE_WAIT_TIMEOUT_US)) {
        printf("TE Interval: Timeout waiting for second HIGH.\n");
        return 0;
    }
    t2_us = time_us_32();
    printf("TE Pulse 2 detected at %lu us\n", t2_us);

    uint32_t interval_us = t2_us - t1_us;
    printf("Measured TE Interval: %lu us (%.2f ms, %.2f Hz)\n", interval_us, interval_us / 1000.0f,
           1000000.0f / (float)interval_us);

    return interval_us;
}