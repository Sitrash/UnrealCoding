// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/MenuGameModeBase.h"
#include "Menu/MenuPlayerController.h"
#include "Menu/UI/MenuHUD.h"

AMenuGameModeBase::AMenuGameModeBase()
{
    PlayerControllerClass = AMenuPlayerController::StaticClass();
    HUDClass = AMenuHUD::StaticClass();
}
