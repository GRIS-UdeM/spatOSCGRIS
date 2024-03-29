
/*
 ==============================================================================
 SpatGRIS: multichannel sound spatialization plug-in.
 
 Copyright (C) 2015  GRIS-UdeM
 
 Developers: Antoine Missout, Vincent Berthiaume, Nicolas Masson
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ==============================================================================
 */


#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SpatGrisAudioProcessor::SpatGrisAudioProcessor()
{
    this->sourceMover = new SourceMover(this);
    this->trajectory = new Trajectory(this);
    
    this->selectItem = new SelectItem();
    this->selectItem->selectID = 1;
    this->selectItem->selecType = SelectedSource;
    
	this->listSources = vector<Source *>();
	this->listSpeakers = vector<Speaker *>();

    
    for(int iSour = 0; iSour < MaxSources; iSour++){
        Source * newS = new Source(this, iSour+1);
        this->listSources.push_back(newS);
    }
    
    for(int iSpeak = 0; iSpeak < MaxSpeakers; iSpeak++){
        Speaker * newS = new Speaker(this, iSpeak+1);
        this->listSpeakers.push_back(newS);
    }
	
    this->numSourceUsed = MaxSources;
    this->numSpeakerUsed = MaxSpeakers;
    
    this->sourceMover->setSourcesPosition();
    
    
    this->oscRun = this->oscSender.connect(this->oscIpAddress, this->oscPort);
    this->startTimerHz(OscTimerHz);
}

SpatGrisAudioProcessor::~SpatGrisAudioProcessor()
{
    delete this->selectItem;
    delete this->sourceMover;
    delete this->trajectory;
}
//==============================================================================
const String SpatGrisAudioProcessor::getName() const {
    String name(JucePlugin_Name);
    return name;
}

//==============================================================================
void SpatGrisAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    this->bufferSize = samplesPerBlock;
}

void SpatGrisAudioProcessor::releaseResources()
{

}
//==============================================================================


//==============================================================================
void SpatGrisAudioProcessor::processBlock(AudioBuffer<float> &pBuffer, MidiBuffer& midiMessages)
{
    if (this->bufferSize != pBuffer.getNumSamples()){
        this->bufferSize = pBuffer.getNumSamples();
    }
    
    //==================================== PROCESS TRAJECTORIES ===========================================
    this->processTrajectory();
    
}
void SpatGrisAudioProcessor::processBlockBypassed (AudioBuffer<float> &buffer, MidiBuffer& midiMessages)
{

}
//==============================================================================


//==============================================================================
AudioProcessorEditor* SpatGrisAudioProcessor::createEditor()
{
    return new SpatGrisAudioProcessorEditor(this);
}
bool SpatGrisAudioProcessor::hasEditor() const
{
    return true;
}

//==============================================================================
double SpatGrisAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}
bool SpatGrisAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}
bool SpatGrisAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}
int SpatGrisAudioProcessor::getNumPrograms()
{
    return 1;
}

int SpatGrisAudioProcessor::getCurrentProgram()
{
    return 1;
}

void SpatGrisAudioProcessor::setCurrentProgram (int index)
{
}

const String SpatGrisAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void SpatGrisAudioProcessor::changeProgramName (int index, const String& newName)
{
}
//==============================================================================


//==============================================================================

void SpatGrisAudioProcessor::setPosXYSource(int idS, float x, float y, bool updateAll)
{
    *(this->listSources[idS]->getX()) = x;
    *(this->listSources[idS]->getY()) = y;
    
    if(updateAll){
        this->sourceMover->updateSourcesPosition(idS, x, y);
    }
}
void SpatGrisAudioProcessor::setPosRayAngSource(int idS, float ray, float ang, bool updateAll)
{
    FPoint xyS = GetXYFromRayAng(ray, DegreeToRadian(ang));

    *(this->listSources[idS]->getX()) = xyS.x;
    *(this->listSources[idS]->getY()) = xyS.y;
    
    if(updateAll){
        this->sourceMover->updateSourcesPosition(idS, xyS.x, xyS.y);
    }
}

void SpatGrisAudioProcessor::setPosRayAngRadSource(int idS, float ray, float ang, bool updateAll)
{
    FPoint xyS = GetXYFromRayAng(ray, ang);
    
    *(this->listSources[idS]->getX()) = xyS.x;
    *(this->listSources[idS]->getY()) = xyS.y;
    
    if(updateAll){
        this->sourceMover->updateSourcesPosition(idS, xyS.x, xyS.y);
    }
}

FPoint SpatGrisAudioProcessor::getXYSource(int idS)
{
    float x = *(this->listSources.at(idS)->getX());
    float y = *(this->listSources.at(idS)->getY());
    return FPoint(x, y);
}
FPoint SpatGrisAudioProcessor::getRayAngleSource(int idS)
{
    float x = *(this->listSources.at(idS)->getX());
    float y = *(this->listSources.at(idS)->getY());
    return FPoint(GetRaySpat(x, y), GetAngleSpat(x, y));
}

void SpatGrisAudioProcessor::setSurfaceValue(float surf)
{
    if(this->linkSurface){
        for(int iSour = 0; iSour < this->numSourceUsed; iSour++){
            *(this->listSources.at(iSour)->getSurf()) = surf;
        }
    }else{
        *(this->listSources.at(this->selectItem->selectID)->getSurf()) = surf;
    }
}

void SpatGrisAudioProcessor::setAzimuthValue(float azim)
{
    if(this->linkAzimuth){
        for(int iSour = 0; iSour < this->numSourceUsed; iSour++){
            *(this->listSources.at(iSour)->getAzim()) = azim;
        }
    }else{
        *(this->listSources.at(this->selectItem->selectID)->getAzim()) = azim;
    }
}

void SpatGrisAudioProcessor::setElevationValue(float elev)
{
    if(this->linkElevation){
        for(int iSour = 0; iSour < this->numSourceUsed; iSour++){
            *(this->listSources.at(iSour)->getElev()) = elev;
        }
    }else{
        *(this->listSources.at(this->selectItem->selectID)->getElev()) = elev;
    }
}

void SpatGrisAudioProcessor::setHeightSValue(float hei)
{
    if(this->linkHeight){
        for(int iSour = 0; iSour < this->numSourceUsed; iSour++){
            *(this->listSources.at(iSour)->getHeigt()) = hei;
        }
    }else{
        *(this->listSources.at(this->selectItem->selectID)->getHeigt()) = hei;
    }
}

//==============================================================================
void SpatGrisAudioProcessor::processTrajectory()
{
    AudioPlayHead::CurrentPositionInfo cpi;
    getPlayHead()->getCurrentPosition(cpi);
    
    if(this->trajectory->getProcessTrajectory() && cpi.isPlaying){
        double bps = cpi.bpm / 60;
        float seconds = this->bufferSize / this->sampleRate;
        float beats = seconds * bps;
        
        bool done = this->trajectory->process(seconds, beats);
    }
    
}


void SpatGrisAudioProcessor::sendOscMessageValues()
{
    if(this->oscOn ){
        switch (this->typeProcess) {
                
            case OSCSpatServer:{
                for(int iCurSrc = 0; iCurSrc < this->numSourceUsed; ++iCurSrc){
                    int   channel_osc   = this->oscFirstIdSource+iCurSrc-1;
                    FPoint rayAng       = getRayAngleSource(iCurSrc);
                    float azim_osc      = ThetaMax - (rayAng.y - QuarterCircle);
                    float elev_osc      = (rayAng.x / RadiusMax) * QuarterCircle;
                    float azimspan_osc  = *this->getListSource().at(iCurSrc)->getAzim();
                    float elevspan_osc  = *this->getListSource().at(iCurSrc)->getElev();
                    float height_osc    = *this->getListSource().at(iCurSrc)->getHeigt();
                    float gain_osc      = 1;
                    
                    OSCAddressPattern oscPattern("/spat/serv");
                    OSCMessage message(oscPattern);
                    
                    message.addInt32(channel_osc);
                    message.addFloat32(azim_osc);
                    message.addFloat32(elev_osc);
                    message.addFloat32(azimspan_osc);
                    message.addFloat32(elevspan_osc);
                    message.addFloat32(height_osc);
                    message.addFloat32(gain_osc);
                    if (!this->oscSender.send(message)) {
                        DBG("Error: could not send OSC message.");
                        return;
                    }
                }
                break;
            }
                
            case OSCZirkonium:{
                for(int iCurSrc = 0; iCurSrc < this->numSourceUsed; ++iCurSrc){
                    int   channel_osc   = this->oscFirstIdSource+iCurSrc-1;             //in gui the range is 1-99, for zirkonium it actually starts at 0 (or potentially lower, but Zirkosc uses 0 as starting channel)
                    FPoint rayAng       = getRayAngleSource(iCurSrc);
                    float azim_osc      = ((rayAng.y+(M_PI/2.0f))/M_PI) - 1.0f;                     //For Zirkonium, -1 is in the back right and +1 in the back left. 0 is forward
                    float elev_osc      = 0.5f -  ((rayAng.x/2.0f)*0.5f);                     //For Zirkonium, 0 is the edge of the dome, .5 is the top
                    float azimspan_osc  = *this->getListSource().at(iCurSrc)->getAzim();     //min azim span is 0, max is 2. I figure this is radians.
                    float elevspan_osc  = *this->getListSource().at(iCurSrc)->getElev();     //min elev span is 0, max is .5
                    float gain_osc      = 1;                                //gain is just locked to max value
                    
                    OSCAddressPattern oscPattern("/pan/az");
                    OSCMessage message(oscPattern);
                    
                    message.addInt32(channel_osc);
                    message.addFloat32(azim_osc);
                    message.addFloat32(elev_osc);
                    message.addFloat32(azimspan_osc);
                    message.addFloat32(elevspan_osc);
                    message.addFloat32(gain_osc);
                    if (!this->oscSender.send(message)) {
                        DBG("Error: could not send OSC message.");
                        return;
                    }
                }
                break;
            }
                
        }
    }
}

void SpatGrisAudioProcessor::timerCallback()
{
    this->sendOscMessageValues();
}

//==============================================================================
void SpatGrisAudioProcessor::getStateInformation (MemoryBlock& destData)
{

}
void SpatGrisAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    
}
//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpatGrisAudioProcessor();
}
