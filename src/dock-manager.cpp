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

#include "dock-manager.hpp"

#include <obs-module.h>
#include <QMainWindow>

#include "properties-dock.hpp"
#include "transform-dock.hpp"

void DockManager::SceneItemSelectSignal(void *ptr, calldata_t *data)
{
    DockManager *dockManager = static_cast<DockManager *>(ptr);

    dockManager->selectedItemsCount++;

    OBSSceneItem item = static_cast<obs_sceneitem_t *>(calldata_ptr(data, "item"));
    OBSSource source = obs_sceneitem_get_source(item);

    dockManager->properties->SetSource(source);
    dockManager->transform->SetSceneItem(item);
}

void DockManager::SceneItemDeselectSignal(void *ptr, calldata_t *)
{
    DockManager *dockManager = static_cast<DockManager *>(ptr);

    dockManager->selectedItemsCount--;
    if (dockManager->selectedItemsCount == 0) {
        dockManager->properties->SetSource(nullptr);
        dockManager->transform->SetSceneItem(nullptr);
    }
}

void DockManager::SceneChangeEvent(enum obs_frontend_event event, void *ptr)
{
    if (event != OBS_FRONTEND_EVENT_PREVIEW_SCENE_CHANGED)
        return;

    DockManager *dockManager = static_cast<DockManager *>(ptr);

    /* Get new current scene */
    OBSSourceAutoRelease currentSceneSource = obs_frontend_preview_program_mode_active()
                                                      ? obs_frontend_get_current_preview_scene()
                                                      : obs_frontend_get_current_scene();

    /* Connect signal to new source */
    signal_handler_t *sh = obs_source_get_signal_handler(currentSceneSource);
    dockManager->selectSignal->Connect(sh, "item_select", SceneItemSelectSignal, dockManager);
    dockManager->deselectSignal->Connect(sh, "item_deselect", SceneItemDeselectSignal, dockManager);

    /* Get current item and count */
    // TODO: This code is in need of improvement
    dockManager->selectedItemsCount = 0;
    struct cb_data {
        int *selectedItemsCount;
        OBSSceneItem selectedItem;
    } data;
    data.selectedItemsCount = &dockManager->selectedItemsCount;
    data.selectedItem = nullptr;
    auto cb = [](obs_scene_t *, obs_sceneitem_t *item, void *data) {
        struct cb_data *cb_data = static_cast<struct cb_data *>(data);
        if (obs_sceneitem_selected(item)) {
            (*cb_data->selectedItemsCount)++;
            /* Always override to get the top-most item */
            cb_data->selectedItem = item;
        }
        return true;
    };
    obs_scene_enum_items(obs_scene_from_source(currentSceneSource), cb, &data);

    OBSSceneItem item = data.selectedItem;
    OBSSource source = obs_sceneitem_get_source(item);
    dockManager->properties->SetSource(source);
    dockManager->transform->SetSceneItem(item);
}

DockManager::DockManager()
{
    QMainWindow *main = static_cast<QMainWindow *>(obs_frontend_get_main_window());
    properties = new PropertiesDock(main);
    transform = new TransformDock(main);
    selectSignal.reset(new OBSSignal());
    deselectSignal.reset(new OBSSignal());

    obs_frontend_add_dock_by_id("properties-dock", obs_module_text("PropertiesDock.Title"), properties.get());
    obs_frontend_add_dock_by_id("transform-dock", obs_module_text("TransformDock.Title"), transform.get());

    obs_frontend_add_event_callback(SceneChangeEvent, this);
}

DockManager::~DockManager()
{
    selectSignal.reset();
    deselectSignal.reset();
}
