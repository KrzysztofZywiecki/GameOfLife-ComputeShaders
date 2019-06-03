#include "BmpFiles.h"

#define ASSERT(x) if (!(x)) DebugBreak();

#define GlCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x))

std::unordered_map<std::string, unsigned int> BmpFiles::files;

BmpFiles::BmpFiles()
{
    //ctor
}

BmpFiles::~BmpFiles()
{
    //dtor
}

unsigned int BmpFiles::GetFile(std::string filename){
    auto result = files.find(filename);
    if(result == files.end()){
        return LoadFile(filename);
    }
    else{
        return result->second;
    }
}

unsigned int BmpFiles::LoadFile(std::string filename){
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cout<<"Error while loading file: "<<filename<<std::endl;
        return files.find("default")->second;
    }

    unsigned char info[54];
    file.get(reinterpret_cast<char*>(info), 54);

    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size];
    file.get(reinterpret_cast<char*>(data), size, '\0');

    //for(int i = 0; i < size; i += 3)
    //{
    //        unsigned char tmp = data[i];
    //        data[i] = data[i+2];
    //        data[i+2] = tmp;
    //}

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    files.insert(std::pair<std::string, unsigned int>(filename, texture));
    return texture;
}

void BmpFiles::LoadDefault(){
    float* DefColor = new float[4];
    DefColor[0] = 1.0f;
    DefColor[1] = 0.0f;
    DefColor[2] = 1.0f;
    DefColor[3] = 1.0f;
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, DefColor);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    files.insert(std::pair<std::string, unsigned int>("default", texture));
    delete[] DefColor;
}
