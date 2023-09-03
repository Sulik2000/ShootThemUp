// Shoot Them Up Game. All rights are reserved.


#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUPlayerState, All, All)

void ASTUPlayerState::LogInfo()
{
    UE_LOG(LogSTUPlayerState, Display, TEXT("=======Stats=======\nTeamID: %i\nKills: %i\nDeaths: %i"), TeamID, KillsNum, DeathsNum);
}
