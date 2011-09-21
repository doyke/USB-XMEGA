// Minimal USB Stack for ATxmega32a4u and related
// (C) 2011 Kevin Mehall (Nonolith Labs) <km@kevinmehall.net>
//
// Heavily borrows from LUFA
// Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)
//
// Licensed under the terms of the GNU GPLv3+

#pragma once

#define USB_CAN_BE_DEVICE
#define ARCH_XMEGA 2
#if !defined(ARCH)
	#define ARCH        ARCH_XMEGA
#endif

#define CPU_TO_LE16(x) x

struct USB_Request_Header;
typedef struct USB_Request_Header USB_Requst_Header_t;

#include "StdDescriptors.h"
#include "StdRequestType.h"
#include "Events.h"

#ifndef USB_MAXEP
	#define USB_MAXEP 1;
#endif

#ifndef USB_EP0SIZE
	#define EP0SIZE 64
#endif

typedef struct USB_EP_pair{
	USB_EP_t out;
	USB_EP_t in;
} ATTR_PACKED USB_EP_pair_t;

extern uint8_t ep0_buf_in[USB_EP0SIZE];
extern uint8_t ep0_buf_out[USB_EP0SIZE];
extern USB_EP_pair_t endpoints[USB_MAXEP];

/* Enums: */
	/** Enum for the various states of the USB Device state machine. Only some states are
	 *  implemented in the LUFA library - other states are left to the user to implement.
	 *
	 *  For information on each possible USB device state, refer to the USB 2.0 specification.
	 *
	 *  \see \ref USB_DeviceState, which stores the current device state machine state.
	 */
	enum USB_Device_States_t
	{
		DEVICE_STATE_Unattached                   = 0, /**< Internally implemented by the library. This state indicates
		                                                *   that the device is not currently connected to a host.
		                                                */
		DEVICE_STATE_Powered                      = 1, /**< Internally implemented by the library. This state indicates
		                                                *   that the device is connected to a host, but enumeration has not
		                                                *   yet begun.
		                                                */
		DEVICE_STATE_Default                      = 2, /**< Internally implemented by the library. This state indicates
		                                                *   that the device's USB bus has been reset by the host and it is
		                                                *   now waiting for the host to begin the enumeration process.
		                                                */
		DEVICE_STATE_Addressed                    = 3, /**< Internally implemented by the library. This state indicates
		                                                *   that the device has been addressed by the USB Host, but is not
		                                                *   yet configured.
		                                                */
		DEVICE_STATE_Configured                   = 4, /**< May be implemented by the user project. This state indicates
		                                                *   that the device has been enumerated by the host and is ready
		                                                *   for USB communications to begin.
		                                                */
		DEVICE_STATE_Suspended                    = 5, /**< May be implemented by the user project. This state indicates
		                                                *   that the USB bus has been suspended by the host, and the device
		                                                *   should power down to a minimal power level until the bus is
		                                                *   resumed.
		                                                */
	};
	
extern volatile uint8_t USB_DeviceState;
extern volatile uint8_t USB_Device_ConfigurationNumber;



/** Configure the XMEGA's clock for use with USB.  */
void USB_ConfigureClock(void);

/** Initialize USB functionality */
void USB_Init(void);
void USB_ResetInterface(void);
void USB_ep_send_packet(uint8_t endpoint, uint16_t size);
void USB_sendFromFlash(uint8_t endpoint, const uint8_t* addr, uint16_t size);
void USB_Task(void);


bool USB_HandleSetup(void);

uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint8_t wIndex,
                                    const void** const DescriptorAddress);

/** Detaches the device from the USB bus. This has the effect of removing the device from any
 *  attached host, ceasing USB communications. If no host is present, this prevents any host from
 *  enumerating the device once attached until \ref USB_Attach() is called.
 */
static inline void USB_Detach(void) ATTR_ALWAYS_INLINE;
static inline void USB_Detach(void)
{
	USB.CTRLB &= ~USB_ATTACH_bm;
}

/** Attaches the device to the USB bus. This announces the device's presence to any attached
 *  USB host, starting the enumeration process. If no host is present, attaching the device
 *  will allow for enumeration once a host is connected to the device.
 */
static inline void USB_Attach(void) ATTR_ALWAYS_INLINE;
static inline void USB_Attach(void)
{
	USB.CTRLB |= USB_ATTACH_bm;
}

#define USB_EP_size_to_gc(x)  ((x <= 8   )?USB_EP_BUFSIZE_8_gc:\
                               (x <= 16  )?USB_EP_BUFSIZE_16_gc:\
                               (x <= 32  )?USB_EP_BUFSIZE_32_gc:\
                               (x <= 64  )?USB_EP_BUFSIZE_64_gc:\
                               (x <= 128 )?USB_EP_BUFSIZE_128_gc:\
                               (x <= 256 )?USB_EP_BUFSIZE_256_gc:\
                               (x <= 512 )?USB_EP_BUFSIZE_512_gc:\
                                           USB_EP_BUFSIZE_1023_gc)
                               

