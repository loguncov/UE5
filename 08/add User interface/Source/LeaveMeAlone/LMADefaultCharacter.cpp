// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/LMAHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LMAWeaponComponent.h"
#include "Engine.h"

// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	/*
		данное условие не позволит нашей камере поворачиватьс¤
		в момент поворота персонажа. В жанре Top - Down Shooter,
		камера обычно находитс¤ статично над игроком.
	*/
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = ArmLength;
	/*
		структура FRotator хранит аргументы в следующей последовательности:
		Pitch, Yaw, Roll.
		Так как нам необходимо определить значени¤ по оси Y, мы устанавливаем Pitch аргумент.
	*/
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	// данное условие запрещаем камере вращаться относительно SpringArmComponent.
	CameraComponent->bUsePawnControlRotation = false;

	WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("Weapon");
	
	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");

	// запретим нашему персонажу поворачиваться в сторону камеры
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CharacterMovement = GetCharacterMovement();
}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}

	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);
}

// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!(HealthComponent->IsDead()))
	{
		RotationPlayerOnCursor();
	}

	if (pressSprintBtn)
	{
		if (GetVelocity().IsZero())
		{
			sprintingStop();
		}
		else
		{
			if (bIsSprinting == false && !FMath::IsNearlyZero(stamina))
			{
				bIsSprinting = true;
				CharacterMovement->MaxWalkSpeed *= 2;	// MaxWalkSpeed

				GetWorld()->GetTimerManager().SetTimer(FTimerHandleStamina, // handle to cancel timer at a later time
					this,													// the owning object
					&ALMADefaultCharacter::countDownStamina,				// function to call on elapsed
					0.05,													// float delay until elapsed
					true);													// looping?
			}
		}
	}
}

// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMADefaultCharacter::pushSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALMADefaultCharacter::releaseSprint);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &ULMAWeaponComponent::FireEnd);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Reload);
}

void ALMADefaultCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ALMADefaultCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::OnDeath()
{
	CurrentCursor->DestroyRenderState_Concurrent();

	PlayAnimMontage(DeathMontage);

	GetCharacterMovement()->DisableMovement();

	SetLifeSpan(5.0f);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}

	if (FTimerHandleStamina.IsValid())
	{
		bIsSprinting = false;
		GetWorld()->GetTimerManager().ClearTimer(FTimerHandleStamina);
	}

	PersonIsDeath.Broadcast();
}

void ALMADefaultCharacter::pushSprint()
{
	pressSprintBtn = true;
}

void ALMADefaultCharacter::releaseSprint()
{
	pressSprintBtn = false;
	sprintingStop();
}

bool ALMADefaultCharacter::isSprintingNow() const
{
	return bIsSprinting;
}

void ALMADefaultCharacter::sprintingStop()
{
	if (bIsSprinting && !FMath::IsNearlyEqual(stamina, STAMINA_MAX))
	{
		bIsSprinting = false;
		CharacterMovement->MaxWalkSpeed /= 2;	// MaxWalkSpeed

		GetWorld()->GetTimerManager().SetTimer(FTimerHandleStamina, // handle to cancel timer at a later time
			this,													// the owning object
			&ALMADefaultCharacter::countUpStamina,					// function to call on elapsed
			0.05,													// float delay until elapsed
			true);													// looping?
	}
}

void ALMADefaultCharacter::countDownStamina()
{
	stamina = FMath::Clamp(stamina - 1.0f, 0.0f, STAMINA_MAX);

	if (FMath::IsNearlyZero(stamina))
	{
		sprintingStop();
	}
}

void ALMADefaultCharacter::countUpStamina()
{
	float restVelocity = (GetVelocity().IsZero()) ? 0.7f : 0.2f;

	stamina += restVelocity;
	if ( !(stamina < STAMINA_MAX) )
	{
		stamina = STAMINA_MAX;
		GetWorld()->GetTimerManager().ClearTimer(FTimerHandleStamina);
	}
}

void ALMADefaultCharacter::RotationPlayerOnCursor()
{
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
