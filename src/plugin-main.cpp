/*
Properties Dock
Copyright (C) 2024 Sebastian Beckmann

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <memory>
#include <QVersionNumber>

#include <obs-module.h>
#include <plugin-support.h>

#include "dock-manager.hpp"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

static std::unique_ptr<DockManager> dockManager;

bool obs_module_load(void)
{
    obs_log(LOG_INFO, "Plguin version %s", PLUGIN_VERSION);
    obs_log(LOG_INFO, "OBS version: %d.%d.%d (compiled), %s (runtime)", LIBOBS_API_MAJOR_VER, LIBOBS_API_MINOR_VER,
            LIBOBS_API_PATCH_VER, obs_get_version_string());
    obs_log(LOG_INFO, "Qt version: %s (compiled), %s (runtime)", QT_VERSION_STR, qVersion());

    dockManager.reset(new DockManager());

    return true;
}

void obs_module_unload()
{
    dockManager.reset();
}
