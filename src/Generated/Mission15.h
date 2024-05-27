//
//  Mission15.h
//
//  Finite state machine for "Mission15" mission
//
//  Copyright (C) 1998 Relic Entertainment Inc.
//  All rights reserved
//
//  This code was autogenerated from Mission15.kp by KAS2C Version 2.05
//


#ifndef __Mission15_H
#define __Mission15_H


//
//  types and exposed game functions
//



//
//  FSM prototypes
//
void Init_Mission15_Dead(void);
void Watch_Mission15_Dead(void);
void Init_Mission15_Events(void);
void Watch_Mission15_Events(void);
void Init_Mission15_Events_PreNIS(void);
void Watch_Mission15_Events_PreNIS(void);
void Init_Mission15_Events_PlayNIS(void);
void Watch_Mission15_Events_PlayNIS(void);
void Init_Mission15_Events_FIIntro1(void);
void Watch_Mission15_Events_FIIntro1(void);
void Init_Mission15_Events_FIIntroLBXIn(void);
void Watch_Mission15_Events_FIIntroLBXIn(void);
void Init_Mission15_Events_FIIntro2(void);
void Watch_Mission15_Events_FIIntro2(void);
void Init_Mission15_Events_FIIntroLBXOut(void);
void Watch_Mission15_Events_FIIntroLBXOut(void);
void Init_Mission15_Events_Countdown(void);
void Watch_Mission15_Events_Countdown(void);
void Init_Mission15_Events_EvaluateWinner(void);
void Watch_Mission15_Events_EvaluateWinner(void);
void Init_Mission15_HeadShot(void);
void Watch_Mission15_HeadShot(void);
void Init_Mission15_HeadShot_Moving(void);
void Watch_Mission15_HeadShot_Moving(void);
void Init_Mission15_Protection(void);
void Watch_Mission15_Protection(void);
void Init_Mission15_Protection_Guarding(void);
void Watch_Mission15_Protection_Guarding(void);
void Init_Mission15_Protection_Retaliate(void);
void Watch_Mission15_Protection_Retaliate(void);
void Init_Mission15_Protection_KillMShip(void);
void Watch_Mission15_Protection_KillMShip(void);
void Init_Mission15_Harrass(void);
void Watch_Mission15_Harrass(void);
void Init_Mission15_Harrass_Bother(void);
void Watch_Mission15_Harrass_Bother(void);
void Init_Mission15_Harrass_Guard(void);
void Watch_Mission15_Harrass_Guard(void);
void Init_Mission15_Harrass_KillMShip(void);
void Watch_Mission15_Harrass_KillMShip(void);
void Init_Mission15_WaitForCountDown(void);
void Watch_Mission15_WaitForCountDown(void);
void Init_Mission15(void);
void Watch_Mission15(void);


#endif
