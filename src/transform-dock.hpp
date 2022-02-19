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

#include "transform-view.hpp"
#include "obs-classes-helper-functions.hpp"

#include <obs.hpp>
#include <obs-module.h>

#include <QDockWidget>
#include <QVBoxLayout>
#include <QLabel>

class TransformDock : public QDockWidget {
	Q_OBJECT

public:
	TransformDock(QWidget *parent = nullptr);
	void SetSceneItem(OBSSceneItem item);

private:
	OBSBasicTransform *transformView = nullptr;
};
