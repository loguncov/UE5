
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
	// Устанавливает значения по умолчанию для свойств данного актора
	ALMAHealthPickup();

protected:
	UPROPERTY(VisibleAnywhere, Category="Pickup")
	USphereComponent* SphereComponent; // Компонент сферы для пикапа

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	float RespawnTime = 5.0f; // Время восстановления актора после взятия

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup", meta = (ClampMin="5.0", ClampMax="100.0"))
	float HealthFromPickup = 100.0f; // Количество здоровья, добавляемое пикапом
	
	// Вызывается при начале игры или при создании актора
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	// Вызывается каждый кадр
	virtual void Tick(float DeltaTime) override;

private:
	bool GivePickup(ALMADefaultCharacter* Character); // Проверка возможности взятия пикапа

	void PickupWasTaken();
	void RespawnPickup(); // Восстановление пикапа
};
