// Fill out your copyright notice in the Description page of Project Settings.

//角色，功能：前进、后退、左右移动、跳跃、视角变换
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FPSProjectile.h"
#include "FPSCharacter.generated.h"

UCLASS()
class FPSPROJECT_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//要生成的子弹类
	//TSubclassOf<T>：该模板类告知编辑器的属性窗口只列出派生自AFPSProjectile的类
	UPROPERTY(EditDefaultsOnly,Category=Projectile)
		TSubclassOf<class AFPSProjectile> ProjectileClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//处理前后移动输入
	UFUNCTION()
		void MoveForward(float value);
	//处理左右移动输入
	UFUNCTION()
		void MoveRight(float value);
	//跳跃
	UFUNCTION()
		void StartJump();
	UFUNCTION()
		void StopJump();
	//处理发射子弹
	UFUNCTION()
		void Fire();
public:
	//角色摄像机
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPSCamerComponent;
	//第一人称网格体（手臂），只对玩家可见
	UPROPERTY(VisibleDefaultsOnly,Category=Mesh)
		USkeletalMeshComponent* FPSMesh;	//骨骼类型
	//枪口相对摄像机的位置偏移（即子弹初始化位置）
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Gameplay)
		FVector MuzzleOffset;
};
