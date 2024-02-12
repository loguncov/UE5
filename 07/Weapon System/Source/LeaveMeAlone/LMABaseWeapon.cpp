// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "LMABaseWeapon.h"
#include "LMADefaultCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

// Sets default values
ALMABaseWeapon::ALMABaseWeapon() : CurrentAmmoWeapon(AmmoWeapon), isReloadClipsFinished(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);
}

void ALMABaseWeapon::Fire(const bool onOff)
{
	if (onOff)
	{
		Shoot();

		GetWorld()->GetTimerManager().SetTimer(FTimerHandleFire, // handle to cancel timer at a later time
			this,												 // the owning object
			&ALMABaseWeapon::Shoot,								 // function to call on elapsed
			0.2,												 // float delay until elapsed
			true);												 // looping?
	}
	else GetWorld()->GetTimerManager().ClearTimer(FTimerHandleFire);
}

bool ALMABaseWeapon::ChangeClip()
{
	if (CurrentAmmoWeapon.Bullets == AmmoWeapon.Bullets)
		return false;	// полная обойма патронов


	if (CurrentAmmoWeapon.Infinite == false)	// не бесконечные патроны
	{
		if (CurrentAmmoWeapon.Clips <= 0)
			return false;

		--CurrentAmmoWeapon.Clips;
	}

	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
	ReloadFinished(false);
	return true;
}

void ALMABaseWeapon::ReloadFinished(const bool yesNo)
{
	isReloadClipsFinished = yesNo;
}

// Called when the game starts or when spawned
void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ALMABaseWeapon::Shoot()
{
	if ( !CanIShoot() ) return;

	const FTransform SocketTransform = WeaponComponent->GetSocketTransform("Muzzle");
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.1f, 0, 2.0f);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 0.5f);
	}

	DecrementBullets();
}

void ALMABaseWeapon::DecrementBullets()
{
	CurrentAmmoWeapon.Bullets--;
	UE_LOG(LogWeapon, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));
	if (CurrentAmmoWeapon.Bullets == 0) OnBulletsEmptyDelegate.Execute();
}

bool ALMABaseWeapon::CanIShoot() const
{
	auto character = Cast<ALMADefaultCharacter>(this->GetSceneOutlinerParent());

	return (CurrentAmmoWeapon.Bullets > 0
		&& isReloadClipsFinished
		&& !character->isSprintingNow());
}

// Called every frame
void ALMABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
