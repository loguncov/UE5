// Заполните свое уведомление о авторском праве на странице описания проекта.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CppBaseActor.generated.h"

UCLASS()
class CPPBASE_API ACppBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Устанавливает значения по умолчанию для свойств этого актера
	ACppBaseActor();

protected:
	// Вызывается при запуске игры или при создании
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	float SinMovement(const float z);

	UPROPERTY(VisibleAnywhere)
	FVector InitialLocation;

public:	
	// Вызывается каждый кадр
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly)
	float Amplitude = 200.0;

	UPROPERTY(EditInstanceOnly)
	float Frequency = 3.0;
};
