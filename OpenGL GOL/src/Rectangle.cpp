#include "Rectangle.h"

Geometry::Rectangle::Rectangle()
{
    //ctor
}

Geometry::Rectangle::~Rectangle()
{
    //dtor
}

void Geometry::Rectangle::SetDimensions(vec2f BL, vec2f TR, float z){
    BottomLeft = BL;
    TopRight = TR;

    float vert[] = {BL.x, BL.y, z,
                    TR.x, BL.y, z,
                    TR.x, TR.y, z,
                    BL.x, TR.y, z};
    float tex[] =  {0.0f, 1.0f,
                    1.0f, 1.0f,
                    1.0f, 0.0f,
                    0.0f, 0.0f};
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)(0));
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &texels);
    glBindBuffer(GL_ARRAY_BUFFER, texels);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (void*)(0));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

unsigned int Geometry::Rectangle::GetVAO(){
    return VAO;
}

void Geometry::Rectangle::GetCorners(vec2f* BL, vec2f* TR){
    *BL = BottomLeft;
    *TR = TopRight;
}
