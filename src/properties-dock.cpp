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
		if (mainWidget &&
		    mainWidget->findChild<OBSPropertiesView *>()->IsObject(
			    source))
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

		OBSDataAutoRelease settings = obs_source_get_settings(source);
		OBSPropertiesView *propertiesView = new OBSPropertiesView(
			settings.Get(), source,
			(PropertiesReloadCallback)obs_source_properties,
			(PropertiesUpdateCallback)updateCb,
			(PropertiesVisualUpdateCb)updateCbWithoutUndo);

		QVBoxLayout *layout = new QVBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(propertiesView, 1);

		obs_properties_t *props = obs_source_properties(source);
		if (obs_properties_get_flags(props) &
		    OBS_PROPERTIES_DEFER_UPDATE) {
			QPushButton *button = new QPushButton(QTStr("Apply"));
			connect(button, &QPushButton::clicked, [this]() {
				mainWidget->findChild<OBSPropertiesView *>()
					->UpdateSettings();
			});
			button->setSizePolicy(QSizePolicy::Fixed,
					      QSizePolicy::Fixed);
			layout->addWidget(button, 0, Qt::AlignRight);
		}
		obs_properties_destroy(props);

		mainWidget = new QWidget();
		mainWidget->setLayout(layout);
		setWidget(mainWidget);
	} else {
		if (mainWidget) {
			delete mainWidget;
			mainWidget = nullptr;
		}

		if (!nothingSelectedLabel)
			nothingSelectedLabel = new QLabel(QTStr(
				"Basic.TransformWindow.NoSelectedSource"));
		setWidget(nothingSelectedLabel);
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
