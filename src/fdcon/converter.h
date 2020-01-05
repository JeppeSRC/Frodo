#pragma once

#include "format.h"

namespace fd {
namespace converter {

uint8 ConvertBinary(const char* binFile, const char* outFile);
uint8 ConvertImage(const char* imageFile, const char* outFile);
uint8 ConvertModel(const char* modelFile, const char* outFile);

}}