// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent,DisplayName = "交互组件") )
class ALSPROJECT_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    //交互距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = 0.0, ClampMax = 1000.0, DisplayName = "交互距离"))
	float InteractionRange = 300.f;

	//交互检测频率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = 0.1, ClampMax = 2.0, DisplayName = "交互检测频率"))
	float InteractionFrequency = 0.2f;

	//交互检测通道
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (DisplayName = "交互检测通道"))
	TEnumAsByte<ECollisionChannel> InteractionTraceChannel = ECC_Visibility;

	//交互检测半径
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (ClampMin = 0.0, ClampMax = 500.0, DisplayName = "交互检测半径"))
	float InteractionRadius = 20.f;
	
	//检测到的Actor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	AActor* InteractedActor = nullptr;

	//Owner的Pawn
	APawn* OwnerPawn = nullptr;

	//玩家控制器
	APlayerController* PlayerController = nullptr;

	//DrawDebug开关
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (DisplayName = "是否绘制调试信息"))
	bool bDrawDebug = false;

public:
	//交互检测函数
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InteractionCheck();

    /*
	 *****输入事件
	*/
	//输入事件:开始交互
	UFUNCTION(BlueprintCallable, Category = "Interaction", Server, Reliable)
	void Server_StartInteract();

	//输入事件:交互中
	UFUNCTION(BlueprintCallable, Category = "Interaction", Server, Reliable)
    void Server_Interacting();

	//输入事件:结束交互
	UFUNCTION(BlueprintCallable, Category = "Interaction", Server, Reliable)
	void Server_EndInteract();

	/*
	 ******检测事件
	*/
	//事件:检测到
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteractionChecked(AActor* InteractedActors);

	//事件:结束检测
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteractionCheckEnded(AActor* InteractedActors);

	/*
	 ******分发事件
	*/
	//分发事件:交互前
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteractionStarted(AActor* InteractedActors);

	//分发事件:交互中
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteractionUpdated(AActor* InteractedActors);

	//分发事件:交互结束
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnInteractionEnded(AActor* InteractedActors);

	/*
	 ******计时器
	*/
	//计时器句柄
	FTimerHandle InteractionTimerHandle;

	//开始计时器
	void StartInteractionTimer();

	//停止计时器
	void StopInteractionTimer();



public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
