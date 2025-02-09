// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

// ��������������� ���������� ������� �����������
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
	// ����������� �� ���������
	ALMADefaultCharacter();

	// ������ ��� ��������� ���������� ��������
	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	// ��������� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	// ��������� ������ �������
	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	// �������� �������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	// ������ �������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	// ��������� ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	// �������� ������
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	// ��������� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	ULMAWeaponComponent* WeaponComponent;

	// ���������� ��� ������ ���� ��� ������ ���������
	virtual void BeginPlay() override;

public:
	// ���������� ������ ����
	virtual void Tick(float DeltaTime) override;

	// ��������� ���������� �������
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ���������� ��� ���������� ��������� �������
	UPROPERTY(BlueprintReadOnly)
	bool IsSprint;

private:
	// ��������� ������ � ����������
	float YRotation = -75.0f;
	float ArmLength = 1400.0f;
	float FOV = 55.0f;

	// ��������� ������ ���������
	void OnDeath();

	// ��������� ��������� � �������� ���������
	void OnHealthChanged(float NewHealth);

	// ���������� ��������� ��������� �� ������� ����
	void RotationPlayerOnCursor();

	// ���������� ��������� ������ � ������
	void MoveForward(float Value);
	void MoveRight(float Value);

	// ���������� ������������ � ��������� ������
	void CameraZoomInOut(float value);
	float ZoomSpeed = 50.f;
	float MinZoom = 500.f;
	float MaxZoom = 2800.f;
	float newZoomF = ArmLength;

	// ���������� ��������
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
