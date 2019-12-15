#ifndef USBDEFINITIONS_H
#define USBDEFINITIONS_H

#include <sys/attribs.h>

#define ASSERT(c) typedef char p__LINE__[(c)?1:-1]

#define	TOTAL_LEN	0x43

#define	W(w) ((w) & 0xFF), ((w) >> 8)

/********************************************************************
 ** Device Descriptor						   **
 *********************************************************************/
const unsigned char dev_desc[] = {
    0x12,       // bLength: 18
    0x01,       // bDescriptorType: DEVICE
    0x00, 0x02, // bcdUSB: (ver 2.0)
    0x02,       // bDeviceClass: Comm
    0x00,       // bDeviceSubClass: unused
    0x00,       // bDeviceProtocol: unused
    0x40,       // bMaxPacketSize0: 64
    0x66, 0x66, // idVendor: 0x6666
    0x34, 0x12, // idProduct: 0x1234
    0x00, 0x01, // bcdDevice: 01.00
    0x01,       // iManufacturer
    0x02,       // iProduct
    0x03,       // iSerialNumber
    0x01        // bNumConfigurations
};

ASSERT(sizeof(dev_desc) == 0x12);

/********************************************************************
 ** Config Descriptor						   **
 *********************************************************************/
const unsigned char conf_desc[] = {
    0x09,         // bLength: 9
    0x02,         // bDescriptorType: CONFIG
    W(TOTAL_LEN), // wTotalLength: all descriptors
    0x02,         // bNumInterfaces
    0x01,         // bConfigurationValue
    0x04,         // iConfiguration
    0x80,         // bmAttributes: bus powered
    0xc8,         // bMaxPower: 400mA max

    /********************************************************************
     ** CDC Communication Interface Descriptor			   **
     *********************************************************************/

    0x09, // bLength: 9
    0x04, // bDescriptorType: INTERFACE
    0x00, // bInterfaceNumber
    0x00, // bAlternateSetting
    0x01, // bNumEndpoints
    0x02, // bInterfaceClass: Comm
    0x02, // bInterfaceSubClass: ACM
    0x00, // bInterfaceProtocol: none
    0x00, // iInterface

    /********************************************************************
     ** CDC Functional Descriptor					   **
     *********************************************************************/

    0x05,       // bLength: 5
    0x24,       // bDescriptorType: CS_INTERFACE
    0x00,       // bDescriptorSubtype: HEADER
    0x10, 0x01, // bcdCDC

    /********************************************************************
     ** ACM Functional Descriptor					   **
     *********************************************************************/

    0x04, // bLength: 4
    0x24, // bDescriptorType: CS_INTERFACE
    0x02, // bDescriptorSubtype: ACM
    0x02, // bmCapabilities

    /********************************************************************
     ** Union Functional Descriptor					   **
     *********************************************************************/

    0x05, // bLength: 5
    0x24, // bDescriptorType: CS_INTERFACE
    0x06, // bDescriptorSubtype: UNION
    0x00, // bControlInterface: 0
    0x01, // bSubordinateInterface0: 1

    /********************************************************************
     ** Call Management Functional Descriptor			   **
     *********************************************************************/

    0x05, // bLength: 5
    0x24, // bDescriptorType: CS_INTERFACE
    0x01, // bDescriptorsubtype: CM
    0x03, // bmCapabilities: DIY
    0x01, // bDataInterface: 1

    /********************************************************************
     ** Notification Endpoint Descriptor				   **
     *********************************************************************/

    0x07,       // bLength: 7
    0x05,       // bDescriptorType: ENDPOINT
    0x81,       // bEndpointAddress
    0x03,       // bmAttributes
    0x40, 0x00, // wMaxPacketSize: 64
    0x08,       // bInterval: 8

    /********************************************************************
     ** CDC Data Interface Descriptor				   **
     *********************************************************************/

    0x09, // bLength: 9
    0x04, // bDescriptorType: INTERFACE
    0x01, // bInterfaceNumber
    0x00, // bAlternateSetting
    0x02, // bNumEndpoints
    0x0A, // bInterfaceClass: Data
    0x00, // bInterfaceSubClass: unused
    0x00, // bInterfaceProtocol: none
    0x00, // iInterface

    /********************************************************************
     ** Data OUT Endpoint Descriptor					   **
     *********************************************************************/

    0x07,       // bLength: 7
    0x05,       // bDescriptorType: ENDPOINT
    0x02,       // bEndpointAddress
    0x02,       // bmAttributes
    0x00, 0x02, // wMaxPacketSize: 512
    0x01,       // bInterval: 1

    /********************************************************************
     ** Data IN Endpoint Descriptor					   **
     *********************************************************************/

    0x07,       // bLength: 7
    0x05,       // bDescriptorType: ENDPOINT
    0x83,       // bEndpointAddress
    0x02,       // bmAttributes
    0x00, 0x02, // wMaxPacketSize: 512
    0x01        // bInterval: 1
};

ASSERT(sizeof(conf_desc) == TOTAL_LEN); // wTotalLength

#endif //USBDEFINITIONS_H
