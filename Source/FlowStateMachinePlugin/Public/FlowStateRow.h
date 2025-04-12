// JellyfishZero All Rights Reserved
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FlowStateBase.h"
#include "Templates/SubclassOf.h"
#include "FlowStateRow.generated.h"

USTRUCT(BlueprintType)
struct FLOWSTATEMACHINEPLUGIN_API FFlowStateRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FFlowStateRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flow State")
	TSubclassOf<UFlowStateBase> StateClass;
};