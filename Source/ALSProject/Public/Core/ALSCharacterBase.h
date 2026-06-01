// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ALSCharacterBase.generated.h"

/**
 * ALS 角色基类
 * 提供基于定时器的摄像机射线检测，用于瞄准、交互预判等玩法逻辑。
 * 检测不依赖 Tick，由 Timer 每 0.1 秒执行一次以降低性能开销。
 */
UCLASS()
class ALSPROJECT_API AALSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AALSCharacterBase();

protected:
	virtual bool ShouldReplicateAcceleration() const override { return true; }

	/** 射线检测间隔（秒），与调试线显示时长保持一致 */
	static constexpr float LineTraceInterval = 0.1f;

public:

	/** 碰撞通道，决定射线能与哪些物体发生碰撞（需在对象碰撞预设中对应响应） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	/** 是否在场景中绘制调试射线（绿=命中，红=未命中） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	bool bDrawDebug = false;

	UFUNCTION(BlueprintCallable, Category = "Trace")
	bool PerformLineTrace(FVector Start, FVector End, FHitResult& OutHit);

};
