#include "Descriptors.h"

USB_ClassInfo_CDC_Device_t console_interface = {
  .Config = {
    .ControlInterfaceNumber = 0,
    .DataINEndpoint = {
      .Address = CDC_TX_EPADDR,
      .Size    = CDC_TX_EPSIZE,
      .Banks   = 2,
    },
    .DataOUTEndpoint = {
      .Address = CDC_RX_EPADDR,
      .Size    = CDC_RX_EPSIZE,
      .Banks   = 1,
    },
    .NotificationEndpoint = {
      .Address = CDC_NOTIFICATION_EPADDR,
      .Size    = CDC_NOTIFICATION_EPSIZE,
      .Banks   = 1,
    },
  },
};
