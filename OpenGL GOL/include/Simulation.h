#ifndef SIMULATION_H
#define SIMULATION_H

#include "Graphics.h"
#include "Map.h"
#include "Button.h"
#include "Slider.h"
#include "MagnifyingGlass.h"

class Simulation
{
public:
    Simulation();
    ~Simulation();
    void InitState();
    void Update();
    bool SwapState();
    void ResetState(int TextureSizeX, int TextureSizeY, float treshold);
private:
    Map mapa;
    Button buttonsBarForAlive[8];
    Button buttonsBarForDead[8];
    Slider treshold;
    Button resetButton;
    Slider slider;
    MagnifyingGlass glass;

    bool swapState = false;
    bool paused = false;
    bool hideGUI = false;
    bool escape = false;
    unsigned char aliveRules = 0;
    unsigned char deadRules = 0;
};

#endif // SIMULATION_H
