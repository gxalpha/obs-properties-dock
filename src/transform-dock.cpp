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

#include "transform-dock.hpp"

void TransformDock::SetSceneItem(OBSSceneItem item)
{
	if (item) {
		if (transformView) {
			transformView->SetNewItem(item);
		} else {
			delete transformView;
			transformView = new OBSBasicTransform(item);
			setWidget(transformView);
		}
	} else {
		if (transformView) {
			delete transformView;
			transformView = nullptr;
		}

		setWidget(new QLabel(
			QTStr("Basic.TransformWindow.NoSelectedSource")));
	}
}

TransformDock::TransformDock(QWidget *parent) : QDockWidget(parent)
{
	setFeatures(DockWidgetMovable | DockWidgetFloatable |
		    DockWidgetClosable);
	setWindowTitle(obs_module_text("TransformDock.Title"));
	setObjectName("TransformDock");
	setFloating(true);
}
