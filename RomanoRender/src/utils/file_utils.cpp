#include "file_utils.h"


bool file_exists(const std::string& abs_filename)
{
    bool ret;
    FILE* fp = fopen(abs_filename.c_str(), "rb");
    if (fp) {
        ret = true;
        fclose(fp);
    }
    else {
        ret = false;
    }

    return ret;
}


std::string get_base_dir(const std::string& filepath) 
{
    //since find_last_of is fairly expensive, it would be good to cache this in a const variable instead of calling it twice
    if (filepath.find_last_of("/\\") != std::string::npos)
        return filepath.substr(0, filepath.find_last_of("/\\"));
    return "";
}