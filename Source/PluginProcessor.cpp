/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Govers_a2AudioProcessor::Govers_a2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    currentSampleRate = 0.0f;
    currentAngle = 0.0f;
    angleDelta = 0.0f;
    sinFreq = 60.0f;
}

Govers_a2AudioProcessor::~Govers_a2AudioProcessor()
{
}

//==============================================================================
const String Govers_a2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Govers_a2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Govers_a2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Govers_a2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Govers_a2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Govers_a2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Govers_a2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Govers_a2AudioProcessor::setCurrentProgram (int index)
{
}

const String Govers_a2AudioProcessor::getProgramName (int index)
{
    return {};
}

void Govers_a2AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Govers_a2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    currentSampleRate = sampleRate;
    sinFreq = 60.0f;
    
    updateAngleDelta();
    
    // initiate mixLevel
    mixLevel.reset(sampleRate, 0.1f);
    mixLevel.setTargetValue(0.25f);
    
    freqLevel.reset(sampleRate, 0.01f);
    freqLevel.setTargetValue(0.0);
    
    // setting gain to call later, in decibels
    gain.setGainDecibels(12.0f);
    
    String message;
    message << "Preparing to play..." << newLine;
    message << "My sample rate is" << currentSampleRate << newLine;
    message << "Buffer size is" << samplesPerBlock << newLine;
    Logger::getCurrentLogger()->writeToLog(message);
    
}

void Govers_a2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
 
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Govers_a2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Govers_a2AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    AudioBuffer<float> wetBuffer(getTotalNumInputChannels(), buffer.getNumSamples());
    wetBuffer.makeCopyOf(buffer);
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    wetBuffer.makeCopyOf(buffer);
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        // we are getting an audio block of size 512
        auto* channelData = buffer.getWritePointer (channel);
        auto* wetData = wetBuffer.getWritePointer(channel);

        
        for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Have left in several different processes as comments for later use if wanted. Didn't want to shift their positions within the code itself!
            
            // generating a set of random values to modulate input amplitude - NOT USED
            // float modulator = random.nextFloat() * 0.25f - 0.125f;
            // wetData[sample] = wetData[sample] * modulator
            
            sinFreq = freqLevel.getNextValue();
            updateAngleDelta();
            
            // This part is the ring mod where it is incorporating the Sin set to 60.0f at the top of the code
            auto currentSinSample = (float) std::sin(currentAngle);
            currentAngle += angleDelta; //currentAngle = currentAngle + angleDelta
            
            // let's make our sin wave wiggle - NOT USED
            // float modulator = random.nextFloat() * 0.1f - 0.05f;
            // wetData[sample] = wetData[sample] * (currentSinSample + modulator);
            
            // creating/setting the wetData value
            wetData[sample] = wetData[sample] * currentSinSample;
            
            
            // this makes it so you can process it first and then chuck it through a wave shaper
            
            // hard clipping waveshaper
            // might not be needed for assessment having both, but thought that they sounded cool together so apologies if we only needed one or the other (hopefully its fine to do!)
            // I liked the values of -0.7 and 0.7, specifically when used with a synth sample with a high and low line/part
            auto shapedSample = jlimit((float) -0.7, (float) 0.7, wetData[sample]);
            
            // soft clip with hypobolic tangent - NOT USED
            //auto shapedSample = (float) std::tanh(wetData[sample]);
            
            
            //auto shapedSample = (32 >> 1) * (float) std::tanh(16 * wetData[sample]/32); - NOT USED
            
            // DECIMATOR CODE - NOT USED
            //float quantum = powf( 2.0f, bits );
            // per sample
            //auto shapedSample = floor(wetData[sample] * quantum) / quantum;
            
            // Finalising the wetData for mix below
            wetData[sample] = shapedSample;
            
            // Here is the wet/dry mix, set to 40% dry + 60% wet
            //channelData[sample] = channelData[sample] * 0.4f + wetData[sample] * 0.6f;
            
           
            
            
            // implementing slider control
            channelData[sample] = channelData[sample] * (1.0f - (mixLevel.getNextValue()*0.01f)) + ((wetData[sample] * mixLevel.getNextValue()) * 0.01f);
            
            // lowering final gain
            channelData[sample] = channelData[sample] * 0.4f;
            
            // Finally, here is a gain control. It wasn't clipping in DAW without this, but added to provide a bit more headroom and for easy volume manipulation rather than chaging mix above NOT USED
            //channelData[sample] = channelData[sample] * 0.9f;
            
            // I like this when used with a synth sample, especially if it has a low and a high synth
            // They really contrast nicely with the hard-clip and overall sound pretty gnarly and demented almost
        }
    }
    dsp::AudioBlock<float> output(buffer);
    gain.process(dsp::ProcessContextReplacing<float>(output));
}

//==============================================================================
bool Govers_a2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Govers_a2AudioProcessor::createEditor()
{
    return new Govers_a2AudioProcessorEditor (*this);
}

//==============================================================================
void Govers_a2AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Govers_a2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// User functions

void Govers_a2AudioProcessor::updateAngleDelta()
{
    // calculate no. of cycles that will need to complete for each output sample
    auto cyclesPerSample = sinFreq / currentSampleRate;
    // multiply by the length of a whole sin wave cycle
    angleDelta = cyclesPerSample * MathConstants<float>::twoPi;
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Govers_a2AudioProcessor();
}
