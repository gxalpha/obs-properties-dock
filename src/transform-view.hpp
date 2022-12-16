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

#include "ui_OBSBasicTransform.h"
#include "obs.hpp"

#include <QWidget>
#include <QDialog>
#include <QMainWindow>
#include <memory>

class OBSBasicTransform : public QDialog {
    Q_OBJECT

public:
    std::unique_ptr<Ui::OBSBasicTransform> ui;
    OBSBasicTransform(OBSSceneItem item);
    void SetNewItem(OBSSceneItem item);

private:
    OBSSceneItem item;
    OBSSignal transformSignal;
    void HookWidgets();
    void HookWidget(QWidget *widget, const char *signal, const char *slot);

    int ignoreTransformSignal = 0;
    int ignoreItemChange = 0;

    static void SceneItemTransformSignal(void *param, calldata_t *data);

private slots:
    void OnControlChanged();
    void OnCropChanged();
    void OnBoundsType(int index);
    void LoadValues();
};
