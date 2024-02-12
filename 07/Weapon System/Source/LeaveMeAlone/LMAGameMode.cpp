// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "LMAGameMode.h"
#include "LMAPlayerController.h"
#include "LMADefaultCharacter.h"

ALMAGameMode::ALMAGameMode()
{
	DefaultPawnClass = ALMADefaultCharacter::StaticClass();
	PlayerControllerClass = ALMAPlayerController::StaticClass();
}
