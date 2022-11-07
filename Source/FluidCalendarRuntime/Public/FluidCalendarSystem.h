
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Actor.h"
#include "FluidCalendarSystem.generated.h"

class AFluidCalendarTimeTaskNotifier;
class UFluidCalendarDataSet;

//Fluid Toolbox Calendar Stat Group
DECLARE_STATS_GROUP(TEXT("Fluid Calendar"), STATGROUP_FluidCalendar, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Fluid Calendar Tasks"),STAT_FluidCalendar_Tasks,STATGROUP_FluidCalendar);

//Fluid Toolbox Calendar Domain Calendar Day TIme Of Day Condition
UENUM(BlueprintType)
enum class EFluidCalendarDomainCalendarDayTimeOfDayConditionMode : uint8
{
	None,
	UltraDynamicSky
};

//Fluid Toolbox Calendar Domain Schedule Task Day Condition
UENUM(BlueprintType)
enum class EFluidCalendarDomainScheduleTaskDayConditionMode : uint8
{
	None,
	DayID,
	DayIDs
};

//Fluid Toolbox Calendar Domain Schedule Task Day Condition
UENUM(BlueprintType)
enum class EFluidCalendarDomainScheduleTaskIntConditionMode : uint8
{
	None,
	Ranged
};

//Fluid Toolbox Calendar Domain Year
USTRUCT() 
struct FFluidCalendarDomainYear
{ 
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
		FName Suffix;
};

//Fluid Toolbox Calendar Domain Month
USTRUCT() 
struct FFluidCalendarDomainMonth
{ 
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
		FName Name;
	UPROPERTY(EditDefaultsOnly)
		int DayAmount;
};

//Fluid Toolbox Calendar Domain Day
USTRUCT(BlueprintType) 
struct FFluidCalendarDomainTimeStamp
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int Year;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int Month;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int Day;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int Hour;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int Minute;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int Second;
};

//Fluid Toolbox Calendar Domain Time Profile
USTRUCT(BlueprintType) 
struct FFluidCalendarDomainDay
{ 
	GENERATED_BODY()

//Name And Time
public:
	
	UPROPERTY(EditDefaultsOnly)
		FName Name;
	UPROPERTY(EditDefaultsOnly,meta = (ClampMin = "1"))
		int HourCount;
	UPROPERTY(EditDefaultsOnly,meta = (ClampMin = "1"))
		int MinuteCount;
	UPROPERTY(EditDefaultsOnly,meta = (ClampMin = "1"))
		int SecondCount;

	UPROPERTY(EditDefaultsOnly)
		EFluidCalendarDomainCalendarDayTimeOfDayConditionMode TimeOfDayConditionMode;
};

//Fluid Toolbox Calendar Domain Schedule Task
USTRUCT() 
struct FFluidCalendarDomainScheduleTask
{ 
	GENERATED_BODY()

//Name
public:
	UPROPERTY(EditDefaultsOnly)
		FName Name;

//Day Condition
public:
	UPROPERTY(EditDefaultsOnly)
		EFluidCalendarDomainScheduleTaskDayConditionMode DayCondition;
	UPROPERTY(EditDefaultsOnly,  meta=(DisplayName = "Day Value (day ID)", EditCondition="DayCondition == EFluidCalendarDomainScheduleTaskDayConditionMode::DayID", EditConditionHides))
		FName DayConditionDayID;
	UPROPERTY(EditDefaultsOnly,  meta=(DisplayName = "Day Values (day ID)", EditCondition="DayCondition == EFluidCalendarDomainScheduleTaskDayConditionMode::DayIDs", EditConditionHides))
		TArray<FName> DayConditionDayIDs;

//Hour Condition
public:

	UPROPERTY(EditDefaultsOnly)
		EFluidCalendarDomainScheduleTaskIntConditionMode HourCondition;
	UPROPERTY(EditDefaultsOnly,  meta=(DisplayName = "Hour Value Min (hour int)", EditCondition="HourCondition == EFluidCalendarDomainScheduleTaskIntConditionMode::Ranged", EditConditionHides))
		int HourConditionMin;
	UPROPERTY(EditDefaultsOnly,  meta=(DisplayName = "Hour Value Max (hour int)", EditCondition="HourCondition == EFluidCalendarDomainScheduleTaskIntConditionMode::Ranged", EditConditionHides))
		int HourConditionMax;

//Minute Condition
public:

	UPROPERTY(EditDefaultsOnly)
		EFluidCalendarDomainScheduleTaskIntConditionMode MinuteCondition;
	UPROPERTY(EditDefaultsOnly,  meta=(DisplayName = "Minute Value Min (hour int)", EditCondition="MinuteCondition == EFluidCalendarDomainScheduleTaskIntConditionMode::Ranged", EditConditionHides))
		int MinuteConditionMin;
	UPROPERTY(EditDefaultsOnly,  meta=(DisplayName = "Minute Value Max (hour int)", EditCondition="MinuteCondition == EFluidCalendarDomainScheduleTaskIntConditionMode::Ranged", EditConditionHides))
		int MinuteConditionMax;
	
};

//Fluid Toolbox Calendar Domain Schedule Task
USTRUCT() 
struct FFluidCalendarDomainSchedule
{ 
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, meta = (TitleProperty = "Name"))
		TArray<FFluidCalendarDomainScheduleTask> Tasks;
};

//Fluid Toolbox Calendar Domain Calendar
USTRUCT() 
struct FFluidCalendarDomainCalendar
{ 
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
		FName Name;
	UPROPERTY(EditDefaultsOnly)
		FFluidCalendarDomainYear Year;
	UPROPERTY(EditDefaultsOnly)
		TMap<FName,FFluidCalendarDomainMonth> Months;
	UPROPERTY(EditDefaultsOnly)
		TMap<FName,FFluidCalendarDomainDay> Days;
	UPROPERTY(EditDefaultsOnly)
		FFluidCalendarDomainTimeStamp StartingTimeStamp;
};

//Fluid Calendar Calendar Time Task
USTRUCT() 
struct FFluidCalendarTimeTask
{
	GENERATED_BODY()

	//Fluid Calendar Calendar Time Task : Main
	TSubclassOf<AFluidCalendarTimeTaskNotifier> NotifierClass;
	AFluidCalendarTimeTaskNotifier* Notifier;
	FFluidCalendarDomainCalendar* Calendar;
	float TimeRate;
	bool bIsValid;
	
	//Fluid Calendar Calendar Time Task : Date
	float DateSecond;
	float DateMinute;
	float DateHour;
	float DateDay;
	float DateMonth;
	float DateYear;

	//Fluid Toolbox Calendar Task : Day Data;
	FFluidCalendarDomainDay Day;
	FName DayName;
	int DayIndex;
	int DayOfYear;
	int DaySecondCount;
	int DayMinuteCount;
	int DayHourCount;
	bool bDayChanged;
};

//Fluid Calendar Task Notifier
UCLASS(Blueprintable, BlueprintType, abstract)
class FLUIDCALENDARRUNTIME_API AFluidCalendarTimeTaskNotifier : public AActor
{
	GENERATED_BODY()
	
	public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnSecondChanged(FName CalendarTaskID);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnMinuteChanged(FName CalendarTaskID);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnHourChanged(FName CalendarTaskID);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnDayChanged(FName CalendarTaskID);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnMonthChanged(FName CalendarTaskID);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnYearChanged(FName CalendarTaskID);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnUltraDynamicSkyUpdate(float TimeOfDay);
	

	virtual void OnSecondChanged_Implementation(FName CalendarTaskID);
	virtual void OnMinuteChanged_Implementation(FName CalendarTaskID);
	virtual void OnHourChanged_Implementation(FName CalendarTaskID);
	virtual void OnDayChanged_Implementation(FName CalendarTaskID);
	virtual void OnMonthChanged_Implementation(FName CalendarTaskID);
	virtual void OnYearChanged_Implementation(FName CalendarTaskID);
	virtual void OnUltraDynamicSkyUpdate_Implementation(float TimeOfDay);
};

//Fluid Toolbox Calendar Calendar Data Set
UCLASS() 
class FLUIDCALENDARRUNTIME_API UFluidCalendarDataSet : public UDataAsset
{ 
	GENERATED_BODY()

	public:

	UPROPERTY(EditDefaultsOnly)
		FName Name;
	UPROPERTY(EditDefaultsOnly, meta = (displayName = "Priority (lower is better)", ClampMin = "0"))
		float Priority;
	UPROPERTY(EditDefaultsOnly)
		TMap<FName,FFluidCalendarDomainCalendar> Calendars;
	UPROPERTY(EditDefaultsOnly)
		TMap <FName,FFluidCalendarDomainSchedule> Schedules;
};

//Fluid Toolbox Calendar SubSystem
UCLASS()
class FLUIDCALENDARRUNTIME_API UFluidCalendarSubSystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

//Fluid Toolbox Calendar SubSystem : Main
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

	bool bInitialized;

//Fluid Toolbox Calendar SubSystem : Data Set
public:

	UFUNCTION(BlueprintCallable)
		void RegisterDataSet(UFluidCalendarDataSet* DataSet);
	UFUNCTION(BlueprintCallable)
		void RegisterDataSets(TArray<UFluidCalendarDataSet*> DataSets, bool bPrintDataSetNamesByPriority);
	UFUNCTION(BlueprintCallable)
		void PrintDataSetNamesByPriority();

	TArray<UFluidCalendarDataSet*> RegisteredDataSets;
	
//Fluid Toolbox Calendar SubSystem : Calendar Time Task
public:

	UFUNCTION(BlueprintCallable)
		void CreateCalendarTimeTask(const FName NewTaskID, const TSubclassOf<AFluidCalendarTimeTaskNotifier> NotifierClass, const FName CalendarID, const float TimeRate, bool& bIsValid_Out);
	UFUNCTION(BlueprintCallable)
		void DestroyCalendarTimeTask(const FName TaskID, bool& bIsValid_Out);
	UFUNCTION(BlueprintCallable)
		void SetCalendarTimeTaskTimeStamp(const FName TaskID, FFluidCalendarDomainTimeStamp TimeStamp);
	UFUNCTION(BlueprintCallable)
		void SetCalendarTimeTaskTimeStampRaw(const FName TaskID, const int Year, const int Month, const int Day, const int Hour, const int Minute, const int Second);
	UFUNCTION(BlueprintCallable)
		void SetCalendarTimeTaskTimeRate(const FName TaskID, const float TimeRate);
	UFUNCTION(BlueprintCallable)
		void GetCalendarTimeTaskDay(const FName TaskID, FFluidCalendarDomainDay& Day_Out, int& DayIndex_Out,int& DayOfYear_Out);
	void DoCalendarTimeTasks(float DeltaTime);

	TMap<FName,FFluidCalendarTimeTask*> CalendarTimeTasks;

//Fluid Toolbox Calendar SubSystem : Schedule
public:

	UFUNCTION(BlueprintCallable)
		void GetScheduleTask(FName CalendarTimeTaskID, FName ScheduleID, FName& ScheduleTask_Out);
	
};
