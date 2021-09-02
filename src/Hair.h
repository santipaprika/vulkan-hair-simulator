#pragma once
#include <cyHairFile.h>

namespace vkr {

class Hair {
   private:
    cyHairFile hair;
    float *dirs;

   public:
    Hair(const char *filename);
    ~Hair();

    void loadHairModel(const char *filename, cyHairFile &hairfile, float *&dirs);
    // void drawHairModel(const cyHairFile &hairfile, float *dirs);
};

}  // namespace vkr
