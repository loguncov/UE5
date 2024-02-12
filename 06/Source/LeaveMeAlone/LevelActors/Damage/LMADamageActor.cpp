// Игра "LeaveMeAlone" от Netologiya. Все права защищены.

#include "LMADamageActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Устанавливает значения по умолчанию
ALMADamageActor::ALMADamageActor()
{
 	// Устанавливаем вызов функции Tick() каждый кадр. Это можно отключить для улучшения производительности.
	PrimaryActorTick.bCanEverTick = true;
	
	// Создаем компонент сферы для обработки радиального урона
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetSphereRadius(SphereRadius);
	SetRootComponent(SphereComponent);

	// Создаем статический меш, который будет виден на сцене
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(SphereComponent);
	StaticMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 0.05f));
	StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -4.0f));
}

// Вызывается при запуске игры или при создании актора на сцене
void ALMADamageActor::BeginPlay()
{
	Super::BeginPlay();
}

// Вызывается каждый кадр
void ALMADamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Применяем радиальный урон в указанной области
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), SphereRadius, nullptr, {}, this, nullptr, false);
}