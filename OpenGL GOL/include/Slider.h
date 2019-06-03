#ifndef SLIDER_H
#define SLIDER_H


#include "Graphics.h"
#include <iostream>



const unsigned int indices [12] = {0, 1, 2,
                        0, 2, 3,
                        4, 5, 6,
                        4, 6, 7};

class Slider
{
public:
    Slider();
    virtual ~Slider();
    bool Clicked();
    void Init(float width, float center_x,
              float center_y, float init_value);
    void Update();
    void Draw();
    float GetValue();
    void SetValue(float value);
private:
    unsigned int VAO;
    unsigned int vertices;
    unsigned int colors;
    unsigned int texels;
    unsigned int shader;

    const char* vertexShaderPath = "Shaders/sliderVS.txt";
    const char* fragmentShaderPath = "Shaders/sliderFS.txt";
    const float sliderHeight = 0.09f;
    const float sliderWidth = 0.012f;
    const float pathHight = 0.02f;
    float xCenter, yCenter;
    float width;
    float value;
    bool click;
    static bool mutex;
};

#endif // SLIDER_H
