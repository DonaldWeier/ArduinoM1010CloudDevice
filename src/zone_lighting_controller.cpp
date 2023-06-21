
#include "zone_lighting_controller.h"

#include <Arduino.h>
#include <unordered_map>

//DO NOT USE THE SPI PINS (8, 9, 10, or 4)
static const int red_pwm_pin = 6;
static const int green_pwm_pin = 7;
static const int blue_pwm_pin = 1;
static const int zone_1_selector_pin = 5;
static const int zone_2_selector_pin = 21;
static const int zone_3_selector_pin = 3;
static const int zone_4_selector_pin = 2;

static const PinStatus select_logic_level = HIGH;
static const PinStatus deselect_logic_level = LOW;


static const std::unordered_map<const Zone, const int> enum_to_pin_lut = {
    {Zone::Zone1, zone_1_selector_pin},
    {Zone::Zone2, zone_2_selector_pin},
    {Zone::Zone3, zone_3_selector_pin},
    {Zone::Zone4, zone_4_selector_pin}
};

static const std::unordered_map<std::string, const Zone> string_to_enum_lut = {
    {"NoZone", Zone::NoZone},
    {"Zone1", Zone::Zone1},
    {"Zone2", Zone::Zone2},
    {"Zone3", Zone::Zone3},
    {"Zone4", Zone::Zone4}
};

ZoneLightingController::ZoneLightingController() :_active_zone(Zone::NoZone) 
{

}

void ZoneLightingController::Initialize()
{
    pinMode(red_pwm_pin, OUTPUT);
    pinMode(green_pwm_pin, OUTPUT);
    pinMode(blue_pwm_pin, OUTPUT);

    InitializeZone(Zone::Zone1);
    InitializeZone(Zone::Zone2);
    InitializeZone(Zone::Zone3);
    InitializeZone(Zone::Zone4);
}

void ZoneLightingController::SetColor(int red, int green, int blue)
{
    analogWrite(red_pwm_pin, red);
    analogWrite(green_pwm_pin, green);
    analogWrite(blue_pwm_pin, blue);
}

void ZoneLightingController::HighlightZone(Zone zone)
{
    if(zone == _active_zone){ return; }

    if(_active_zone != Zone::NoZone)
    {
        DeselectZone(_active_zone);
    }

    if(zone != Zone::NoZone)
    {
        SelectZone(zone);
    }

    _active_zone = zone;

    return;
}

int ZoneLightingController::HighlightZone(std::string str)
{
    bool entry_does_not_exist = (string_to_enum_lut.find(str) == string_to_enum_lut.end());

    if(entry_does_not_exist)
    {
        return -1;
    }

    Zone the_intended_zone = string_to_enum_lut.at(str);

    HighlightZone(the_intended_zone);

    return 0;
};

void ZoneLightingController::InitializeZone(Zone zone)
{
    if(zone == Zone::NoZone){ return; }

    //deselect THEN setup as an output to avoid flickering during power up
    DeselectZone(zone);
    pinMode(enum_to_pin_lut.at(zone), OUTPUT);

}

void ZoneLightingController::SelectZone(Zone zone)
{
    //guard clause not strictly necessary since this is a private function. Kept regardless.
    if(zone == Zone::NoZone){ return; }

    digitalWrite(enum_to_pin_lut.at(zone), select_logic_level);
}

void ZoneLightingController::DeselectZone(Zone zone)
{
    //guard clause not strictly necessary since this is a private function. Kept regardless.    
    if(zone == Zone::NoZone){ return; }

    digitalWrite(enum_to_pin_lut.at(zone), deselect_logic_level);
}

