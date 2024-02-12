// Заполните свое уведомление о авторском праве на странице описания проекта.

#include "CppBaseActor.h"
#include "Engine/Engine.h"

// Устанавливает значения по умолчанию
ACppBaseActor::ACppBaseActor()
{
 	// Настройте вызов функции Tick() для этого актера каждый кадр. Можно отключить для улучшения производительности, если это не требуется.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

// Вызывается при запуске игры или при создании
void ACppBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitialLocation = FVector(FMath::FRandRange(-100.0f, 100.0f), FMath::FRandRange(0.0f, 350.0f), 0.0f);
	SetActorLocation(InitialLocation);
}


float ACppBaseActor::SinMovement(const float z)
{
	UWorld* World = GetWorld();

	float multJump = (FMath::Sin(Frequency * World->GetTimeSeconds()) + 1.0) / 2;

	return (Amplitude * multJump + z);
}

// Вызывается каждый кадр
void ACppBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
