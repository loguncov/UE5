// Авторские права Epic Games, Inc. Все права защищены.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "LeaveMeAloneCharacter.generated.h"


UCLASS(config=Game)
class ALeaveMeAloneCharacter : public ACharacter
{
	GENERATED_BODY()

	// Камера механизм расположения камеры позади персонажа 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Камера следования 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	// Контекст сопоставления 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	// Действие прыжка 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	// Действие движения 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	// Действие обзора 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	ALeaveMeAloneCharacter();
	

protected:

	// Вызывается при вводе движения 
	void Move(const FInputActionValue& Value);

	// Вызывается при вводе обзора 
	void Look(const FInputActionValue& Value);
			

protected:
	// Интерфейс APawn
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Для добавления контекста сопоставления
	virtual void BeginPlay();

public:
	// Возвращает подобъект камеры 
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// Возвращает подобъект камеры следования 
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

