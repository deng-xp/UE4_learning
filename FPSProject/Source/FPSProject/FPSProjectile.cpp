// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//设置根组件（如果根组件不存在的话）
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileScenceComponent"));
	}
	//初始化球体碰撞组件
	if (!CollisionComponent)
	{
		//用球体进行碰撞实现
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		//设置球体半径
		CollisionComponent->SetSphereRadius(15.0f);
		//根组件设置为碰撞组件
		RootComponent = CollisionComponent;
	}

	//初始化发射物移动组件
	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		//绑定要控制的组件；碰撞后弹跳及朝着目标归位等行为由此类组件支持
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		//设置初始速度
		ProjectileMovementComponent->InitialSpeed=3000.0f;
		//设置最大速度
		ProjectileMovementComponent->MaxSpeed=3000.0f;
		//子弹的旋转将在每一帧中更新，以匹配它的速度方向
		ProjectileMovementComponent->bRotationFollowsVelocity=true;
		//设置反弹
		ProjectileMovementComponent->bShouldBounce=true;
		//设置反弹系数，反弹能力的强弱
		ProjectileMovementComponent->Bounciness=0.3f;
		//设置重力系数
		ProjectileMovementComponent->ProjectileGravityScale=0.0f;
		//设置生命周期
		InitialLifeSpan=3.0f;
	}

	//设置材质
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Material/SphereMaterial.SphereMaterial'"));
	if (Material.Succeeded())
	{
		//这部分还没看懂，为啥后面加静态网格体
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
		
	}
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));	//设置组件相对于其父组件的比例
	ProjectileMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	//为子弹初始化静态网格体，即显示外观 
	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Sphere.Sphere"));
		if (Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//初始化速度
void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
	//速度方向*速度大小=速度向量；大小由ProjectileMovementComponent管理设置
	ProjectileMovementComponent->Velocity=ShootDirection*ProjectileMovementComponent->InitialSpeed;
}
