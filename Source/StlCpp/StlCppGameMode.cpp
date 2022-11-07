// Fill out your copyright notice in the Description page of Project Settings.


#include "StlCppGameMode.h"
#include "StlCppCharacter.h"
#include "StlCppPlayerController.h"

AStlCppGameMode::AStlCppGameMode()
{
	DefaultPawnClass = AStlCppCharacter::StaticClass();
	PlayerControllerClass = AStlCppPlayerController::StaticClass();

	
}