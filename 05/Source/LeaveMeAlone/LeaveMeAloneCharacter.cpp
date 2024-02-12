// Авторские права Epic Games, Inc. Все права защищены.

#include "LeaveMeAloneCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// ALeaveMeAloneCharacter

ALeaveMeAloneCharacter::ALeaveMeAloneCharacter()
{
	// Задает размер капсулы коллизии
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Не вращать, когда контроллер вращается. Пусть это влияет только на камеру.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Настройка движения персонажа
	GetCharacterMovement()->bOrientRotationToMovement = true; // Персонаж двигается в направлении ввода...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...по этой скорости вращения

	// Примечание: Для более быстрой итерации можно настроить эти переменные, а также множество других, в «Схеме персонажа» вместо перекомпиляции, чтобы их настраивать
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Создание камерного механизма (при необходимости сближается к игроку при столкновении)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // Камера следует за персонажем на этом расстоянии	
	CameraBoom->bUsePawnControlRotation = true; // Поворачивать механизм на основе контроллера

	// Создание камеры следования
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Прикрепить камеру к концу механизма и позволить ему подстраиваться под ориентацию контроллера
	FollowCamera->bUsePawnControlRotation = false; // Камера не вращается относительно механизма


}

void ALeaveMeAloneCharacter::BeginPlay()
{
	// Вызвать базовый класс  
	Super::BeginPlay();

	// Добавить контекст сопоставления ввода
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Ввод

void ALeaveMeAloneCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Настроить привязки действий
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Прыжок
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Движение
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALeaveMeAloneCharacter::Move);

		// Обзор
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALeaveMeAloneCharacter::Look);

	}

}

void ALeaveMeAloneCharacter::Move(const FInputActionValue& Value)
{
	// Ввод в виде Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Определение направления вперед
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Получение вектора вперед
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// Получение вектора вправо
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Добавление движения
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ALeaveMeAloneCharacter::Look(const FInputActionValue& Value)
{
	// Ввод в виде Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Добавление ввода направления взгляда к контроллеру
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}




