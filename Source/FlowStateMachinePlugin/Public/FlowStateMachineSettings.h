// JellyfishZero All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FlowStateMachineSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig)
class FLOWSTATEMACHINEPLUGIN_API UFlowStateMachineSettings : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, config, Category = "Game State Machine")
	TSoftObjectPtr<UDataTable> StateTable;
};
