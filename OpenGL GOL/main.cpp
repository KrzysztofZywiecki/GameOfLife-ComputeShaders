#include <iostream>
#include "Menu.h"
#include "Simulation.h"

#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920

enum State{IN_MENU, SIMULATING};

int main(void)
{
    Graphics::Init(WINDOW_WIDTH, WINDOW_HEIGHT, "Magister");

    Menu menu;
    menu.InitState();
    Simulation simulation;
    simulation.InitState();

    State currentState = IN_MENU;

    while (!glfwWindowShouldClose(Graphics::GetWindow())){
        switch(currentState){
        case(SIMULATING):
            simulation.Update();
            if(simulation.SwapState()){
                menu.ResetState();
                currentState = IN_MENU;
            }
            break;
        case(IN_MENU):
            menu.Update();
            if(menu.SwapState()){
                simulation.ResetState(WINDOW_WIDTH * menu.GetResolutionMultiplier(),
                WINDOW_HEIGHT * menu.GetResolutionMultiplier(),
                menu.GetTreshold());
                currentState = SIMULATING;
            }
            break;
        }
        Graphics::UpdateDisplay();
        Graphics::PollEvents();
        }
    Graphics::Clear();
    return 0;
}
