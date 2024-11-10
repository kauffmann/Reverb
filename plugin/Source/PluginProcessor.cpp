#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ), apvts(*this, nullptr, "Parameters", createParameterLayout()) {
    
    // Register the processor as a listener to the parameters
    apvts.addParameterListener("SIZE", this);
    apvts.addParameterListener("DECAY", this);
    apvts.addParameterListener("DRY", this);
    apvts.addParameterListener("DIFFUSSER", this);
    apvts.addParameterListener("WET_REFLECTIONS", this);
    apvts.addParameterListener("PREDELAY", this);
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() 
{
    apvts.removeParameterListener("SIZE", this);
    apvts.removeParameterListener("DECAY", this);
    apvts.removeParameterListener("DRY", this);
    apvts.removeParameterListener("DIFFUSSER", this);
    apvts.removeParameterListener("WET_REFLECTIONS", this);
    apvts.removeParameterListener("PREDELAY", this);
}

const juce::String AudioPluginAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
             // programs, so this should be at least 1, even if you're not
             // really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram() {
  return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index,
                                                  const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void AudioPluginAudioProcessor::prepareToPlay(double sampleRate,
                                              int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  juce::ignoreUnused(sampleRate, samplesPerBlock);

  reverb.configure(sampleRate);

  
}

void AudioPluginAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;

    auto* channelDataL = buffer.getWritePointer(0);
    auto* channelDataR = buffer.getWritePointer(1);

    reverb.process(channelDataL, channelDataR, buffer.getNumSamples());
      
}

bool AudioPluginAudioProcessor::hasEditor() const {
  return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor() {
  return new juce::GenericAudioProcessorEditor(*this);
}

void AudioPluginAudioProcessor::getStateInformation(
    juce::MemoryBlock& destData) {
  
    
    juce::ValueTree params("Params");

    for (auto& param : getParameters())
    {
        juce::ValueTree paramTree(getParamID(param));
        paramTree.setProperty("Value", param->getValue(), nullptr);
        params.appendChild(paramTree, nullptr);
    }


    copyXmlToBinary(*params.createXml(), destData);


}

void AudioPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes) 
{


    auto xml = getXmlFromBinary(data, sizeInBytes);

    if (xml != nullptr)
    {
        auto preset = juce::ValueTree::fromXml(*xml);

        for (auto& param : getParameters())
        {
            auto paramTree = preset.getChildWithName(getParamID(param));

            if (paramTree.isValid())
                param->setValueNotifyingHost(paramTree["Value"]);
        }
    }
}


// This creates new instances of the plugin.
// This function definition must be in the global namespace.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new AudioPluginAudioProcessor();
}




juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SIZE",
        "Room Size",
        juce::NormalisableRange<float>(10.0f, 200.0f, 10.0f),
        50.0f)); // default:

    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY",
        "Decay",
        juce::NormalisableRange<float>(0.2f, 40.0f, 0.1f),
        1.2f)); 


    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRY",
        "Dry",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.1f),
        0.2f)); // default

    params.push_back(std::make_unique<juce::AudioParameterFloat>("DIFFUSSER",
        "Diffusser Gain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.1f),
        0.2f)); // default

    params.push_back(std::make_unique<juce::AudioParameterFloat>("WET_REFLECTIONS",
        "Early Reflection Gain",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.1f), 0.2f, String(), AudioProcessorParameter::genericParameter,
        [](float value, int) -> String
        {


            String valueToText =
                String(value * 100, 1) + " %";


            return valueToText;
        })); // default

    params.push_back(std::make_unique<juce::AudioParameterFloat>("PREDELAY",
        "Predelay",
        juce::NormalisableRange<float>(0.0f, 500.0f, 1.0f), 20.0f, String(), AudioProcessorParameter::genericParameter,
        [](float value, int) -> String
        {
            

            String valueToText =
                String(value, 1) + " ms";


            return valueToText;
        })); // default



    

    return { params.begin(), params.end() };
}

void AudioPluginAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue) 
{
    if (parameterID == "SIZE")
    {
        reverb.setRoomSize(newValue);

    }
    else if (parameterID == "DECAY")
    {
        reverb.setDecay(newValue);
       
    }

    else if (parameterID == "DRY")
    {
       reverb.setDry(newValue);
    }

    else if (parameterID == "DIFFUSSER") 
    {
        reverb.setDiffusionGain(newValue);
    }

    else if (parameterID == "WET_REFLECTIONS") 
    {
        reverb.setEarlyReflections(newValue);
    }

    else if (parameterID == "PREDELAY")
    {
        reverb.setPreDelay(newValue);
    }
}