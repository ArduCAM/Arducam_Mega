#pragma once

#include "micro_speech_main_functions.h"
#include "pico/pdm_microphone.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CFG_TUD_AUDIO_FUNC_1_N_BYTES_PER_SAMPLE_TX 2                                                                     // Driver gets this info from the descriptors - we define it here to use it to setup the descriptors and to do calculations with it below
#define CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX 1                                                                             // Driver gets this info from the descriptors - we define it here to use it to setup the descriptors and to do calculations with it below - be aware: for different number of channels you need another descriptor!
#define CFG_TUD_AUDIO_EP_SZ_IN (16 + 1) * CFG_TUD_AUDIO_FUNC_1_N_BYTES_PER_SAMPLE_TX *CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX // 16 Samples (16 kHz) x 2 Bytes/Sample x 1 Channel

#ifndef SAMPLE_BUFFER_SIZE
#define SAMPLE_BUFFER_SIZE ((CFG_TUD_AUDIO_EP_SZ_IN / 2) - 1)
#endif

#ifndef SAMPLE_RATE
#define SAMPLE_RATE ((CFG_TUD_AUDIO_EP_SZ_IN / 2) - 1) * 1000 // 16000
#endif

#define FILENAME_LEN 17
#define MAX_BMP_FILES 25
#define IMG_WIDTH 96
#define IMG_HEIGHT 96
#define DIS_OFFSET 10

#define pdm_kAudioCaptureBufferSize 4096

    extern int16_t pdm_g_audio_capture_buffer[];
    extern volatile int32_t g_latest_audio_timestamp;
    void on_pdm_samples_ready();
    void pdm_init(pdm_samples_ready_handler_t handler);

#ifdef __cplusplus
}
#endif