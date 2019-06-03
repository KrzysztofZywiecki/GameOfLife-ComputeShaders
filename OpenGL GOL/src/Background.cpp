#include "Background.h"

Background::Background()
{
    //ctor
}

Background::~Background()
{
    //dtor
}

void Background::Draw(){
    glBindVertexArray(rect.GetVAO());
    glUseProgram(shader);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, Geometry::rect_indices);
    glUseProgram(0);
    glBindVertexArray(0);
}

void Background::Create(){
    rect.SetDimensions({-1.0f, -1.0f}, {1.0f, 1.0f}, -0.1f);
    shader = glCreateProgram();
    glAttachShader(shader, Graphics::LoadShader(GL_VERTEX_SHADER, vertexShaderPath));
    glAttachShader(shader, Graphics::LoadShader(GL_FRAGMENT_SHADER, fragmentShaderPath));
    glLinkProgram(shader);
}
