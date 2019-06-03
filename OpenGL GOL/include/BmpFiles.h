#ifndef BMPFILES_H
#define BMPFILES_H

#include <unordered_map>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include "Graphics.h"


class BmpFiles
{
public:
    BmpFiles();
    ~BmpFiles();
    static unsigned int GetFile(std::string filename);
    static void LoadDefault();
private:
    static unsigned int LoadFile(std::string filename);
    static std::unordered_map<std::string, unsigned int> files;

};

#endif // BMPFILES_H
