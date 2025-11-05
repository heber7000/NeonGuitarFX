cmake_minimum_required(VERSION 3.22)

project(NeonGuitarFX VERSION 1.0.0)

# Adiciona o JUCE como subdiretório
# Detecta o caminho do JUCE automaticamente
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../JUCE")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../JUCE ${CMAKE_CURRENT_BINARY_DIR}/JUCE)
elseif(EXISTS "/home/ubuntu/JUCE")
    add_subdirectory(/home/ubuntu/JUCE ${CMAKE_CURRENT_BINARY_DIR}/JUCE)
else()
    message(FATAL_ERROR "JUCE not found! Please clone JUCE to the parent directory or /home/ubuntu/JUCE")
endif()

juce_add_plugin(NeonGuitarFX
    PRODUCT_NAME "NeonGuitarFX"
    COMPANY_NAME "Manus"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Manu
    PLUGIN_CODE NeGF
    FORMATS VST3 CLAP
    PRODUCT_NAME "NeonGuitarFX")

target_sources(NeonGuitarFX
    PRIVATE
        Source/PluginEditor.cpp
        Source/PluginProcessor.cpp
        Source/NAMProcessor.cpp)

target_compile_definitions(NeonGuitarFX
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(NeonGuitarFX
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# Configurações para NeuralAmpModelerCore (desabilitado por enquanto)
# set(NAM_CORE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Libs/NeuralAmpModelerCore)
# if(EXISTS ${NAM_CORE_PATH}/CMakeLists.txt)
#     add_subdirectory(${NAM_CORE_PATH} ${CMAKE_CURRENT_BINARY_DIR}/NAMCore)
#     target_link_libraries(NeonGuitarFX PRIVATE NeuralAmpModelerCore)
#     target_include_directories(NeonGuitarFX PRIVATE ${NAM_CORE_PATH})
# endif()

# Configurações para Signalsmith Stretch (desabilitado por enquanto)
# set(SIGNALSMITH_STRETCH_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Libs/SignalsmithStretch)
# target_include_directories(NeonGuitarFX PRIVATE ${SIGNALSMITH_STRETCH_PATH})
