# KNX_IP_WiFi Product Research：
  NX_IP --> standardized KNX multicast or tunneling protocol 
Pros:
  1. Still ETS programmable configuration support;
  2. Still very stable as KNX_TP products;
  3. More freedom, without KNX_TP chip limitation in stock;
  4. Low cost, without KNX_TP IC dependency and KNX cable installation;
  5. Easy installation without KNX-specified Power Supply and cable;
  6. Compatible (and easy upgrade/extension) with the existing KNX system including KNX_TP, KNX_RF, and KNX_IP;
  7. Almost zero learning curve if you have the experience with KNX system.
  
Constrain:
  1. Strongly dependent on the quality of the network as KNX_IP;
  2. The nature of IP multicast connection reliability on the transport layer.

# KNX_Sonoff_4Ch_Pro_Rev2
ETS programmable/configurable Firmware for ESP8266 MCU platform based on the Sonoff S20 example in thelsings KNX-Library.

How to Use:
- Use PlatformIO to compile and upload to device based on ESP8266 MCU platform. 
- You can solder RX, TX, 3v3 and GND directly to the corresponding pads of the extension PCB before flashing the FW.
- Pay attention to not connect the device to mains while using external connections.
- After removing the temporary connections and connecting the device to mains you can connect to the WiFi-Network called "MT_Switch_4Ch".
- Set up your WiFi Connection via the webpage (more information https://github.com/tzapu/WiFiManager).
- Now it´s possible to programm the device via ETS
- To enter the programming mode: press the KNX Prog Button for > 1 second then the KNX Prog Led will turn on.

How to Generate the .knxprod file:
- SignKnxProd (https://github.com/basilfx/SignKnxProd)
- KNXprodEditor (https://github.com/knxprodeditor/KNXprodEditor)
- SignKnxProd (https://github.com/KARDUINO/SignKnxProd)
- multiply-channels (https://github.com/mumpf/multiply-channels)
- OpenKNXproducer (https://github.com/OpenKNX/OpenKNXproducer)

# Hardware 
- Sonoff 4Ch Pro Rev2
- NodeMCU_DevKit_V1.0 + 4Ch Relay Module
- Sonoff S26 (original repo from https://github.com/dev-git-usr/KNX-Sonoff-S26 by dev-git-usr)