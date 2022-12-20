/****************************************************************************************************************************
  AsyncWebServer_ESP32_SC_ENC.cpp - Dead simple Ethernet AsyncWebServer.

  For LwIP ENC28J60 Ethernet in ESP32_SC_ENC (ESP32_S2/3, ESP32_C3 + LwIP ENC28J60)

  AsyncWebServer_ESP32_SC_ENC is a library for the LwIP Ethernet ENC28J60 in ESP32_S2/S3/C3 to run AsyncWebServer

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_ENC
  Licensed under GPLv3 license

  Original author: Hristo Gochkov

  Copyright (c) 2016 Hristo Gochkov. All rights reserved.

  This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with this library;
  if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Version: 1.7.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.6.3   K Hoang      15/12/2022 Initial porting for ENC28J60 + ESP32_S3. Sync with AsyncWebServer_ESP32_ENC v1.6.3
  1.7.0   K Hoang      19/12/2022 Add support to ESP32_S2_ENC (ESP32_S2 + LwIP ENC28J60)
 *****************************************************************************************************************************/

#include "AsyncWebServer_ESP32_SC_ENC.h"

bool ESP32_ENC_eth_connected = false;

/////////////////////////////////////////////////

void ESP32_ENC_onEvent()
{
  WiFi.onEvent(ESP32_ENC_event);
}

/////////////////////////////////////////////////

void ESP32_ENC_waitForConnect()
{
  while (!ESP32_ENC_eth_connected)
    delay(100);
}

/////////////////////////////////////////////////

bool ESP32_ENC_isConnected()
{
  return ESP32_ENC_eth_connected;
}

/////////////////////////////////////////////////

void ESP32_ENC_event(WiFiEvent_t event)
{
  switch (event)
  {
#if ( ( defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 2) ) && ( ARDUINO_ESP32_GIT_VER != 0x46d5afb1 ) )

    // For breaking core v2.0.0
    // Why so strange to define a breaking enum arduino_event_id_t in WiFiGeneric.h
    // compared to the old system_event_id_t, now in tools/sdk/esp32/include/esp_event/include/esp_event_legacy.h
    // You can preserve the old enum order and just adding new items to do no harm
    case ARDUINO_EVENT_ETH_START:
      AWS_LOG(F("\nETH Started"));
      //set eth hostname here
      ETH.setHostname("ESP32-ENC");
      break;

    case ARDUINO_EVENT_ETH_CONNECTED:
      AWS_LOG(F("ETH Connected"));
      break;

    case ARDUINO_EVENT_ETH_GOT_IP:
      if (!ESP32_ENC_eth_connected)
      {
        AWS_LOG3(F("ETH MAC: "), ETH.macAddress(), F(", IPv4: "), ETH.localIP());

        if (ETH.fullDuplex())
        {
          AWS_LOG0(F("FULL_DUPLEX, "));
        }
        else
        {
          AWS_LOG0(F("HALF_DUPLEX, "));
        }

        AWS_LOG1(ETH.linkSpeed(), F("Mbps"));

        ESP32_ENC_eth_connected = true;
      }

      break;

    case ARDUINO_EVENT_ETH_DISCONNECTED:
      AWS_LOG("ETH Disconnected");
      ESP32_ENC_eth_connected = false;
      break;

    case ARDUINO_EVENT_ETH_STOP:
      AWS_LOG("\nETH Stopped");
      ESP32_ENC_eth_connected = false;
      break;

#else

    // For old core v1.0.6-
    // Core v2.0.0 defines a stupid enum arduino_event_id_t, breaking any code for ESP32_ENC written for previous core
    // Why so strange to define a breaking enum arduino_event_id_t in WiFiGeneric.h
    // compared to the old system_event_id_t, now in tools/sdk/esp32/include/esp_event/include/esp_event_legacy.h
    // You can preserve the old enum order and just adding new items to do no harm
    case SYSTEM_EVENT_ETH_START:
      AWS_LOG(F("\nETH Started"));
      //set eth hostname here
      ETH.setHostname("ESP32-ENC28J60");
      break;

    case SYSTEM_EVENT_ETH_CONNECTED:
      AWS_LOG(F("ETH Connected"));
      break;

    case SYSTEM_EVENT_ETH_GOT_IP:
      if (!ESP32_ENC_eth_connected)
      {
        AWS_LOG3(F("ETH MAC: "), ETH.macAddress(), F(", IPv4: "), ETH.localIP());

        if (ETH.fullDuplex())
        {
          AWS_LOG0(F("FULL_DUPLEX, "));
        }
        else
        {
          AWS_LOG0(F("HALF_DUPLEX, "));
        }

        AWS_LOG1(ETH.linkSpeed(), F("Mbps"));

        ESP32_ENC_eth_connected = true;
      }

      break;

    case SYSTEM_EVENT_ETH_DISCONNECTED:
      AWS_LOG("ETH Disconnected");
      ESP32_ENC_eth_connected = false;
      break;

    case SYSTEM_EVENT_ETH_STOP:
      AWS_LOG("\nETH Stopped");
      ESP32_ENC_eth_connected = false;
      break;
#endif

    default:
      break;
  }
}


