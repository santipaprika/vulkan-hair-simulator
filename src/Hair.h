#pragma once
#include <cyHairFile.h>

class Hair
{
private:
    cyHairFile hair;
    float* dirs;
public:
    Hair(const char *filename);
    ~Hair();

    void LoadHairModel( const char *filename, cyHairFile &hairfile, float *&dirs );
};
