#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAHealthComponent.generated.h"

// Декларация делегата, уведомляющего о смерти
DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LEAVEMEALONE_API ULMAHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Устанавливаем значения по умолчанию для свойств компонента
	ULMAHealthComponent();
	
	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }
	
	// Функция, которая будет уведомлять клиентов о смерти
	UFUNCTION(BlueprintCallable)
	bool IsDead() const;
	
	bool AddHealth(float NewHealth);
	bool IsHealthFull() const;

	// Объявляем делегаты
	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 100.0f; // Максимальное здоровье
	
	// Вызывается при начале игры
	virtual void BeginPlay() override;

private:
	float Health = 0.0f; // Здоровье персонажа

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
