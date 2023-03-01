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

#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_MAGIC_WAND_MAIN_FUNCTIONS_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_MAGIC_WAND_MAIN_FUNCTIONS_H_

#include <stdbool.h>
#include <stdint.h>
//// Expose a C friendly interface for main functions.
#ifdef __cplusplus
extern "C" {
#endif

// Initializes all data needed for the example. The name is important, and needs
// to be setup() for Arduino compatibility.
void magic_wand_setup();

// Runs one iteration of data gathering and inference. This should be called
// repeatedly from the application code. The name needs to be loop() for Arduino
// compatibility.
bool magic_wand_loop(int gyroscope_samples_read, float *gyroscope_data,
                     int gyroscope_data_index, int accelerometer_samples_read,
                     float *acceleration_data, int acceleration_data_index);

extern uint16_t displayBuf[];
extern const char labels[];
extern int8_t max_score;
extern int max_index;

extern int8_t pre_max_score;
extern int pre_max_index;
#ifdef __cplusplus
}
#endif

#endif // TENSORFLOW_LITE_MICRO_EXAMPLES_MAGIC_WAND_MAIN_FUNCTIONS_H_
