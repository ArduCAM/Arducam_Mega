
cmake_minimum_required(VERSION 3.12)

project(micro_speech C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)


add_library(micro_speech "")

target_include_directories(micro_speech
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/.
)

set_target_properties(
  micro_speech
  PROPERTIES
  COMPILE_FLAGS -fno-rtti
  COMPILE_FLAGS -fno-exceptions
  COMPILE_FLAGS -fno-threadsafe-statics
  COMPILE_FLAGS -nostdlib
)

target_sources(micro_speech
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/fft.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/fft_util.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/filterbank.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/filterbank_util.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/frontend.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/frontend_util.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/log_lut.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/log_scale.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/log_scale_util.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/noise_reduction.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/noise_reduction_util.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/pcan_gain_control.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/pcan_gain_control_util.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/window.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/window_util.c
  ${CMAKE_CURRENT_LIST_DIR}/src/audio_provider.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/command_responder.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/feature_provider.cpp
  ${CMAKE_CURRENT_LIST_DIR}/micro_speech_main_functions.cpp
  ${CMAKE_CURRENT_LIST_DIR}/micro_features/micro_features_generator.cpp
  ${CMAKE_CURRENT_LIST_DIR}/micro_features/micro_model_settings.cpp
  ${CMAKE_CURRENT_LIST_DIR}/micro_features/model.cpp
  ${CMAKE_CURRENT_LIST_DIR}/micro_features/no_micro_features_data.cpp
  ${CMAKE_CURRENT_LIST_DIR}/micro_features/yes_micro_features_data.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/recognize_commands.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/tools/make/downloads/kissfft/kiss_fft.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/tools/make/downloads/kissfft/tools/kiss_fftr.c
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/bits.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/fft.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/fft_util.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/filterbank.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/filterbank_util.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/frontend.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/frontend_util.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/log_lut.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/log_scale.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/log_scale_util.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/noise_reduction.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/noise_reduction_util.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/pcan_gain_control.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/pcan_gain_control_util.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/window.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/experimental/microfrontend/lib/window_util.h
  ${CMAKE_CURRENT_LIST_DIR}/src/audio_provider.h
  ${CMAKE_CURRENT_LIST_DIR}/src/command_responder.h
  ${CMAKE_CURRENT_LIST_DIR}/src/feature_provider.h
  ${CMAKE_CURRENT_LIST_DIR}/micro_speech_main_functions.h
  ${CMAKE_CURRENT_LIST_DIR}/micro_features/micro_features_generator.h
  ${CMAKE_CURRENT_LIST_DIR}/micro_features/micro_model_settings.h
  ${CMAKE_CURRENT_LIST_DIR}/micro_features/model.h
  ${CMAKE_CURRENT_LIST_DIR}/micro_features/no_micro_features_data.h
  ${CMAKE_CURRENT_LIST_DIR}/micro_features/yes_micro_features_data.h
  ${CMAKE_CURRENT_LIST_DIR}/src/recognize_commands.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/tools/make/downloads/kissfft/COPYING
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/tools/make/downloads/kissfft/_kiss_fft_guts.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/tools/make/downloads/kissfft/kiss_fft.h
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/tools/make/downloads/kissfft/tools/kiss_fftr.h
)

target_link_libraries(
  micro_speech
  pico-tflmicro
  hardware_pwm
)