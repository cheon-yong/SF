// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SFOnlineSubsystem.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include <Online/OnlineSessionNames.h>

USFOnlineSubsystem::USFOnlineSubsystem()
{
}

void USFOnlineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        SessionInterface = OnlineSubsystem->GetSessionInterface();
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
            FString::Printf(TEXT("Find Subsystem : %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
    }
}

void USFOnlineSubsystem::Deinitialize()
{
    Super::Deinitialize();

    SessionInterface.Reset();
}

void USFOnlineSubsystem::CreateGameSession()
{
    if (!SessionInterface.IsValid())
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
            FString::Printf(TEXT("SessionInterface�� ��ȿ���� �ʽ��ϴ�.")));
        UE_LOG(LogTemp, Warning, TEXT("SessionInterface�� ��ȿ���� �ʽ��ϴ�."));
        return;
    }

    // Delegate ���ε�
    OnCreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
        FOnCreateSessionCompleteDelegate::CreateUObject(this, &USFOnlineSubsystem::OnCreateSessionComplete));

    // ���� ���� ����
    FOnlineSessionSettings SessionSettings;
    
    if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
    {
        SessionSettings.bIsLANMatch = false;
    }
    else
    {
        SessionSettings.bIsLANMatch = true;               // �¶��� ��ġ�� ���� (LAN�̸� true�� ����)
    }
    SessionSettings.NumPublicConnections = 2;            // �ִ� �÷��̾� �� (ȣ��Ʈ + ������)
    SessionSettings.bShouldAdvertise = true;             // �˻��� �����ϵ��� ����
    SessionSettings.bAllowJoinInProgress = true;         // ���� �� ���� ���
    SessionSettings.bAllowJoinViaPresence = true;
    SessionSettings.bUseLobbiesIfAvailable = true;
    SessionSettings.bUsesPresence = true;                // ģ�� �ʴ� �� Presence ��� ���
    SessionSettings.bIsDedicated = false;

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionSettings);
}

void USFOnlineSubsystem::FindGameSessions()
{
    if (!SessionInterface.IsValid())
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
            FString::Printf(TEXT("SessionInterface�� ��ȿ���� �ʽ��ϴ�.")));
        UE_LOG(LogTemp, Warning, TEXT("SessionInterface�� ��ȿ���� �ʽ��ϴ�."));
        return;
    }

    
    // �˻� ���� ����
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->bIsLanQuery = false;
    SessionSearch->MaxSearchResults = 100;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    // Delegate ���ε�
    OnFindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
        FOnFindSessionsCompleteDelegate::CreateUObject(this, &USFOnlineSubsystem::OnFindSessionsComplete));

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void USFOnlineSubsystem::JoinGameSession(int SessionIndex)
{
    if (!SessionInterface.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("SessionInterface�� ��ȿ���� �ʽ��ϴ�."));
        return;
    }

    // Check Index
    if (SessionIndex < 0 || SessionIndex >= SessionSearch->SearchResults.Num() || SessionIndex == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("Session Index�� ��ȿ���� �ʽ��ϴ�."));
        return;
    }

    // Delegate ���ε�
    OnJoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
        FOnJoinSessionCompleteDelegate::CreateUObject(this, &USFOnlineSubsystem::OnJoinSessionComplete));

    SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[SessionIndex]);
}


void USFOnlineSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
    }

    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("���� '%s' ���� ����!"), *SessionName.ToString());
        // ���� ���� ��, ������ ���� ������ listen �ɼǰ� �Բ� �̵��մϴ�.
        UGameplayStatics::OpenLevel(GetWorld(), FName("Stage"), true, "listen");
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("���� '%s' ���� ����!"), *SessionName.ToString());
    }
}

UE_DISABLE_OPTIMIZATION
void USFOnlineSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
    }

    if (bWasSuccessful && SessionSearch.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("���� �˻� �Ϸ�: %d ���� ���"), SessionSearch->SearchResults.Num());
        // ���⼭ �˻� ���(SessionSearch->SearchResults)�� ���� �޴� �������� �����Ͽ� UI�� ǥ���ϵ��� �� �� �ֽ��ϴ�.
        // ���� ���, �̺�Ʈ�� delegate�� ����Ͽ� ������ ������Ʈ�� Ʈ������ �� �ֽ��ϴ�.
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
            FString::Printf(TEXT("Find : %d Session"), SessionSearch->SearchResults.Num()));

        int32 ArrayIndex = 0;
        for (FOnlineSessionSearchResult& FindSession : SessionSearch->SearchResults)
        {
            FSessionInfo SessionInfo;

            int32 MaxPlayer = FindSession.Session.SessionSettings.NumPublicConnections;
            int32 CurrentPlayer = MaxPlayer - FindSession.Session.NumOpenPublicConnections;
            SessionInfo.ServerIndex = ArrayIndex++;
            SessionInfo.ServerName = "Test Session"; // Session ID
            SessionInfo.MaxPlayerCount = FindSession.Session.SessionSettings.NumPublicConnections; // Max Connection

            // Max Connection - Remain Connection = Current Connection
            SessionInfo.CurrentPlayerCount = SessionInfo.MaxPlayerCount - FindSession.Session.NumOpenPublicConnections; 

            GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
                FString::Printf(TEXT("Session Name : %s, %d/%d"), *FindSession.GetSessionIdStr(), CurrentPlayer, MaxPlayer));

            OnFindSession.Broadcast(SessionInfo);
        }
        
    }
    else
    {

        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
            FString::Printf(TEXT("���� �˻� ����")));
        UE_LOG(LogTemp, Warning, TEXT("���� �˻� ����"));
    }
}
UE_ENABLE_OPTIMIZATION

void USFOnlineSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
    }

    if (Result == EOnJoinSessionCompleteResult::Success)
    {
        FString ConnectString;
        if (SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
        {
            UE_LOG(LogTemp, Log, TEXT("���� ���� ����: %s"), *ConnectString);
            // Ŭ���̾�Ʈ���� ������ ����
            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                PC->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("���� ���� ����: %d"), static_cast<int32>(Result));
    }
}
