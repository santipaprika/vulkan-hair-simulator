#include "Hair.h"

namespace vkr {

Hair::Hair(const char *filename) {
    loadHairModel(filename, this->hair, dirs);
}

Hair::~Hair() {
    delete[] dirs;
}

void Hair::loadHairModel(const char *filename, cyHairFile &hairfile, float *&dirs) {
    // Load the hair model
    int result = hairfile.LoadFromFile(filename);
    // Check for errors
    switch (result) {
        case CY_HAIR_FILE_ERROR_CANT_OPEN_FILE:
            printf("Error: Cannot open hair file!\n");
            return;
        case CY_HAIR_FILE_ERROR_CANT_READ_HEADER:
            printf("Error: Cannot read hair file header!\n");
            return;
        case CY_HAIR_FILE_ERROR_WRONG_SIGNATURE:
            printf("Error: File has wrong signature!\n");
            return;
        case CY_HAIR_FILE_ERROR_READING_SEGMENTS:
            printf("Error: Cannot read hair segments!\n");
            return;
        case CY_HAIR_FILE_ERROR_READING_POINTS:
            printf("Error: Cannot read hair points!\n");
            return;
        case CY_HAIR_FILE_ERROR_READING_COLORS:
            printf("Error: Cannot read hair colors!\n");
            return;
        case CY_HAIR_FILE_ERROR_READING_THICKNESS:
            printf("Error: Cannot read hair thickness!\n");
            return;
        case CY_HAIR_FILE_ERROR_READING_TRANSPARENCY:
            printf("Error: Cannot read hair transparency!\n");
            return;
        default:
            printf("Hair file \"%s\" loaded.\n", filename);
    }
    int hairCount = hairfile.GetHeader().hair_count;
    int pointCount = hairfile.GetHeader().point_count;
    printf("Number of hair strands = %d\n", hairCount);
    printf("Number of hair points = %d\n", pointCount);

    // Allocate space for directions
    dirs = (float *)malloc(sizeof(float) * pointCount * 3);
    // Compute directions
    if (hairfile.FillDirectionArray(dirs) == 0) {
        printf("Error: Cannot compute hair directions!\n");
    }
}

// void Hair::drawHairModel(const cyHairFile &hairfile, float *dirs) {
//     // Set point array
//     glVertexPointer(3, GL_FLOAT, 0, hairfile.GetPointsArray());
//     glEnableClientState(GL_VERTEX_ARRAY);
//     // Set normal array
//     glNormalPointer(GL_FLOAT, 0, dirs);
//     glEnableClientState(GL_NORMAL_ARRAY);
//     // Set color array (if exists)
//     float *colors = hairfile.GetColorsArray();
//     if (colors) {
//         glColorPointer(3, GL_FLOAT, 0, colors);
//         glEnableClientState(GL_COLOR_ARRAY);
//     }
//     // Draw arrays
//     int pointIndex = 0;
//     int hairCount = hairfile.GetHeader().hair_count;
//     const unsigned short *segments = hairfile.GetSegmentsArray();
//     if (segments) {
//         // If segments array exists
//         for (int hairIndex = 0; hairIndex < hairCount; hairIndex++) {
//             glDrawArrays(GL_LINE_STRIP, pointIndex, segments[hairIndex] + 1);
//             pointIndex += segments[hairIndex] + 1;
//         }
//     } else {
//         // If segments array does not exist, use default segment count
//         int dsegs = hairfile.GetHeader().d_segments;
//         for (int hairIndex = 0; hairIndex < hairCount; hairIndex++) {
//             glDrawArrays(GL_LINE_STRIP, pointIndex, dsegs + 1);
//             pointIndex += dsegs + 1;
//         }
//     }
//     // Disable arrays
//     glDisableClientState(GL_VERTEX_ARRAY);
//     glDisableClientState(GL_NORMAL_ARRAY);
//     glDisableClientState(GL_COLOR_ARRAY);
// }

}  // namespace vkr
