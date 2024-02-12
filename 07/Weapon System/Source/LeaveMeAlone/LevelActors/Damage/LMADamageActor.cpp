
#include "LMADamageActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Устанавливаем значения по умолчанию
ALMADamageActor::ALMADamageActor()
{
 	// Устанавливаем вызов Tick() для этого актора каждый кадр. Это можно отключить, чтобы улучшить производительность, если это необходимо.
	PrimaryActorTick.bCanEverTick = true;
	
	// Создаем компонент сферы для нанесения урона
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetSphereRadius(SphereRadius);
	SetRootComponent(SphereComponent);

	// Создаем статический меш для отображения объекта урона
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(SphereComponent);
	StaticMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 0.05f));
	StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -4.0f));
}

// Вызывается при начале игры или когда актор создан
void ALMADamageActor::BeginPlay()
{
	Super::BeginPlay();
}

// Вызывается каждый кадр
void ALMADamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Применяем радиальный урон
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), SphereRadius, nullptr, {}, this, nullptr, false);
}
