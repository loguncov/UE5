// Игра "LeaveMeAlone" от Netologiya. Все права защищены.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAHealthComponent.generated.h"

// Декларация делегата для оповещения о смерти
DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LEAVEMEALONE_API ULMAHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Устанавливает значения по умолчанию для свойств компонента
	ULMAHealthComponent();
	
	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }
	
	// Функция для определения, мертв ли персонаж
	UFUNCTION(BlueprintCallable)
	bool IsDead() const;
	
	bool AddHealth(float NewHealth);
	bool IsHealthFull() const;

	// Делегат, оповещающий о смерти
	FOnDeath OnDeath;
	// Делегат, оповещающий об изменении здоровья
	FOnHealthChanged OnHealthChanged;

protected:
	// Максимальное здоровье персонажа
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 100.0f;

	// Вызывается при запуске игры
	virtual void BeginPlay() override;

private:
	// Текущее здоровье персонажа
	float Health = 0.0f;

	// Обработчик получения урона
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};