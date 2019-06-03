#include "Map.h"

#define ASSERT(x) if (!(x)) DebugBreak();

#define GlCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x))

#define SOURCE_LEN 2048

Map::Map()
{

}

Map::~Map()
{
    //dtor
}

void Map::CreateTextures(size_t x, size_t y){
    if(textures[0] != 0 && textures[1] != 0){
        glDeleteTextures(2, textures);
    }
    glGenTextures(2, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, x, y);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, x, y);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

    glBindTexture(GL_TEXTURE_2D, 0);

    this->x = x;
    this->y = y;
    workgroups_x = x/8;
    workgroups_y = y/8;
    rect.SetDimensions({-1.0f, -1.0f}, {1.0f, 1.0f}, 0.0f);
}

void Map::LoadShaders(const char* vertexPath, const char* fragmentPath, const char* computePath, const char* fillWithNoisePath){
    renderingProgram = glCreateProgram();
    glAttachShader(renderingProgram,
    Graphics::LoadShader(GL_VERTEX_SHADER, vertexPath));
    glAttachShader(renderingProgram,
    Graphics::LoadShader(GL_FRAGMENT_SHADER, fragmentPath));
    glLinkProgram(renderingProgram);

    GLint program_linked;
    glGetProgramiv(renderingProgram, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[SOURCE_LEN];
        glGetProgramInfoLog(renderingProgram, SOURCE_LEN, &log_length, message);
        std::cout<<message;
    }

    shaderForLoadingNoise = glCreateProgram();
    glAttachShader(shaderForLoadingNoise, Graphics::LoadShader(GL_COMPUTE_SHADER, fillWithNoisePath));
    glLinkProgram(shaderForLoadingNoise);

    glGetProgramiv(shaderForLoadingNoise, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[SOURCE_LEN];
        glGetProgramInfoLog(shaderForLoadingNoise, SOURCE_LEN, &log_length, message);
        std::cout<<message;
    }

    computeProgram = glCreateProgram();
    glAttachShader(computeProgram,
    Graphics::LoadShader(GL_COMPUTE_SHADER, computePath));
    glLinkProgram(computeProgram);

    glGetProgramiv(computeProgram, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[SOURCE_LEN];
        glGetProgramInfoLog(computeProgram, SOURCE_LEN, &log_length, message);
        std::cout<<message;
    }

    aliveConditionalLocation = glGetUniformLocation(computeProgram, "aliveConditional");
    deadConditionalLocation = glGetUniformLocation(computeProgram, "deadConditional");
}

void Map::FillActiveTextureRandomly(float treshold){
    srand(time(nullptr));
    glBindTexture(GL_TEXTURE_2D, textures[activeTexture]);
    float* data = new float[x*y*4];

    for(float* i=data; i<data + x*y*4; i+=4){
        float color = (double)rand()/(double)RAND_MAX;
        if(color>treshold){
            color = 1.0f;
        }
        else{
            color = 0.0f;
        }
        *i = color;
        *(i + 1) = color;
        *(i + 2) = color;
        *(i + 3) = 1.0f;
    }
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, x, y, GL_RGBA, GL_FLOAT, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    delete[] data;
}

void Map::FillActiveTextureWithNoise(float* data, size_t size){
    unsigned int noise;
    glGenTextures(1, &noise);
    glBindTexture(GL_TEXTURE_2D, noise);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    int resolution[2];
    resolution[0] = x;
    resolution[1] = y;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, noise);

    glUseProgram(shaderForLoadingNoise);
    glBindImageTexture(0, textures[activeTexture], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glUniform2i(glGetUniformLocation(shaderForLoadingNoise, "resolution"), resolution[0], resolution[1]);
    glDispatchCompute(workgroups_x, workgroups_y, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glUseProgram(0);
}

void Map::Update(float time){

    timer+= time/1000;
    if(timer>(1.0f/(float)(maxFPS*FPSmultiplier))){
        glUseProgram(computeProgram);

        glUniform1ui(aliveConditionalLocation, aliveConditional);
        glUniform1ui(deadConditionalLocation, deadConditional);

        glBindImageTexture(0, textures[activeTexture], 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindImageTexture(1, textures[complementaryTexture], 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

        glDispatchCompute(workgroups_x, workgroups_y, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        int buf = complementaryTexture;
        complementaryTexture = activeTexture;
        activeTexture = buf;
        glUseProgram(0);
        timer = timer-(1.0f/((float)(maxFPS*FPSmultiplier)))*(int)(timer/(1.0f/(float)(maxFPS*FPSmultiplier)));
    }

}

void Map::Draw(){
    glBindVertexArray(rect.GetVAO());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[activeTexture]);
    glUseProgram(renderingProgram);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, Geometry::rect_indices);
    glUseProgram(0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Map::GetActiveTexture(){
    return textures[activeTexture];
}

void Map::ChangeFPS(float value){
    FPSmultiplier = value;
}

void Map::ChangeRules(unsigned char forAlive, unsigned char forDead){
    aliveConditional = (unsigned int)(forAlive);
    deadConditional = (unsigned int)(forDead);
}

