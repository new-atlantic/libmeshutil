/*
 * Copyright © 2012 Torsti Schulz
 *
 * This file is part of the meshutil library.
 *
 * libmeshutil is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libmeshutil is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MESHUTIL_BATMAN_ADV_KERNELMODULE_H
#define MESHUTIL_BATMAN_ADV_KERNELMODULE_H 1

#ifdef __linux

#include <string>

namespace meshutil {
namespace batman_adv {

	class KernelModule {
		public:
			bool isAvailable();
			bool isLoaded();
			std::string Version();
	};

}
}

#endif
#endif

