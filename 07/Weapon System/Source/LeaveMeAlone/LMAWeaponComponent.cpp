// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "LMAWeaponComponent.h"
#include "GameFramework/Character.h"
#include "LMABaseWeapon.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "LMADefaultCharacter.h"
#include "LMABaseWeapon.h"

// Sets default values for this component's properties
ULMAWeaponComponent::ULMAWeaponComponent() : isReloadingNow(false)
{
	PrimaryComponentTick.bCanEverTick = false; // тик в любом случае НЕ работает!???
}

void ULMAWeaponComponent::Fire()
{
	if (Weapon)
		Weapon->Fire(true);
}

void ULMAWeaponComponent::FireEnd()
{
	if (Weapon)
		Weapon->Fire(false);
}

void ULMAWeaponComponent::Reload()
{
	if (CanReload())
	{
		if (Weapon->ChangeClip())
		{
			isReloadingNow = true;
			ACharacter* Character = Cast<ACharacter>(GetOwner());
			Character->PlayAnimMontage(ReloadMontage);
		}
	}
}

// Called when the game starts
void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
	InitAnimNotify();
}


// Called every frame
void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULMAWeaponComponent::SpawnWeapon()
{
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	if (Weapon)
	{
		const auto Character = Cast<ACharacter>(GetOwner());
		if (Character)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "r_Weapon_Socket");
			Weapon->OnBulletsEmptyDelegate.BindLambda([this]() { Reload(); });
		}
	}
}

void ULMAWeaponComponent::InitAnimNotify()
{
	if (!ReloadMontage) return;

	const auto &NotifiesEvents = ReloadMontage->Notifies;
	for (auto &NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		isReloadingNow = false;
		Weapon->ReloadFinished(true);
	}
}

bool ULMAWeaponComponent::CanReload()
{
	auto ch = Cast<ALMADefaultCharacter>(GetOwner());
	return (!isReloadingNow && !ch->isSprintingNow() && Weapon);
}
