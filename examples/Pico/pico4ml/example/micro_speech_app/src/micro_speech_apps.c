#include <stdio.h>

#include "micro_speech_apps.h"

/**
 * @brief micro_speech
 */
const int16_t audio_buffer_size = 256;
volatile int32_t g_latest_audio_timestamp = 0;
int16_t pdm_g_audio_capture_buffer[pdm_kAudioCaptureBufferSize];
// variables
volatile int samples_read = 0;


// configuration
const struct pdm_microphone_config config = {
    // GPIO pin for the PDM DAT signal
    .gpio_data = 26,
    // GPIO pin for the PDM CLK signal
    .gpio_clk = 27,
    // PIO instance to use
    .pio = pio0,
    // PIO State Machine instance to use
    .pio_sm = 0,
    // sample rate in Hz
    .sample_rate = SAMPLE_RATE,
    // number of samples to buffer
    .sample_buffer_size = audio_buffer_size,
};

void on_pdm_samples_ready()
{
    // callback from library when all the samples in the library
    // internal sample buffer are ready for reading
    const int32_t time_in_ms = g_latest_audio_timestamp + (audio_buffer_size / (SAMPLE_RATE / 1000));
    const int32_t start_sample_offset = g_latest_audio_timestamp * (SAMPLE_RATE / 1000);
    const int capture_index = start_sample_offset % pdm_kAudioCaptureBufferSize;
    samples_read = pdm_microphone_read(pdm_g_audio_capture_buffer + capture_index, audio_buffer_size);
    g_latest_audio_timestamp = time_in_ms;
}

void pdm_init(pdm_samples_ready_handler_t handler)
{
    // initialize the PDM microphone
    if (pdm_microphone_init(&config) < 0)
    {
        printf("PDM microphone initialization failed!\n");
        while (1)
        {
            tight_loop_contents();
        }
    }
    pdm_microphone_set_samples_ready_handler(handler);
    // start capturing data from the PDM microphone
    if (pdm_microphone_start() < 0)
    {
        printf("PDM microphone start failed!\n");
        while (1)
        {
            tight_loop_contents();
        }
    }
}