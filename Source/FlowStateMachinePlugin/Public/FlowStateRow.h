// JellyfishZero All Rights Reserved
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FlowStateRow.generated.h"

USTRUCT(BlueprintType)
struct FLOWSTATEMACHINEPLUGIN_API FFlowStateRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "State Name"))
	FName StateName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow State")
	TSubclassOf<class UFlowStateBase> StateClass;
};