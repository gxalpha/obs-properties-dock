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

#include "moc_transform-view.cpp"

OBSBasicTransform::OBSBasicTransform(OBSSceneItem item) : ui(new Ui::OBSBasicTransform), item(item)
{
    ui->setupUi(this);

    // Remove buttons on the bottom
    delete ui->buttonBox;

    HookWidgets();
    LoadValues();

    OBSSource source = obs_scene_get_source(obs_sceneitem_get_scene(item));
    signal_handler_t *sh = obs_source_get_signal_handler(source);
    transformSignal.Connect(sh, "item_transform", SceneItemTransformSignal, this);
}

void OBSBasicTransform::SetNewItem(OBSSceneItem item)
{
    this->item = item;
    LoadValues();
}

/*------------------------------------------------------------*/
/* From here on mostly copied from window-basic-transform.cpp */
/*------------------------------------------------------------*/

#define COMBO_CHANGED &QComboBox::currentIndexChanged
#define ISCROLL_CHANGED &QSpinBox::valueChanged
#define DSCROLL_CHANGED &QDoubleSpinBox::valueChanged

void OBSBasicTransform::HookWidgets()
{
    HookWidget(ui->positionX, DSCROLL_CHANGED, &OBSBasicTransform::OnControlChanged);
    HookWidget(ui->positionY, DSCROLL_CHANGED, &OBSBasicTransform::OnControlChanged);
    HookWidget(ui->rotation, DSCROLL_CHANGED, &OBSBasicTransform::OnControlChanged);
    HookWidget(ui->sizeX, DSCROLL_CHANGED, &OBSBasicTransform::OnControlChanged);
    HookWidget(ui->sizeY, DSCROLL_CHANGED, &OBSBasicTransform::OnControlChanged);
    HookWidget(ui->align, COMBO_CHANGED, &OBSBasicTransform::OnControlChanged);
    HookWidget(ui->boundsType, COMBO_CHANGED, &OBSBasicTransform::OnBoundsType);
    HookWidget(ui->boundsAlign, COMBO_CHANGED, &OBSBasicTransform::OnControlChanged);
    HookWidget(ui->boundsWidth, DSCROLL_CHANGED, &OBSBasicTransform::OnControlChanged);
    HookWidget(ui->boundsHeight, DSCROLL_CHANGED, &OBSBasicTransform::OnControlChanged);
    HookWidget(ui->cropLeft, ISCROLL_CHANGED, &OBSBasicTransform::OnCropChanged);
    HookWidget(ui->cropRight, ISCROLL_CHANGED, &OBSBasicTransform::OnCropChanged);
    HookWidget(ui->cropTop, ISCROLL_CHANGED, &OBSBasicTransform::OnCropChanged);
    HookWidget(ui->cropBottom, ISCROLL_CHANGED, &OBSBasicTransform::OnCropChanged);
}

static const uint32_t listToAlign[] = {OBS_ALIGN_TOP | OBS_ALIGN_LEFT,
                                       OBS_ALIGN_TOP,
                                       OBS_ALIGN_TOP | OBS_ALIGN_RIGHT,
                                       OBS_ALIGN_LEFT,
                                       OBS_ALIGN_CENTER,
                                       OBS_ALIGN_RIGHT,
                                       OBS_ALIGN_BOTTOM | OBS_ALIGN_LEFT,
                                       OBS_ALIGN_BOTTOM,
                                       OBS_ALIGN_BOTTOM | OBS_ALIGN_RIGHT};

static int AlignToList(uint32_t align)
{
    int index = 0;
    for (uint32_t curAlign : listToAlign) {
        if (curAlign == align)
            return index;

        index++;
    }

    return 0;
}

void OBSBasicTransform::OnControlChanged()
{
    if (ignoreItemChange > 0)
        return;

    obs_source_t *source = obs_sceneitem_get_source(item);
    double width = double(obs_source_get_width(source));
    double height = double(obs_source_get_height(source));

    obs_transform_info oti;
    oti.pos.x = float(ui->positionX->value());
    oti.pos.y = float(ui->positionY->value());
    oti.rot = float(ui->rotation->value());
    oti.scale.x = float(ui->sizeX->value() / width);
    oti.scale.y = float(ui->sizeY->value() / height);
    oti.alignment = listToAlign[ui->align->currentIndex()];

    oti.bounds_type = (obs_bounds_type)ui->boundsType->currentIndex();
    oti.bounds_alignment = listToAlign[ui->boundsAlign->currentIndex()];
    oti.bounds.x = float(ui->boundsWidth->value());
    oti.bounds.y = float(ui->boundsHeight->value());

    ignoreTransformSignal++;
    obs_sceneitem_set_info2(item, &oti);
    ignoreTransformSignal--;
}

void OBSBasicTransform::OnCropChanged()
{
    if (ignoreItemChange > 0)
        return;

    obs_sceneitem_crop crop;
    crop.left = uint32_t(ui->cropLeft->value());
    crop.right = uint32_t(ui->cropRight->value());
    crop.top = uint32_t(ui->cropTop->value());
    crop.bottom = uint32_t(ui->cropBottom->value());

    ignoreTransformSignal++;
    obs_sceneitem_set_crop(item, &crop);
    ignoreTransformSignal--;
}

void OBSBasicTransform::OnBoundsType(int index)
{
    if (index == -1)
        return;

    obs_bounds_type type = (obs_bounds_type)index;
    bool enable = (type != OBS_BOUNDS_NONE);

    ui->boundsAlign->setEnabled(enable);
    ui->boundsWidth->setEnabled(enable);
    ui->boundsHeight->setEnabled(enable);

    if (ignoreItemChange == 0) {
        obs_bounds_type lastType = obs_sceneitem_get_bounds_type(item);
        if (lastType == OBS_BOUNDS_NONE) {
            OBSSource source = obs_sceneitem_get_source(item);
            int width = (int)obs_source_get_width(source);
            int height = (int)obs_source_get_height(source);

            ui->boundsWidth->setValue(width);
            ui->boundsHeight->setValue(height);
        }
    }

    OnControlChanged();
}

void OBSBasicTransform::LoadValues()
{
    if (!item)
        return;

    obs_transform_info osi;
    obs_sceneitem_crop crop;
    obs_sceneitem_get_info2(item, &osi);
    obs_sceneitem_get_crop(item, &crop);

    obs_source_t *source = obs_sceneitem_get_source(item);
    float width = float(obs_source_get_width(source));
    float height = float(obs_source_get_height(source));

    int alignIndex = AlignToList(osi.alignment);
    int boundsAlignIndex = AlignToList(osi.bounds_alignment);

    ignoreItemChange++;
    ui->positionX->setValue(osi.pos.x);
    ui->positionY->setValue(osi.pos.y);
    ui->rotation->setValue(osi.rot);
    ui->sizeX->setValue(osi.scale.x * width);
    ui->sizeY->setValue(osi.scale.y * height);
    ui->align->setCurrentIndex(alignIndex);

    ui->boundsType->setCurrentIndex(int(osi.bounds_type));
    ui->boundsAlign->setCurrentIndex(boundsAlignIndex);
    ui->boundsWidth->setValue(osi.bounds.x);
    ui->boundsHeight->setValue(osi.bounds.y);

    ui->cropLeft->setValue(int(crop.left));
    ui->cropRight->setValue(int(crop.right));
    ui->cropTop->setValue(int(crop.top));
    ui->cropBottom->setValue(int(crop.bottom));
    ignoreItemChange--;
}

void OBSBasicTransform::SceneItemTransformSignal(void *param, calldata_t *data)
{
    OBSBasicTransform *window = reinterpret_cast<OBSBasicTransform *>(param);
    OBSSceneItem item = (obs_sceneitem_t *)calldata_ptr(data, "item");

    if (item == window->item && window->ignoreTransformSignal == 0)
        QMetaObject::invokeMethod(window, "LoadValues");
}
