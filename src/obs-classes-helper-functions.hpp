/*
OBS Properties Dock
Copyright (C) 2022 Sebastian Beckmann

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

#pragma once

#include <obs-frontend-api.h>
#include <QMainWindow>

#define QTStr(lookupVal) QString(obs_frontend_get_locale_string(lookupVal))

/* Modern problems require modern solutions */
class YupThisReallyIsAThing {
public:
	QMainWindow *GetMainWindow()
	{
		return static_cast<QMainWindow *>(
			obs_frontend_get_main_window());
	}
	/* TODO: Support light mode */
	inline bool IsThemeDark() { return true; }
};
YupThisReallyIsAThing *const dontUseThisPlzKThx = new YupThisReallyIsAThing();

inline YupThisReallyIsAThing *App()
{
	return dontUseThisPlzKThx;
}
