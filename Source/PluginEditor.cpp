 /*
 ==============================================================================

 SpatGRIS: multichannel sound spatialization plug-in.
 
 Copyright (C) 2015  GRIS-UdeM
 
 PluginEditor.cpp
 
 Developers: Antoine Missout, Vincent Berthiaumen, Nicolas Masson
 
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


#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
SpatGrisAudioProcessorEditor::SpatGrisAudioProcessorEditor(SpatGrisAudioProcessor * filter) :
    AudioProcessorEditor (filter)
{
    
    LookAndFeel::setDefaultLookAndFeel(&this->grisFeel);
    this->filter = filter;

    this->spatFieldComp = new SpatComponent(this, this->filter, &this->grisFeel);
    this->addAndMakeVisible(this->spatFieldComp);
    
    //BOX-----------------------------------------------------------------
    this->boxSourceParam = new Box(&this->grisFeel, "Source parameters");
    this->addAndMakeVisible(this->boxSourceParam);
    
    ///this->boxOutputParam = new Box(&this->grisFeel, "Output parameters");
    //this->addAndMakeVisible(this->boxOutputParam);
    
    this->boxTrajectory = new Box(&this->grisFeel, "Trajectories");
    this->addAndMakeVisible(this->boxTrajectory);

    //OctTabbedComponent---------------------------------------------------
    this->octTab = new OctTabbedComponent(&this->grisFeel, TabbedButtonBar::TabsAtTop, this->filter);

    this->octTab->addTab("Settings",           this->grisFeel.getBackgroundColour(), new Component(), true);
    //this->octTab->addTab("Volume & Filters",   this->grisFeel.getBackgroundColour(), new Component(), true);
    this->octTab->addTab("Sources",            this->grisFeel.getBackgroundColour(), new Component(), true);
   	//this->octTab->addTab("Speakers",           this->grisFeel.getBackgroundColour(), new Component(), true);
    this->octTab->addTab("Interfaces",         this->grisFeel.getBackgroundColour(), new Component(), true);
    this->octTab->setTabBarDepth(28);
    this->addAndMakeVisible(this->octTab);
    
    
    //Add All Component-------------------------------------------------------------------------------------
    this->labAzimSpan           = addLabel("Azimuth Span", "Azimuth Span Selected Source", 0, 4, DefaultLabWidth, DefaultLabHeight, this->boxSourceParam->getContent());
    this->togLinkAzimSpan       = addToggleButton("Link", "Link other sources", 90, 2, DefaultLabWidth, DefaultLabHeight,  this->boxSourceParam->getContent());
    this->sliAzimSpan           = addSlider("", "", 4, 24, 180, DefaultLabHeight, this->boxSourceParam->getContent(), MinAzimSource, MaxAzimSource, ShowSliderInter);
    
    this->labElevSpan           = addLabel("Elevation Span", "Elevation Span Selected Source", 0, 50, DefaultLabWidth, DefaultLabHeight, this->boxSourceParam->getContent());
    this->togLinkElevSpan       = addToggleButton("Link", "Link other sources", 90, 48, DefaultLabWidth, DefaultLabHeight,  this->boxSourceParam->getContent());
    this->sliElevSpan           = addSlider("", "", 4, 70, 180, DefaultLabHeight, this->boxSourceParam->getContent(), MinElevSource, MaxElevSource, ShowSliderInter);
    
    this->labHeightSour         = addLabel("Height", "Height Selected Source", 0, 96, DefaultLabWidth, DefaultLabHeight, this->boxSourceParam->getContent());
    this->togLinkHeightSour     = addToggleButton("Link", "Link other sources", 90, 94, DefaultLabWidth, DefaultLabHeight,  this->boxSourceParam->getContent());
    this->sliHeightSour         = addSlider("", "", 4, 116, 180, DefaultLabHeight, this->boxSourceParam->getContent(), MinHeigSource, MaxHeigSource, ShowSliderInter);
    //-----------------------------


    
    
    //Trajectories
    //-----------------------------
    this->labMouvement      = addLabel("Mouvements :", "Mouvement with other sources", 0, 4, DefaultLabWidth, DefaultLabHeight, this->boxTrajectory->getContent());
    this->comMouvement      = addComboBox("", "Mouvement with other sources", 90, 4, DefaultLabWidth, DefaultLabHeight, this->boxTrajectory->getContent());
    
    this->labTypeTrajectory = addLabel("Types :", "Types of trajectories", 0, 30, DefaultLabWidth, DefaultLabHeight, this->boxTrajectory->getContent());
    this->comTypeTrajectory = addComboBox("", "Types of trajectories", 60, 30, DefaultLabWidth+30, DefaultLabHeight, this->boxTrajectory->getContent());
    for(int i = 0; i  < TrajectoryType::SIZE_TT; i++){
        this->comTypeTrajectory->addItem(GetTrajectoryName((TrajectoryType)i), i+1);
    }
    this->comTypeTrajectory->setSelectedId(1);
    
    
    this->labTimeTrajectory = addLabel("Time :", "Time of trajectory", 0, 50, DefaultLabWidth, DefaultLabHeight, this->boxTrajectory->getContent());
    this->texTimeTrajectory = addTextEditor("", "", "Time of trajectory", 60, 50, DefaultTexWidth, DefaultLabHeight, this->boxTrajectory->getContent());
    this->comTimeTrajectory = addComboBox("", "Per cycle(s)", 124, 50, DefaultLabWidth-34, DefaultLabHeight, this->boxTrajectory->getContent());
    this->comTimeTrajectory->addItem("Second(s)", 1);
    this->comTimeTrajectory->addItem("Beat(s)", 2);
    this->comTimeTrajectory->setSelectedId(1);
    
    this->labCycleTrajectory = addLabel("Cycle(s) :", "Cycle of trajectory", 0, 70, DefaultLabWidth, DefaultLabHeight, this->boxTrajectory->getContent());
    this->texCycleTrajectory = addTextEditor("", "", "Numbers of cycle(s) 0=inf", 60, 70, DefaultTexWidth, DefaultLabHeight, this->boxTrajectory->getContent());
    
    this->butReadyTrajectory = addButton("Ready", "Valid trajectory param", 60, 100, DefaultLabWidth, DefaultLabHeight, this->boxTrajectory->getContent());
    
    this->progressBarTraject = new ProgressBarTraj();
    this->progressBarTraject->setBounds(60, 124, DefaultLabWidth+30, DefaultLabHeight);
    this->boxTrajectory->getContent()->addAndMakeVisible(this->progressBarTraject);
    this->progressBarTraject->setVisible(false);
    
    this->sliSpeedTrajectory = addSlider("Speed :", "Speed of trajectory", 14, 150, 204, DefaultLabHeight, this->boxTrajectory->getContent(), MinSpeedTrajectory, MaxSpeedTrajectory,  ShowSliderInter);
    
    //Other param Trajectories hided---
    int rowX = 260;
    
    this->labTrajEllipseWidth = addLabel("Width :", "Width of ellipse", rowX, 30, DefaultLabWidth, DefaultLabHeight, this->boxTrajectory->getContent());
    this->texTrajEllipseWidth = addTextEditor("", "", "Width of ellipse", rowX+50, 30, DefaultTexWidth, DefaultLabHeight, this->boxTrajectory->getContent());
    
    this->comTrajOneWayReturn = addComboBox("", "Trajectory cycle", rowX, 30, DefaultLabWidth-30, DefaultLabHeight, this->boxTrajectory->getContent());
    this->comTrajOneWayReturn->addItem("One Way", 1);
    this->comTrajOneWayReturn->addItem("Return", 2);
    this->comTrajOneWayReturn->setSelectedId(1);

    this->labTrajRadAngEnd = addLabel("Radius / Angle of end point :", "Radius and angle end point", rowX-5, 50, 200, DefaultLabHeight, this->boxTrajectory->getContent());
    this->texTrajRadiusEnd = addTextEditor("", "", "Radius (0-2)", rowX, 70, 40, DefaultLabHeight, this->boxTrajectory->getContent());
    this->texTrajAngleEnd = addTextEditor("", "", "Angle (0-360)", rowX+50, 70, 40, DefaultLabHeight, this->boxTrajectory->getContent());

    this->butTrajSetEnd = addButton("Set", "Set end point", rowX+100, 70, 70, DefaultLabHeight, this->boxTrajectory->getContent());
    this->butTrajResetEnd = addButton("Reset", "Reset end point", rowX+180, 70, 70, DefaultLabHeight, this->boxTrajectory->getContent());

    this->labTrajPendDampe = addLabel("Dampening :", "Dampening (0-1)", rowX-5, 94, 80, DefaultLabHeight, this->boxTrajectory->getContent());
    this->texTrajPendDampe = addTextEditor("", "", "Dampening (0-1)", rowX+80, 94, 40, DefaultLabHeight, this->boxTrajectory->getContent());

    this->labTrajPendDevia = addLabel("Deviation :", "Deviation (0-360)", rowX-5, 114, 80, DefaultLabHeight, this->boxTrajectory->getContent());
    this->texTrajPendDevia = addTextEditor("", "", "Deviation (0-360)", rowX+80, 114, 40, DefaultLabHeight, this->boxTrajectory->getContent());
    
    this->labTrajRandSpeed = addLabel("Speed :", "Speed Random", rowX-5, 30, 80, DefaultLabHeight, this->boxTrajectory->getContent());
    this->sliTrajRandSpeed = addSlider("Speed :", "Speed Random", rowX+48, 30, 160, DefaultLabHeight, this->boxTrajectory->getContent(), MinTrajRandomSpeed, MaxTrajRandomSpeed,  0.01f);
    this->togTrajRandSepare = addToggleButton("Separate sources :", "Force separate automation sources", rowX, 50, DefaultLabWidth, DefaultLabHeight,  this->boxTrajectory->getContent());

    //Add in list for lock
    this->listLockCompTrajectory.push_back(this->comTypeTrajectory);
    this->listLockCompTrajectory.push_back(this->texTimeTrajectory);
    this->listLockCompTrajectory.push_back(this->comTimeTrajectory);
    this->listLockCompTrajectory.push_back(this->texCycleTrajectory);
    
    this->listLockCompTrajectory.push_back(this->texTrajEllipseWidth);
    this->listLockCompTrajectory.push_back(this->comTrajOneWayReturn);
    
    this->listLockCompTrajectory.push_back(this->texTrajRadiusEnd);
    this->listLockCompTrajectory.push_back(this->texTrajAngleEnd);
    
    this->listLockCompTrajectory.push_back(this->butTrajSetEnd);
    this->listLockCompTrajectory.push_back(this->butTrajResetEnd);
    
    this->listLockCompTrajectory.push_back(this->texTrajPendDampe);
    this->listLockCompTrajectory.push_back(this->texTrajPendDevia);
    
    this->listLockCompTrajectory.push_back(this->sliTrajRandSpeed);
    this->listLockCompTrajectory.push_back(this->togTrajRandSepare);
    
    //OctTabbedComponent-----------------------
    
    //Settings
    //-----------------------------
    Component * settingsBox = this->octTab->getTabContentComponent(0);
    this->labTypeProcess    = addLabel("Mode :", "Process mode", 0, 4, DefaultLabWidth, DefaultLabHeight, settingsBox);
    this->comTypeProcess    = addComboBox("", "Process mode", 60, 4, DefaultLabWidth+30, DefaultLabHeight, settingsBox);
    for(int i = 0; i  < ProcessType::SIZE_PT; i++){
        this->comTypeProcess->addItem(GetProcessTypeName((ProcessType)i), i+1);
    }
    this->comTypeProcess->setSelectedId(1);
    
    this->labInOutMode      = addLabel("Input :", "Input mode", 0, 30, DefaultLabWidth, DefaultLabHeight, settingsBox);
    this->comInOutMode      = addComboBox("", "Input mode", 60, 30, DefaultLabWidth-40, DefaultLabHeight, settingsBox);
    this->butInOutMode      = addButton("Apply", "Apply Input/Output mode", 150, 30, 60, DefaultLabHeight, settingsBox);
    
    
    this->togOSCActive      = addToggleButton("OSC On/Off", "OSC Active (On/Off)", 4, 60, DefaultLabWidth, DefaultLabHeight, settingsBox);
    this->togOSCActive->setToggleState(this->filter->getOscOn(), dontSendNotification);
    
    this->labOSCSourceIDF   = addLabel("OSC 1er ID :", "OSC 1er Source ID", 4, 80, DefaultLabWidth, DefaultLabHeight, settingsBox);
    this->texOSCSourceIDF   = addTextEditor("", "", "OSC 1er Source ID", 80, 80, 60, DefaultLabHeight, settingsBox);
    this->texOSCSourceIDF->setText(String(this->filter->getOscFirstIdSource()));
    
    this->labOSCPort        = addLabel("OSC Port :", "OSC Port", 4, 100, DefaultLabWidth, DefaultLabHeight, settingsBox);
    this->texOSCPort        = addTextEditor("", "", "OSC Port", 80, 100, 60, DefaultLabHeight, settingsBox);
    this->texOSCPort->setText(String(this->filter->getOscPort()));
    
    //Sources
    //-----------------------------
    Component * sourcesBox = this->octTab->getTabContentComponent(1);
    this->labSourcePos          = addLabel("Source position :", "Source position", 0, 4, DefaultLabWidth, DefaultLabHeight, sourcesBox);
    this->comSourcePos          = addComboBox("", "Source position", 110, 4, DefaultLabWidth+20, DefaultLabHeight, sourcesBox);
    for(int i = 0; i  < PositionSourceSpeaker::SIZE_PSS; i++){
        this->comSourcePos->addItem(GetPositionSourceSpeakerName((PositionSourceSpeaker)i), i+1);
    }
    this->comSourcePos->setSelectedId(1);
    
    this->butSourcePos          = addButton("Apply", "Apply Source position", 254, 4, 60, DefaultLabHeight, sourcesBox);
    
    
    
    this->labSourceSelectPos    = addLabel("Selected :", "Source Selected", 0, 30, DefaultLabWidth, DefaultLabHeight, sourcesBox);
    this->comSourceSelectPos    = addComboBox("", "Source Selected", 80, 30, 40, DefaultLabHeight, sourcesBox);
    
    this->labSourceSelectRay    = addLabel("Ray :", "Ray (0 - 2)", 0, 50, DefaultLabWidth, DefaultLabHeight, sourcesBox);
    this->comSourceSelectRay    = addTextEditor("","", "Ray (0 - 2)", 50, 50, 70, DefaultLabHeight, sourcesBox);
    this->labSourceInfoRay      = addLabel("(0 - 2)", "", 120, 50, DefaultLabWidth, DefaultLabHeight, sourcesBox);
    
    this->labSourceSelectAngle  = addLabel("Angle :", "Angle (0 - 360)", 0, 70, DefaultLabWidth, DefaultLabHeight, sourcesBox);
    this->comSourceSelectAngle  = addTextEditor("","", "Angle (0 - 360)", 50, 70, 70, DefaultLabHeight, sourcesBox);
    this->labSourceInfoAngle    = addLabel("(0 - 360)", "", 120, 70, DefaultLabWidth, DefaultLabHeight, sourcesBox);
    

    //Interfaces
    //-----------------------------
    Component * interfaceBox = this->octTab->getTabContentComponent(2);
    addLabel("Comming soon...", "", 0, 4, DefaultLabWidth, DefaultLabHeight, interfaceBox);
    //------------------------------------------------------------------------------------------------------
    
    
    
    //Size window
    this->resizeWindow.setSizeLimits (MinFieldSize + (2*Margin), MinFieldSize + (2*Margin), 1920, 1080);
    this->addAndMakeVisible (this->resizer = new ResizableCornerComponent (this, &this->resizeWindow));
    this->setSize(DefaultUItWidth, DefaultUIHeight);
    
    this->updateComMouvement();
    this->updateSourceParam();
    this->updateTrajectoryParam();
    this->updateInputOutputMode();
    this->comInOutMode->setSelectedId(1);
    this->updateSelectSource();
    
	this->startTimerHz(HertzRefresh);
}

SpatGrisAudioProcessorEditor::~SpatGrisAudioProcessorEditor()
{
    for (auto&& it : this->vecLevelOut)
    {
        delete (it);
    }
    
    delete this->progressBarTraject;
    
    delete this->spatFieldComp;
    delete this->boxSourceParam;
    delete this->boxTrajectory;
    delete this->octTab;
}
//==============================================================================

Label* SpatGrisAudioProcessorEditor::addLabel(const String &s, const String &stooltip, int x, int y, int w, int h, Component *into)
{
    Label *lb = new Label();
    lb->setText(s, NotificationType::dontSendNotification);
    lb->setTooltip (stooltip);
    lb->setJustificationType(Justification::left);
    lb->setFont(this->grisFeel.getFont());
    lb->setLookAndFeel(&this->grisFeel);
    lb->setColour(Label::textColourId, this->grisFeel.getFontColour());
    lb->setBounds(x, y, w, h);
    into->addAndMakeVisible(lb);
    return lb;
}

TextButton* SpatGrisAudioProcessorEditor::addButton(const String &s, const String &stooltip, int x, int y, int w, int h, Component *into)
{
    TextButton *tb = new TextButton();
    tb->setTooltip (stooltip);
    tb->setButtonText(s);
    tb->setSize(w, h);
    tb->setTopLeftPosition(x, y);
    tb->addListener(this);
    tb->setColour(ToggleButton::textColourId, this->grisFeel.getFontColour());
    tb->setLookAndFeel(&this->grisFeel);
    into->addAndMakeVisible(tb);
    return tb;
}

ToggleButton* SpatGrisAudioProcessorEditor::addToggleButton(const String &s, const String &stooltip, int x, int y, int w, int h, Component *into, bool toggle)
{
    ToggleButton *tb = new ToggleButton();
    tb->setTooltip (stooltip);
    tb->setButtonText(s);
    tb->setToggleState(toggle, dontSendNotification);
    tb->setSize(w, h);
    tb->setTopLeftPosition(x, y);
    tb->addListener(this);
    tb->setColour(ToggleButton::textColourId, this->grisFeel.getFontColour());
    tb->setLookAndFeel(&this->grisFeel);
    into->addAndMakeVisible(tb);
    return tb;
}

TextEditor* SpatGrisAudioProcessorEditor::addTextEditor(const String &s, const String &emptyS, const String &stooltip, int x, int y, int w, int h, Component *into, int wLab)
{
    TextEditor *te = new TextEditor();
    te->setTooltip (stooltip);
    te->setTextToShowWhenEmpty(emptyS, this->grisFeel.getOffColour());
    te->setColour(ToggleButton::textColourId, this->grisFeel.getFontColour());
    te->setLookAndFeel(&this->grisFeel);
    
    if (s.isEmpty()){
        te->setBounds(x, y, w, h);
    }else{
        te->setBounds(x+wLab, y, w, h);
        Label *lb =addLabel(s, "", x, y, wLab, h, into);
        lb->setJustificationType(Justification::centredRight);
    }
    
    te->addListener(this);
    into->addAndMakeVisible(te);
    return te;
}

Slider* SpatGrisAudioProcessorEditor::addSlider(const String &s, const String &stooltip, int x, int y, int w, int h, Component *into, float minF, float maxF, float defF, juce::Slider::TextEntryBoxPosition tebp)
{
    Slider *sd = new Slider();
    sd->setTooltip (stooltip);
    //sd->setTextValueSuffix(s);
    sd->setSize(w, h);
    sd->setTopLeftPosition(x, y);
    sd->setRange(minF, maxF, defF);
    //sd->setSliderStyle(Slider::Rotary);
    //sd->setRotaryParameters(M_PI * 1.3f, M_PI * 2.7f, true);
    sd->setTextBoxStyle (tebp, false, 40, DefaultLabHeight);
    sd->setColour(ToggleButton::textColourId, this->grisFeel.getFontColour());
    sd->setLookAndFeel(&this->grisFeel);
    sd->addListener(this);
    into->addAndMakeVisible(sd);
    return sd;
}
ComboBox* SpatGrisAudioProcessorEditor::addComboBox(const String &s, const String &stooltip, int x, int y, int w, int h, Component *into)
{
    ComboBox *cb = new ComboBox();
    cb->setTooltip(stooltip);
    cb->setSize(w, h);
    cb->setTopLeftPosition(x, y);
    cb->setLookAndFeel(&this->grisFeel);
    cb->addListener(this);
    into->addAndMakeVisible(cb);
    return cb;
}
//==============================================================================
void SpatGrisAudioProcessorEditor::updateSourceParam()
{
    //this->togLinkSurfaceOrPan->setToggleState(this->filter->getLinkSurface(),   dontSendNotification);
    this->togLinkAzimSpan->setToggleState(this->filter->getLinkAzimuth(),       dontSendNotification);
    this->togLinkElevSpan->setToggleState(this->filter->getLinkElevation(),     dontSendNotification);
    
    const int idS = this->filter->getSelectItem()->selectID;
    //this->sliSurfaceOrPan->setValue(*(this->filter->getListSource().at(idS)->getSurf()),dontSendNotification);
    //this->sliSurfaceOrPan->setTooltip("S:"+String(this->sliSurfaceOrPan->getValue(),2));
    
    this->sliAzimSpan->setValue(*(this->filter->getListSource().at(idS)->getAzim()),    dontSendNotification);
    this->sliAzimSpan->setTooltip("A:"+String(this->sliAzimSpan->getValue(),2));
    
    this->sliElevSpan->setValue(*(this->filter->getListSource().at(idS)->getElev()),  dontSendNotification);
    this->sliElevSpan->setTooltip("E:"+String(this->sliElevSpan->getValue(),2));
    
    this->sliHeightSour->setValue(*(this->filter->getListSource().at(idS)->getHeigt()),  dontSendNotification);
    this->sliHeightSour->setTooltip("E:"+String(this->sliHeightSour->getValue(),2));
}


void SpatGrisAudioProcessorEditor::updateComMouvement()
{
    this->comMouvement->clear();
    for(int i = 0; i  < MouvementMode::SIZE_MM; i++){
        if(((MouvementMode)i == MouvementMode::SymmetricX || (MouvementMode)i == MouvementMode::SymmetricY)){
            if (this->filter->getNumSourceUsed() == 2){
                this->comMouvement->addItem(GetMouvementModeName((MouvementMode)i), i+1);
            }
        }
        else{
            this->comMouvement->addItem(GetMouvementModeName((MouvementMode)i), i+1);
        }
    }
    this->comMouvement->setSelectedId(this->filter->getSourceMover()->getMouvementMode()+1);
}

void SpatGrisAudioProcessorEditor::updateTrajectoryParam()
{
    this->comTypeTrajectory->setSelectedId(this->filter->getTrajectory()->getTrajectoryType()+1);
    this->texTimeTrajectory->setText(String(this->filter->getTrajectory()->getTimeDuration()));
    this->comTimeTrajectory->setSelectedId(this->filter->getTrajectory()->getInSeconds() ? 1 : 2);
    this->texCycleTrajectory->setText(String(this->filter->getTrajectory()->getCycle()));
    this->sliSpeedTrajectory->setValue(this->filter->getTrajectory()->getSpeed());
    
    this->texTrajEllipseWidth->setText(String(this->filter->getTrajectory()->getEllipseWidth()));
    this->comTrajOneWayReturn->setSelectedId(this->filter->getTrajectory()->getInOneWay() ? 1 : 2);
    this->texTrajRadiusEnd->setText(String(this->filter->getTrajectory()->getRadiusEnd()));
    this->texTrajAngleEnd->setText(String(this->filter->getTrajectory()->getAngleEnd()));
    
    this->texTrajPendDampe->setText(String(this->filter->getTrajectory()->getPendDampening()));
    this->texTrajPendDevia->setText(String(this->filter->getTrajectory()->getPendDeviation()));
    
    this->sliTrajRandSpeed->setValue(this->filter->getTrajectory()->getRandSpeed());
    this->togTrajRandSepare->setToggleState(this->filter->getTrajectory()->getRandSeparate(),dontSendNotification);
    
    if(!(this->filter->getTrajectory()->getProcessTrajectory())){   //Start ready...
        for (auto&& it : this->listLockCompTrajectory)
        {
            it->setEnabled(false);
        }
        
        this->progressBarTraject->setVisible(true);
        this->filter->getTrajectory()->setProcessTrajectory(true);
        
    }else{
        for (auto&& it : this->listLockCompTrajectory)
        {
            it->setEnabled(true);
        }
        
        this->progressBarTraject->setVisible(false);
        this->filter->getTrajectory()->setProcessTrajectory(false);
    }
    
    this->butReadyTrajectory->setToggleState(this->filter->getTrajectory()->getProcessTrajectory(), dontSendNotification);

}

void SpatGrisAudioProcessorEditor::updateInputOutputMode()
{

    for(int i = 0; i  < this->filter->getTotalNumInputChannels(); i++){
        this->comInOutMode->addItem(String(i+1),  i+1);
    }
    
    this->comSourceSelectPos->clear();
    for(int i = 0; i  < this->filter->getNumSourceUsed(); i++){
        this->comSourceSelectPos->addItem(String(i+1), i+1);
    }
    
}

void SpatGrisAudioProcessorEditor::updateSelectSource()
{
    this->comSourceSelectPos->setSelectedId(this->filter->getSelectItem()->selectID+1, dontSendNotification);
    FPoint rayAngleS = this->filter->getRayAngleSource(this->filter->getSelectItem()->selectID);
    this->comSourceSelectRay->setText(String(rayAngleS.x,4), dontSendNotification);
    this->comSourceSelectAngle->setText(String(RadianToDegree(rayAngleS.y) ,4), dontSendNotification);
}

//==============================================================================

void SpatGrisAudioProcessorEditor::buttonClicked (Button *button)
{
    /*if(this->togLinkSurfaceOrPan == button){
        this->filter->setLinkSurface(this->togLinkSurfaceOrPan->getToggleState());
        
    }*/
    if(this->togLinkAzimSpan == button){
        this->filter->setLinkAzimuth(this->togLinkAzimSpan->getToggleState());
        
    }
    else if(this->togLinkElevSpan == button){
        this->filter->setLinkElevation(this->togLinkElevSpan->getToggleState());
        
    }
    else if(this->togLinkHeightSour == button){
        this->filter->setLinkHeight(this->togLinkHeightSour->getToggleState());
    }
    else if(this->togTrajRandSepare == button){
        this->filter->getTrajectory()->setRandSeparate(this->togTrajRandSepare->getToggleState());
        
    }
    else if(this->butReadyTrajectory == button){
        this->butReadyTrajectory->setToggleState(this->filter->getTrajectory()->getProcessTrajectory(), dontSendNotification);
        this->updateTrajectoryParam();
    }
    else if(this->butSourcePos == button){
        this->filter->getSourceMover()->setSourcesPosition((PositionSourceSpeaker)this->comSourcePos->getSelectedItemIndex());
        this->updateSelectSource();
    }
    else if(this->togOSCActive == button){
       this->filter->setOscOn(this->togOSCActive->getToggleState());
    }
    else {
        cout << "buttonClicked not found !" << newLine;
    }
}
void SpatGrisAudioProcessorEditor::sliderValueChanged (Slider *slider)
{
    
    /*if(this->sliSurfaceOrPan == slider){
        this->filter->setSurfaceValue(this->sliSurfaceOrPan->getValue());
        this->sliSurfaceOrPan->setTooltip("S:"+String(this->sliSurfaceOrPan->getValue(),2));
        
    }*/
    if(this->sliAzimSpan == slider){
        this->filter->setAzimuthValue(this->sliAzimSpan->getValue());
        this->sliAzimSpan->setTooltip("A:"+String(this->sliAzimSpan->getValue(),2));
        
    }
    else if(this->sliElevSpan == slider){
        this->filter->setElevationValue(this->sliElevSpan->getValue());
        this->sliElevSpan->setTooltip("E:"+String(this->sliElevSpan->getValue(),2));
        
    }
    else if(this->sliHeightSour == slider){
        this->filter->setHeightSValue(this->sliHeightSour->getValue());
        this->sliHeightSour->setTooltip("H:"+String(this->sliHeightSour->getValue(),2));
        
    }
    else if(this->sliSpeedTrajectory == slider){
        this->filter->getTrajectory()->setSpeed(this->sliSpeedTrajectory->getValue());
        
    }
    else if(this->sliTrajRandSpeed == slider){
        this->filter->getTrajectory()->setRandSpeed(this->sliTrajRandSpeed->getValue());
        
    }
    else {
        cout << "sliderValueChanged not found !" << newLine;
    }
}
void SpatGrisAudioProcessorEditor::comboBoxChanged (ComboBox* comboBox)
{
    if(this->comMouvement == comboBox){
        this->filter->getSourceMover()->setMouvementMode((MouvementMode)this->comMouvement->getSelectedItemIndex());
        
        
        
    }
    else if(this->comTypeTrajectory == comboBox){

        this->labTrajEllipseWidth->setVisible(false);
        this->texTrajEllipseWidth->setVisible(false);
        this->comTrajOneWayReturn->setVisible(false);
        this->labTrajRadAngEnd->setVisible(false);
        this->texTrajRadiusEnd->setVisible(false);
        this->texTrajAngleEnd->setVisible(false);
        this->butTrajSetEnd->setVisible(false);
        this->butTrajResetEnd->setVisible(false);
        this->labTrajPendDampe->setVisible(false);
        this->texTrajPendDampe->setVisible(false);
        this->labTrajPendDevia->setVisible(false);
        this->texTrajPendDevia->setVisible(false);
        this->labTrajRandSpeed->setVisible(false);
        this->sliTrajRandSpeed->setVisible(false);
        this->togTrajRandSepare->setVisible(false);
        
        switch ((TrajectoryType)this->comTypeTrajectory->getSelectedItemIndex()) {
                
            case Ellipse:
                this->labTrajEllipseWidth->setVisible(true);
                this->texTrajEllipseWidth->setVisible(true);
                break;
                
            case Spiral:
                this->comTrajOneWayReturn->setVisible(true);
                this->labTrajRadAngEnd->setVisible(true);
                this->texTrajRadiusEnd->setVisible(true);
                this->texTrajAngleEnd->setVisible(true);
                this->butTrajSetEnd->setVisible(true);
                this->butTrajResetEnd->setVisible(true);
                break;
                
            case Pendulum:
                this->comTrajOneWayReturn->setVisible(true);
                this->labTrajRadAngEnd->setVisible(true);
                this->texTrajRadiusEnd->setVisible(true);
                this->texTrajAngleEnd->setVisible(true);
                this->butTrajSetEnd->setVisible(true);
                this->butTrajResetEnd->setVisible(true);
                
                this->labTrajPendDampe->setVisible(true);
                this->texTrajPendDampe->setVisible(true);
                this->labTrajPendDevia->setVisible(true);
                this->texTrajPendDevia->setVisible(true);
                break;
                
            case RandomTraj:
                this->labTrajRandSpeed->setVisible(true);
                this->sliTrajRandSpeed->setVisible(true);
                this->togTrajRandSepare->setVisible(true);
                break;
            
            case RandomTarget:
                this->comTrajOneWayReturn->setVisible(true);
                this->togTrajRandSepare->setVisible(true);
                break;
            
            case FreeDrawing:
                this->comTrajOneWayReturn->setVisible(true);
                break;
                
            default:
                break;
        }
        this->filter->getTrajectory()->setTrajectoryType((TrajectoryType)this->comTypeTrajectory->getSelectedItemIndex());
        
    }
    else if(this->comTimeTrajectory == comboBox){
        this->filter->getTrajectory()->setInSeconds(!(this->comTimeTrajectory->getSelectedItemIndex()==1));
        
    }
    else if(this->comTrajOneWayReturn == comboBox){
        this->filter->getTrajectory()->setInOneWay(!(this->comTrajOneWayReturn->getSelectedItemIndex()==1));
    }
    else if(this->comSourceSelectPos == comboBox){
        this->filter->getSelectItem()->selecType = SelectedSource;
        this->filter->getSelectItem()->selectID = this->comSourceSelectPos->getSelectedId()-1;
        this->updateSelectSource();
    }
    
    else if(this->comTypeProcess == comboBox){
        this->filter->setTypeProcess((ProcessType)this->comTypeProcess->getSelectedItemIndex());
    }
}

void SpatGrisAudioProcessorEditor::textEditorFocusLost (TextEditor &textEditor)
{
    textEditorReturnKeyPressed(textEditor);
}
void SpatGrisAudioProcessorEditor::textEditorReturnKeyPressed (TextEditor &textEditor)
{
    if(this->texTimeTrajectory == &textEditor){
        this->filter->getTrajectory()->setTimeDuration(this->texTimeTrajectory->getText().getFloatValue());
        
    }else if(this->texCycleTrajectory == &textEditor){
        this->filter->getTrajectory()->setCycle(this->texCycleTrajectory->getText().getFloatValue());
        
    }else if(this->texTrajEllipseWidth == &textEditor){
        float w = GetValueInRange(this->texTrajEllipseWidth->getText().getFloatValue(), MinTrajWidthEllipse, MaxTrajWidthEllipse);
        this->filter->getTrajectory()->setEllipseWidth(w);
        this->texTrajEllipseWidth->setText(String(w,2), dontSendNotification);
        
    }else if(this->texTrajRadiusEnd == &textEditor){
        this->filter->getTrajectory()->setRadiusEnd(this->texTrajRadiusEnd->getText().getFloatValue());
    }
    else if(this->texTrajAngleEnd == &textEditor){
        this->filter->getTrajectory()->setAngleEnd(this->texTrajAngleEnd->getText().getFloatValue());
    }
    else if(this->texTrajPendDampe == &textEditor){
        this->filter->getTrajectory()->setPendDampening(this->texTrajPendDampe->getText().getFloatValue());
    }
    else if(this->texTrajPendDevia == &textEditor){
        this->filter->getTrajectory()->setPendDeviation(this->texTrajPendDevia->getText().getFloatValue());
    }
    else if(this->comSourceSelectRay == &textEditor || this->comSourceSelectAngle == &textEditor){
        float r = GetValueInRange(this->comSourceSelectRay->getText().getFloatValue(), 0.0f, RadiusMax);
        float a = GetValueInRange(this->comSourceSelectAngle->getText().getFloatValue(), 0.0f, AngleDegMax);
        this->filter->setPosRayAngSource(this->filter->getSelectItem()->selectID, r, a, false);
        this->comSourceSelectRay->setText(String(r,4), dontSendNotification);
        this->comSourceSelectAngle->setText(String(a ,4), dontSendNotification);
    }
}

void SpatGrisAudioProcessorEditor::timerCallback()
{
	this->spatFieldComp->repaint();
    
    if(this->filter->getTrajectory()->getProcessTrajectory()){
        this->progressBarTraject->setValue(this->filter->getTrajectory()->getProgressBar());
    }
}

void SpatGrisAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(this->grisFeel.getWinBackgroundColour());
}

void SpatGrisAudioProcessorEditor::resized()
{
    const int w = getWidth();
    const int h = getHeight();
    
    const int fieldWidth = w - (Margin + Margin + CenterColumnWidth + Margin + RightColumnWidth + Margin);
    const int fieldHeight = h - (Margin + Margin);
    int fieldSize = jmin(fieldWidth, fieldHeight);
    if (fieldSize < MinFieldSize){
        fieldSize = MinFieldSize;
    }
    int x = Margin + fieldSize + Margin + Margin;
    int y = Margin;
    
    //SpatComponent----------------------
    this->spatFieldComp->setBounds(Margin, Margin, fieldSize, fieldSize);
    this->spatFieldComp->resized(fieldSize);
    
    //BOX------------------------------------
    this->boxSourceParam->setBounds(x, y, CenterColumnWidth, 160);
    this->boxSourceParam->correctSize(CenterColumnWidth, 140);
    
    x += CenterColumnWidth + Margin + Margin;
    this->octTab->setBounds(x, y, w-(fieldSize+ CenterColumnWidth + (Margin * 7)), 160);
    //this->boxOutputParam->setBounds(x, y, w-(fieldSize+ CenterColumnWidth + (Margin * 7)), 160);
    //this->boxOutputParam->correctSize(((unsigned int )this->vecLevelOut.size()*(SizeWidthLevelComp))+4, 130);
    
    x = Margin + fieldSize + Margin + Margin;
    this->boxTrajectory->setBounds(x, 170, w-(fieldSize + (Margin * 5)), 200);
    this->boxTrajectory->correctSize(510, 170);    //w-(fieldSize + (Margin * 5))
    
    //OctTabbedComponent-----------------------
    //this->octTab->setBounds(x, 170+210, w-(fieldSize + (Margin * 4)), h - (170+200+(Margin*6)) );

    
    this->resizer->setBounds (w - 16, h - 16, 16, 16);

}
//==============================================================================
