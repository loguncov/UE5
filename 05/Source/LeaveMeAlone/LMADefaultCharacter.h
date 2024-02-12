#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"


class UCameraComponent;

class USpringArmComponent;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Устанавливает значения по умолчанию для свойств персонажа
	ALMADefaultCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);


	UPROPERTY(EditDefaultsOnly, Category = "Camera Zoom")
	float MaxZoomInDistance = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera Zoom")
	float MaxZoomOutDistance = 2000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera Zoom")
	float ZoomSpeed = 100.0f;


	// Вызывается при запуске игры или при создании
	virtual void BeginPlay() override;

public:	
	// Вызывается каждый кадр
	virtual void Tick(float DeltaTime) override;

	// Вызывается для привязки функциональности к вводу
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Вызывается при прокрутке колеса мыши
	virtual void MouseWheelZoom(float DeltaWheel) override; 
private:
	float YRotation = -75.0f;	
	float ArmLength = 1400.0f;	
	float FOV = 55.0f;			

	void MoveForward(float Value);	
	void MoveRight(float Value);	
};
