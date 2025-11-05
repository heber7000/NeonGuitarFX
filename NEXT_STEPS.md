# Próximas Etapas de Desenvolvimento - NeonGuitarFX

Este documento detalha os passos necessários para completar o desenvolvimento do plugin NeonGuitarFX, incluindo exemplos de código e referências.

## Fase 3: Implementação do Processamento de Áudio Core

### 3.1 Integração Completa do Neural Amp Modeler

O NAMProcessor já possui a estrutura básica. Para completar a integração, é necessário incluir os headers do NAM e implementar o carregamento de modelos.

**Arquivo**: `Source/NAMProcessor.cpp`

```cpp
// Adicionar no topo do arquivo:
#include "NAM/dsp.h"
#include "NAM/get_dsp.h"

// Implementar o método loadModel:
bool NAMProcessor::loadModel(const juce::String& modelPath)
{
    try {
        // Limpar modelo anterior se existir
        if (namDSP != nullptr) {
            delete namDSP;
            namDSP = nullptr;
        }
        
        // Carregar novo modelo
        auto dsp = nam::get_dsp(modelPath.toStdString());
        namDSP = dsp.release();
        
        if (namDSP != nullptr) {
            // Preparar com a taxa de amostragem atual
            namDSP->prepare(currentSampleRate);
            modelLoaded = true;
            return true;
        }
    } catch (const std::exception& e) {
        juce::Logger::writeToLog("Erro ao carregar modelo NAM: " + juce::String(e.what()));
    }
    
    modelLoaded = false;
    return false;
}

// Implementar o método process:
void NAMProcessor::process(juce::AudioBuffer<float>& buffer)
{
    if (!modelLoaded || namDSP == nullptr)
        return;
    
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    
    // Processar cada canal
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        
        // Aplicar ganho de entrada
        juce::FloatVectorOperations::multiply(channelData, inputGain, numSamples);
        
        // Processar com NAM
        namDSP->process(channelData, channelData, numSamples);
        
        // Aplicar ganho de saída
        juce::FloatVectorOperations::multiply(channelData, outputGain, numSamples);
    }
}
```

**Atualizar CMakeLists.txt** para incluir o NAM:

```cmake
# Descomentar e ajustar:
set(NAM_CORE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Libs/NeuralAmpModelerCore)
add_subdirectory(${NAM_CORE_PATH} ${CMAKE_CURRENT_BINARY_DIR}/NAMCore)
target_link_libraries(NeonGuitarFX PRIVATE NeuralAmpModelerCore)
target_include_directories(NeonGuitarFX PRIVATE ${NAM_CORE_PATH})
```

### 3.2 Implementação do IR Loader

O JUCE já possui a classe `juce::dsp::Convolution` que é usada para IR loading. A implementação básica já está no `PluginProcessor.h`.

**Adicionar método para carregar IR** em `PluginProcessor.h`:

```cpp
public:
    bool loadIRLeft(const juce::File& irFile);
    bool loadIRRight(const juce::File& irFile);
```

**Implementar em** `PluginProcessor.cpp`:

```cpp
bool NeonGuitarFXAudioProcessor::loadIRLeft(const juce::File& irFile)
{
    if (!irFile.existsAsFile())
        return false;
    
    try {
        irLoaderLeft.loadImpulseResponse(irFile, 
                                         juce::dsp::Convolution::Stereo::yes,
                                         juce::dsp::Convolution::Trim::yes,
                                         0); // Tamanho do IR (0 = usar todo o arquivo)
        return true;
    } catch (const std::exception& e) {
        juce::Logger::writeToLog("Erro ao carregar IR Left: " + juce::String(e.what()));
        return false;
    }
}

bool NeonGuitarFXAudioProcessor::loadIRRight(const juce::File& irFile)
{
    if (!irFile.existsAsFile())
        return false;
    
    try {
        irLoaderRight.loadImpulseResponse(irFile, 
                                          juce::dsp::Convolution::Stereo::yes,
                                          juce::dsp::Convolution::Trim::yes,
                                          0);
        return true;
    } catch (const std::exception& e) {
        juce::Logger::writeToLog("Erro ao carregar IR Right: " + juce::String(e.what()));
        return false;
    }
}
```

### 3.3 Integração do Signalsmith Stretch (Pitch Shifter)

**Criar arquivo** `Source/PitchShifter.h`:

```cpp
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "signalsmith-stretch.h"

class PitchShifter
{
public:
    PitchShifter();
    ~PitchShifter();
    
    void prepare(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();
    
    // Configurações
    void setPitchShift(float semitones); // -24 a +24
    void setMix(float mix); // 0.0 a 1.0
    
    // Harmonização inteligente
    void setIntelligentHarmony(bool enabled);
    void setScale(int root, int scaleType); // root: 0-11, scaleType: enum
    
private:
    signalsmith::stretch::SignalsmithStretch<float> stretcher;
    float pitchShiftSemitones = 0.0f;
    float mix = 0.5f;
    bool intelligentHarmonyEnabled = false;
    
    // Buffer para processar o sinal com pitch shift
    juce::AudioBuffer<float> shiftedBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchShifter)
};
```

**Implementar em** `Source/PitchShifter.cpp`:

```cpp
#include "PitchShifter.h"

PitchShifter::PitchShifter()
{
}

PitchShifter::~PitchShifter()
{
}

void PitchShifter::prepare(double sampleRate, int samplesPerBlock, int numChannels)
{
    stretcher.presetDefault(numChannels, sampleRate);
    shiftedBuffer.setSize(numChannels, samplesPerBlock);
}

void PitchShifter::process(juce::AudioBuffer<float>& buffer)
{
    if (pitchShiftSemitones == 0.0f || mix == 0.0f)
        return;
    
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    
    // Calcular o ratio de pitch shift
    float pitchRatio = std::pow(2.0f, pitchShiftSemitones / 12.0f);
    
    // Configurar o stretcher
    stretcher.setTransposeFactor(pitchRatio);
    
    // Processar
    shiftedBuffer.makeCopyOf(buffer);
    
    // TODO: Implementar processamento com Signalsmith Stretch
    // stretcher.process(shiftedBuffer.getArrayOfWritePointers(), 
    //                   numSamples, 
    //                   buffer.getArrayOfWritePointers());
    
    // Mixar o sinal original com o sinal processado
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* dry = buffer.getWritePointer(channel);
        auto* wet = shiftedBuffer.getReadPointer(channel);
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            dry[sample] = dry[sample] * (1.0f - mix) + wet[sample] * mix;
        }
    }
}

void PitchShifter::setPitchShift(float semitones)
{
    pitchShiftSemitones = juce::jlimit(-24.0f, 24.0f, semitones);
}

void PitchShifter::setMix(float newMix)
{
    mix = juce::jlimit(0.0f, 1.0f, newMix);
}
```

## Fase 4: Implementação dos Efeitos de Delay e Reverb

### 4.1 Delay Stereo Avançado

**Criar arquivo** `Source/StereoDelay.h`:

```cpp
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class StereoDelay
{
public:
    enum class DelayType
    {
        Digital,
        Analog,
        Tape
    };
    
    StereoDelay();
    ~StereoDelay();
    
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();
    
    // Configurações
    void setDelayTimeLeft(float timeMs);
    void setDelayTimeRight(float timeMs);
    void setFeedback(float feedback);
    void setMix(float mix);
    void setPingPong(bool enabled);
    void setDottedEighth(bool enabled, double bpm);
    void setDelayType(DelayType type);
    void setMono(bool mono);
    
private:
    juce::dsp::DelayLine<float> delayLineLeft;
    juce::dsp::DelayLine<float> delayLineRight;
    
    float delayTimeLeftMs = 500.0f;
    float delayTimeRightMs = 500.0f;
    float feedback = 0.5f;
    float mix = 0.5f;
    bool pingPongEnabled = false;
    bool dottedEighthEnabled = false;
    DelayType delayType = DelayType::Digital;
    bool monoMode = false;
    
    double sampleRate = 44100.0;
    
    // Buffers para feedback
    juce::AudioBuffer<float> feedbackBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StereoDelay)
};
```

### 4.2 Reverb

O reverb já está implementado usando `juce::dsp::Reverb`. Apenas conectar os parâmetros no `processBlock`:

```cpp
// No método processBlock do PluginProcessor.cpp:

// Atualizar parâmetros do reverb
reverbParams.roomSize = apvts.getRawParameterValue("REVERB_SIZE")->load();
reverbParams.damping = apvts.getRawParameterValue("REVERB_DAMP")->load();
reverbParams.wetLevel = apvts.getRawParameterValue("REVERB_WET")->load();
reverbParams.dryLevel = apvts.getRawParameterValue("REVERB_DRY")->load();
reverbParams.width = apvts.getRawParameterValue("REVERB_WIDTH")->load();
reverbParams.freezeMode = apvts.getRawParameterValue("REVERB_FREEZE")->load();

reverb.setParameters(reverbParams);

// Processar reverb
juce::dsp::AudioBlock<float> block(buffer);
juce::dsp::ProcessContextReplacing<float> context(block);
reverb.process(context);
```

## Fase 5: Interface Gráfica Neon

### 5.1 Criar Componente de Knob Neon

**Criar arquivo** `Source/NeonKnob.h`:

```cpp
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class NeonKnob : public juce::Slider
{
public:
    NeonKnob();
    ~NeonKnob() override;
    
    void paint(juce::Graphics& g) override;
    
    void setNeonColor(juce::Colour color);
    
private:
    juce::Colour neonColor = juce::Colours::cyan;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeonKnob)
};
```

**Implementar em** `Source/NeonKnob.cpp`:

```cpp
#include "NeonKnob.h"

NeonKnob::NeonKnob()
{
    setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
}

NeonKnob::~NeonKnob()
{
}

void NeonKnob::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto centre = bounds.getCentre();
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 10.0f;
    
    // Desenhar trilha do knob com efeito neon
    juce::Path track;
    track.addCentredArc(centre.x, centre.y, radius, radius, 
                        0.0f, 
                        juce::MathConstants<float>::pi * 1.2f, 
                        juce::MathConstants<float>::pi * 2.8f, 
                        true);
    
    // Efeito de brilho neon
    g.setColour(neonColor.withAlpha(0.3f));
    g.strokePath(track, juce::PathStrokeType(6.0f));
    
    g.setColour(neonColor.withAlpha(0.6f));
    g.strokePath(track, juce::PathStrokeType(4.0f));
    
    g.setColour(neonColor);
    g.strokePath(track, juce::PathStrokeType(2.0f));
    
    // Desenhar indicador de valor
    auto angle = juce::jmap(getValue(), getMinimum(), getMaximum(),
                           juce::MathConstants<float>::pi * 1.2f,
                           juce::MathConstants<float>::pi * 2.8f);
    
    juce::Point<float> thumbPoint(centre.x + radius * std::cos(angle - juce::MathConstants<float>::pi / 2.0f),
                                  centre.y + radius * std::sin(angle - juce::MathConstants<float>::pi / 2.0f));
    
    g.setColour(neonColor);
    g.fillEllipse(thumbPoint.x - 5.0f, thumbPoint.y - 5.0f, 10.0f, 10.0f);
    
    // Desenhar linha do centro ao thumb
    g.drawLine(centre.x, centre.y, thumbPoint.x, thumbPoint.y, 2.0f);
}

void NeonKnob::setNeonColor(juce::Colour color)
{
    neonColor = color;
    repaint();
}
```

### 5.2 Atualizar o PluginEditor

**Adicionar knobs neon em** `PluginEditor.h`:

```cpp
#include "NeonKnob.h"

private:
    NeonKnob delayFeedbackKnob;
    NeonKnob delayMixKnob;
    NeonKnob reverbSizeKnob;
    // ... adicionar mais knobs
    
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;
```

**Implementar em** `PluginEditor.cpp`:

```cpp
NeonGuitarFXAudioProcessorEditor::NeonGuitarFXAudioProcessorEditor (NeonGuitarFXAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Configurar knobs
    delayFeedbackKnob.setNeonColor(juce::Colours::cyan);
    addAndMakeVisible(delayFeedbackKnob);
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "DELAY_FEEDBACK", delayFeedbackKnob));
    
    delayMixKnob.setNeonColor(juce::Colours::cyan);
    addAndMakeVisible(delayMixKnob);
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "DELAY_MIX", delayMixKnob));
    
    // ... configurar mais knobs
    
    setSize(800, 600);
    setResizable(true, true);
}

void NeonGuitarFXAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    // Layout responsivo
    auto delaySection = bounds.removeFromTop(bounds.getHeight() / 3);
    delayFeedbackKnob.setBounds(delaySection.removeFromLeft(100).reduced(10));
    delayMixKnob.setBounds(delaySection.removeFromLeft(100).reduced(10));
    
    // ... posicionar mais componentes
}
```

## Compilação e Testes

Após implementar cada fase:

```bash
cd /home/ubuntu/NeonGuitarFX/build
cmake ..
make -j$(nproc)
```

Testar o plugin em uma DAW:

```bash
# Copiar para o diretório de plugins do sistema
cp -r NeonGuitarFX_artefacts/VST3/NeonGuitarFX.vst3 ~/.vst3/
```

## Recursos Adicionais

- **Documentação do JUCE**: https://docs.juce.com/
- **Neural Amp Modeler**: https://github.com/sdatkinson/NeuralAmpModelerCore
- **Signalsmith Stretch**: https://github.com/Signalsmith-Audio/signalsmith-stretch
- **Tutorial de Plugins JUCE**: https://www.youtube.com/watch?v=Mo0Oco3Vimo

## Observações Importantes

1. **Performance**: Otimizar o processamento de áudio para evitar glitches
2. **Thread Safety**: Garantir que a GUI não acesse variáveis de áudio diretamente
3. **State Saving**: Implementar `getStateInformation` e `setStateInformation` para salvar presets
4. **Validação**: Validar todos os parâmetros antes de processar

---

**Boa sorte com o desenvolvimento!** 🎸🔊
