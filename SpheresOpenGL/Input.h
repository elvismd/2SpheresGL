#pragma once

#include "Include.h"

void InitInput();
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void UpdateInput();
extern bool keys[1024];