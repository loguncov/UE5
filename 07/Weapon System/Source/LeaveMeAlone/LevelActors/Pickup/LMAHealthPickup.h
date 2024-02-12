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
	// Устанавливает значения по умолчанию для свойств этого актора
	ALMAHealthPickup();

protected:
	UPROPERTY(VisibleAnywhere, Category="Pickup")
	USphereComponent* SphereComponent; // Компонент сферы для пикапа

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	float RespawnTime = 5.0f;	// Время респауна актора

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup", meta = (ClampMin="5.0", ClampMax="100.0"))
	float HealthFromPickup = 100.0f;	// Количество жизней, которое прибавит пикап
	
	// Вызывается при запуске игры или при появлении актора
	virtual void BeginPlay() override;

	/* 
		Метод, реализованный в базовом классе Актора,
		который реагирует на пересечение сетки коллизии с
		другими Акторами, и в нашем классе мы его
		переопределяем
	*/
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	// Вызывается каждый кадр
	virtual void Tick(float DeltaTime) override;

private:
	bool GivePickup(ALMADefaultCharacter* Character); // Взяли ли мы пикап
	/* 
		Будет запускать таймер респауна и скрывать наш
		пикап на временной промежуток после взятия пикапа
	*/
	void PickupWasTaken();
	void RespawnPickup(); // Будет возвращать видимость пикапа
};
