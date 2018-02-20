#pragma once

#include "Config.h"

#include "BpmPage.h"
#include "BusyPage.h"
#include "ClockSetupPage.h"
#include "ConfirmationPage.h"
#include "CurveSequencePage.h"
#include "CurveSequenceSetupPage.h"
#include "MonitorPage.h"
#include "NoteSequencePage.h"
#include "NoteSequenceSetupPage.h"
#include "PatternPage.h"
#include "PerformerPage.h"
#include "ProjectPage.h"
#include "ProjectSelectPage.h"
#include "TextInputPage.h"
#include "TopPage.h"
#include "TrackSetupPage.h"

#ifdef CONFIG_ENABLE_ASTEROIDS
#include "AsteroidsPage.h"
#endif

struct Pages {
    TopPage top;
    // main pages
    ProjectPage project;
    PatternPage pattern;
    TrackSetupPage trackSetup;
    NoteSequencePage noteSequence;
    CurveSequencePage curveSequence;
    NoteSequenceSetupPage noteSequenceSetup;
    CurveSequenceSetupPage curveSequenceSetup;
    PerformerPage performer;
    // modal pages
    BpmPage bpm;
    ClockSetupPage clockSetup;
    MonitorPage monitor;
    ProjectSelectPage projectSelect;
    // helper pages
    TextInputPage textInput;
    ConfirmationPage confirmation;
    BusyPage busy;

#ifdef CONFIG_ENABLE_ASTEROIDS
    AsteroidsPage asteroids;
#endif

    Pages(PageManager &manager, PageContext &context) :
        top(manager, context),
        // main pages
        project(manager, context),
        pattern(manager, context),
        trackSetup(manager, context),
        noteSequence(manager, context),
        curveSequence(manager, context),
        noteSequenceSetup(manager, context),
        curveSequenceSetup(manager, context),
        performer(manager, context),
        // modal pages
        bpm(manager, context),
        clockSetup(manager, context),
        monitor(manager, context),
        projectSelect(manager, context),
        // helper pages
        textInput(manager, context),
        confirmation(manager, context),
        busy(manager, context)
#ifdef CONFIG_ENABLE_ASTEROIDS
        ,asteroids(manager, context)
#endif
    {}
};

