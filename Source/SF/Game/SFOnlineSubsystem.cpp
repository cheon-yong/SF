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
            FString::Printf(TEXT("SessionInterface가 유효하지 않습니다.")));
        UE_LOG(LogTemp, Warning, TEXT("SessionInterface가 유효하지 않습니다."));
        return;
    }

    // Delegate 바인딩
    OnCreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
        FOnCreateSessionCompleteDelegate::CreateUObject(this, &USFOnlineSubsystem::OnCreateSessionComplete));

    // 세션 설정 구성
    FOnlineSessionSettings SessionSettings;
    
    if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
    {
        SessionSettings.bIsLANMatch = false;
    }
    else
    {
        SessionSettings.bIsLANMatch = true;               // 온라인 매치로 설정 (LAN이면 true로 설정)
    }
    SessionSettings.NumPublicConnections = 2;            // 최대 플레이어 수 (호스트 + 참가자)
    SessionSettings.bShouldAdvertise = true;             // 검색이 가능하도록 설정
    SessionSettings.bAllowJoinInProgress = true;         // 진행 중 참가 허용
    SessionSettings.bAllowJoinViaPresence = true;
    SessionSettings.bUseLobbiesIfAvailable = true;
    SessionSettings.bUsesPresence = true;                // 친구 초대 등 Presence 기능 사용
    SessionSettings.bIsDedicated = false;

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionSettings);
}

void USFOnlineSubsystem::FindGameSessions()
{
    if (!SessionInterface.IsValid())
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
            FString::Printf(TEXT("SessionInterface가 유효하지 않습니다.")));
        UE_LOG(LogTemp, Warning, TEXT("SessionInterface가 유효하지 않습니다."));
        return;
    }

    
    // 검색 조건 설정
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->bIsLanQuery = false;
    SessionSearch->MaxSearchResults = 100;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    // Delegate 바인딩
    OnFindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
        FOnFindSessionsCompleteDelegate::CreateUObject(this, &USFOnlineSubsystem::OnFindSessionsComplete));

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void USFOnlineSubsystem::JoinGameSession(int SessionIndex)
{
    if (!SessionInterface.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("SessionInterface가 유효하지 않습니다."));
        return;
    }

    // Check Index
    if (SessionIndex < 0 || SessionIndex >= SessionSearch->SearchResults.Num() || SessionIndex == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("Session Index가 유효하지 않습니다."));
        return;
    }

    // Delegate 바인딩
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
        UE_LOG(LogTemp, Log, TEXT("세션 '%s' 생성 성공!"), *SessionName.ToString());
        // 생성 성공 시, 지정한 게임 레벨로 listen 옵션과 함께 이동합니다.
        UGameplayStatics::OpenLevel(GetWorld(), FName("Stage"), true, "listen");
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("세션 '%s' 생성 실패!"), *SessionName.ToString());
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
        UE_LOG(LogTemp, Log, TEXT("세션 검색 완료: %d 개의 결과"), SessionSearch->SearchResults.Num());
        // 여기서 검색 결과(SessionSearch->SearchResults)를 메인 메뉴 위젯으로 전달하여 UI에 표시하도록 할 수 있습니다.
        // 예를 들어, 이벤트나 delegate를 사용하여 위젯의 업데이트를 트리거할 수 있습니다.
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
            FString::Printf(TEXT("세션 검색 실패")));
        UE_LOG(LogTemp, Warning, TEXT("세션 검색 실패"));
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
            UE_LOG(LogTemp, Log, TEXT("세션 참가 성공: %s"), *ConnectString);
            // 클라이언트에서 서버로 접속
            APlayerController* PC = GetWorld()->GetFirstPlayerController();
            if (PC)
            {
                PC->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("세션 참가 실패: %d"), static_cast<int32>(Result));
    }
}
