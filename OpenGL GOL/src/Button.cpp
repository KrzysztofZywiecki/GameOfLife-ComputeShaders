#include "Button.h"

Button::Button()
{
    //ctor
}

Button::Button(vec2f BL, vec2f TR, unsigned char value, State state)
{
    CreateButton(BL, TR, value, state);
}

Button::~Button()
{
    //dtor
}

bool Button::Clicked(){
    float x, y;
    vec2f BL, TR;
    rect.GetCorners(&BL, &TR);
    Graphics::CursorPosInWindowSpace(&x, &y);

    if(x>=BL.x && x<=TR.x && y>=BL.y && y<=TR.y){
        return true;
    }
    return false;
}

void Button::CreateButton(vec2f BL, vec2f TR, unsigned char value, State state){
    rect.SetDimensions(BL, TR, -0.15f);
    representedNumber = value;
    if(state != SINGLE_ACTION)
        this->state = state;
    else
        this->state = INACTIVE;

    shader = glCreateProgram();
    glAttachShader(shader, Graphics::LoadShader(GL_VERTEX_SHADER, vertexShaderPath));
    glAttachShader(shader, Graphics::LoadShader(GL_FRAGMENT_SHADER, fragmentShaderPath));
    glLinkProgram(shader);
    this->color = glGetUniformLocation(shader, "color");

    glUseProgram(shader);
    if(state == SINGLE_ACTION){
        float color[3] = {0.8f, 0.8f, 0.8f};
        glUniform3f(this->color, color[0], color[1], color[2]);
    }
    if(state == INACTIVE){
    float color[3] = {0.8f, 0.0f, 0.2f};
    glUniform3f(this->color, color[0], color[1], color[2]);
    }
    if(state == ACTIVE){
    float color[3] = {0.0f, 0.8f, 0.2f};
    glUniform3f(this->color, color[0], color[1], color[2]);
    }
    glUseProgram(0);
}

void Button::SetState(State newState){
    float color[3];
    if(newState == ACTIVE){
        state = newState;
        color[0] = 0.0f;
        color[1] = 0.8f;
        color[2] = 0.2f;
        glUseProgram(shader);
        glUniform3f(this->color, color[0], color[1], color[2]);
        glUseProgram(0);
    }
    else if(newState == INACTIVE){
        state = newState;
        color[0] = 0.8f;
        color[1] = 0.0f;
        color[2] = 0.2f;
        glUseProgram(shader);
        glUniform3f(this->color, color[0], color[1], color[2]);
        glUseProgram(0);
    }
    else if(newState == SINGLE_ACTION){
        state = INACTIVE;
        color[0] = 0.8f;
        color[1] = 0.8f;
        color[2] = 0.8f;
        glUseProgram(shader);
        glUniform3f(this->color, color[0], color[1], color[2]);
        glUseProgram(0);
    }
}

void Button::Update(){
    if(glfwGetMouseButton(Graphics::GetWindow(), GLFW_MOUSE_BUTTON_LEFT)){
        if(!clicked){
            if(Clicked()){
                if(state == ACTIVE){
                    SetState(INACTIVE);
                }
                else{
                    SetState(ACTIVE);
                }
            }
        }
        clicked = true;
    }
    else{
        clicked = false;
    }
}

void Button::Draw(){
    glBindVertexArray(rect.GetVAO());
    glUseProgram(shader);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, Geometry::rect_indices);
    glUseProgram(0);
    glBindVertexArray(0);
}

Button::ButtonData Button::GetData(){
    ButtonData data;
    data.value = representedNumber;
    data.state = state;
    return data;
}

void Button::SetValue(unsigned int value){
    representedNumber = value;
}
