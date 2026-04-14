
#pragma once

// ─── UE_LOG ───────────────────────────────────────────────────────────────────
#define UELOG_L(LogName, Format, ...) UE_LOG(LogName, Log,     TEXT("[%s] ") Format, *FString(__FUNCTION__), ##__VA_ARGS__)
#define UELOG_W(LogName, Format, ...) UE_LOG(LogName, Warning, TEXT("[%s] ") Format, *FString(__FUNCTION__), ##__VA_ARGS__)
#define UELOG_E(LogName, Format, ...) UE_LOG(LogName, Error,   TEXT("[%s] ") Format, *FString(__FUNCTION__), ##__VA_ARGS__)

// ─── On-Screen ────────────────────────────────────────────────────────────────
// Key = -1  : 호출마다 새 줄 추가
// Key = 정수: 같은 Key면 같은 자리 갱신 (Tick 출력용)
#define UESCREEN_L(Color, Format, ...)		 if(GEngine) GEngine->AddOnScreenDebugMessage(-1,  3.f, Color,  FString::Printf(Format, ##__VA_ARGS__))
#define UESCREEN_W(Color, Format, ...)		 if(GEngine) GEngine->AddOnScreenDebugMessage(-1,  3.f, Color, FString::Printf(Format, ##__VA_ARGS__))
#define UESCREEN_E(Color, Format, ...)		 if(GEngine) GEngine->AddOnScreenDebugMessage(-1,  3.f, Color,    FString::Printf(Format, ##__VA_ARGS__))
#define UESCREEN_K(Key, Color, Format, ...) if(GEngine) GEngine->AddOnScreenDebugMessage(Key, 3.f, Color,   FString::Printf(Format, ##__VA_ARGS__))
