#pragma once

#ifndef __EMSCRIPTEN__
#include <Windows.h>


class FileDialog
{
public:
	static char* Open();
};
#endif