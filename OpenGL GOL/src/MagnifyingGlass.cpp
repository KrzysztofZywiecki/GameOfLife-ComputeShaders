#include "MagnifyingGlass.h"

MagnifyingGlass::MagnifyingGlass()
{
    //ctor
}

MagnifyingGlass::~MagnifyingGlass()
{
    //dtor
}

void MagnifyingGlass::Init(float radious, float zoom){
    shader = glCreateProgram();
    glAttachShader(shader, Graphics::LoadShader(GL_VERTEX_SHADER, vertexShaderPath));
    glAttachShader(shader, Graphics::LoadShader(GL_GEOMETRY_SHADER, geometryShaderPath));
    glAttachShader(shader, Graphics::LoadShader(GL_FRAGMENT_SHADER, fragmentShaderPath));
    glLinkProgram(shader);

    this->radious = radious;
    this->zoom = zoom;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &position_L);
    glBindBuffer(GL_ARRAY_BUFFER, position_L);
    glBufferData(GL_ARRAY_BUFFER, 2*sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    Graphics::CursorPosInWindowSpace(position, position+1);
    memcpy(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY), position, sizeof(float)*2);
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glGenBuffers(1, &radious_L);
    glBindBuffer(GL_ARRAY_BUFFER, radious_L);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float), &radious, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(0));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &zoom_L);
    glBindBuffer(GL_ARRAY_BUFFER, zoom_L);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float), &zoom, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(0));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MagnifyingGlass::Update(){
    static bool buttonClicked = false;
    Graphics::CursorPosInWindowSpace(position, position+1);
    glBindBuffer(GL_ARRAY_BUFFER, position_L);
    memcpy(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY), position, sizeof(float)*2);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ProcessKeyboardInput();
}

void MagnifyingGlass::Draw(unsigned int texture){
    if(show){
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUseProgram(shader);
        glDrawArrays(GL_POINTS, 0, 1);
        glUseProgram(0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void MagnifyingGlass::ProcessKeyboardInput(){
    if(glfwGetKey(Graphics::GetWindow(), GLFW_KEY_W)==GLFW_PRESS){
        if(!buttonClicked){
            zoom *=2.0f;
            glBindBuffer(GL_ARRAY_BUFFER, zoom_L);
            memcpy(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY), &zoom, sizeof(float));
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }
        buttonClicked = true;
    }
    else if(glfwGetKey(Graphics::GetWindow(), GLFW_KEY_S)==GLFW_PRESS){
        if(!buttonClicked && zoom != 1.0f){
            zoom /=2.0f;
            glBindBuffer(GL_ARRAY_BUFFER, zoom_L);
            memcpy(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY), &zoom, sizeof(float));
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }
        buttonClicked = true;
    }
    else if(glfwGetKey(Graphics::GetWindow(), GLFW_KEY_H)==GLFW_PRESS){
        if(!buttonClicked){
            show = show?false:true;
        }
        buttonClicked = true;
    }
    else if(glfwGetKey(Graphics::GetWindow(), GLFW_KEY_F)==GLFW_PRESS){
        if(!buttonClicked){
            SwapFullscreen();
        }
        buttonClicked = true;
    }
    else{
        buttonClicked = false;
    }
}

void MagnifyingGlass::SwapFullscreen(){
    float* data;
    if(fullscreen == false){
        fullscreen = true;
        glBindBuffer(GL_ARRAY_BUFFER, radious_L);
        data = (float*)(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
        *data = 1.0f;
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    else if(fullscreen == true){
        fullscreen = false;
        glBindBuffer(GL_ARRAY_BUFFER, radious_L);
        data = (float*)(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
        *data = radious;
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
