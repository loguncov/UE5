// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "Player/LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/LMAHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/LMAWeaponComponent.h"
#include "Engine/Engine.h"

// Конструктор по умолчанию
ALMADefaultCharacter::ALMADefaultCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Создание и настройка пружинной арматуры
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true);

	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	// Создание и настройка компонента камеры
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false;

	// Отключение использования поворота контроллера
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Создание компонентов здоровья и оружия
	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("Weapon");
}

// Вызывается при начале игры или спавне персонажа
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Создание декали курсора, если материал курсора задан
	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}

	// Инициализация переменной здоровья и подписка на делегаты
	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaultCharacter::OnHealthChanged);
}

// Вызывается каждый кадр
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Обработка поворота курсора только если персонаж жив
	if (!(HealthComponent->IsDead()))
	{
		RotationPlayerOnCursor();
	}
}

// Установка управления игроком
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Привязка осей и действий к методам персонажа
	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAxis("CameraZoomInOut", this, &ALMADefaultCharacter::CameraZoomInOut);

	PlayerInputComponent->BindAction("SprintRun", IE_Pressed, this, &ALMADefaultCharacter::SprintRun);
	PlayerInputComponent->BindAction("SprintRun", IE_Released, this, &ALMADefaultCharacter::StopSprint);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &ULMAWeaponComponent::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::ReloadNeeded);
}

// Обработка смерти персонажа
void ALMADefaultCharacter::OnDeath()
{
	// Уничтожение курсора
	CurrentCursor->DestroyRenderState_Concurrent();

	// Воспроизведение анимации смерти и отключение движения персонажа
	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);

	// Изменение состояния контроллера на наблюдение
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

// Обработка изменений в здоровье персонажа
void ALMADefaultCharacter::OnHealthChanged(float NewHealth)
{
	// Вывод сообщения на экран при изменении здоровья
	// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));
}

// Управление поворотом персонажа по курсору мыши
void ALMADefaultCharacter::RotationPlayerOnCursor()
{
	// Получение контроллера игрока и вычисление поворота курсора
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC))
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
}

// Управление движением вперед
void ALMADefaultCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

// Управление движением вправо
void ALMADefaultCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

// Управление приближением и удалением камеры
void ALMADefaultCharacter::CameraZoomInOut(float Value)
{
	// Изменение длины пружинной арматуры для управления приближением и удалением камеры
	if (Value > 0 && newZoomF < MaxZoom)
	{
		newZoomF = newZoomF + ZoomSpeed;
	}
	else if (Value < 0 && newZoomF > MinZoom)
	{
		newZoomF = newZoomF - ZoomSpeed;
	}
	SpringArmComponent->TargetArmLength = newZoomF;
}

// Методы работы Спринта
void ALMADefaultCharacter::SprintRun()
{
	// Активация спринта и установка максимальной скорости движения
	if (GetVelocity().Length() >= 1.f)
	{
		GetWorldTimerManager().SetTimer(SprintTimer, this, &ALMADefaultCharacter::DecreaseStamina, SprintCount, true);
		if (GetWorldTimerManager().IsTimerActive(StaminaTimer))
			GetWorldTimerManager().ClearTimer(StaminaTimer);

		IsSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = 700.0f;
		GetCharacterMovement()->MaxWalkSpeedCrouched = 700.0f;
	}
}

void ALMADefaultCharacter::StopSprint()
{
	// Остановка спринта и восстановление стандартной скорости движения
	if (GetWorldTimerManager().IsTimerActive(SprintTimer))
	{
		GetWorldTimerManager().ClearTimer(SprintTimer);

		if (Stamina < MaxStamina)
		{
			GetWorldTimerManager().SetTimer(StaminaTimer, this, &ALMADefaultCharacter::IncreaseStamina, StaminaCount, true);
		}
	}
	IsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
}

void ALMADefaultCharacter::DecreaseStamina()
{
	// Уменьшение стамины при активном спринте
	Stamina = FMath::Clamp(Stamina - WasteStamina, MinStamina, MaxStamina);
	if (FMath::IsNearlyZero(Stamina))
	{
		StopSprint();
	}
	// Вывод сообщения о стамине на экран
	// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, FString::Printf(TEXT("Decrease Stamina = %f"), Stamina));
}

void ALMADefaultCharacter::IncreaseStamina()
{
	// Увеличение стамины при отсутствии спринта
	if (IsSprint == false)
	{
		Stamina = FMath::Clamp(Stamina + AccumulationStamina, MinStamina, MaxStamina);
		if (Stamina == MaxStamina)
		{
			if (GetWorldTimerManager().IsTimerActive(StaminaTimer))
			{
				GetWorldTimerManager().ClearTimer(StaminaTimer);
			}
		}
		// Вывод сообщения о стамине на экран
		// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Increase Stamina = %f"), Stamina));
	}
}
