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

#include "properties-dock.hpp"

void PropertiesDock::SetSource(OBSSource source)
{
	if (source) {
		/* Check if the properties changed, otherwise return early */
		if (propertiesView && propertiesView->IsObject(source))
			return;

		auto updateCb = [](void *obj, obs_data_t *old_settings,
				   obs_data_t *new_settings) {
			UNUSED_PARAMETER(old_settings); // TODO: Undo/Redo
			OBSSource source = static_cast<obs_source_t *>(obj);
			obs_source_update(source, new_settings);
		};

		auto updateCbWithoutUndo = [](void *obj, obs_data_t *settings) {
			OBSSource source = static_cast<obs_source_t *>(obj);
			obs_source_update(source, settings);
		};

		if (propertiesView)
			delete propertiesView;

		OBSDataAutoRelease settings = obs_source_get_settings(source);
		propertiesView = new OBSPropertiesView(
			settings.Get(), source,
			(PropertiesReloadCallback)obs_source_properties,
			(PropertiesUpdateCallback)updateCb,
			(PropertiesVisualUpdateCb)updateCbWithoutUndo);

		setWidget(propertiesView);
	} else {
		if (propertiesView) {
			delete propertiesView;
			propertiesView = nullptr;
		}

		setWidget(new QLabel(
			QTStr("Basic.TransformWindow.NoSelectedSource")));
	}
}

PropertiesDock::PropertiesDock(QWidget *parent) : QDockWidget(parent)
{
	setFeatures(DockWidgetMovable | DockWidgetFloatable |
		    DockWidgetClosable);
	setWindowTitle(obs_module_text("PropertiesDock.Title"));
	setObjectName("PropertiesDock");
	setFloating(true);
}
