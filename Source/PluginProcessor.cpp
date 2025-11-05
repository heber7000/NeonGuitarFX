#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NeonGuitarFXAudioProcessor::NeonGuitarFXAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsJuceAudio
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    apvts (*this, nullptr, "Parameters", createParameterLayout())
{
}

NeonGuitarFXAudioProcessor::~NeonGuitarFXAudioProcessor()
{
}

//==============================================================================
const juce::String NeonGuitarFXAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NeonGuitarFXAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NeonGuitarFXAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NeonGuitarFXAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NeonGuitarFXAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NeonGuitarFXAudioProcessor::getNumPrograms()
{
    return 1;   // Only one program
}

int NeonGuitarFXAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NeonGuitarFXAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NeonGuitarFXAudioProcessor::getProgramName (int index)
{
    return {};
}

void NeonGuitarFXAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NeonGuitarFXAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Inicialização dos módulos DSP
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32)samplesPerBlock;
    spec.numChannels = (juce::uint32)getTotalNumOutputChannels();

    // Inicializar Delay Lines
    delayLineLeft.prepare(spec);
    delayLineRight.prepare(spec);
    delayLineLeft.setMaximumDelayInSamples(sampleRate * 2.0); // Max 2 seconds
    delayLineRight.setMaximumDelayInSamples(sampleRate * 2.0);

    // Inicializar Reverb
    reverb.prepare(spec);
    reverb.setParameters(reverbParams);

    // Inicializar IR Loaders (apenas prepare, o carregamento do IR será feito via APVTS)
    irLoaderLeft.prepare(spec);
    irLoaderRight.prepare(spec);

    // Inicializar Pitch Shifter (Signalsmith Stretch)
    // O Signalsmith Stretch precisa ser configurado com o número de canais e sample rate
    // pitchShifter.presetDefault(spec.numChannels, spec.sampleRate);
}

void NeonGuitarFXAudioProcessor::releaseResources()
{
    // Limpar recursos
    delayLineLeft.reset();
    delayLineRight.reset();
    reverb.reset();
    irLoaderLeft.reset();
    irLoaderRight.reset();
    // pitchShifter.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NeonGuitarFXAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // Apenas mono/stereo para entrada e saída
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // Apenas mono/stereo para entrada
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();
  #endif
}
#endif

void NeonGuitarFXAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // O processamento real será implementado na Fase 3
    // Por enquanto, apenas passa o áudio
    // ...
}

//==============================================================================
bool NeonGuitarFXAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NeonGuitarFXAudioProcessor::createEditor()
{
    return new NeonGuitarFXAudioProcessorEditor (*this);
}

//==============================================================================
void NeonGuitarFXAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do this by just using the parameter block's toXml() method, but
    // it's generally better to store the whole juce::ValueTree.
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void NeonGuitarFXAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout NeonGuitarFXAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // NAM Parameters (Amp) - File paths will be handled via GUI file chooser
    // layout.add (std::make_unique<juce::AudioParameterString> ("NAM_AMP_PATH", "NAM Amp File", juce::String(), juce::String()));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("NAM_AMP_GAIN", "NAM Amp Gain", juce::NormalisableRange<float> (0.0f, 2.0f, 0.01f), 1.0f));

    // NAM Parameters (Pedal/Distortion) - File paths will be handled via GUI file chooser
    // layout.add (std::make_unique<juce::AudioParameterString> ("NAM_PEDAL_PATH", "NAM Pedal File", juce::String(), juce::String()));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("NAM_PEDAL_GAIN", "NAM Pedal Gain", juce::NormalisableRange<float> (0.0f, 2.0f, 0.01f), 1.0f));

    // IR Parameters - File paths will be handled via GUI file chooser
    // layout.add (std::make_unique<juce::AudioParameterString> ("IR_LEFT_PATH", "IR Left File", juce::String(), juce::String()));
    // layout.add (std::make_unique<juce::AudioParameterString> ("IR_RIGHT_PATH", "IR Right File", juce::String(), juce::String()));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("IR_MIX", "IR Mix", juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 1.0f));

    // Delay Parameters
    layout.add (std::make_unique<juce::AudioParameterFloat> ("DELAY_FEEDBACK", "Delay Feedback", juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.5f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("DELAY_MIX", "Delay Mix", juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.5f));
    layout.add (std::make_unique<juce::AudioParameterBool> ("DELAY_PINGPONG", "Delay Ping Pong", false));
    layout.add (std::make_unique<juce::AudioParameterBool> ("DELAY_DOTTED", "Delay Dotted 8th", false));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("DELAY_TIME_L", "Delay Time Left (ms)", juce::NormalisableRange<float> (1.0f, 2000.0f, 1.0f), 500.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("DELAY_TIME_R", "Delay Time Right (ms)", juce::NormalisableRange<float> (1.0f, 2000.0f, 1.0f), 500.0f));
    layout.add (std::make_unique<juce::AudioParameterChoice> ("DELAY_TYPE", "Delay Type", juce::StringArray {"Digital", "Analog", "Tape"}, 0));
    layout.add (std::make_unique<juce::AudioParameterBool> ("DELAY_MONO", "Delay Mono", false));

    // Reverb Parameters
    layout.add (std::make_unique<juce::AudioParameterFloat> ("REVERB_SIZE", "Reverb Room Size", juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.5f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("REVERB_DAMP", "Reverb Damping", juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.5f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("REVERB_WET", "Reverb Wet Level", juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.33f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("REVERB_DRY", "Reverb Dry Level", juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.4f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("REVERB_WIDTH", "Reverb Width", juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 1.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("REVERB_FREEZE", "Reverb Freeze Mode", juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterBool> ("REVERB_STEREO", "Reverb Stereo", true));

    // Pitch Shifter/Harmonist Parameters
    layout.add (std::make_unique<juce::AudioParameterFloat> ("PITCH_MIX", "Pitch Mix", juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.5f));
    layout.add (std::make_unique<juce::AudioParameterChoice> ("PITCH_MODE", "Pitch Mode", juce::StringArray {"Fixed Interval", "Intelligent Harmony"}, 0));
    layout.add (std::make_unique<juce::AudioParameterInt> (juce::ParameterID{"PITCH_INTERVAL", 1}, "Pitch Interval (st)", -24, 24, 0));
    layout.add (std::make_unique<juce::AudioParameterChoice> ("PITCH_SCALE_ROOT", "Harmony Root", juce::StringArray {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"}, 0));
    layout.add (std::make_unique<juce::AudioParameterChoice> ("PITCH_SCALE_TYPE", "Harmony Scale", juce::StringArray {"Major", "Minor", "Phrygian", "Ionian", "Dorian", "Locrian", "Arabic"}, 0));

    // Panorama (para o sinal processado pelo NAM/IR)
    layout.add (std::make_unique<juce::AudioParameterFloat> ("PANORAMA", "Panorama", juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f), 0.0f));

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NeonGuitarFXAudioProcessor();
}
