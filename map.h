//
// Created by SamuelCai on 2023/3/26.
//

#ifndef CODECRAFTSDK_MAP_H
#define CODECRAFTSDK_MAP_H

#include "arguments.h"
#include "point.h"

inline Argument load_args(Point x)
{
    Argument args;
    // TODO: compare map by comparing map string
    if (x == Point{24.75, 49.25})
    {
        args.ANTICOL_NEGLECT_R = 0.23957960991794727;
        args.ANTICOL_ALERT_R = 6.578759536194944;
        args.ANTICOL_WHEEL_K = 1.0;
        args.ANTICOL_FLEX_WHEEL = true;
        args.ANTICOL_WEIGHT_A = 5.255566623178697;
        args.ANTICOL_WEIGHT_B = 1.7914091554704017;
        args.DRIVING_P_ERROR_CORRECTION = 0.3;
        args.DRIVING_SPEEDDOWN_A = 24.0;
        args.DRIVING_SPEEDDOWN_B = -0.5;
        args.DRIVING_TURN_BREAK = 0.5;
        args.DRIVING_WEIGHT_A = 33.7574946698258;
        args.DRIVING_WEIGHT_B = 1.2383027145290981;
        args.DRIVING_THRESHOLD = 1.0;
        args.IF_ALREADY_PRODUCING = 17.0;
        args.IF_NOTHING_INSIDE = 11.0;
        args.AVERAGING_456 = 4.0;
        args.DONT_FILL_9_WORKBENCH = 6.0;
        args.AVOIDING_COLLISION = 1.7;
    } else if (x == Point{0.75, 49.25})
    {
        args.ANTICOL_NEGLECT_R = 1.6420353109039447;
        args.ANTICOL_ALERT_R = 12.530216545146402;
        args.ANTICOL_WHEEL_K = 1.997704906148585;
        args.ANTICOL_FLEX_WHEEL = true;
        args.ANTICOL_WEIGHT_A = 3.5157264628606244;
        args.ANTICOL_WEIGHT_B = 1.9726861734071066;
        args.DRIVING_P_ERROR_CORRECTION = 0.29944847574620864;
        args.DRIVING_SPEEDDOWN_A = 21.067356535455477;
        args.DRIVING_SPEEDDOWN_B = -5.584823114601609;
        args.DRIVING_TURN_BREAK = 0.7866997346553299;
        args.DRIVING_WEIGHT_A = 11.242537390628794;
        args.DRIVING_WEIGHT_B = 0.02221901961262164;
        args.DRIVING_THRESHOLD = 1.4957767556765207;
        args.IF_ALREADY_PRODUCING = 20.749978002564745;
        args.IF_NOTHING_INSIDE = 3.089696441230803;
        args.AVERAGING_456 = 3.433518168290904;
        args.DONT_FILL_9_WORKBENCH = 1.1739647036324201;
        args.AVOIDING_COLLISION = 1.4155358190352905;
    } else if (x == Point{23.25, 49.25})
    {
        args.ANTICOL_NEGLECT_R = 2.203783069775898;
        args.ANTICOL_ALERT_R = 13.812000031011042;
        args.ANTICOL_WHEEL_K = 0.35968251361356995;
        args.ANTICOL_FLEX_WHEEL = true;
        args.ANTICOL_WEIGHT_A = 6.2017682006934765;
        args.ANTICOL_WEIGHT_B = 0.6651048979452713;
        args.DRIVING_P_ERROR_CORRECTION = 0.14944377740615733;
        args.DRIVING_SPEEDDOWN_A = 21.70978516195917;
        args.DRIVING_SPEEDDOWN_B = -4.2406390249286;
        args.DRIVING_TURN_BREAK = 0.3393069004844464;
        args.DRIVING_WEIGHT_A = 33.75286737413469;
        args.DRIVING_WEIGHT_B = 1.3667407063360713;
        args.DRIVING_THRESHOLD = 0.9399129294407225;
        args.IF_ALREADY_PRODUCING = 13.76818505476071;
        args.IF_NOTHING_INSIDE = 2.179456693717632;
        args.AVERAGING_456 = 2.9403897795274343;
        args.DONT_FILL_9_WORKBENCH = 5.332738355088549;
        args.AVOIDING_COLLISION = 0.41454751085306457;
    } else if (x == Point{24.75, 46.25})
    {
        args.ANTICOL_NEGLECT_R = 1.7769706776343717;
        args.ANTICOL_ALERT_R = 8.25269751426907;
        args.ANTICOL_WHEEL_K = 1.4315307414300629;
        args.ANTICOL_FLEX_WHEEL = true;
        args.ANTICOL_WEIGHT_A = 1.0381780599300932;
        args.ANTICOL_WEIGHT_B = 1.2359315632139416;
        args.DRIVING_P_ERROR_CORRECTION = 0.28470274599424544;
        args.DRIVING_SPEEDDOWN_A = 23.67124924857717;
        args.DRIVING_SPEEDDOWN_B = -2.7742227989128083;
        args.DRIVING_TURN_BREAK = 0.37866581894636575;
        args.DRIVING_WEIGHT_A = 5.626657105495406;
        args.DRIVING_WEIGHT_B = 1.8525973636225481;
        args.DRIVING_THRESHOLD = 1.304694666010485;
        args.IF_ALREADY_PRODUCING = 9.498615181337625;
        args.IF_NOTHING_INSIDE = 12.69915057680902;
        args.AVERAGING_456 = 3.504698077126747;
        args.DONT_FILL_9_WORKBENCH = 3.49603681523397;
        args.AVOIDING_COLLISION = 1.1749533703482933;
    } else
    {
        args.ANTICOL_NEGLECT_R = 0.9806788456239878;
        args.ANTICOL_ALERT_R = 12.036732986121907;
        args.ANTICOL_WHEEL_K = 0.7141583375918185;
        args.ANTICOL_FLEX_WHEEL = true;
        args.ANTICOL_WEIGHT_A = 6.198009444731479;
        args.ANTICOL_WEIGHT_B = 0.2607339058329514;
        args.DRIVING_P_ERROR_CORRECTION = 0.26040513136804144;
        args.DRIVING_SPEEDDOWN_A = 16.804086975362935;
        args.DRIVING_SPEEDDOWN_B = -1.2897034982798041;
        args.DRIVING_TURN_BREAK = 0.5226281746137855;
        args.DRIVING_WEIGHT_A = 30.983172409547485;
        args.DRIVING_WEIGHT_B = 0.8758114513044636;
        args.DRIVING_THRESHOLD = 1.483720038130667;
        args.IF_ALREADY_PRODUCING = 9.53713702506606;
        args.IF_NOTHING_INSIDE = 8.566686164536742;
        args.AVERAGING_456 = 4.0;
        args.DONT_FILL_9_WORKBENCH = 2.614597672335828;
        args.AVOIDING_COLLISION = 0.7271317046011986;
    }
    return args;
}

#endif//CODECRAFTSDK_MAP_H
