/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "image_provider.h"

#include "model_settings.h"

TfLiteStatus GetImageFromRGB(tflite::ErrorReporter *error_reporter, int max_image_size, uint16_t *src_data, int8_t *image_data)
{
    for (int i = 0; i < max_image_size; ++i)
    {
        uint8_t gm_red = (src_data[i] & 0xF800) >> 8;
        uint8_t gm_green = (src_data[i] & 0x07E0) >> 3;
        uint8_t gm_blue = (src_data[i] & 0x001F) << 3;

        image_data[i] = (uint8_t)((gm_red * 77 + gm_green * 150 + gm_blue * 29 + 128) / 256);
    }
    return kTfLiteOk;
}
TfLiteStatus GetImageFromYUV(tflite::ErrorReporter *error_reporter, int max_image_size, uint16_t *src_data, int8_t *image_data)
{
    for (int i = 0; i < max_image_size; ++i)
    {
        image_data[i] = (uint8_t)(src_data[i] & 0x00ff); //(uint8_t)(src_ptr[i]);
    }
    return kTfLiteOk;
}

TfLiteStatus GetImage(tflite::ErrorReporter *error_reporter, int max_image_size, uint16_t *src_data, int8_t *image_data)
{
    uint8_t *src_data_8 = (uint8_t*)src_data;
    for (int i = 0; i < max_image_size; ++i)
    {
        image_data[i] = src_data_8[i];
    }
    return kTfLiteOk;
}
