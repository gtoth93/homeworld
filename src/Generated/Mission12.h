//
//  Mission12.h
//
//  Finite state machine for "Mission12" mission
//
//  Copyright (C) 1998 Relic Entertainment Inc.
//  All rights reserved
//
//  This code was autogenerated from Mission12.kp by KAS2C Version 2.05
//


#ifndef __Mission12_H
#define __Mission12_H


//
//  types and exposed game functions
//



//
//  FSM prototypes
//
void Init_Mission12_FleetIntel(void);
void Watch_Mission12_FleetIntel(void);
void Init_Mission12_FleetIntel_FCIntro(void);
void Watch_Mission12_FleetIntel_FCIntro(void);
void Init_Mission12_FleetIntel_FICaughtInField(void);
void Watch_Mission12_FleetIntel_FICaughtInField(void);
void Init_Mission12_FleetIntel_FIUnderAttack(void);
void Watch_Mission12_FleetIntel_FIUnderAttack(void);
void Init_Mission12_FleetIntel_FIMissileTechReady(void);
void Watch_Mission12_FleetIntel_FIMissileTechReady(void);
void Init_Mission12_FleetIntel_FINewFighterTech(void);
void Watch_Mission12_FleetIntel_FINewFighterTech(void);
void Init_Mission12_FleetIntel_FocusOnDefector(void);
void Watch_Mission12_FleetIntel_FocusOnDefector(void);
void Init_Mission12_FleetIntel_DefectorHelpMe(void);
void Watch_Mission12_FleetIntel_DefectorHelpMe(void);
void Init_Mission12_FleetIntel_DefectorHelpMe2(void);
void Watch_Mission12_FleetIntel_DefectorHelpMe2(void);
void Init_Mission12_FleetIntel_FIHelpDefector(void);
void Watch_Mission12_FleetIntel_FIHelpDefector(void);
void Init_Mission12_FleetIntel_FIDefectorOnBoard(void);
void Watch_Mission12_FleetIntel_FIDefectorOnBoard(void);
void Init_Mission12_FleetIntel_FIIdle(void);
void Watch_Mission12_FleetIntel_FIIdle(void);
void Init_Mission12_TestTeam(void);
void Watch_Mission12_TestTeam(void);
void Init_Mission12_TestTeam_Hide(void);
void Watch_Mission12_TestTeam_Hide(void);
void Init_Mission12_GravWells(void);
void Watch_Mission12_GravWells(void);
void Init_Mission12_GravWells_Move(void);
void Watch_Mission12_GravWells_Move(void);
void Init_Mission12_GravWells_Wait(void);
void Watch_Mission12_GravWells_Wait(void);
void Init_Mission12_GravWells_Wait2(void);
void Watch_Mission12_GravWells_Wait2(void);
void Init_Mission12_GravWells_GravityOn(void);
void Watch_Mission12_GravWells_GravityOn(void);
void Init_Mission12_GravWells_GravityOff(void);
void Watch_Mission12_GravWells_GravityOff(void);
void Init_Mission12_GravWellSupport(void);
void Watch_Mission12_GravWellSupport(void);
void Init_Mission12_GravWellSupport_Choose(void);
void Watch_Mission12_GravWellSupport_Choose(void);
void Init_Mission12_GravWellSupport_ServiceGravWell(void);
void Watch_Mission12_GravWellSupport_ServiceGravWell(void);
void Init_Mission12_GravWellSupport_ServiceCarrier(void);
void Watch_Mission12_GravWellSupport_ServiceCarrier(void);
void Init_Mission12_GravWellSupport_ServiceCruiser(void);
void Watch_Mission12_GravWellSupport_ServiceCruiser(void);
void Init_Mission12_GravWellSupport_GiveToAI(void);
void Watch_Mission12_GravWellSupport_GiveToAI(void);
void Init_Mission12_GuardGravWells(void);
void Watch_Mission12_GuardGravWells(void);
void Init_Mission12_GuardGravWells_Move(void);
void Watch_Mission12_GuardGravWells_Move(void);
void Init_Mission12_GuardGravWells_AttackFleet(void);
void Watch_Mission12_GuardGravWells_AttackFleet(void);
void Init_Mission12_GuardGravWells_AttackMothership(void);
void Watch_Mission12_GuardGravWells_AttackMothership(void);
void Init_Mission12_GuardGravWells_NullState(void);
void Watch_Mission12_GuardGravWells_NullState(void);
void Init_Mission12_AIShips(void);
void Watch_Mission12_AIShips(void);
void Init_Mission12_AIShips_GiveToAI(void);
void Watch_Mission12_AIShips_GiveToAI(void);
void Init_Mission12_Carrier(void);
void Watch_Mission12_Carrier(void);
void Init_Mission12_Carrier_ACT1Patrol(void);
void Watch_Mission12_Carrier_ACT1Patrol(void);
void Init_Mission12_Carrier_NullState(void);
void Watch_Mission12_Carrier_NullState(void);
void Init_Mission12_GuardCarrier(void);
void Watch_Mission12_GuardCarrier(void);
void Init_Mission12_GuardCarrier_Guard(void);
void Watch_Mission12_GuardCarrier_Guard(void);
void Init_Mission12_GuardCarrier_GuardCruiser(void);
void Watch_Mission12_GuardCarrier_GuardCruiser(void);
void Init_Mission12_GuardCarrier_AllDeadWaitForShips(void);
void Watch_Mission12_GuardCarrier_AllDeadWaitForShips(void);
void Init_Mission12_GuardCarrier_Kamikaze(void);
void Watch_Mission12_GuardCarrier_Kamikaze(void);
void Init_Mission12_GuardCarrier_Dock(void);
void Watch_Mission12_GuardCarrier_Dock(void);
void Init_Mission12_GuardCarrier_Launch(void);
void Watch_Mission12_GuardCarrier_Launch(void);
void Init_Mission12_GuardCarrier_NullState(void);
void Watch_Mission12_GuardCarrier_NullState(void);
void Init_Mission12_Cruiser(void);
void Watch_Mission12_Cruiser(void);
void Init_Mission12_Cruiser_Hide(void);
void Watch_Mission12_Cruiser_Hide(void);
void Init_Mission12_Cruiser_ComeOnIn(void);
void Watch_Mission12_Cruiser_ComeOnIn(void);
void Init_Mission12_Cruiser_AttackNearby(void);
void Watch_Mission12_Cruiser_AttackNearby(void);
void Init_Mission12_Cruiser_AttackFleet(void);
void Watch_Mission12_Cruiser_AttackFleet(void);
void Init_Mission12_Cruiser_AttackMothership(void);
void Watch_Mission12_Cruiser_AttackMothership(void);
void Init_Mission12_Cruiser_NullState(void);
void Watch_Mission12_Cruiser_NullState(void);
void Init_Mission12_GuardCruiser(void);
void Watch_Mission12_GuardCruiser(void);
void Init_Mission12_GuardCruiser_Hide(void);
void Watch_Mission12_GuardCruiser_Hide(void);
void Init_Mission12_GuardCruiser_Guard(void);
void Watch_Mission12_GuardCruiser_Guard(void);
void Init_Mission12_GuardCruiser_GuardCarrier(void);
void Watch_Mission12_GuardCruiser_GuardCarrier(void);
void Init_Mission12_GuardCruiser_Kamikaze(void);
void Watch_Mission12_GuardCruiser_Kamikaze(void);
void Init_Mission12_GuardCruiser_Dock(void);
void Watch_Mission12_GuardCruiser_Dock(void);
void Init_Mission12_GuardCruiser_Launch(void);
void Watch_Mission12_GuardCruiser_Launch(void);
void Init_Mission12_GuardCruiser_NullState(void);
void Watch_Mission12_GuardCruiser_NullState(void);
void Init_Mission12_STRInterceptors(void);
void Watch_Mission12_STRInterceptors(void);
void Init_Mission12_STRInterceptors_WaitForLaunchOrder(void);
void Watch_Mission12_STRInterceptors_WaitForLaunchOrder(void);
void Init_Mission12_STRInterceptors_Build(void);
void Watch_Mission12_STRInterceptors_Build(void);
void Init_Mission12_STRInterceptors_Decide(void);
void Watch_Mission12_STRInterceptors_Decide(void);
void Init_Mission12_STRInterceptors_AttackFleetAtMothership(void);
void Watch_Mission12_STRInterceptors_AttackFleetAtMothership(void);
void Init_Mission12_STRInterceptors_OnTheMoveToMothership(void);
void Watch_Mission12_STRInterceptors_OnTheMoveToMothership(void);
void Init_Mission12_STRInterceptors_AttackNearby(void);
void Watch_Mission12_STRInterceptors_AttackNearby(void);
void Init_Mission12_STRInterceptors_AttackRestOfWorld(void);
void Watch_Mission12_STRInterceptors_AttackRestOfWorld(void);
void Init_Mission12_STRInterceptors_OnTheMoveToRestOfWorld(void);
void Watch_Mission12_STRInterceptors_OnTheMoveToRestOfWorld(void);
void Init_Mission12_STRInterceptors_GuardCarrier(void);
void Watch_Mission12_STRInterceptors_GuardCarrier(void);
void Init_Mission12_STRInterceptors_Dock(void);
void Watch_Mission12_STRInterceptors_Dock(void);
void Init_Mission12_STRInterceptors_Launch(void);
void Watch_Mission12_STRInterceptors_Launch(void);
void Init_Mission12_STRInterceptors_NullState(void);
void Watch_Mission12_STRInterceptors_NullState(void);
void Init_Mission12_STRBombers(void);
void Watch_Mission12_STRBombers(void);
void Init_Mission12_STRBombers_WaitForLaunchOrder(void);
void Watch_Mission12_STRBombers_WaitForLaunchOrder(void);
void Init_Mission12_STRBombers_Build(void);
void Watch_Mission12_STRBombers_Build(void);
void Init_Mission12_STRBombers_Decide(void);
void Watch_Mission12_STRBombers_Decide(void);
void Init_Mission12_STRBombers_Guard(void);
void Watch_Mission12_STRBombers_Guard(void);
void Init_Mission12_STRBombers_AttackFleetAtMothership(void);
void Watch_Mission12_STRBombers_AttackFleetAtMothership(void);
void Init_Mission12_STRBombers_OnTheMoveToMothership(void);
void Watch_Mission12_STRBombers_OnTheMoveToMothership(void);
void Init_Mission12_STRBombers_AttackNearby(void);
void Watch_Mission12_STRBombers_AttackNearby(void);
void Init_Mission12_STRBombers_AttackRestOfWorld(void);
void Watch_Mission12_STRBombers_AttackRestOfWorld(void);
void Init_Mission12_STRBombers_OnTheMoveToRestOfWorld(void);
void Watch_Mission12_STRBombers_OnTheMoveToRestOfWorld(void);
void Init_Mission12_STRBombers_GuardCarrier(void);
void Watch_Mission12_STRBombers_GuardCarrier(void);
void Init_Mission12_STRBombers_Dock(void);
void Watch_Mission12_STRBombers_Dock(void);
void Init_Mission12_STRBombers_Launch(void);
void Watch_Mission12_STRBombers_Launch(void);
void Init_Mission12_STRBombers_NullState(void);
void Watch_Mission12_STRBombers_NullState(void);
void Init_Mission12_STRMG(void);
void Watch_Mission12_STRMG(void);
void Init_Mission12_STRMG_WaitForLaunchOrder(void);
void Watch_Mission12_STRMG_WaitForLaunchOrder(void);
void Init_Mission12_STRMG_Build(void);
void Watch_Mission12_STRMG_Build(void);
void Init_Mission12_STRMG_Decide(void);
void Watch_Mission12_STRMG_Decide(void);
void Init_Mission12_STRMG_Guard(void);
void Watch_Mission12_STRMG_Guard(void);
void Init_Mission12_STRMG_AttackFleetAtMothership(void);
void Watch_Mission12_STRMG_AttackFleetAtMothership(void);
void Init_Mission12_STRMG_OnTheMoveToMothership(void);
void Watch_Mission12_STRMG_OnTheMoveToMothership(void);
void Init_Mission12_STRMG_AttackNearby(void);
void Watch_Mission12_STRMG_AttackNearby(void);
void Init_Mission12_STRMG_AttackRestOfWorld(void);
void Watch_Mission12_STRMG_AttackRestOfWorld(void);
void Init_Mission12_STRMG_OnTheMoveToRestOfWorld(void);
void Watch_Mission12_STRMG_OnTheMoveToRestOfWorld(void);
void Init_Mission12_STRMG_GuardCarrier(void);
void Watch_Mission12_STRMG_GuardCarrier(void);
void Init_Mission12_STRMG_Dock(void);
void Watch_Mission12_STRMG_Dock(void);
void Init_Mission12_STRMG_Launch(void);
void Watch_Mission12_STRMG_Launch(void);
void Init_Mission12_STRMG_NullState(void);
void Watch_Mission12_STRMG_NullState(void);
void Init_Mission12_STRGW(void);
void Watch_Mission12_STRGW(void);
void Init_Mission12_STRGW_WaitForLaunchOrder(void);
void Watch_Mission12_STRGW_WaitForLaunchOrder(void);
void Init_Mission12_STRGW_Build(void);
void Watch_Mission12_STRGW_Build(void);
void Init_Mission12_STRGW_Decide(void);
void Watch_Mission12_STRGW_Decide(void);
void Init_Mission12_STRGW_Guard(void);
void Watch_Mission12_STRGW_Guard(void);
void Init_Mission12_STRGW_GuardGravityOn(void);
void Watch_Mission12_STRGW_GuardGravityOn(void);
void Init_Mission12_STRGW_GuardGravityOff(void);
void Watch_Mission12_STRGW_GuardGravityOff(void);
void Init_Mission12_STRGW_AttackFleetAtMothership(void);
void Watch_Mission12_STRGW_AttackFleetAtMothership(void);
void Init_Mission12_STRGW_OnTheMoveToMothership(void);
void Watch_Mission12_STRGW_OnTheMoveToMothership(void);
void Init_Mission12_STRGW_AttackNearby(void);
void Watch_Mission12_STRGW_AttackNearby(void);
void Init_Mission12_STRGW_AttackRestOfWorld(void);
void Watch_Mission12_STRGW_AttackRestOfWorld(void);
void Init_Mission12_STRGW_OnTheMoveToRestOfWorld(void);
void Watch_Mission12_STRGW_OnTheMoveToRestOfWorld(void);
void Init_Mission12_STRGW_GuardCarrier(void);
void Watch_Mission12_STRGW_GuardCarrier(void);
void Init_Mission12_STRGW_NullState(void);
void Watch_Mission12_STRGW_NullState(void);
void Init_Mission12_STRCG(void);
void Watch_Mission12_STRCG(void);
void Init_Mission12_STRCG_WaitForLaunchOrder(void);
void Watch_Mission12_STRCG_WaitForLaunchOrder(void);
void Init_Mission12_STRCG_Build(void);
void Watch_Mission12_STRCG_Build(void);
void Init_Mission12_STRCG_Decide(void);
void Watch_Mission12_STRCG_Decide(void);
void Init_Mission12_STRCG_Guard(void);
void Watch_Mission12_STRCG_Guard(void);
void Init_Mission12_STRCG_GuardCloakityOn(void);
void Watch_Mission12_STRCG_GuardCloakityOn(void);
void Init_Mission12_STRCG_GuardCloakityOff(void);
void Watch_Mission12_STRCG_GuardCloakityOff(void);
void Init_Mission12_STRCG_AttackFleetAtMothership(void);
void Watch_Mission12_STRCG_AttackFleetAtMothership(void);
void Init_Mission12_STRCG_OnTheMoveToMothership(void);
void Watch_Mission12_STRCG_OnTheMoveToMothership(void);
void Init_Mission12_STRCG_AttackNearby(void);
void Watch_Mission12_STRCG_AttackNearby(void);
void Init_Mission12_STRCG_AttackRestOfWorld(void);
void Watch_Mission12_STRCG_AttackRestOfWorld(void);
void Init_Mission12_STRCG_OnTheMoveToRestOfWorld(void);
void Watch_Mission12_STRCG_OnTheMoveToRestOfWorld(void);
void Init_Mission12_STRCG_GuardCarrier(void);
void Watch_Mission12_STRCG_GuardCarrier(void);
void Init_Mission12_STRCG_NullState(void);
void Watch_Mission12_STRCG_NullState(void);
void Init_Mission12_Defector(void);
void Watch_Mission12_Defector(void);
void Init_Mission12_Defector_HyperspaceOut(void);
void Watch_Mission12_Defector_HyperspaceOut(void);
void Init_Mission12_Defector_HyperspaceIn(void);
void Watch_Mission12_Defector_HyperspaceIn(void);
void Init_Mission12_Defector_GetToPlayerMothership(void);
void Watch_Mission12_Defector_GetToPlayerMothership(void);
void Init_Mission12_Defector_WaitToHyperspaceOut(void);
void Watch_Mission12_Defector_WaitToHyperspaceOut(void);
void Init_Mission12_Defector_HyperspaceOut2(void);
void Watch_Mission12_Defector_HyperspaceOut2(void);
void Init_Mission12_Defector_NullState(void);
void Watch_Mission12_Defector_NullState(void);
void Init_Mission12_ChaseDefector(void);
void Watch_Mission12_ChaseDefector(void);
void Init_Mission12_ChaseDefector_WaitToHyperspace(void);
void Watch_Mission12_ChaseDefector_WaitToHyperspace(void);
void Init_Mission12_ChaseDefector_HyperspaceOut(void);
void Watch_Mission12_ChaseDefector_HyperspaceOut(void);
void Init_Mission12_ChaseDefector_WaitToAttack(void);
void Watch_Mission12_ChaseDefector_WaitToAttack(void);
void Init_Mission12_ChaseDefector_AttackDefector(void);
void Watch_Mission12_ChaseDefector_AttackDefector(void);
void Init_Mission12_ChaseDefector_MoveAttackDefector(void);
void Watch_Mission12_ChaseDefector_MoveAttackDefector(void);
void Init_Mission12_ChaseDefector_AttackNearby(void);
void Watch_Mission12_ChaseDefector_AttackNearby(void);
void Init_Mission12_ChaseDefector_Dock(void);
void Watch_Mission12_ChaseDefector_Dock(void);
void Init_Mission12_ChaseDefector_WaitToLaunch(void);
void Watch_Mission12_ChaseDefector_WaitToLaunch(void);
void Init_Mission12_ChaseDefector_LaunchInts(void);
void Watch_Mission12_ChaseDefector_LaunchInts(void);
void Init_Mission12_ChaseDefector_NullState(void);
void Watch_Mission12_ChaseDefector_NullState(void);
void Init_Mission12_StrikeTeamDispatch(void);
void Watch_Mission12_StrikeTeamDispatch(void);
void Init_Mission12_StrikeTeamDispatch_WaitForACT2(void);
void Watch_Mission12_StrikeTeamDispatch_WaitForACT2(void);
void Init_Mission12_StrikeTeamDispatch_BeginSTRInterceptors(void);
void Watch_Mission12_StrikeTeamDispatch_BeginSTRInterceptors(void);
void Init_Mission12_StrikeTeamDispatch_BeginSTRBombers(void);
void Watch_Mission12_StrikeTeamDispatch_BeginSTRBombers(void);
void Init_Mission12_StrikeTeamDispatch_BeginSTRMGs(void);
void Watch_Mission12_StrikeTeamDispatch_BeginSTRMGs(void);
void Init_Mission12_StrikeTeamDispatch_BeginSTRGWs(void);
void Watch_Mission12_StrikeTeamDispatch_BeginSTRGWs(void);
void Init_Mission12_StrikeTeamDispatch_BeginSTRCGs(void);
void Watch_Mission12_StrikeTeamDispatch_BeginSTRCGs(void);
void Init_Mission12_StrikeTeamDispatch_NullState(void);
void Watch_Mission12_StrikeTeamDispatch_NullState(void);
void Init_Mission12(void);
void Watch_Mission12(void);


#endif
