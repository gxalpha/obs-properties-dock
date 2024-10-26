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

#pragma once

#include <memory>
#include <QPointer>

#include <obs.hpp>
#include <obs-frontend-api.h>

class PropertiesDock;
class TransformDock;
class OBSSignal;

class DockManager {
public:
    DockManager();
    ~DockManager();

private:
    QPointer<PropertiesDock> properties;
    QPointer<TransformDock> transform;
    std::unique_ptr<OBSSignal> selectSignal;
    std::unique_ptr<OBSSignal> deselectSignal;
    int selectedItemsCount;
    OBSSceneItem selectedItem;

    static void SceneChangeEvent(enum obs_frontend_event event, void *);
    static void SceneItemSelectSignal(void *, calldata_t *data);
    static void SceneItemDeselectSignal(void *, calldata_t *);
};
