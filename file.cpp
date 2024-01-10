
#include <fstream>
#include "file.h"

void save_csv(const std::string &path, std::vector<std::string> header, array& arr) {
    if(header.size() != arr.shape(-1)) {
        throw std::runtime_error("Header size does not match data size");
    }
    arr.eval();
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file");
    }
    for(int i = 0; i < header.size(); i++) {
        
        file << header[i];
        if(i < header.size() - 1) {
            file << ",";
        }
    }
    file << std::endl;
    auto data = arr.data<float>();
    for(int i = 0; i < arr.shape(0); i++) {
        for(int j = 0; j < arr.shape(1); j++) {
            file << data[i * arr.shape(1) + j];
            if(j < arr.shape(1) - 1) {
                file << ",";
            }
        }
        file << std::endl;
    }
    file.close();
}

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
        width = 1;
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
    float f[width * height];
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