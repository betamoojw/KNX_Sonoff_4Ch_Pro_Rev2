#include <Arduino.h>
#include <knx.h>
#include <knx/bau57B0.h>  //KNX 
#ifdef ARDUINO_ARCH_ESP8266
#include <WiFiManager.h>
#endif

#define DEV_NAME "MT_Switch_4Ch"

// KNX Prog Hardware Define on the NodeMCU platform
#define PROG_BUTTON_PIN         0  //GPIO 0 --> Flash Button
#define PROG_LED_PIN            2  //GPIO 2 --> Built-in Led
#define PROG_LED_PIN_ACTIVE_ON  LOW

// Relay Channel Pin Define
#ifdef ESP8285
// #define MAN_BUTTON_A_PIN         0  //GPIO 0 --> same as Flash Button, actually not used
#define MAN_BUTTON_B_PIN         9  //GPIO 9 --> Button B
#define MAN_BUTTON_C_PIN         14 //GPIO 14 --> Button D
#define MAN_BUTTON_D_PIN         10 //GPIO 10 --> Button C

bool isManual = false;

uint8_t relayChDef[] = {12,  //GPIO 12 --> CH A
                        5,   //GPIO 5 --> CH B
                        15,  //GPIO 15 --> CH C
                        4};  //GPIO 4 --> CH D
#else
uint8_t relayChDef[] = {12,  //GPIO 12 --> CH A
                        13,  //GPIO 13 --> CH B
                        14,  //GPIO 14 --> CH C
                        15}; //GPIO 15 --> CH D
#endif
uint8_t relayChCount = sizeof(relayChDef)/sizeof(relayChDef[0]);

// Relay Channel Index Name
enum {
  CH_A = 0,
  CH_B,
  CH_C,
  CH_D
};
// Relay Channel
#define RELAY_CH_A relayChDef[CH_A]
#define RELAY_CH_B relayChDef[CH_B]
#define RELAY_CH_C relayChDef[CH_C]
#define RELAY_CH_D relayChDef[CH_D]

// Relay Channel Initialize
void relayInit(uint8_t chCount)
{   
    if (chCount <= 0 || chCount > relayChCount) return;
    for (uint8_t ch = 0; ch <= (chCount - 1); ch++)
    {
        pinMode(relayChDef[ch], OUTPUT);
    }
}

// KNX Parameter List


// KNX Comm/Group Object List
#define Relay_KoOffset 0
#define Relay_KoBlockSize 3

// Create named references for easy access to group objects
#define groupObjChSwitch(Idx) (knx.getGroupObject(Relay_KoOffset + Relay_KoBlockSize * Idx + 1))
#define groupObjChBlock(Idx) (knx.getGroupObject(Relay_KoOffset + Relay_KoBlockSize * Idx + 2))
#define groupObjChStatus(Idx) (knx.getGroupObject(Relay_KoOffset + Relay_KoBlockSize * Idx + 3))

#define groupObjChASwitch groupObjChSwitch(CH_A)
#define groupObjChABlock groupObjChBlock(CH_A)
#define groupObjChAStatus groupObjChStatus(CH_A)
#define groupObjChBSwitch groupObjChSwitch(CH_B)
#define groupObjChBBlock groupObjChBlock(CH_B)
#define groupObjChBStatus groupObjChStatus(CH_B)
#define groupObjChCSwitch groupObjChSwitch(CH_C)
#define groupObjChCBlock groupObjChBlock(CH_C)
#define groupObjChCStatus groupObjChStatus(CH_C)
#define groupObjChDSwitch groupObjChSwitch(CH_D)
#define groupObjChDBlock groupObjChBlock(CH_D)
#define groupObjChDStatus groupObjChStatus(CH_D)

// callback from switch-GO
void chASwitchCallback(GroupObject& go)
{
    Serial.println("group Obj ChA Switch");
    if (groupObjChABlock.value())
    {
        Serial.println("\tChA Block --> enabled");
        return;
    }
    
    bool value = groupObjChASwitch.value();
    digitalWrite(RELAY_CH_A, value);
    groupObjChAStatus.value(value);
}

void chBSwitchCallback(GroupObject& go)
{
    Serial.println("group Obj ChB Switch");
    if (groupObjChBBlock.value())
    {
        Serial.println("\tChB Block --> enabled");
        return;
    }
    
    bool value = groupObjChBSwitch.value();
    digitalWrite(RELAY_CH_B, value);
    groupObjChBStatus.value(value);
}

void chCSwitchCallback(GroupObject& go)
{
    Serial.println("group Obj ChC Switch");
    if (groupObjChCBlock.value())
    {
        Serial.println("\tChC Block --> enabled");
        return;
    }
    
    bool value = groupObjChCSwitch.value();
    digitalWrite(RELAY_CH_C, value);
    groupObjChCStatus.value(value);
}

void chDSwitchCallback(GroupObject& go)
{
    Serial.println("group Obj ChD Switch");
    if (groupObjChDBlock.value())
    {
        Serial.println("\tChD Block --> enabled");
        return;
    }
    
    bool value = groupObjChDSwitch.value();
    digitalWrite(RELAY_CH_D, value);
    groupObjChDStatus.value(value);
}

// callback previous to OTA-Update
void wifimgr_pre_ota() {
    knx.enabled(false);
}

WiFiManager wifiManager;
WiFiManagerParameter custom_field; // global param ( for non blocking w params )
char custom_field_str[200];
void setup()
{
    Serial.begin(115200);
    randomSeed(millis());
    ArduinoPlatform::SerialDebug = &Serial;

    delay(500);
    
    Serial.println();
    Serial.println("ESP8266 Info.");
    Serial.println("****************************************");
  
    Serial.print("WiFi MAC: ");
    Serial.println(WiFi.macAddress());
    Serial.printf("Chip ID as a 32-bit integer:\t%08X\n", ESP.getChipId());  
    Serial.printf("Flash chip ID as a 32-bit integer:\t\t%08X\n", ESP.getFlashChipId()); 
    Serial.printf("Flash chip frequency:\t\t\t\t%d (Hz)\n", ESP.getFlashChipSpeed());
    // ESP.getFlashChipSize() returns the flash chip size, in bytes as seen by the SDK (may be less than actual size).
    Serial.printf("Flash chip size:\t\t\t\t%d (bytes)\n", ESP.getFlashChipSize());
    Serial.printf("Free heap size:\t\t\t\t\t%d (bytes)\n", ESP.getFreeHeap());

    relayInit(relayChCount);

#ifdef ESP8285
// initialize the pushbutton pin as an input:
    pinMode(MAN_BUTTON_B_PIN, INPUT);
    pinMode(MAN_BUTTON_C_PIN, INPUT);
    pinMode(MAN_BUTTON_D_PIN, INPUT);
#endif

    wifiManager.setHostname(DEV_NAME);
    wifiManager.setClass("invert"); // Dark Mode
    wifiManager.setBreakAfterConfig(false);
    wifiManager.setTitle(DEV_NAME);
    wifiManager.setConfigPortalBlocking(false); //Not waiting for Connection/Configuration

    //Configure Menu 
    std::vector<const char *> menu = {"wifi","info","param","update","sep","restart", "erase"};
    wifiManager.setMenu(menu);

    // wifiManager.autoConnect(DEV_NAME);
    wifiManager.setTimeout(60000*8);
    if (!wifiManager.autoConnect(DEV_NAME)) {
        Serial.println("Failed to connect and hit timeout");
        delay(3000);
        // ESP.reset();
        delay(5000);
    }

    wifiManager.startWebPortal(); //Enable Webpage after initial Configuration
    wifiManager.setPreOtaUpdateCallback(wifimgr_pre_ota);

    // read adress table, association table, groupobject table and parameters from eeprom
    knx.readMemory();

    if (knx.configured())
    {
      Serial.println("Device is Configured!");
      
      groupObjChASwitch.callback(chASwitchCallback); // register callback for reset GO
      groupObjChASwitch.dataPointType(Dpt(1, 1));
      groupObjChABlock.dataPointType(Dpt(1, 3));
      groupObjChAStatus.dataPointType(Dpt(1, 2));

      groupObjChBSwitch.callback(chBSwitchCallback);
      groupObjChBSwitch.dataPointType(Dpt(1, 1));
      groupObjChBBlock.dataPointType(Dpt(1, 3));
      groupObjChBStatus.dataPointType(Dpt(1, 2));

      groupObjChCSwitch.callback(chCSwitchCallback);
      groupObjChCSwitch.dataPointType(DPT_Switch);
      groupObjChCBlock.dataPointType(DPT_Enable);
      groupObjChCStatus.dataPointType(DPT_Bool);

      groupObjChDSwitch.callback(chDSwitchCallback);
      groupObjChDSwitch.dataPointType(DPT_Switch);
      groupObjChDBlock.dataPointType(DPT_Enable);
      groupObjChDStatus.dataPointType(DPT_Bool);
    } 
    else {
        Serial.println("Device is not Configured!");
    }
    
    // pin or GPIO the programming led is connected to. Default is LED_BUILTIN
    knx.ledPin(PROG_LED_PIN);
    // is the led active on HIGH or low? Default is LOW
    knx.ledPinActiveOn(PROG_LED_PIN_ACTIVE_ON);
    // pin or GPIO programming button is connected to. Default is 0
    knx.buttonPin(PROG_BUTTON_PIN);
    knx.setButtonISRFunction(NULL);

    // start the framework. Will get wifi first.
    knx.start();

    //Setup Page for detailed Information
    //show only programmed Physical Address
    uint16_t area = (knx.individualAddress() & 0xF000) >> 12;
    uint16_t line = (knx.individualAddress() & 0x0F00) >> 8;
    uint16_t device = knx.individualAddress() & 0x00FF;
    sprintf(custom_field_str,"<br/><label for='customfieldid'>Device is programmed with ETS</label><br>Physical-Address: %d.%d.%d", area, line, device);

    new (&custom_field) WiFiManagerParameter(custom_field_str); // custom html input
  
    wifiManager.addParameter(&custom_field);
}

const int SHORT_PRESS_TIME = 1000; // 1000 ms
long pressedTime[4] = {0};
long releasedTime[4] = {0};
bool lastState[4] = {HIGH, HIGH, HIGH, HIGH};
void loop() 
{
    // don't delay here to much. Otherwise you might lose packages or mess up the timing with ETS
    knx.loop();
    wifiManager.process(); 

    //Button Functions
    if (lastState[CH_A] == HIGH && digitalRead(PROG_BUTTON_PIN) == LOW) {
        pressedTime[CH_A] = millis();
        lastState[CH_A] = LOW;
    }
    releasedTime[CH_A] = millis();
    long pressDuration = releasedTime[CH_A] - pressedTime[CH_A];
    if (lastState[CH_A] == LOW && (digitalRead(PROG_BUTTON_PIN) == HIGH || pressDuration > SHORT_PRESS_TIME)) {
        lastState[CH_A] = HIGH;
        if (pressDuration < SHORT_PRESS_TIME) {
#ifdef ESP8285
            if (isManual) digitalWrite(RELAY_CH_A, !digitalRead(RELAY_CH_A));
#endif
        } 
        else {
            knx.toggleProgMode();
        }
    }

#ifdef ESP8285
    // Button B Functions
    if (lastState[CH_B] == HIGH && digitalRead(MAN_BUTTON_B_PIN) == LOW) {
        pressedTime[CH_B] = millis();
        lastState[CH_B] = LOW;
    }
    releasedTime[CH_B] = millis();
    pressDuration = releasedTime[CH_B] - pressedTime[CH_B];
    if (lastState[CH_B] == LOW && (digitalRead(MAN_BUTTON_B_PIN) == HIGH || pressDuration > SHORT_PRESS_TIME)) {
        lastState[CH_B] = HIGH;
        if (pressDuration < SHORT_PRESS_TIME) {
            if (isManual) digitalWrite(RELAY_CH_B, !digitalRead(RELAY_CH_B));
        } 
        else {
            isManual = !isManual;
            if (isManual)
            {
                knx.enabled(false);
                digitalWrite(PROG_LED_PIN, LOW);
                Serial.println("Local direct control is enabled");
            }
            else {
                knx.enabled(true);
                digitalWrite(PROG_LED_PIN, HIGH);
                Serial.println("KNX telegram control is enabled");
            }
        }
    }

    // Button C Functions
    if (lastState[CH_C] == HIGH && digitalRead(MAN_BUTTON_C_PIN) == LOW) {
        pressedTime[CH_C] = millis();
        lastState[CH_C] = LOW;
    }
    releasedTime[CH_C] = millis();
    pressDuration = releasedTime[CH_C] - pressedTime[CH_C];
    if (lastState[CH_C] == LOW && (digitalRead(MAN_BUTTON_C_PIN) == HIGH || pressDuration > SHORT_PRESS_TIME)) {
        lastState[CH_C] = HIGH;
        if (pressDuration < SHORT_PRESS_TIME) {
            if (isManual) digitalWrite(RELAY_CH_C, !digitalRead(RELAY_CH_C));
        } 
        else {
            // do nothing
        }
    }

    // Button D Functions
    if (lastState[CH_D] == HIGH && digitalRead(MAN_BUTTON_D_PIN) == LOW) {
        pressedTime[CH_D] = millis();
        lastState[CH_D] = LOW;
    }
    releasedTime[CH_D] = millis();
    pressDuration = releasedTime[CH_D] - pressedTime[CH_D];
    if (lastState[CH_D] == LOW && (digitalRead(MAN_BUTTON_D_PIN) == HIGH || pressDuration > SHORT_PRESS_TIME)) {
        lastState[CH_D] = HIGH;
        if (pressDuration < SHORT_PRESS_TIME) {
            if (isManual) digitalWrite(RELAY_CH_D, !digitalRead(RELAY_CH_D));
        } 
        else {
            // do nothing
        }
    }
#endif

    // only run the application code if the device was configured with ETS
    if (!knx.configured())  return;

    // nothing else to do.
}
