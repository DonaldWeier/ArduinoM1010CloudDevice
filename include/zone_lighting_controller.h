
#pragma once
#include <cstdint>
#include <string>

enum class Zone
{
    NoZone,
    Zone1,
    Zone2,
    Zone3,
    Zone4
};

class ZoneLightingController
{
    public:

    ZoneLightingController();
    ~ZoneLightingController() = default;
    void Initialize();
    void SetColor(int red, int green, int blue);
    void HighlightZone(Zone zone);
    int HighlightZone(std::string str);

    private:

    void InitializeZone(Zone zone);
    void SelectZone(Zone zone);
    void DeselectZone(Zone zone);

    Zone _active_zone;

};