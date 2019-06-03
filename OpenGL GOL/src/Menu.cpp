#include "Menu.h"

Menu::Menu()
{
    //ctor
}

Menu::~Menu()
{
    //dtor
}

void Menu::ResetState(){
    startButton.SetState(Button::State::SINGLE_ACTION);
}

void Menu::InitState(){
    background.Create();
    const float width = 1.0f;
    const float height = 0.2f;
    float x = 0-width/2, y = 0.6f;

    for(int i=0; i<3; i++){
        resolutionSelect[i].CreateButton({x+(width/3*i)+width/20, y-height},
                                         {x+(width/3*(i+1))-width/20, y}, 0, Button::State::INACTIVE);
    }
    resolutionSelect[1].SetState(Button::State::ACTIVE);
    startButton.CreateButton({-0.2f, -0.3f}, {0.2f, -0.1f}, 0, Button::State::SINGLE_ACTION);
    exitButton.CreateButton({0.6f, -0.8f}, {0.9f, -0.6f}, 0, Button::State::INACTIVE);
    treshold.Init(0.6f, 0.0f, 0.1f, 0.7f);
    ghost.Init(0.0f, 2.0f, 1.0f, 0.0f);
}

void Menu::Update(){
    for(int i=0; i<3; i++){
        resolutionSelect[i].Update();
        if(resolutionSelect[i].GetData().state == Button::State::ACTIVE && i != activeButton){
            resolutionSelect[activeButton].SetState(Button::State::INACTIVE);
            activeButton = i;
        }
        resolutionSelect[i].Draw();
    }
    for(int i=0; i<3; i++){
        resolutionSelect[i].Draw();
    }
    startButton.Update();
    if(startButton.GetData().state == Button::State::ACTIVE);//todo instrukcja
    startButton.Draw();
    exitButton.Update();
    if(exitButton.GetData().state == Button::State::ACTIVE){
        glfwSetWindowShouldClose(Graphics::GetWindow(), true);
    }
    exitButton.Draw();
    treshold.Update();
    treshold.Draw();
    ghost.Update(); //B£¥D: Na ekranie musz¹ byæ co najmniej 2 suwaki, inaczej zmienna mutex nie zachowuje siê poprawnie
    background.Draw();
    if(startButton.GetData().state==Button::State::ACTIVE){
        swapState = true;
    }

}

bool Menu::SwapState(){
    if(swapState){
        swapState = false;
        return true;
    }
    return false;
}

float Menu::GetTreshold(){
    return treshold.GetValue();
}

int Menu::GetResolutionMultiplier(){
    if(activeButton == 0)
        return 1;
    if(activeButton == 1)
        return 2;
    if(activeButton == 2)
        return 4;
}
