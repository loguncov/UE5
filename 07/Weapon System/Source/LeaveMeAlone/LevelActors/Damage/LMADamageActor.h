// Игра "Оставь Меня В Покое" от Netologiya. Все права защищены.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMADamageActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class LEAVEMEALONE_API ALMADamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Устанавливаем значения по умолчанию для свойств актора
	ALMADamageActor();
	
	// Компонент сферы для нанесения урона
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* SphereComponent;

	// Меш сферы для отображения объекта урона
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage = 5.0f;	// Урон, наносимый

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float SphereRadius = 100.0f; // Радиус для нанесения урона

protected:
	// Вызывается при начале игры или когда актор создан
	virtual void BeginPlay() override;

public:	
	// Вызывается каждый кадр
	virtual void Tick(float DeltaTime) override;
};
