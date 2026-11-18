#define STB_IMAGE_IMPLEMENTATION
#include "rtw_stb_image.hpp"

rtw_image::~rtw_image() {
    delete[] bdata;
    STBI_FREE(fdata);
}

