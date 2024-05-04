# KNX_IP_WiFi Product Research：
  KNX_IP --> standardized KNX multicast or tunneling protocol 
Pros/Advantages:
  1. Still ETS programmable configuration support;
  2. Still very stable as KNX_TP products;
  3. More freedom, without KNX_TP chip limitation in stock;
  4. Low cost, without KNX_TP IC dependency and KNX cable installation;
  5. Easy installation without KNX-specified Power Supply and cable;
  6. Compatible (and easy upgrade/extension) with the existing KNX system including KNX_TP, KNX_RF, and KNX_IP;
  7. Almost zero learning curve if you have the experience with KNX system.
  
Cons/Disadvantages:
  1. Strongly dependent on the quality of the local wireless WiFi network as KNX_IP;
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

# Constraints
- It only supports the 2.4GHz wireless WiFi network because of the limitation of the WiFi capability of ESP8266 MCU platform. 
- Ensure that the host computer running ETS5.x(or 6.x) and ESP8266 MCU platform connects into the same 2.4GHz local wireless WiFi network before ETS programming.
- Not fully supports the ETS programming capabilities (e.g. unloading the application but bypassing it with loading the new application again) by the limitation of https://github.com/thelsing/knx.git knx stack.
- ZigBee and Wi-Fi channels both exist in the 2.4 GHz band, existing in the exact same frequency space. When deploying both Wi-Fi and ZigBee in the same environments, careful planning must be performed to make sure that they don't interfere with each other.
Operating a ZigBee network and a Wi-Fi network on the same frequency will cause them to interfere with each other. Usually, the ZigBee network will take the hit.

# How-to-do
- 1. Power cycle the device (i.e. plug out/in the power supply) after flashing the firmware.
- 2. Connect the device to the 2.4GHz local wireless WiFi network as same as the host machine running ETS before ETS programming.
- 2. When it prompts during ETS programming (i.e. downloading the physical address), switch the device to programming mode by long pressing the progBtn, and the progLed lights up corresponding.
- 3. Once downloading the physical address successfully and then confirm the physical address there by going the device web portal, download the application if you have assigned the group address/links.
- 4. It should work fine. Now welcome the KNX_IP_WiFi world. 