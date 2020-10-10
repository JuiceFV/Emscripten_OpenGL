#pragma once

#include "config.h"

namespace cfg
{
	namespace defaults
	{
		// The default window's width
        constexpr unsigned int WIN_WIDTH = 640;

        // The default window's height
		constexpr unsigned int WIN_HEIGHT = 480;

		// specifically these globals are like flags for get-property method.
		enum class WIN_PROPS
        {
			HEIGHT,
			WIDTH
		};
	}
} // namespace cfg