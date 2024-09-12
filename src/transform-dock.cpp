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

#include "moc_transform-dock.cpp"

void TransformDock::SetSceneItem(OBSSceneItem item)
{
    if (item) {
        if (transformView) {
            transformView->SetNewItem(item);
        } else {
            transformView = new OBSBasicTransform(item);
            layout->removeWidget(nothingSelectedLabel);
            nothingSelectedLabelSet = false;
            layout->addWidget(transformView);
        }
    } else {
        if (transformView) {
            // TODO: There's no need to delete the transform view really.
            layout->removeWidget(transformView);
            transformView->deleteLater();
            transformView = nullptr;
        }

        if (!nothingSelectedLabel) {
            nothingSelectedLabel = new QLabel(tr("Basic.TransformWindow.NoSelectedSource"));
        }

        if (!nothingSelectedLabelSet) {
            layout->addWidget(nothingSelectedLabel);
            nothingSelectedLabelSet = true;
        }
    }
}
