// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemTypes.h"

#include "SFOnlineSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfo
{
    GENERATED_BODY()
    
public:
    UPROPERTY(BlueprintReadOnly)
    int32 ServerIndex;

    UPROPERTY(BlueprintReadOnly)
    FString ServerName;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentPlayerCount;

    UPROPERTY(BlueprintReadOnly)
    int32 MaxPlayerCount;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFindSession, FSessionInfo, SessionInfos);

/**
 * 
 */
UCLASS()
class USFOnlineSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	USFOnlineSubsystem();

    /** Implement this for initialization of instances of the system */
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    /** Implement this for deinitialization of instances of the system */
    virtual void Deinitialize() override;

    // 메뉴에서 호출할 세션 생성, 검색, 참가 함수
    UFUNCTION(BlueprintCallable)
    void CreateGameSession();

    UFUNCTION(BlueprintCallable)
    void FindGameSessions();

    UFUNCTION(BlueprintCallable)
    void JoinGameSession(int SessionIndex);

public:
    UPROPERTY(BlueprintAssignable)
    FOnFindSession OnFindSession;

protected:
    // OnlineSubsystem 관련 인터페이스
    IOnlineSessionPtr SessionInterface;
    TSharedPtr<FOnlineSessionSearch> SessionSearch;

    // 각 기능의 delegate 콜백 함수들
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    void OnFindSessionsComplete(bool bWasSuccessful);
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    // Delegate 핸들 (추가적인 관리가 필요할 경우 보관)
    FDelegateHandle OnCreateSessionCompleteDelegateHandle;
    FDelegateHandle OnFindSessionsCompleteDelegateHandle;
    FDelegateHandle OnJoinSessionCompleteDelegateHandle;
};