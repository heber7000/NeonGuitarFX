#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class NeonGuitarFXAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    NeonGuitarFXAudioProcessorEditor (NeonGuitarFXAudioProcessor&);
    ~NeonGuitarFXAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Referência ao processador
    NeonGuitarFXAudioProcessor& audioProcessor;

    // Sliders e seus attachments
    juce::Slider namAmpGainSlider;
    juce::Slider namPedalGainSlider;
    juce::Slider irMixSlider;
    
    juce::Slider delayFeedbackSlider;
    juce::Slider delayMixSlider;
    juce::Slider delayTimeLeftSlider;
    juce::Slider delayTimeRightSlider;
    
    juce::Slider reverbSizeSlider;
    juce::Slider reverbDampSlider;
    juce::Slider reverbWetSlider;
    juce::Slider reverbDrySlider;
    juce::Slider reverbWidthSlider;
    
    juce::Slider pitchMixSlider;
    juce::Slider pitchIntervalSlider;
    
    juce::Slider panoramaSlider;
    
    // Labels
    juce::Label titleLabel;
    juce::Label namAmpLabel, namPedalLabel, irLabel;
    juce::Label delayLabel, reverbLabel, pitchLabel;
    
    // Botões
    juce::ToggleButton delayPingPongButton;
    juce::ToggleButton delayDottedButton;
    juce::ToggleButton delayMonoButton;
    juce::ToggleButton reverbStereoButton;
    
    // ComboBoxes
    juce::ComboBox delayTypeCombo;
    juce::ComboBox pitchModeCombo;
    juce::ComboBox pitchScaleRootCombo;
    juce::ComboBox pitchScaleTypeCombo;
    
    // Attachments para conectar com APVTS
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> namAmpGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> namPedalGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> irMixAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayFeedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeLeftAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeRightAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbSizeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbDampAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbWetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbDryAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbWidthAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchIntervalAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> panoramaAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> delayPingPongAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> delayDottedAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> delayMonoAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> reverbStereoAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> delayTypeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> pitchModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> pitchScaleRootAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> pitchScaleTypeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeonGuitarFXAudioProcessorEditor)
};
