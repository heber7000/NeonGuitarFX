#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <memory>
#include <string>

// Forward declarations para evitar incluir o NAM diretamente no header
namespace nam
{
    class DSP;
}

/**
 * Wrapper para o Neural Amp Modeler (NAM) DSP
 * Gerencia o carregamento e processamento de arquivos .nam
 */
class NAMProcessor
{
public:
    NAMProcessor();
    ~NAMProcessor();

    /**
     * Carrega um modelo NAM de um arquivo .nam
     * @param modelPath Caminho completo para o arquivo .nam
     * @return true se o carregamento foi bem-sucedido, false caso contrário
     */
    bool loadModel(const juce::String& modelPath);

    /**
     * Prepara o processador para processar áudio
     * @param sampleRate Taxa de amostragem
     * @param samplesPerBlock Número de amostras por bloco
     */
    void prepare(double sampleRate, int samplesPerBlock);

    /**
     * Processa um bloco de áudio
     * @param buffer Buffer de áudio a ser processado (in-place)
     */
    void process(juce::AudioBuffer<float>& buffer);

    /**
     * Reseta o estado interno do processador
     */
    void reset();

    /**
     * Verifica se um modelo está carregado
     * @return true se um modelo está carregado, false caso contrário
     */
    bool isModelLoaded() const { return modelLoaded; }

    /**
     * Define o ganho de entrada
     * @param gain Ganho de entrada (0.0 a 2.0)
     */
    void setInputGain(float gain) { inputGain = gain; }

    /**
     * Define o ganho de saída
     * @param gain Ganho de saída (0.0 a 2.0)
     */
    void setOutputGain(float gain) { outputGain = gain; }

private:
    // Usar ponteiro bruto para evitar problemas com tipo incompleto
    // O unique_ptr requer que o tipo seja completo no destrutor
    nam::DSP* namDSP = nullptr;
    bool modelLoaded = false;
    float inputGain = 1.0f;
    float outputGain = 1.0f;
    double currentSampleRate = 44100.0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NAMProcessor)
};
