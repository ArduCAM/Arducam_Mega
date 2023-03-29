#include "magic_wand_app.h"
#include "ICM42622.h"
#include "magic_wand_main_functions.h"
#include <stdio.h>
// A buffer holding the last 600 sets of 3-channel values from the
// accelerometer.
#define ARRAY_LENGTH 1800
const int acceleration_data_length = 600 * 3;
float acceleration_data[ARRAY_LENGTH] = {};
float acceleration_data_tmp[ARRAY_LENGTH] = {};

// The next free entry in the data array.
int acceleration_data_index = 0;
float acceleration_sample_rate = 0.0f;

// A buffer holding the last 600 sets of 3-channel values from the gyroscope.
const int gyroscope_data_length = 600 * 3;
float gyroscope_data[ARRAY_LENGTH] = {};
float gyroscope_data_tmp[ARRAY_LENGTH] = {};
// The next free entry in the data array.
int gyroscope_data_index = 0;
float gyroscope_sample_rate = 0.0f;

bool done_just_triggered = false;

void on_imu_samples_ready() {
    int new_accelerometer_samples = 0;
    int new_gyroscope_samples = 0;
    while (icm42622_data_ready()) {
        const int gyroscope_index =
            (gyroscope_data_index % gyroscope_data_length);
        gyroscope_data_index += 3;
        float *current_gyroscope_data = &gyroscope_data[gyroscope_index];
        float *current_gyroscope_data_tmp =
            &gyroscope_data_tmp[gyroscope_index];

        if (!icm42622_read_gyro(&current_gyroscope_data_tmp[0],
                                &current_gyroscope_data_tmp[1],
                                &current_gyroscope_data_tmp[2])) {
            printf("Failed to read gyroscope data");
            break;
        }
        current_gyroscope_data[0] = -current_gyroscope_data_tmp[1];
        current_gyroscope_data[1] = current_gyroscope_data_tmp[0];
        current_gyroscope_data[2] = current_gyroscope_data_tmp[2];
        new_gyroscope_samples += 1;

        const int acceleration_index =
            (acceleration_data_index % acceleration_data_length);
        acceleration_data_index += 3;
        float *current_acceleration_data =
            &acceleration_data[acceleration_index];
        float *current_acceleration_data_tmp =
            &acceleration_data_tmp[acceleration_index];
        // Read each sample, removing it from the device's FIFO buffer
        if (!icm42622_read_accel(&current_acceleration_data_tmp[0],
                                 &current_acceleration_data_tmp[1],
                                 &current_acceleration_data_tmp[2])) {
            printf("Failed to read acceleration data");
            break;
        }
        current_acceleration_data[0] = -current_acceleration_data_tmp[1];
        current_acceleration_data[1] = current_acceleration_data_tmp[0];
        current_acceleration_data[2] = current_acceleration_data_tmp[2];
        new_accelerometer_samples += 1;
    }

    // if (pending_initial_data && imu_offset >= 200) {
    //     pending_initial_data = false;
    // }
    // if (new_data == true && pending_initial_data == false) {
    // magic_wand_kind =
    // printf("magic_wand_loop");
    done_just_triggered = magic_wand_loop(
        new_gyroscope_samples, gyroscope_data, gyroscope_data_index,
        new_accelerometer_samples, acceleration_data, acceleration_data_index);
    // }
}