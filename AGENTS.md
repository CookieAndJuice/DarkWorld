# AGENTS.md

This file provides guidance to Codex (Codex.ai/code) when working with code in this repository.

## Project

**DarkWorld** — UE 5.7, single C++ runtime module, Soulslike 3rd-person action RPG. Engine version is set in `DarkWorld.uproject`.

## Build

Open `DarkWorld.uproject` in Unreal Editor (UE 5.7). There is no standalone build script — compilation is driven by UnrealBuildTool via the editor or CLI:

```bash
# Rebuild C++ (editor closed)
"C:/Program Files/Epic Games/UE_5.7/Engine/Build/BatchFiles/Build.bat" DarkWorldEditor Win64 Development "D:/UnrealEngine/Project/DarkWorld/DarkWorld.uproject"

# Generate project files
"C:/Program Files/Epic Games/UE_5.7/Engine/Build/BatchFiles/GenerateProjectFiles.bat" "D:/UnrealEngine/Project/DarkWorld/DarkWorld.uproject" -game
```

There are no automated tests in this project.

## Module Layout (`Source/DarkWorld/`)

| Directory | Contents |
|---|---|
| `AI/` | BTService/Task/Decorator nodes, `IDWEnemyAIInterface`, `DWBBKey.h` |
| `Animation/` | `UDWPlayerAnimInstance`, `UDWKhaimeraAnimInstance`, AnimNotifies for combo and hit detection |
| `Character/Base/` | `ADWCharacterBase`, `UDWCharacterBaseComponent`, `UDWCharacterStatComponent`, `IDWCombatInterface` |
| `Character/Player/` | `ADWPlayerCharacter` + move/combat/stat/lock-on components |
| `Character/Enemy/` | `ADWKhaimeraEnemy`, `UDWEnemyStatComponent` |
| `Data/` | `UDWCharacterAppearanceData`, `UDWPlayerControlData`, `UDWAttackComboAnimData` (all `UPrimaryDataAsset`) |
| `GameCore/` | `ADWGameMode`, `ADWPlayerController`, `ADWAIController` |
| `Helper/` | `DWCollisionName.h` (profile/channel macros), `DWLogCategories.h`, `LogDebugger.h` |
| `StatData/` | Stat row structs (`FDWCharacterCommonStatData`, `FDWPlayerDefaultStatData`, `FDWEnemyStatData`), `UDWCharacterStatManager` singleton |
| `UI/` | `UDWHpBarWidget` |

## Architecture

### Character Hierarchy

```
ACharacter
└── ADWCharacterBase          (mesh/capsule setup, CharacterAppearanceData)
    ├── ADWPlayerCharacter    (implements IDWCombatInterface; SpringArm + Camera)
    └── ADWKhaimeraEnemy      (implements IDWEnemyAIInterface; BT + Blackboard refs)
```

### Component Pattern

Every character subsystem is a `UDWCharacterBaseComponent` subclass. The base provides:
- `ADWCharacterBase* OwnerCharacter` back-reference
- Virtual `SetInputBinding()` — only player-side components implement this
- `BeginPlay()` / `InitializeComponent()` hooks

Player components:
- `UDWPlayerMoveComponent` — Enhanced Input binding for Move/Look/Jump/Run; walk (275)/run (400) speed switching
- `UDWPlayerCombatComponent` — montage-based 3-hit combo; delegates to `IDWCombatInterface` methods on the character
- `UDWPlayerStatComponent` — extends `UDWCharacterStatComponent`; reads from `DWPlayerDefaultStatTable.csv`
- `UDWPlayerLockOnComponent` — view-angle/range overlap scan; sets `bIsLockOn` on `UDWPlayerAnimInstance`

Enemy component:
- `UDWEnemyStatComponent` — extends `UDWCharacterStatComponent`; adds `DetectRange`, `Groggy`; init from `UDWEnemyStatData`

### Stat System

`UDWCharacterStatManager` is a **GameSingleton** (registered in `DefaultEngine.ini`). Access via `UDWCharacterStatManager::Get()`. Player base stats come from `GameData/DWPlayerDefaultStatTable.csv`; enemy stats come from `UDWEnemyStatData` (DataAsset). Damage application flows through `UDWCharacterStatComponent::ApplyDamage()`, which fires `FOnHpZeroDelegate` on death and `FOnHpChangedDelegate` on every change — `UDWHpBarWidget` binds to these.

### AI System

`ADWAIController` takes `GetBehaviorTree()` / `GetBlackboardData()` from `IDWEnemyAIInterface` on possess. BT nodes:
- `UBTService_DetectPlayer` — sphere overlap each tick interval → writes `BBKEY_TARGET` ("Target") to blackboard
- `UBTDecorator_AttackInRange` — evaluates `IDWEnemyAIInterface::GetAttackRange()` against distance to target
- `UBTTask_Attack` — triggers attack montage

### Combat Flow

1. Player input → `UDWPlayerCombatComponent::StartAttack()` → play montage section `"Attack_N"`
2. `UAnimNotify_PlayerComboCheck` (AnimNotifyState) → calls `IDWCombatInterface::CombatCheckCombo()` during the window
3. `UAnimNotify_CheckHitByAttack` (AnimNotifyState) → activates weapon overlap on `CPROFILE_DWPLAYERWEAPON` mesh
4. Hit registered → `ADWKhaimeraEnemy::TakeDamage()` → `UDWEnemyStatComponent::ApplyDamage()`

### Data Assets

Loaded at construction via `ConstructorHelpers::FObjectFinder` and live under `/Game/Dark_World/Data/`. Prefixed `DA_` in Content. Three types:
- `UDWCharacterAppearanceData` — mesh, skeleton, capsule dims, spring arm/camera offsets
- `UDWPlayerControlData` — movement flags, rotation rates, spring arm settings, `InputMappingContext` ref
- `UDWAttackComboAnimData` — `AttackMontage`, section name prefix, max combo count, frame rate

### Collision

All collision identifiers are macros in `Helper/DWCollisionName.h`:

```cpp
CPROFILE_DWPLAYERCAPSULE   // player capsule preset
CPROFILE_DWENEMYCAPSULE    // enemy capsule preset
CPROFILE_DWPLAYERWEAPON    // weapon mesh (hit detection)
CCHANNEL_DWPLAYER          // ECC_GameTraceChannel1
CCHANNEL_DWENEMY           // ECC_GameTraceChannel2
```

Custom channel numbers must match the `DefaultEngine.ini` collision channel registrations.

### Logging

Use the project macros from `Helper/LogDebugger.h` — they embed `__FUNCTION__` automatically:

```cpp
UELOG_L(DWCombat, TEXT("msg %s"), *Value);   // Log
UELOG_W(DWCombat, TEXT("msg %s"), *Value);   // Warning
UELOG_E(DWCombat, TEXT("msg %s"), *Value);   // Error
```

Log categories: `DWCombat`, `LogDWCharacterStatManager` (declared in `DWLogCategories.h`).

## Naming Conventions

- All project types carry the `DW` namespace prefix.
- Standard UE prefixes apply: `A` actors, `U` UObjects/components, `I` interfaces, `F` structs, `E` enums.
- Data asset content files: `DA_` prefix (e.g., `DA_DWPlayerControl`).
- Blackboard key strings live in `DWBBKey.h` as `constexpr` constants — never hardcode string literals in BT nodes.