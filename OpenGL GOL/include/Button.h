#ifndef BUTTON_H
#define BUTTON_H

#include "Rectangle.h"

class Button
{
public:
    enum State{INACTIVE, ACTIVE, SINGLE_ACTION};
    enum WorksFor{DEAD, ALIVE};
    struct ButtonData{
        unsigned char value;
        State state;
    };

    Button();
    Button(vec2f BL, vec2f TR, unsigned char value, State state);
    ~Button();
    ButtonData GetData();
    void Update();
    void Draw();
    void SetState(State newState);
    void SetValue(unsigned int value);
    void CreateButton(vec2f BL, vec2f TR, unsigned char value, State state);
private:
    const char* vertexShaderPath = "Shaders/vs.txt";
    const char* fragmentShaderPath = "Shaders/butFS.txt";


    bool Clicked();
    bool clicked = false;
    Geometry::Rectangle rect;
    unsigned int shader;
    unsigned int color;
    unsigned char representedNumber;
    WorksFor WF;
    State state;
};

#endif // BUTTON_H
