#ifndef MENU_H
#define MENU_H
#include "Button.h"
#include "Slider.h"
#include "Graphics.h"
#include "Background.h"


class Menu
{
public:
    Menu();
    ~Menu();
    bool GetSwitchState();
    void Update();
    void ResetState();
    void InitState();
    int GetResolutionMultiplier();
    float GetTreshold();
    bool SwapState();
private:
    int activeButton = 1;
    bool swapState = false;
    Background background;
    Button resolutionSelect[3];
    Button startButton;
    Button exitButton;
    Slider treshold;
    Slider ghost;
};

#endif // MENU_H
