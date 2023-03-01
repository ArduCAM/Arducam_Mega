#pragma once
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
extern void on_imu_samples_ready();

// extern int magic_wand_kind;
extern bool done_just_triggered;

extern int current_app;
#ifdef __cplusplus
}
#endif