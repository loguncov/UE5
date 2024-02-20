// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

// Предварительное объявление классов компонентов
class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class UAnimMontage;
class ULMAWeaponComponent;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Конструктор по умолчанию
	ALMADefaultCharacter();

	// Геттер для получения компонента здоровья
	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	// Компонент камеры
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	// Компонент декаля курсора
	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	// Материал курсора
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	// Размер курсора
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	// Компонент здоровья
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	// Анимация смерти
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	// Компонент оружия
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	ULMAWeaponComponent* WeaponComponent;

	// Вызывается при начале игры или спавне персонажа
	virtual void BeginPlay() override;

public:
	// Вызывается каждый кадр
	virtual void Tick(float DeltaTime) override;

	// Установка управления игроком
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Переменная для сохранения состояния спринта
	UPROPERTY(BlueprintReadOnly)
	bool IsSprint;

private:
	// Параметры камеры и управления
	float YRotation = -75.0f;
	float ArmLength = 1400.0f;
	float FOV = 55.0f;

	// Обработка смерти персонажа
	void OnDeath();

	// Обработка изменений в здоровье персонажа
	void OnHealthChanged(float NewHealth);

	// Управление поворотом персонажа по курсору мыши
	void RotationPlayerOnCursor();

	// Управление движением вперед и вправо
	void MoveForward(float Value);
	void MoveRight(float Value);

	// Управление приближением и удалением камеры
	void CameraZoomInOut(float value);
	float ZoomSpeed = 50.f;
	float MinZoom = 500.f;
	float MaxZoom = 2800.f;
	float newZoomF = ArmLength;

	// Управление спринтом
	void SprintRun();
	void StopSprint();

	UPROPERTY(EditAnywhere)
	float SprintCount = 0.1f;

	UPROPERTY(EditAnywhere)
	float StaminaCount = 0.1f;
	void DecreaseStamina();
	void IncreaseStamina();

	FTimerHandle SprintTimer;
	FTimerHandle StaminaTimer;

	const float MaxStamina = 100.f;
	const float MinStamina = 0.f;

	float Stamina = MaxStamina;
	float WasteStamina = 5.f;
	float AccumulationStamina = 3.f;
};
