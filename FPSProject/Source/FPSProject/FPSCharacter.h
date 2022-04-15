// Fill out your copyright notice in the Description page of Project Settings.

//��ɫ�����ܣ�ǰ�������ˡ������ƶ�����Ծ���ӽǱ任
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
	//Ҫ���ɵ��ӵ���
	//TSubclassOf<T>����ģ�����֪�༭�������Դ���ֻ�г�������AFPSProjectile����
	UPROPERTY(EditDefaultsOnly,Category=Projectile)
		TSubclassOf<class AFPSProjectile> ProjectileClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//����ǰ���ƶ�����
	UFUNCTION()
		void MoveForward(float value);
	//���������ƶ�����
	UFUNCTION()
		void MoveRight(float value);
	//��Ծ
	UFUNCTION()
		void StartJump();
	UFUNCTION()
		void StopJump();
	//�������ӵ�
	UFUNCTION()
		void Fire();
public:
	//��ɫ�����
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPSCamerComponent;
	//��һ�˳������壨�ֱۣ���ֻ����ҿɼ�
	UPROPERTY(VisibleDefaultsOnly,Category=Mesh)
		USkeletalMeshComponent* FPSMesh;	//��������
	//ǹ������������λ��ƫ�ƣ����ӵ���ʼ��λ�ã�
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Gameplay)
		FVector MuzzleOffset;
};
