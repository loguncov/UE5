// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMAHealthPickup.generated.h"

class USphereComponent;
class ALMADefaultCharacter;

UCLASS()
class LEAVEMEALONE_API ALMAHealthPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALMAHealthPickup();

protected:
	UPROPERTY(VisibleAnywhere, Category="Pickup")
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	float RespawnTime = 5.0f;	// время респауна актора

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup", meta = (ClampMin="5.0", ClampMax="100.0"))
	float HealthFromPickup = 100.0f;	// количество жизней, которое прибавит пикап
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/*
		метод реализованный в базовом классе Актора,
		он реагирует на пересечении сетки коллизии с
		другими Акторами, а в нашем классе мы его
		переопределяем
	*/
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool GivePickup(ALMADefaultCharacter* Character);// взяли ли мы пикап
	/* 
		будет запускать таймер респауна и скрывать наш
		пикап на временной промежуток, после взятия пикапа
	*/
	void PickupWasTaken();
	void RespawnPickup();// будет возвращать видимость пикапа
};
