#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Graphics.h"

struct vec2f{
    float x,y;
};

namespace Geometry{

const unsigned int rect_indices[6] = {0, 1, 2, 0, 2, 3};

class Rectangle
{
    public:
        Rectangle();
        ~Rectangle();
        void SetDimensions(vec2f BL, vec2f TR, float z);
        void GetCorners(vec2f* BL, vec2f* TR);
        unsigned int GetVAO();
    private:
    vec2f BottomLeft;
    vec2f TopRight;
    unsigned int VAO;
    unsigned int vertices;
    unsigned int texels;
};
}

#endif // RECTANGLE_H
