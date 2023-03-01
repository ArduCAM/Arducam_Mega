/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

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

#include "audio_provider.h"

#include "micro_features/micro_model_settings.h"

namespace
{
    int16_t g_audio_output_buffer[kMaxAudioSampleSize];
    constexpr int pdm_kAudioCaptureBufferSize = 4096;
} // namespace

TfLiteStatus GetAudioSamples(tflite::ErrorReporter *error_reporter,
                             int start_ms, int duration_ms,
                             int *audio_samples_size, int16_t *caputre_audio_buffer,int16_t **audio_samples)
{
    const int start_offset = start_ms * (kAudioSampleFrequency / 1000);
    const int duration_sample_count = duration_ms * (kAudioSampleFrequency / 1000);

    for (int i = 0; i < duration_sample_count; ++i)
    {
        const int capture_index = (start_offset + i) % pdm_kAudioCaptureBufferSize;
        g_audio_output_buffer[i] = caputre_audio_buffer[capture_index];
    }
    *audio_samples_size = kMaxAudioSampleSize;
    *audio_samples = g_audio_output_buffer;
    return kTfLiteOk;
}

// int32_t LatestAudioTimestamp()
// {
//     // g_latest_audio_timestamp += 100;
//     return g_latest_audio_timestamp;
// }
