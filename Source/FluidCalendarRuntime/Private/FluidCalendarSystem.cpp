
#include "FluidCalendarSystem.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Calendar SubSystem : Debugging

static TAutoConsoleVariable<int32> CDebugFluidCalendarTimeTask(
	TEXT("Fluid.Calendar.Time.Tasks.Debug"),
	0,
	TEXT("Fluid.Calendar.Time.Tasks.Debug\n")
	TEXT("<=0: off \n")
	TEXT("  1: on\n"),
	ECVF_SetByConsole);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Calendar Task Notifier

void AFluidCalendarTimeTaskNotifier::OnSecondChanged_Implementation(FName CalendarTaskID)
{
}

void AFluidCalendarTimeTaskNotifier::OnMinuteChanged_Implementation(FName CalendarTaskID)
{
}

void AFluidCalendarTimeTaskNotifier::OnHourChanged_Implementation(FName CalendarTaskID)
{
}

void AFluidCalendarTimeTaskNotifier::OnDayChanged_Implementation(FName CalendarTaskID)
{
}

void AFluidCalendarTimeTaskNotifier::OnMonthChanged_Implementation(FName CalendarTaskID)
{
}

void AFluidCalendarTimeTaskNotifier::OnYearChanged_Implementation(FName CalendarTaskID)
{
}

void AFluidCalendarTimeTaskNotifier::OnUltraDynamicSkyUpdate_Implementation(float TimeOfDay)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Calendar SubSystem : Main

void UFluidCalendarSubSystem::Initialize(FSubsystemCollectionBase & Collection)
{
	//Return If Already Initialized | Set Initialized
	if (bInitialized) return;
	bInitialized = true;
}

void UFluidCalendarSubSystem::Deinitialize()
{
}

void UFluidCalendarSubSystem::Tick(float DeltaTime)
{
	//Return If Not Initialized
	if (!bInitialized) return;

	DoCalendarTimeTasks(DeltaTime);
}

bool UFluidCalendarSubSystem::IsTickable() const
{
	//Validate
	return !IsTemplate() && bInitialized;
}

bool UFluidCalendarSubSystem::IsTickableInEditor() const
{
	return false;
}

bool UFluidCalendarSubSystem::IsTickableWhenPaused() const
{
	return false;
}

TStatId UFluidCalendarSubSystem::GetStatId() const
{
	return TStatId();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Calendar SubSystem : Data Set

void UFluidCalendarSubSystem::RegisterDataSet(UFluidCalendarDataSet* DataSet)
{
	//Return If Data Set Is Not Valid
	if (DataSet == nullptr)
		return;
	
	//Add Data Set
	RegisteredDataSets.AddUnique(DataSet);

	//Sort Data Sets By Priority (Bubble Sort, Lower Is Better)
	{
		int Iterations = RegisteredDataSets.Num() - 1;
		while (Iterations > 0)
		{
			for (int32 x = 0; x > RegisteredDataSets.Num() - 1; x++)
				if ( RegisteredDataSets [x]->Priority > RegisteredDataSets [x + 1]->Priority)
					RegisteredDataSets.Swap(x, x + 1);
			Iterations--;
		}
	}
}

void UFluidCalendarSubSystem::RegisterDataSets(TArray<UFluidCalendarDataSet*>  DataSets,  bool bPrintDataSetNamesByPriority)
{
	//Register Data Sets
	for (int i = 0; i < DataSets.Num(); i++)
		RegisterDataSet(DataSets[i]);

	//Print Data Set Names By Priority
	if (bPrintDataSetNamesByPriority)
		PrintDataSetNamesByPriority();
}

void UFluidCalendarSubSystem::PrintDataSetNamesByPriority()
{
	//Print Data Sets By Priority (We Loop Backwards Since Print Is Displayed Backwards)
	for (int i = RegisteredDataSets.Num(); i--; )
		GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,RegisteredDataSets[i]->Name.ToString());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Calendar SubSystem : Time Task

void UFluidCalendarSubSystem::CreateCalendarTimeTask(const FName NewTaskID, const TSubclassOf<AFluidCalendarTimeTaskNotifier> NotifierClass, const FName CalendarID, const float TimeRate, bool& bIsValid_Out)
{
	//Return If Time Task Is Already Created
	bIsValid_Out = false;
	if (CalendarTimeTasks.Contains(NewTaskID))
		return;
		
	//Create Time Task
	FFluidCalendarTimeTask* Task = new FFluidCalendarTimeTask();
	CalendarTimeTasks.Add(NewTaskID,Task);
	Task->TimeRate = TimeRate;
	Task->NotifierClass = NotifierClass;
	
	//Find Calendar And Make Sure Its Valid
	for (int i = 0; i <  RegisteredDataSets.Num(); i++)
	{
		if (RegisteredDataSets[i]->Calendars.Contains(CalendarID) && RegisteredDataSets[i]->Calendars[CalendarID].Days.Num() != 0 && RegisteredDataSets[i]->Calendars[CalendarID].Months.Num() != 0)
		{
			Task->Calendar = &RegisteredDataSets[i]->Calendars[CalendarID];
			Task->bDayChanged = true; // Changed Calendar So We Need To Update Current Day Data
			Task->bIsValid = true;

			//Set Starting Time Stamp
			SetCalendarTimeTaskTimeStamp(NewTaskID, Task->Calendar->StartingTimeStamp);
		}
	}
}

void UFluidCalendarSubSystem::DestroyCalendarTimeTask(const FName TaskID, bool& bIsValid_Out)
{
	//Return If Time Task Is Not Created | Get Task
	bIsValid_Out = false;
	if (!CalendarTimeTasks.Contains(TaskID))
		return;

	//Remove Calendar Time Task
	CalendarTimeTasks.Remove(TaskID);
	bIsValid_Out = true;
}


void UFluidCalendarSubSystem::SetCalendarTimeTaskTimeStamp(const FName TaskID, FFluidCalendarDomainTimeStamp TimeStamp)
{
	// Set Calendar Time Task Time Stamp Raw
	SetCalendarTimeTaskTimeStampRaw(TaskID, TimeStamp.Year, TimeStamp.Month,TimeStamp.Day,TimeStamp.Hour,TimeStamp.Minute,TimeStamp.Second);
}

void UFluidCalendarSubSystem::SetCalendarTimeTaskTimeStampRaw(const FName TaskID, const int Year, const int Month, const int Day, const int Hour, const int Minute, const int Second)
{
	//Return If Time Task Is Not Created | Get Task
	if (!CalendarTimeTasks.Contains(TaskID))
		return;
	FFluidCalendarTimeTask* Task = CalendarTimeTasks[TaskID];

	//Retern As Task Is Not Valid
	if(!Task->bIsValid)
		return;
	
	//Get Months Array From Months Map
	TArray<FFluidCalendarDomainMonth> MonthsAsArray;
	Task->Calendar->Months.GenerateValueArray(MonthsAsArray);
	
	//Set Task Time Stamp
	Task->DateYear = Year;
	Task->DateMonth = FMath::Clamp(Month,0,Task->Calendar->Months.Num() - 1);
	Task->DateDay = FMath::Clamp(Day,0,MonthsAsArray[Task->DateMonth].DayAmount);
	Task->DateHour = FMath::Clamp(Hour,1,Day); // Force 1 Min To Prevent Crashing In Time Task
	Task->DateMinute = FMath::Clamp(Minute,1,Minute); // Force 1 Min To Prevent Crashing In Time Task
	Task->DateSecond = FMath::Clamp(Month,1,60); // Force 1 Min To Prevent Crashing In Time Task
	Task->bDayChanged = true;
}

void UFluidCalendarSubSystem::SetCalendarTimeTaskTimeRate(const FName TaskID, const float TimeRate)
{
	//Return If Time Task Is Not Created | Get Task
	if (!CalendarTimeTasks.Contains(TaskID))
		return;
	FFluidCalendarTimeTask* Task = CalendarTimeTasks[TaskID];

	//Set Task Time Rate
	Task->TimeRate = TimeRate;
}

void UFluidCalendarSubSystem::GetCalendarTimeTaskDay(const FName TaskID, FFluidCalendarDomainDay& Day_Out, int& DayIndex_Out, int& DayOfYear_Out)
{
	//Return If Time Task Is Not Created | Get Task
	if (!CalendarTimeTasks.Contains(TaskID))
		return;
	FFluidCalendarTimeTask* Task = CalendarTimeTasks[TaskID];

	//Retern As Task Is Not Valid
	if(!Task->bIsValid)
		return;

	//Get Months Array From Months Map
	TArray<FFluidCalendarDomainMonth> MonthsAsArray;
	Task->Calendar->Months.GenerateValueArray(MonthsAsArray);
	
	//Get Days Array From Months Map
	TArray<FFluidCalendarDomainDay> DaysAsArray;
	Task->Calendar->Days.GenerateValueArray(DaysAsArray);
	
	//Loop Through The Year From Beginning To Find Our Day Index Related To Day In Month (Int)
	int DayIndex = 0;
	int DayIter = 0;
	bool bFinished = false;
	for (int i = 0; i < MonthsAsArray.Num(); i++)
	{
		for (int j = 0; j < MonthsAsArray[i].DayAmount; j++)
		{
			//Have Not Found Our Date Yet So Iterate Day
			if (!bFinished)
			{
				DayIter++;
				DayIndex++;
				if (DayIndex > DaysAsArray.Num() - 1)
					DayIndex = 0;
			}

			//Found Our Day So Mark As Finished
			if ((int)Task->DateMonth == i && (int)Task->DateDay == j)
				bFinished = true;
		}
	}
	
	Day_Out = DaysAsArray[DayIndex];
	DayIndex_Out = DayIndex;
}

void UFluidCalendarSubSystem::DoCalendarTimeTasks(float DeltaTime)
{
	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidCalendar_Tasks);
	{
		for (const TPair<FName, FFluidCalendarTimeTask*>& Pair : CalendarTimeTasks)
		{
			//Get Task
			const auto Name = Pair.Key;
			const auto Task = Pair.Value;

			//Debug
			{
				//Debug Time Stamp (Valid)
				if (CDebugFluidCalendarTimeTask.GetValueOnAnyThread() != 0 && Task->bIsValid)
				{
					//Get Months Array From Months Map
					TArray<FFluidCalendarDomainMonth> MonthsAsArray;
					Task->Calendar->Months.GenerateValueArray(MonthsAsArray);
					
					GEngine->AddOnScreenDebugMessage(-1,0,FColor::Green, "Time Rate : " + FString::FromInt(Task->TimeRate));
					GEngine->AddOnScreenDebugMessage(-1,0,FColor::Green, "Second : " + FString::FromInt(Task->DateSecond));
					GEngine->AddOnScreenDebugMessage(-1,0,FColor::Green, "Minute : " + FString::FromInt(Task->DateMinute));
					GEngine->AddOnScreenDebugMessage(-1,0,FColor::Green, "Hour : " + FString::FromInt(Task->DateHour));
					GEngine->AddOnScreenDebugMessage(-1,0,FColor::Green, "Day : " + FString::FromInt(Task->DateDay) + " : " + Task->DayName.ToString());
					GEngine->AddOnScreenDebugMessage(-1,0,FColor::Green, "Month : " + FString::FromInt(Task->DateMonth) + " : " + MonthsAsArray[Task->DateMonth].Name.ToString());
					GEngine->AddOnScreenDebugMessage(-1,0,FColor::Green, "Year : " + FString::FromInt(Task->DateYear) + " " + Task->Calendar->Year.Suffix.ToString());
					GEngine->AddOnScreenDebugMessage(-1,0,FColor::White, "Fluid Calendar Time Task : " + Name.ToString());
				}

				//Debug Time Stamp (Invalid)
				if (CDebugFluidCalendarTimeTask.GetValueOnAnyThread() != 0 && !Task->bIsValid)
					GEngine->AddOnScreenDebugMessage(-1,0,FColor::Red, "Fluid Calendar Time Task : " + Name.ToString() + " (Invalid)");
			}
			
			//Retern As Task Is Not Valid
			if(!Task->bIsValid)
				continue;

			//Make Sure Notifier Is Created If Class Valid (Could Be Destroyed On World Changed And So On)
			if (Task->NotifierClass != nullptr && Task->Notifier == nullptr)
				Task->Notifier = (AFluidCalendarTimeTaskNotifier*)GetWorld()->SpawnActor(Task->NotifierClass);
			
			//Get Months Array From Months Map
			TArray<FFluidCalendarDomainMonth> MonthsAsArray;
			Task->Calendar->Months.GenerateValueArray(MonthsAsArray);
			
			//Day Changed
			if (Task->bDayChanged)
			{
				//Get Day
				FFluidCalendarDomainDay Day = FFluidCalendarDomainDay();
				int DayIndex = 0;
				int DayOfYear = 0;
				GetCalendarTimeTaskDay(Name,Day,DayIndex,DayOfYear);
				
				//Set Day
				Task->Day = Day;
				Task->DayName = Day.Name;;
				Task->DayIndex = DayIndex;
				Task->DayOfYear = DayOfYear;
				Task->DaySecondCount = Day.SecondCount;
				Task->DayMinuteCount = Day.MinuteCount;
				Task->DayHourCount = Day.HourCount;
				Task->bDayChanged = false;
			}
			
			//Ultra Dynamic Sky
			if (Task->Day.TimeOfDayConditionMode == EFluidCalendarDomainCalendarDayTimeOfDayConditionMode::UltraDynamicSky)
			{
				if (Task->Notifier)
				{
					const int ToltalSecondsInDay = Task->Day.SecondCount * Task->Day.MinuteCount * Task->Day.HourCount;
					int CurrentSecondOfDay = 0;
					for (int i = 0; i < Task->DateHour - 1; i++)
						CurrentSecondOfDay += Task->Day.MinuteCount * Task->Day.SecondCount;
					for (int i = 0; i < Task->DateMinute - 1; i++)
						CurrentSecondOfDay += Task->Day.SecondCount;
					CurrentSecondOfDay += Task->DateSecond;

					float TimeOfDay = (float)CurrentSecondOfDay / (float)ToltalSecondsInDay * 2400;
					Task->Notifier->OnUltraDynamicSkyUpdate(TimeOfDay); //0 - 2400 Is Time Of Day In Ultra Dynamic Sky
				}
			}
			
			//Time Forward
			if (Task->TimeRate > 0)
			{
				//Second
				Task->DateSecond += DeltaTime * Task->TimeRate;
				if (Task->Notifier)
					Task->Notifier->OnSecondChanged(Name);
				
				if (Task->DateSecond > Task->DaySecondCount - 1)
				{
					Task->DateSecond = 0;

					//Minute
					Task->DateMinute+=  1 + (Task->TimeRate * 0.001f);
					if (Task->Notifier)
						Task->Notifier->OnMinuteChanged(Name);
					if (Task->DateMinute > Task->DayMinuteCount - 1)
					{
						Task->DateMinute = 0;

						//Hour
						Task->DateHour+= 1 + (Task->TimeRate * 0.00001f);
						if (Task->Notifier)
							Task->Notifier->OnHourChanged(Name);
						if (Task->DateHour > Task->DayHourCount - 1)
						{
							Task->DateHour = 0;

							//Day
							Task->DateDay+= 1 + (Task->TimeRate * 0.0000001f);
							Task->bDayChanged = true;
							if (Task->Notifier)
								Task->Notifier->OnDayChanged(Name);
							
							if (Task->DateDay > MonthsAsArray[Task->DateMonth].DayAmount - 1)
							{
								Task->DateDay = 0;

								//Month
								Task->DateMonth++;
								if (Task->Notifier)
									Task->Notifier->OnMonthChanged(Name);
								
								if (Task->DateMonth > Task->Calendar->Months.Num() - 1)
								{
									Task->DateMonth = 0;

									//Year
									Task->DateYear++;
									if (Task->Notifier)
										Task->Notifier->OnYearChanged(Name);
								}
							}
						}
					}
				}
			}

			//Time Backward
			if (Task->TimeRate < 0)
			{
				//Second
				Task->DateSecond += DeltaTime * Task->TimeRate;
				if (Task->Notifier)
					Task->Notifier->OnSecondChanged(Name);
				
				if (Task->DateSecond < 0)
				{
					Task->DateSecond = Task->DaySecondCount - 1;

					//Minute
					Task->DateMinute -=  1 - (Task->TimeRate * 0.001f);
					if (Task->Notifier)
						Task->Notifier->OnMinuteChanged(Name);
					
					if (Task->DateMinute < 0)
					{
						Task->DateMinute = Task->DayMinuteCount - 1;

						//Hour
						Task->DateHour -= 1 - (Task->TimeRate * 0.00001f);
						if (Task->Notifier)
							Task->Notifier->OnHourChanged(Name);
						
						if (Task->DateHour < 0)
						{
							Task->DateHour = Task->DayHourCount - 1;

							//Day
							Task->DateDay -= 1 - (Task->TimeRate * 0.0000001f);;
							Task->bDayChanged = true;
							if (Task->Notifier)
								Task->Notifier->OnDayChanged(Name);
							
							if (Task->DateDay < 0)
							{
								Task->DateDay = MonthsAsArray[Task->DateMonth].DayAmount - 1;

								//Month
								Task->DateMonth--;
								if (Task->Notifier)
									Task->Notifier->OnMonthChanged(Name);
								
								if (Task->DateMonth < 0)
								{
									Task->DateMonth = Task->Calendar->Months.Num() -1;

									//Year
									Task->DateYear--;
									if (Task->Notifier)
										Task->Notifier->OnYearChanged(Name);
								}
							}
						}
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Calendar SubSystem : Schedule

void UFluidCalendarSubSystem::GetScheduleTask(FName CalendarTimeTaskID, FName ScheduleID, FName& ScheduleTask_Out)
{
	//Start With Task None
	ScheduleTask_Out = "None";

	//Get Calendar Task
	FFluidCalendarTimeTask* CalendarTimeTask = nullptr;
	if (!CalendarTimeTasks.Contains(CalendarTimeTaskID))
		return;
	CalendarTimeTask = CalendarTimeTasks[CalendarTimeTaskID];
	
	//Find Schedule Tasks
	TArray<FFluidCalendarDomainScheduleTask> ScheduleTasks;
	for (int i = 0; i < RegisteredDataSets.Num(); i++)
		if (RegisteredDataSets[i]->Schedules.Contains(ScheduleID))
		{
			ScheduleTasks = RegisteredDataSets[i]->Schedules[ScheduleID].Tasks;
			break;
		}
	
	//Find First Schedule Task That Fits Conditions
	bool bPassed = true;
	for (int i = 0; i < ScheduleTasks.Num(); i++)
	{
		//Start With We Passed By Default
		bPassed = true;
		
		//Day Condition : ID
		if (bPassed && ScheduleTasks[i].DayCondition == EFluidCalendarDomainScheduleTaskDayConditionMode::DayID)
			if (CalendarTimeTask->DayName != ScheduleTasks[i].DayConditionDayID)
				bPassed = false;

		//Day Condition : IDs
		if (bPassed && ScheduleTasks[i].DayCondition == EFluidCalendarDomainScheduleTaskDayConditionMode::DayIDs)
			if (!ScheduleTasks[i].DayConditionDayIDs.Contains(CalendarTimeTask->DayName))
				bPassed = false;

		//Hour Condition : Int Range
		if (bPassed && ScheduleTasks[i].HourCondition == EFluidCalendarDomainScheduleTaskIntConditionMode::Ranged)
			if ((int)CalendarTimeTask->DateHour < ScheduleTasks[i].HourConditionMin || (int)CalendarTimeTask->DateHour > ScheduleTasks[i].HourConditionMax)
				bPassed = false;

		//Minute Condition : Int Range
		if (bPassed && ScheduleTasks[i].MinuteCondition == EFluidCalendarDomainScheduleTaskIntConditionMode::Ranged)
			if ((int)CalendarTimeTask->DateMinute < ScheduleTasks[i].MinuteConditionMin || (int)CalendarTimeTask->DateMinute > ScheduleTasks[i].MinuteConditionMax)
				bPassed = false;
		
		//Still Passing So Return As Task
		if (bPassed)
		{
			ScheduleTask_Out = ScheduleTasks[i].Name;
			return;
		}
	}
}