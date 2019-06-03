#ifndef MAP_H
#define MAP_H

#include "Graphics.h"
#include "Rectangle.h"
#include <ctime>
#include <immintrin.h>

class Map{
public:
    Map();
    ~Map();

    void Update(float time); //Zamienia teksture src na dst, uruchamia obliczenia dla mapy, czy cos
    void Draw(); //Rysuje aktualnie przechowywana teksture
    void CreateTextures(size_t x, size_t y); //Ustawia rozmiar mapy. x i y musza byc podzielne przez 16
    void RecreateTextures(size_t x, size_t y);
    void LoadShaders(const char* vertexPath, const char* fragmentPath, const char* computePath, const char* fillWithNoisePath);
    void FillActiveTextureWithNoise(float* data, size_t size);
    unsigned int GetActiveTexture();
    void FillActiveTextureRandomly(float treshold);
    void ChangeFPS(float value);
    void ChangeRules(unsigned char forAlive, unsigned char forDead);
private:
    const int maxFPS = 60;
    float FPSmultiplier = 0.5;
    float timer = 0;

    unsigned char aliveConditional;
    unsigned char deadConditional;

    unsigned int aliveConditionalLocation;
    unsigned int deadConditionalLocation;
    unsigned int renderingProgram;
    unsigned int computeProgram;
    unsigned int shaderForLoadingNoise;
    Geometry::Rectangle rect;
    size_t x, y;
    size_t workgroups_x, workgroups_y;
    unsigned int textures[2] = {0, 0};
    int activeTexture = 0; //Tekstura u¿ywana jako source, oraz do rysowania
    int complementaryTexture = 1;
};

#endif // MAP_H
