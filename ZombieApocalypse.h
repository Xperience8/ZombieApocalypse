// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

#include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionStimuliSourceComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Hearing.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Sight.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Hearing.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Runtime/AIModule/Classes/Perception/AISenseEvent_Hearing.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#define ECC_BULLET ECollisionChannel::ECC_GameTraceChannel1
#define ECC_USABLE ECollisionChannel::ECC_GameTraceChannel2
#define ECC_QUEST ECollisionChannel::ECC_GameTraceChannel3

#define ZOMBIE_HEAD_SURFACE_TYPE EPhysicalSurface::SurfaceType1
#define ZOMBIE_BODY_SURFACE_TYPE EPhysicalSurface::SurfaceType2
#define ZOMBIE_LIMB_SURFACE_TYPE EPhysicalSurface::SurfaceType3