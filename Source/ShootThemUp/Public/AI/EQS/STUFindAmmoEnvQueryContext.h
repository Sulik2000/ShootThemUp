// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "STUFindAmmoEnvQueryContext.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API USTUFindAmmoEnvQueryContext : public UEnvQueryContext
{
    GENERATED_BODY()

  protected:
    virtual void ProvideContext(FEnvQueryInstance &QueryInstance, FEnvQueryContextData &ContextData) const override;
};
