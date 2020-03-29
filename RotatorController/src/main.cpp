#include <Arduino.h>
#include <SSD1306Wire.h>
#include "BluetoothSerial.h"
#include <Streaming.h>

// DIY Sat Tracker Software by Tysonpower.de
// NOTE: The code works but it is NOT optimal nor bug free

// Implementation of the Yaesu GS232B Protocol
// B              - Report elevation                                EL=eee
// C              - Report azimuth                                  AZ=aaa
// C2             - Report azimuth and elevation                    AZ=888 EL=eee
// S              - Stop all rotation
// A              - Stop azimuth rotation
// E              - Stop elevation rotation
// L              - Rotate azimuth left (CCW)
// R              - Rotate azimuth right (CW)
// D              - Rotate elevation down
// U              - Rotate elevation up
// Mxxx           - Move to azimuth
// Wxxx yyy       - Move to azimuth xxxx and elevation yyy
// X1             - Change to azimuth rotation speed setting 1
// X2             - Change to azimuth rotation speed setting 2
// X3             - Change to azimuth rotation speed setting 3
// X4             - Change to azimuth rotation speed setting 4
// O              - Azimuth offset calibration
// F              - Azimuth full scale calibration
// O2             - Elevation offset calibration
// F2             - Elevation full scale calibration
// P36            - Switch to 360 degree mode
// P45            - Switch to 450 degree mode
// Z              - Toggle north / south centered mode
// H              - Help

#define EL_ENDSTOP 25
#define AZ_ENDSTOP 26

#define AZ_ENABLE 16
#define EL_ENABLE 2
// Pins Stepper EL
#define EL_PUL 13
#define EL_DIR 12
// Pins Stepper AZ
#define AZ_PUL 15
#define AZ_DIR 14

void drawDisplay();
void serialEventBT();
void handleSerialCommand(String);
int getElevation();
int getAzimuth();
void setElevation(int elevation);
void setAzimuth(int azimuth);

void setStepFrequency(int frequencyHz);

// Function to stop the EL axis
void stopElevationMoving();

// Function to stop the AZ axis
void stopAzimuthMoving();

// Function to zero out the EL axis
void findElevationZero();

// Function to zero out the AZ axis
void findAzimuthZero();

// FUll calibration to get STepps per 360°
// Function to calibrate the trackers EL axis
void calibrateStepsOnElevation();

// Function to calibrate the trackers AZ axis
void calibrateAzimuthSteps();

void azInterrupt();
void elInterrupt();

void elEndstopInterrupt();
void azEndstopInterrupt();

int getAzDirection();
int getElDirection();

void moveToElevation();
void moveToAzimuth();

void moveStepperOneDeg(int PULpin, int DIRpin, bool rev);
void moveStepperOneStep(int PULpin, int DIRpin, bool rev);
//////////////////////Variables//////////////////////////
volatile int m_azDirection{0};
volatile int m_elDirection{0};
volatile unsigned m_azimuthSteps{0}, m_elevationSteps{0};

static constexpr unsigned STEPS_PER_REVOLUTION{51250};
static constexpr unsigned STEPS_PER_DEGREE{51250 / 360}; //142

static constexpr unsigned PWM_RESOUTION{8};
static constexpr unsigned PWM_DUTY_CYCLE_ON{255 / 4};
static constexpr unsigned PWM_DUTY_CYCLE_OFF{0};
static constexpr unsigned CHANNEL_EL{0};
static constexpr unsigned CHANNEL_AZ{1};

volatile bool justCalibratedAz{false};
volatile bool justCalibratedEl{false};

unsigned m_step_frequency{2500};
unsigned m_desiredAzimuthSteps{0}, m_desiredElevationSteps{0};

SSD1306Wire display(0x3c, 5, 4, GEOMETRY_128_64);
BluetoothSerial ESP_BT;

String sdata = "";
bool processCommandFlag{false};

void setup()
{
    ESP_BT.begin("ESP32 Rotator");
    Serial.begin(115200);

    ESP_BT.println("Sat rotator!");

    display.init();
    display.flipScreenVertically();
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    pinMode(AZ_ENABLE, OUTPUT);
    pinMode(EL_ENABLE, OUTPUT);

    pinMode(EL_DIR, OUTPUT);
    pinMode(AZ_DIR, OUTPUT);

    pinMode(EL_ENDSTOP, INPUT);
    pinMode(AZ_ENDSTOP, INPUT);

    ledcSetup(CHANNEL_EL, m_step_frequency, PWM_RESOUTION);
    ledcSetup(CHANNEL_AZ, m_step_frequency, PWM_RESOUTION);

    ledcAttachPin(EL_PUL, CHANNEL_EL);
    ledcAttachPin(AZ_PUL, CHANNEL_AZ);

    attachInterrupt(EL_PUL, elInterrupt, RISING);
    attachInterrupt(AZ_PUL, azInterrupt, RISING);

    attachInterrupt(EL_ENDSTOP, elEndstopInterrupt, FALLING);
    attachInterrupt(AZ_ENDSTOP, azEndstopInterrupt, FALLING);
}

void loop()
{
    drawDisplay();

    if(ESP_BT.available())
    {
        serialEventBT();
    }

    if(processCommandFlag)
    {
        processCommandFlag = false;
        handleSerialCommand(sdata);
        sdata = "";
    }
}

void drawDisplay()
{
    display.clear();
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 0, "AZ=" + String(getAzimuth()));
    display.drawString(0, 22, "EL=" + String(getElevation()));
    if(ESP_BT.hasClient())
    {
        display.drawString(100, 0, "C");
    }
    else
    {
        display.drawString(100, 0, "N");
    }
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 48, "E" + String(digitalRead(EL_ENDSTOP)) +
                       "A" + String(digitalRead(AZ_ENDSTOP)) +
                       " E" + String(getElDirection()) +
                       " A" + String(getAzDirection())) ;
    display.display();
}

void handleSerialCommand(String serialIn)
{
    Serial.println("Command:" + serialIn);
    if(serialIn.startsWith("C2"))
    {
        Serial.println("Report az and el");
        ESP_BT.print("AZ=" + String(getAzimuth()) + " EL=" + String(getElevation()));
        ESP_BT.print("\r\n");
        return;
    }
    if(serialIn.startsWith("S"))
    {
        Serial.println("Stop all rotation");
        ESP_BT.print("\r\n");
        stopElevationMoving();
        stopAzimuthMoving();
        justCalibratedAz = false;
        justCalibratedEl = false;
        return;
    }
    if(serialIn == "B")                                   // Report elevation (el)
    {
        ESP_BT.print("EL=" + String(getElevation()));
        ESP_BT.print("\r\n");
    }
    else if(serialIn == "C")                              // Report azimuth (az)
    {
        ESP_BT.print("AZ=" + String(getAzimuth()));
        ESP_BT.print("\r\n");
    }
    if(serialIn == "A")                            // Stop az rotation
    {
        stopAzimuthMoving();
    }
    else if(serialIn == "E")                              // Stop el rotation
    {
        stopElevationMoving();
    }
    else if(serialIn == "L")                              // rotate azimuth left (CCW)
    {

    }
    else if(serialIn == "R")                              // rotate azimuth right (CW)
    {

    }
    else if(serialIn == "D")                              // rotate elevation down
    {

    }
    else if(serialIn == "U")                              // rotate elevation up
    {

    }
    else if(serialIn.substring(0, 1) == "M")              // Move to azimuth
    {
        ESP_BT.print("\r\n");
        setAzimuth(serialIn.substring(1, 4).toInt());
    }
    else if(serialIn.substring(0, 1) == "W")              // Move to azimuth and elevation
    {
        ESP_BT.print("\r\n");
        setAzimuth(serialIn.substring(1, 4).toInt());
        setElevation(serialIn.substring(5, 8).toInt());
    }
    else if(serialIn == "X1")                             // Azimuth rotation speed 1
    {
        setStepFrequency(7000);
    }
    else if(serialIn == "X2")                             // Azimuth rotation speed 2
    {
        setStepFrequency(5000);
    }
    else if(serialIn == "X3")                             // Azimuth rotation speed 3
    {
        setStepFrequency(3500);
    }
    else if(serialIn == "X4")                             // Azimuth rotation speed 4
    {
        setStepFrequency(2500);
    }
    else if(serialIn == "O")                              // Azimuth offset calibration
    {
        findAzimuthZero();
    }
    else if(serialIn == "F")                              // Azimuth full scale calibration
    {

    }
    else if(serialIn == "O2")                             // Elevation offset calibration
    {
        findElevationZero();
    }
    else if(serialIn == "F2")                             // Elevation full scale calibration
    {
        calibrateStepsOnElevation();
    }
    else if(serialIn == "P36")                            // Switch to 360 degree mode
    {

    }
    else if(serialIn == "P45")                            // Switch to 450 degree mode
    {

    }
    else if(serialIn == "Z")                              // Toggle north/South centered mode
    {

    }
    else if(serialIn == "H")                              // Help
    {

    }
}

void serialEventBT()
{
    while(ESP_BT.available())
    {
        auto inChar = (char)ESP_BT.read();
        //Serial.println(inChar, HEX);
        sdata += inChar;

        if(inChar == 0x0D)
        {
            sdata.trim();
            processCommandFlag = true;
        }
    }
}

void azInterrupt()
{
    m_azimuthSteps += m_azDirection;
    if(m_azimuthSteps == m_desiredAzimuthSteps)
    {
        justCalibratedAz = false;
        m_azDirection = 0;
        ledcWrite(CHANNEL_AZ, PWM_DUTY_CYCLE_OFF);
    }
}

void elInterrupt()
{
    //Serial << __FUNCTION__ << m_elevationSteps << " " << m_elDirection << endl;
    m_elevationSteps += m_elDirection;
    if(m_elevationSteps == m_desiredElevationSteps)
    {
        justCalibratedEl = false;
        m_elDirection = 0;
        ledcWrite(CHANNEL_EL, PWM_DUTY_CYCLE_OFF);
    }
}

int getAzDirection()
{
    return m_azDirection;
}

int getElDirection()
{
    return m_elDirection;
}

int getElevation()
{
    return (m_elevationSteps + 1) / STEPS_PER_DEGREE;
}
int getAzimuth()
{
    return (m_azimuthSteps + 1) / STEPS_PER_DEGREE;
}

void setElevation(int elevation)
{
    if(elevation > 180)
    {
        return;
    }
    m_desiredElevationSteps = elevation * STEPS_PER_DEGREE;
    moveToElevation();
}

void setAzimuth(int azimuth)
{
    if(azimuth > 360)
    {
        return;
    }
    m_desiredAzimuthSteps = azimuth * STEPS_PER_DEGREE;
    moveToAzimuth();
}

void startELdown()
{
    digitalWrite(EL_DIR, HIGH);
    ledcWrite(CHANNEL_EL, PWM_DUTY_CYCLE_ON);
}

void startELUp()
{
    digitalWrite(EL_DIR, LOW);
    ledcWrite(CHANNEL_EL, PWM_DUTY_CYCLE_ON);
}

void moveToElevation()
{
    Serial << __FUNCTION__ << " desired:" << m_desiredElevationSteps << " is:" << m_elevationSteps << endl;
    if(m_desiredElevationSteps < m_elevationSteps)
    {
        startELdown();
        m_elDirection = -1;
    }
    else if(m_desiredElevationSteps > m_elevationSteps)
    {
        startELUp();
        m_elDirection = +1;
    }
}

void startAzRight()
{
    ledcWrite(CHANNEL_AZ, PWM_DUTY_CYCLE_ON);
    digitalWrite(AZ_DIR, LOW);
}

void startAzLeft()
{
    ledcWrite(CHANNEL_AZ, PWM_DUTY_CYCLE_ON);
    digitalWrite(AZ_DIR, HIGH);
}

void moveToAzimuth()
{
    Serial << __FUNCTION__ << " desired:" << m_desiredAzimuthSteps << " is:" << m_azimuthSteps << endl;
    if(m_desiredAzimuthSteps < m_azimuthSteps)
    {
        startAzLeft();
        m_azDirection = -1;
    }
    else if(m_desiredAzimuthSteps > m_azimuthSteps)
    {
        startAzRight();
        m_azDirection = +1;
    }
}

void stopElevationMoving()
{
    Serial << __FUNCTION__ << endl;
    m_elDirection = 0;
    ledcWrite(CHANNEL_EL, PWM_DUTY_CYCLE_OFF);
}

// Function to stop the AZ axis
void stopAzimuthMoving()
{
    Serial << __FUNCTION__ << endl;
    m_azDirection = 0;
    ledcWrite(CHANNEL_AZ, PWM_DUTY_CYCLE_OFF);
}

// Function to zero out the EL axis
void findElevationZero()
{
    m_desiredElevationSteps = 0xEFFF;
    Serial << __FUNCTION__ << endl;
    startELdown();
}

// Function to zero out the AZ axis
void findAzimuthZero()
{
    Serial << __FUNCTION__ << endl;
    m_desiredAzimuthSteps = 0xEFFF;
    startAzRight();
}

//Shall run between two endstops, but i have just one on 0
void calibrateStepsOnElevation()
{
    Serial << __FUNCTION__ << endl;
    //calibrate steps
}

//Shall run one complete round and calculate steps
void calibrateAzimuthSteps()
{
    Serial << __FUNCTION__ << endl;
    //calibrate steps
}

void setStepFrequency(int frequencyHz)
{
    m_step_frequency = frequencyHz;
    ledcWriteTone(CHANNEL_EL, m_step_frequency);
    ledcWriteTone(CHANNEL_AZ, m_step_frequency);
}

void elEndstopInterrupt()
{
    if(!justCalibratedEl)
    {
        m_elevationSteps = 0;
        Serial.println("Reached Elevation endstop!");
        stopElevationMoving();
    }
    justCalibratedEl = true;
}

void azEndstopInterrupt()
{
    if(!justCalibratedAz)
    {
        m_azimuthSteps = 0;
        Serial.println("Reached Azimuth endstop!");
        stopAzimuthMoving();
    }
    justCalibratedAz = true;
}
