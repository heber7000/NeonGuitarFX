#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "NAMProcessor.h" // Incluir a classe NAMProcessor
// #include "signalsmith-stretch/signalsmith-stretch.h" // Incluir Signalsmith Stretch

//==============================================================================
/**
*/
class NeonGuitarFXAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    NeonGuitarFXAudioProcessor();
    ~NeonGuitarFXAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    //==============================================================================
    // Módulos de Efeitos
    // NAMProcessor (2 slots: Amp e Pedal)
    NAMProcessor namAmpProcessor;
    NAMProcessor namPedalProcessor;
    
    // IR Loader (simples, para 2 slots)
    juce::dsp::Convolution irLoaderLeft;
    juce::dsp::Convolution irLoaderRight;

    // Delay Stereo Avançado
    // Usaremos duas instâncias de juce::dsp::DelayLine para o delay stereo
    juce::dsp::DelayLine<float> delayLineLeft;
    juce::dsp::DelayLine<float> delayLineRight;
    
    // Pitch Shifter/Harmonist
    // signalsmith::stretch::SignalsmithStretch<float> pitchShifter;

    // Reverb
    juce::dsp::Reverb reverb;
    juce::dsp::Reverb::Parameters reverbParams;

    // Parâmetros para o delay
    float delayFeedback { 0.5f };
    float delayMix { 0.5f };
    bool isPingPong { false };
    bool isDottedEighth { false };
    float delayTimeMsLeft { 500.0f };
    float delayTimeMsRight { 500.0f };

    // Parâmetros para o pitch shifter
    float pitchMix { 0.5f };
    int pitchInterval { 0 }; // Intervalo fixo em semitons
    int pitchScaleRoot { 0 }; // Raiz da escala para harmonização inteligente
    int pitchScaleType { 0 }; // Tipo de escala (maior, menor, etc.)

    // Variáveis de estado para o NAM e IR
    juce::String namAmpPath;
    juce::String namPedalPath;
    juce::String irLeftPath;
    juce::String irRightPath;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeonGuitarFXAudioProcessor)
};
