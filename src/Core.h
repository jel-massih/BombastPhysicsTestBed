#ifndef CORE_H
#define CORE_H

#include <stdint.h>

uint32_t hashString(const char string[], uint32_t hash = 0);

uint32_t loadShader(const char* vertexFilePath, const char* fragmentFilePath);
void deleteShader(uint32_t shader);

#endif