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

    // �޴����� ȣ���� ���� ����, �˻�, ���� �Լ�
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
    // OnlineSubsystem ���� �������̽�
    IOnlineSessionPtr SessionInterface;
    TSharedPtr<FOnlineSessionSearch> SessionSearch;

    // �� ����� delegate �ݹ� �Լ���
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    void OnFindSessionsComplete(bool bWasSuccessful);
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    // Delegate �ڵ� (�߰����� ������ �ʿ��� ��� ����)
    FDelegateHandle OnCreateSessionCompleteDelegateHandle;
    FDelegateHandle OnFindSessionsCompleteDelegateHandle;
    FDelegateHandle OnJoinSessionCompleteDelegateHandle;
};