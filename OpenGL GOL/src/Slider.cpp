#include "Slider.h"

#define ASSERT(x) if (!(x)) DebugBreak();

#define GlCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x))

bool Slider::mutex = false;

Slider::Slider()
{
    //ctor
}

Slider::~Slider()
{
    //dtor
}

void Slider::Init(float width, float center_x,
                  float center_y, float init_value){
    float vert[] = {center_x - sliderWidth/2.0 + (init_value-0.5)*width, center_y - sliderHeight/2.0, -0.5f,
                    center_x + sliderWidth/2.0 + (init_value-0.5)*width, center_y - sliderHeight/2.0, -0.5f,
                    center_x + sliderWidth/2.0 + (init_value-0.5)*width, center_y + sliderHeight/2.0, -0.5f,
                    center_x - sliderWidth/2.0 + (init_value-0.5)*width, center_y + sliderHeight/2.0, -0.5f,

                    center_x - width/2.0, center_y - pathHight/2.0, -0.25f,
                    center_x + width/2.0, center_y - pathHight/2.0, -0.25f,
                    center_x + width/2.0, center_y + pathHight/2.0, -0.25f,
                    center_x - width/2.0, center_y + pathHight/2.0, -0.25f};
    float color[] = {0.75f, 0.75f, 0.75f, 1.0f,
                    0.75f, 0.75f, 0.75f, 1.0f,
                    0.75f, 0.75f, 0.75f, 1.0f,
                    0.75f, 0.75f, 0.75f, 1.0f,

                    0.5f, 0.5f, 0.5f, 1.0f,
                    0.5f, 0.5f, 0.5f, 1.0f,
                    0.5f, 0.5f, 0.5f, 1.0f,
                    0.5f, 0.5f, 0.5f, 1.0f};
    float tex[] = {0.0f, 0.0f,
                    1.0f, 0.0f,
                    1.0f, 1.0f,
                    0.0f, 1.0f,

                    0.0f, 0.0f,
                    1.0f, 0.0f,
                    1.0f, 1.0f,
                    0.0f, 1.0f};

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &colors);
    glBindBuffer(GL_ARRAY_BUFFER, colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(0));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &texels);
    glBindBuffer(GL_ARRAY_BUFFER, texels);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)(0));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader = glCreateProgram();
    glAttachShader(shader, Graphics::LoadShader(GL_VERTEX_SHADER, vertexShaderPath));
    glAttachShader(shader, Graphics::LoadShader(GL_FRAGMENT_SHADER, fragmentShaderPath));
    glLinkProgram(shader);
    xCenter = center_x;
    yCenter = center_y;
    this->width = width;
    this->value = init_value;
}

void Slider::Draw(){
    glBindVertexArray(VAO);
    glUseProgram(shader);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, indices);
    glUseProgram(0);
    glBindVertexArray(0);
}

bool Slider::Clicked(){
    if(glfwGetMouseButton(Graphics::GetWindow(), GLFW_MOUSE_BUTTON_LEFT)){
        float x, y;
        Graphics::CursorPosInWindowSpace(&x, &y);
        if(((x-xCenter)<=(width/2.0) && -(x-xCenter)<=(width/2.0)
           &&(y-yCenter)<=(sliderHeight/2.0) && -(y-yCenter)<=(sliderHeight/2.0)) || click){
            if(mutex == false){
                click = true;
                mutex = true;
                return true;
            }
            if(mutex == true){
                return false;
            }
        }
        else{
            mutex = false;
        }
    }
    click = false;
    return false;
}

void Slider::Update(){
float x, y;
float* data;
    if(Clicked()){
        Graphics::CursorPosInWindowSpace(&x, &y);
        value = (x-xCenter+width/2)/width;
        if(value<0){
            value = 0;
        }
        if(value>1){
            value = 1;
        }
    }
    x = xCenter-width/2+width*value;

    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    data = (float*)(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    data[0] = x - sliderWidth/2.0;
    data[3] = x + sliderWidth/2.0;
    data[6] = x + sliderWidth/2.0;
    data[9] = x - sliderWidth/2.0;
    glUnmapBuffer(GL_ARRAY_BUFFER);
    data = nullptr;
}

float Slider::GetValue(){
    return value;
}

void Slider::SetValue(float value){
    if(value>1.0f)
        value = 1.0f;
    else if(value<0.0f)
        value = 0.0f;
    this->value = value;
}
