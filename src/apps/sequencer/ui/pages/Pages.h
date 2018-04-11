#pragma once

#include "Config.h"

#include "BpmPage.h"
#include "BusyPage.h"
#include "ClockSetupPage.h"
#include "ConfirmationPage.h"
#include "ContextMenuPage.h"
#include "CurveSequenceEditPage.h"
#include "CurveSequencePage.h"
#include "LayoutPage.h"
#include "MonitorPage.h"
#include "NoteSequenceEditPage.h"
#include "NoteSequencePage.h"
#include "PatternPage.h"
#include "PerformerPage.h"
#include "ProjectPage.h"
#include "ProjectSelectPage.h"
#include "RoutePage.h"
#include "RoutingPage.h"
#include "TextInputPage.h"
#include "TopPage.h"
#include "TrackPage.h"
#include "UserScalePage.h"

#include "EuclideanPage.h"

#ifdef CONFIG_ENABLE_INTRO
#include "IntroPage.h"
#endif
#ifdef CONFIG_ENABLE_ASTEROIDS
#include "AsteroidsPage.h"
#endif

struct Pages {
    TopPage top;
    // main pages
    ProjectPage project;
    LayoutPage layout;
    TrackPage track;
    NoteSequencePage noteSequence;
    CurveSequencePage curveSequence;
    NoteSequenceEditPage noteSequenceEdit;
    CurveSequenceEditPage curveSequenceEdit;
    PatternPage pattern;
    PerformerPage performer;
    RoutingPage routing;
    RoutePage route;
    UserScalePage userScale;
    // modal pages
    BpmPage bpm;
    ClockSetupPage clockSetup;
    MonitorPage monitor;
    ProjectSelectPage projectSelect;
    ContextMenuPage contextMenu;
    // helper pages
    TextInputPage textInput;
    ConfirmationPage confirmation;
    BusyPage busy;
    // misc
    EuclideanPage euclidean;

#ifdef CONFIG_ENABLE_INTRO
    IntroPage intro;
#endif
#ifdef CONFIG_ENABLE_ASTEROIDS
    AsteroidsPage asteroids;
#endif

    Pages(PageManager &manager, PageContext &context) :
        top(manager, context),
        // main pages
        project(manager, context),
        layout(manager, context),
        track(manager, context),
        noteSequence(manager, context),
        curveSequence(manager, context),
        noteSequenceEdit(manager, context),
        curveSequenceEdit(manager, context),
        pattern(manager, context),
        performer(manager, context),
        routing(manager, context),
        route(manager, context),
        userScale(manager, context),
        // modal pages
        bpm(manager, context),
        clockSetup(manager, context),
        monitor(manager, context),
        projectSelect(manager, context),
        contextMenu(manager, context),
        // helper pages
        textInput(manager, context),
        confirmation(manager, context),
        busy(manager, context),
        // misc
        euclidean(manager, context)
#ifdef CONFIG_ENABLE_INTRO
        ,intro(manager, context)
#endif
#ifdef CONFIG_ENABLE_ASTEROIDS
        ,asteroids(manager, context)
#endif
    {}
};

