
#include <fstream>
#include "file.h"

array load_csv(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file");
    }

    int width, height = 0;
    std::string line;
    if (std::getline(file, line))
    {
        width = 0;
        for (auto c : line)
        {
            if (c == ',')
            {
                width++;
            }
        }
    }
    while (std::getline(file, line))
    {
        height++;
    }
    file.clear();
    file.seekg(0);
    // skip the header
    std::getline(file, line);
    // preallocate the array
    auto f = new float[width * height];
    int y = 0;
    while(std::getline(file, line)) {
        int start = 0;
        for(int j = 0; j < width; j++) {
            int end = line.find(',', start);
            std::string val = line.substr(start, end - start);
            f[y * width + j] = std::stof(val);
            start = end + 1;
        }
        y++;
    }


    file.close();
    return array(f, {height, width});
}