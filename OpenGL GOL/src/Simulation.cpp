#include "Simulation.h"


void MakeButtons(Button* buttons, int count, float xPos, float yPos, float width, float height, unsigned char activation){
    vec2f BL, TR;
    float buttonHeight = (height/2)/count;
    for(int i=0; i<count; i++){
        BL.x = xPos-width/2;
        TR.x = xPos+width/2;
        BL.y = yPos - ((2*i+1)*buttonHeight);
        TR.y = yPos - ((2*i)*buttonHeight);

        if(activation/128==1){
            buttons[i].CreateButton(BL, TR, 1<<i, Button::ACTIVE);
        }
        else{
            buttons[i].CreateButton(BL, TR, 1<<i, Button::INACTIVE);
        }
        activation<<=1;
    }
}

void Keyboard(bool* paused, Slider* slider, bool* hideGUI, bool* escape){
    static bool spacebarClicked = false;
    static bool ADclicked = false;
    static bool GClicked = false;
    static bool ESCClicked = false;
                if(glfwGetKey(Graphics::GetWindow(), GLFW_KEY_SPACE)==GLFW_PRESS){
                if(!*paused && !spacebarClicked){
                    *paused = true;
                }
                else if(*paused && !spacebarClicked){
                    *paused = false;
                }
                spacebarClicked = true;
            }
            else
                spacebarClicked = false;

            if(glfwGetKey(Graphics::GetWindow(), GLFW_KEY_A)==GLFW_PRESS){
                if(!ADclicked)
                    slider->SetValue(slider->GetValue()-0.1f);
                ADclicked = true;
            }
            else if(glfwGetKey(Graphics::GetWindow(), GLFW_KEY_D)==GLFW_PRESS){
                if(!ADclicked)
                    slider->SetValue(slider->GetValue()+0.1f);
                ADclicked = true;
            }
            else
                ADclicked = false;

            if(glfwGetKey(Graphics::GetWindow(), GLFW_KEY_G)==GLFW_PRESS){
                if(!GClicked)
                    *hideGUI = *hideGUI?false:true;
                GClicked = true;
            }
            else
                GClicked = false;
        if(glfwGetKey(Graphics::GetWindow(), GLFW_KEY_ESCAPE)==GLFW_PRESS){
            if(!ESCClicked)
                *escape = true;
            ESCClicked = true;
        }
        else{
            *escape = false;
            ESCClicked = false;
        }
}

Simulation::Simulation()
{
    //ctor
}

Simulation::~Simulation()
{
    //dtor
}

void Simulation::InitState(){
    mapa.LoadShaders("Shaders/vs.txt", "Shaders/fs.txt", "Shaders/cs.txt", "Shaders/fillWithNoise.txt");
    resetButton.CreateButton({-0.95f, 0.7f}, {-0.8f, 0.8f}, 0, Button::State::SINGLE_ACTION);
    treshold.Init(0.4f, -0.75f, 0.9f, 0.9f);
    MakeButtons(buttonsBarForAlive, 8, 0.9f, 0.95f, 0.14f, 1.4f, 249); //Komorka zmienia stan na przeciwny gdy (00000100)
    MakeButtons(buttonsBarForDead, 8, 0.7f, 0.95f, 0.14f, 1.4f, 4); //Komorka zmienia stan na przeciwny gdy (11111001)
    slider.Init(1.0f, 0.0f, -0.8f, 0.0f);
    glass.Init(0.4f, 1.0f);
}

void Simulation::Update(){
            if(!hideGUI){
                aliveRules = 0;
                deadRules = 0;
                for(int i=0; i<8; i++){
                    deadRules = deadRules<<1;
                    aliveRules = aliveRules<<1;
                    buttonsBarForAlive[i].Update();
                    buttonsBarForAlive[i].Draw();
                    if(buttonsBarForAlive[i].GetData().state==Button::State::ACTIVE){
                        aliveRules += 1;
                    }
                    buttonsBarForDead[i].Update();
                    buttonsBarForDead[i].Draw();
                    if(buttonsBarForDead[i].GetData().state==Button::State::ACTIVE){
                        deadRules += 1;
                    }
                }
                treshold.Update();
                treshold.Draw();

                resetButton.Update();
                if(resetButton.GetData().state == Button::State::ACTIVE){
                    slider.SetValue(0.0f);
                    mapa.FillActiveTextureRandomly(treshold.GetValue());
                    resetButton.SetState(Button::State::SINGLE_ACTION);
                }
                resetButton.Draw();

                slider.Update();
                slider.Draw();
            }

            mapa.Draw();
            mapa.ChangeRules(aliveRules, deadRules);
            mapa.ChangeFPS(!paused?slider.GetValue():0.0f);
            mapa.Update(Graphics::GetPreviousFrameTime());

            glass.Update();
            glass.Draw(mapa.GetActiveTexture());
            Keyboard(&paused, &slider, &hideGUI, &escape);

            if(escape){
                swapState = true;
            }
}

bool Simulation::SwapState(){
    if(swapState){
        swapState = false;
        return true;
    }
    return false;
}

void Simulation::ResetState(int TextureSizeX, int TextureSizeY, float treshold){
    mapa.CreateTextures(TextureSizeX, TextureSizeY);
    mapa.FillActiveTextureRandomly(treshold);
    slider.SetValue(0.0f);
}
