#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NeonGuitarFXAudioProcessorEditor::NeonGuitarFXAudioProcessorEditor (NeonGuitarFXAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Configuração do título
    titleLabel.setText ("NeonGuitarFX", juce::dontSendNotification);
    titleLabel.setFont (juce::Font (28.0f, juce::Font::bold));
    titleLabel.setJustificationType (juce::Justification::centred);
    titleLabel.setColour (juce::Label::textColourId, juce::Colour (0xff00FFFF));
    addAndMakeVisible (titleLabel);
    
    // Configurar sliders NAM
    namAmpGainSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    namAmpGainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    namAmpGainSlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xff00FFFF));
    namAmpGainSlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff00FFFF));
    addAndMakeVisible (namAmpGainSlider);
    namAmpGainAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "NAM_AMP_GAIN", namAmpGainSlider));
    
    namPedalGainSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    namPedalGainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    namPedalGainSlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xff00FFFF));
    namPedalGainSlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff00FFFF));
    addAndMakeVisible (namPedalGainSlider);
    namPedalGainAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "NAM_PEDAL_GAIN", namPedalGainSlider));
    
    irMixSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    irMixSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    irMixSlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xff00FFFF));
    irMixSlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff00FFFF));
    addAndMakeVisible (irMixSlider);
    irMixAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "IR_MIX", irMixSlider));
    
    // Configurar sliders de Delay
    delayFeedbackSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    delayFeedbackSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    delayFeedbackSlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xff9D00FF));
    delayFeedbackSlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff9D00FF));
    addAndMakeVisible (delayFeedbackSlider);
    delayFeedbackAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "DELAY_FEEDBACK", delayFeedbackSlider));
    
    delayMixSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    delayMixSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    delayMixSlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xff9D00FF));
    delayMixSlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff9D00FF));
    addAndMakeVisible (delayMixSlider);
    delayMixAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "DELAY_MIX", delayMixSlider));
    
    delayTimeLeftSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    delayTimeLeftSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 20);
    delayTimeLeftSlider.setColour (juce::Slider::trackColourId, juce::Colour (0xff9D00FF));
    delayTimeLeftSlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff9D00FF));
    addAndMakeVisible (delayTimeLeftSlider);
    delayTimeLeftAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "DELAY_TIME_L", delayTimeLeftSlider));
    
    delayTimeRightSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    delayTimeRightSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 20);
    delayTimeRightSlider.setColour (juce::Slider::trackColourId, juce::Colour (0xff9D00FF));
    delayTimeRightSlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff9D00FF));
    addAndMakeVisible (delayTimeRightSlider);
    delayTimeRightAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "DELAY_TIME_R", delayTimeRightSlider));
    
    // Configurar sliders de Reverb
    reverbSizeSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    reverbSizeSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    reverbSizeSlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xff0099FF));
    reverbSizeSlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff0099FF));
    addAndMakeVisible (reverbSizeSlider);
    reverbSizeAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "REVERB_SIZE", reverbSizeSlider));
    
    reverbDampSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    reverbDampSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    reverbDampSlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xff0099FF));
    reverbDampSlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff0099FF));
    addAndMakeVisible (reverbDampSlider);
    reverbDampAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "REVERB_DAMP", reverbDampSlider));
    
    reverbWetSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    reverbWetSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    reverbWetSlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xff0099FF));
    reverbWetSlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff0099FF));
    addAndMakeVisible (reverbWetSlider);
    reverbWetAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "REVERB_WET", reverbWetSlider));
    
    reverbDrySlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    reverbDrySlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    reverbDrySlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xff0099FF));
    reverbDrySlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff0099FF));
    addAndMakeVisible (reverbDrySlider);
    reverbDryAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "REVERB_DRY", reverbDrySlider));
    
    reverbWidthSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    reverbWidthSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    reverbWidthSlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xff0099FF));
    reverbWidthSlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff0099FF));
    addAndMakeVisible (reverbWidthSlider);
    reverbWidthAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "REVERB_WIDTH", reverbWidthSlider));
    
    // Configurar sliders de Pitch
    pitchMixSlider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    pitchMixSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    pitchMixSlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xffFF00FF));
    pitchMixSlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xffFF00FF));
    addAndMakeVisible (pitchMixSlider);
    pitchMixAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "PITCH_MIX", pitchMixSlider));
    
    pitchIntervalSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    pitchIntervalSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 20);
    pitchIntervalSlider.setColour (juce::Slider::trackColourId, juce::Colour (0xffFF00FF));
    pitchIntervalSlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xffFF00FF));
    addAndMakeVisible (pitchIntervalSlider);
    pitchIntervalAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "PITCH_INTERVAL", pitchIntervalSlider));
    
    // Panorama
    panoramaSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    panoramaSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 20);
    panoramaSlider.setColour (juce::Slider::trackColourId, juce::Colour (0xff00FFFF));
    panoramaSlider.setColour (juce::Slider::thumbColourId, juce::Colour (0xff00FFFF));
    addAndMakeVisible (panoramaSlider);
    panoramaAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.apvts, "PANORAMA", panoramaSlider));
    
    // Configurar botões
    delayPingPongButton.setButtonText ("Ping Pong");
    delayPingPongButton.setColour (juce::ToggleButton::textColourId, juce::Colour (0xff9D00FF));
    delayPingPongButton.setColour (juce::ToggleButton::tickColourId, juce::Colour (0xff9D00FF));
    addAndMakeVisible (delayPingPongButton);
    delayPingPongAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.apvts, "DELAY_PINGPONG", delayPingPongButton));
    
    delayDottedButton.setButtonText ("Dotted 8th");
    delayDottedButton.setColour (juce::ToggleButton::textColourId, juce::Colour (0xff9D00FF));
    delayDottedButton.setColour (juce::ToggleButton::tickColourId, juce::Colour (0xff9D00FF));
    addAndMakeVisible (delayDottedButton);
    delayDottedAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.apvts, "DELAY_DOTTED", delayDottedButton));
    
    delayMonoButton.setButtonText ("Mono");
    delayMonoButton.setColour (juce::ToggleButton::textColourId, juce::Colour (0xff9D00FF));
    delayMonoButton.setColour (juce::ToggleButton::tickColourId, juce::Colour (0xff9D00FF));
    addAndMakeVisible (delayMonoButton);
    delayMonoAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.apvts, "DELAY_MONO", delayMonoButton));
    
    reverbStereoButton.setButtonText ("Stereo");
    reverbStereoButton.setColour (juce::ToggleButton::textColourId, juce::Colour (0xff0099FF));
    reverbStereoButton.setColour (juce::ToggleButton::tickColourId, juce::Colour (0xff0099FF));
    addAndMakeVisible (reverbStereoButton);
    reverbStereoAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.apvts, "REVERB_STEREO", reverbStereoButton));
    
    // Configurar ComboBoxes
    delayTypeCombo.addItem ("Digital", 1);
    delayTypeCombo.addItem ("Analog", 2);
    delayTypeCombo.addItem ("Tape", 3);
    delayTypeCombo.setColour (juce::ComboBox::textColourId, juce::Colour (0xff9D00FF));
    addAndMakeVisible (delayTypeCombo);
    delayTypeAttachment.reset (new juce::AudioProcessorValueTreeState::ComboBoxAttachment (audioProcessor.apvts, "DELAY_TYPE", delayTypeCombo));
    
    pitchModeCombo.addItem ("Fixed Interval", 1);
    pitchModeCombo.addItem ("Intelligent Harmony", 2);
    pitchModeCombo.setColour (juce::ComboBox::textColourId, juce::Colour (0xffFF00FF));
    addAndMakeVisible (pitchModeCombo);
    pitchModeAttachment.reset (new juce::AudioProcessorValueTreeState::ComboBoxAttachment (audioProcessor.apvts, "PITCH_MODE", pitchModeCombo));
    
    pitchScaleRootCombo.addItem ("C", 1);
    pitchScaleRootCombo.addItem ("C#", 2);
    pitchScaleRootCombo.addItem ("D", 3);
    pitchScaleRootCombo.addItem ("D#", 4);
    pitchScaleRootCombo.addItem ("E", 5);
    pitchScaleRootCombo.addItem ("F", 6);
    pitchScaleRootCombo.addItem ("F#", 7);
    pitchScaleRootCombo.addItem ("G", 8);
    pitchScaleRootCombo.addItem ("G#", 9);
    pitchScaleRootCombo.addItem ("A", 10);
    pitchScaleRootCombo.addItem ("A#", 11);
    pitchScaleRootCombo.addItem ("B", 12);
    pitchScaleRootCombo.setColour (juce::ComboBox::textColourId, juce::Colour (0xffFF00FF));
    addAndMakeVisible (pitchScaleRootCombo);
    pitchScaleRootAttachment.reset (new juce::AudioProcessorValueTreeState::ComboBoxAttachment (audioProcessor.apvts, "PITCH_SCALE_ROOT", pitchScaleRootCombo));
    
    pitchScaleTypeCombo.addItem ("Major", 1);
    pitchScaleTypeCombo.addItem ("Minor", 2);
    pitchScaleTypeCombo.addItem ("Phrygian", 3);
    pitchScaleTypeCombo.addItem ("Ionian", 4);
    pitchScaleTypeCombo.addItem ("Dorian", 5);
    pitchScaleTypeCombo.addItem ("Locrian", 6);
    pitchScaleTypeCombo.addItem ("Arabic", 7);
    pitchScaleTypeCombo.setColour (juce::ComboBox::textColourId, juce::Colour (0xffFF00FF));
    addAndMakeVisible (pitchScaleTypeCombo);
    pitchScaleTypeAttachment.reset (new juce::AudioProcessorValueTreeState::ComboBoxAttachment (audioProcessor.apvts, "PITCH_SCALE_TYPE", pitchScaleTypeCombo));
    
    // Labels
    namAmpLabel.setText ("Amp Gain", juce::dontSendNotification);
    namAmpLabel.setJustificationType (juce::Justification::centred);
    namAmpLabel.setColour (juce::Label::textColourId, juce::Colour (0xff00FFFF));
    addAndMakeVisible (namAmpLabel);
    
    namPedalLabel.setText ("Pedal Gain", juce::dontSendNotification);
    namPedalLabel.setJustificationType (juce::Justification::centred);
    namPedalLabel.setColour (juce::Label::textColourId, juce::Colour (0xff00FFFF));
    addAndMakeVisible (namPedalLabel);
    
    irLabel.setText ("IR Mix", juce::dontSendNotification);
    irLabel.setJustificationType (juce::Justification::centred);
    irLabel.setColour (juce::Label::textColourId, juce::Colour (0xff00FFFF));
    addAndMakeVisible (irLabel);
    
    delayLabel.setText ("DELAY", juce::dontSendNotification);
    delayLabel.setFont (juce::Font (16.0f, juce::Font::bold));
    delayLabel.setJustificationType (juce::Justification::centred);
    delayLabel.setColour (juce::Label::textColourId, juce::Colour (0xff9D00FF));
    addAndMakeVisible (delayLabel);
    
    reverbLabel.setText ("REVERB", juce::dontSendNotification);
    reverbLabel.setFont (juce::Font (16.0f, juce::Font::bold));
    reverbLabel.setJustificationType (juce::Justification::centred);
    reverbLabel.setColour (juce::Label::textColourId, juce::Colour (0xff0099FF));
    addAndMakeVisible (reverbLabel);
    
    pitchLabel.setText ("PITCH SHIFTER", juce::dontSendNotification);
    pitchLabel.setFont (juce::Font (16.0f, juce::Font::bold));
    pitchLabel.setJustificationType (juce::Justification::centred);
    pitchLabel.setColour (juce::Label::textColourId, juce::Colour (0xffFF00FF));
    addAndMakeVisible (pitchLabel);

    setResizable (true, true);
    setResizeLimits (800, 600, 1600, 1200);
    setSize (1000, 700);
}

NeonGuitarFXAudioProcessorEditor::~NeonGuitarFXAudioProcessorEditor()
{
}

//==============================================================================
void NeonGuitarFXAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fundo escuro (cores frias)
    g.fillAll (juce::Colour (0xff0A0A12)); // Azul escuro quase preto
    
    // Desenhar bordas neon
    g.setColour (juce::Colour (0xff00FFFF).withAlpha(0.3f));
    g.drawRect (getLocalBounds().reduced (5), 3);
    
    g.setColour (juce::Colour (0xff00FFFF));
    g.drawRect (getLocalBounds().reduced (7), 1);
    
    // Desenhar separadores de seção
    g.setColour (juce::Colour (0xff9D00FF).withAlpha(0.5f));
    g.drawLine (20, 200, getWidth() - 20, 200, 2);
    
    g.setColour (juce::Colour (0xff0099FF).withAlpha(0.5f));
    g.drawLine (20, 400, getWidth() - 20, 400, 2);
}

void NeonGuitarFXAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced (20);
    
    // Título
    titleLabel.setBounds (bounds.removeFromTop (50));
    
    // Seção NAM e IR (topo)
    auto namSection = bounds.removeFromTop (140);
    int knobWidth = namSection.getWidth() / 3;
    
    auto ampArea = namSection.removeFromLeft (knobWidth);
    namAmpLabel.setBounds (ampArea.removeFromTop (20));
    namAmpGainSlider.setBounds (ampArea.reduced (10));
    
    auto pedalArea = namSection.removeFromLeft (knobWidth);
    namPedalLabel.setBounds (pedalArea.removeFromTop (20));
    namPedalGainSlider.setBounds (pedalArea.reduced (10));
    
    auto irArea = namSection.removeFromLeft (knobWidth);
    irLabel.setBounds (irArea.removeFromTop (20));
    irMixSlider.setBounds (irArea.reduced (10));
    
    bounds.removeFromTop (20); // Espaço
    
    // Seção Delay
    auto delaySection = bounds.removeFromTop (180);
    delayLabel.setBounds (delaySection.removeFromTop (30));
    
    auto delayKnobs = delaySection.removeFromTop (100);
    int delayKnobWidth = delayKnobs.getWidth() / 2;
    delayFeedbackSlider.setBounds (delayKnobs.removeFromLeft (delayKnobWidth).reduced (10));
    delayMixSlider.setBounds (delayKnobs.removeFromLeft (delayKnobWidth).reduced (10));
    
    delayTimeLeftSlider.setBounds (delaySection.removeFromTop (25).reduced (10, 0));
    delayTimeRightSlider.setBounds (delaySection.removeFromTop (25).reduced (10, 0));
    
    auto delayButtons = delaySection;
    int buttonWidth = delayButtons.getWidth() / 4;
    delayPingPongButton.setBounds (delayButtons.removeFromLeft (buttonWidth).reduced (5));
    delayDottedButton.setBounds (delayButtons.removeFromLeft (buttonWidth).reduced (5));
    delayMonoButton.setBounds (delayButtons.removeFromLeft (buttonWidth).reduced (5));
    delayTypeCombo.setBounds (delayButtons.removeFromLeft (buttonWidth).reduced (5));
    
    bounds.removeFromTop (20); // Espaço
    
    // Seção Reverb
    auto reverbSection = bounds.removeFromTop (180);
    reverbLabel.setBounds (reverbSection.removeFromTop (30));
    
    auto reverbKnobs = reverbSection.removeFromTop (100);
    int reverbKnobWidth = reverbKnobs.getWidth() / 5;
    reverbSizeSlider.setBounds (reverbKnobs.removeFromLeft (reverbKnobWidth).reduced (5));
    reverbDampSlider.setBounds (reverbKnobs.removeFromLeft (reverbKnobWidth).reduced (5));
    reverbWetSlider.setBounds (reverbKnobs.removeFromLeft (reverbKnobWidth).reduced (5));
    reverbDrySlider.setBounds (reverbKnobs.removeFromLeft (reverbKnobWidth).reduced (5));
    reverbWidthSlider.setBounds (reverbKnobs.removeFromLeft (reverbKnobWidth).reduced (5));
    
    reverbStereoButton.setBounds (reverbSection.reduced (10));
    
    bounds.removeFromTop (20); // Espaço
    
    // Seção Pitch Shifter
    auto pitchSection = bounds;
    pitchLabel.setBounds (pitchSection.removeFromTop (30));
    
    auto pitchKnobs = pitchSection.removeFromTop (100);
    int pitchKnobWidth = pitchKnobs.getWidth() / 2;
    pitchMixSlider.setBounds (pitchKnobs.removeFromLeft (pitchKnobWidth).reduced (10));
    pitchIntervalSlider.setBounds (pitchKnobs.removeFromLeft (pitchKnobWidth).reduced (10));
    
    auto pitchCombos = pitchSection.removeFromTop (30);
    int comboWidth = pitchCombos.getWidth() / 3;
    pitchModeCombo.setBounds (pitchCombos.removeFromLeft (comboWidth).reduced (5));
    pitchScaleRootCombo.setBounds (pitchCombos.removeFromLeft (comboWidth).reduced (5));
    pitchScaleTypeCombo.setBounds (pitchCombos.removeFromLeft (comboWidth).reduced (5));
    
    // Panorama no final
    if (pitchSection.getHeight() > 30)
        panoramaSlider.setBounds (pitchSection.removeFromTop (30).reduced (10, 0));
}
