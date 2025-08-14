#include "AI/BaseAICharacter.h"
#include "AI/BaseAIController.h"
#include "AI/AIHealthComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CH3Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "BrainComponent.h"
#include "NiagaraFunctionLibrary.h"	
#include "NiagaraComponent.h"



ABaseAICharacter::ABaseAICharacter()
{
	AIControllerClass = ABaseAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = WalkSpeed;	
	Movement->RotationRate = FRotator(0.0f, 360.0f, 0.0f);

	// 캐릭터의 회전을 컨트롤러의 회전과 일치
	bUseControllerRotationYaw = true;	
	// 캐릭터의 이동 방향으로 자동 회전하는 기능 off AI컨트롤러의 SetFocus에 제어됨
	GetCharacterMovement()->bOrientRotationToMovement = false;

	HealthComponent = CreateDefaultSubobject<UAIHealthComponent>(TEXT("HealthComponent"));

	WeaponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComp"));
	WeaponMeshComp->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("AI Character %s has begun play."), *GetName());
}

void ABaseAICharacter::setMoveSpeed(float NewSpeed)
{
	if(UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = NewSpeed;
		UE_LOG(LogTemp, Warning, TEXT("AI Character %s speed set to: %f"), *GetName(), NewSpeed);
	}
}

void ABaseAICharacter::PerformMeleeAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Performs MELEE ATTACK!"), *GetName());
}

void ABaseAICharacter::PerformRangedAttack()
{
	// AIController와 Blackboard 컴포넌트 가져오기, 플레이어의 위치 가져옴
	ABaseAIController* AIController = Cast<ABaseAIController>(GetController());
	if (AIController == nullptr) return;
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (BlackboardComp == nullptr) return;
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
	if (TargetActor == nullptr) return;
	if (GetWorld() == nullptr || GetMesh() == nullptr) return;

	// 총구의 조준선 계산
	const FVector StartLocation = GetMesh()->GetSocketLocation(TEXT("AIMuzzleSocket"));
	const FVector TargetLocation = TargetActor->GetActorLocation();
	//총구와 플레이어를 바라보는 값 계산
	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
	//계산된 회전값을 순수 방향으로 변환
	const FVector FireDirection = TargetRotation.Vector();

	//라인 트레이스 발사
	const float FireRange = 5000.0f; // 50m 사거리
	const FVector EndLocation = StartLocation + (FireDirection * FireRange);
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Pawn,
		CollisionParams
	);

	if (MuzzleEffect)
	{
		// 총구 이펙트 생성
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			MuzzleEffect,
			StartLocation,
			TargetRotation
		);
	}

	if (FireSound)
	{
		// 총격 사운드 재생
		UGameplayStatics::PlaySoundAtLocation(
			this,
			FireSound,
			StartLocation
		);
	}

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		//Enemy 태그가 있는 경우에만 처리
		if (HitActor && HitActor->ActorHasTag("Enemy"))
		{
			UGameplayStatics::ApplyDamage(
				HitActor,
				10.0f, 
				AIController, 
				this,
				UDamageType::StaticClass() 
			);
			if (HitEffect)
			{
				// 적에게 맞았을 때 이펙트 생성
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					HitEffect,
					HitResult.ImpactPoint,
					HitResult.ImpactNormal.Rotation()	
				);
			}
		}
	}

	DrawDebugLine(
		GetWorld(),
		StartLocation,
		EndLocation,
		FColor::Green,
		false,
		2.0f
	);

	UE_LOG(LogTemp, Warning, TEXT("%s Performs RANGED ATTACK!"), *GetName());
}


void ABaseAICharacter::HandleDeath()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{	// 비헤이비어트리 를 중지하고, AI의 뇌 컴포넌트를 정지
		if (AIController && AIController->GetBrainComponent())
		{
			AIController->GetBrainComponent()->StopLogic(TEXT("Death"));
		}
		// 시야 고정 해제
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}
	   // 총기 컴포넌트 제거
	if (WeaponMeshComp)
	{
		WeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	// 애니메이션 클래스 제거
	GetMesh()->SetAnimClass(nullptr); 
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	// 콜리전 제거 및 캡슐컴포넌느 모든 종류 충돌 무시 메시가 충돌 담당
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	// 레그돌 변경 및 시뮬레이션 활성화
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	SetLifeSpan(10.0f); 
}
