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

#include <fstream>
#include <istream>
#include <system_error>

#include <errno.h>
#include <sys/utsname.h>
#include <unistd.h>

#include "KernelModule.hpp"

bool meshutil::batman_adv::KernelModule::isAvailable() {
	struct  utsname system_version_info;
	std::string KernelModulePath = "/lib/modules/";

	if (uname(&system_version_info)) {
		throw std::system_error(errno, std::system_category());
	}

	KernelModulePath += system_version_info.release;
	KernelModulePath += "/kernel/net/batman-adv/batman-adv.ko";

	if (!access(KernelModulePath.c_str(), F_OK)) {
		return true;
	} else if (errno == ENOENT) {
		return false;
	} else {
		throw std::system_error(errno, std::system_category());
	}
}

bool meshutil::batman_adv::KernelModule::isLoaded() {
	if (!access("/sys/module/batman_adv/version", F_OK)) {
		return true;
	} else if (errno == ENOENT) {
		return false;
	} else {
		throw std::system_error(errno, std::system_category());
	}
}

std::string meshutil::batman_adv::KernelModule::Version() {
	std::ifstream VersionFile("/sys/module/batman_adv/version");
	std::string Version;

	std::getline(VersionFile, Version);

	return Version.substr(0, Version.length() - 1);
}
