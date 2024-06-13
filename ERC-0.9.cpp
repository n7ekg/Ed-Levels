#include <iterator>
#include <vector>

#include "sierrachart.h"
#include "scstudyfunctions.h"
#include "scstudyfunctions.cpp"

#define EC_HLC 0
#define EC_HL 1
#define EC_OPEN 2
#define EC_HIGH 3
#define EC_LOW 4
#define EC_LAST 5
#define EC_BE 6
#define EC_SE 7
#define EC_PROJHI 8
#define EC_PROJLO 9
#define EC_END 9

#define MH5 EC_END+1
#define MH4 EC_END+2
#define MH3 EC_END+3
#define ML3 EC_END+4
#define ML4 EC_END+5
#define ML5 EC_END+6

char *ML[] = { "Murali H5", "Murali H4", "Murali H3", "Murali L3", "Murali L4", "Murali L5" };

SCDLLName("Ed's Studies v0.9")
// Derived from 0.5c

/*==========================================================================*/
SCSFExport scsf_EdDailyTargets(SCStudyInterfaceRef sc)
{
	SCInputRef UseDaySessionOnly=sc.Input[7]; /* for today's open */
	SCInputRef UseDaySessionOnly2=sc.Input[8]; /* for yesterday's high/low */
	SCInputRef GraphOpenCloseHistorically = sc.Input [9];
	SCInputRef DisplayOnDaySessionOnly = sc.Input [10];
	SCInputRef DrawRetracement = sc.Input [11];
	SCInputRef FibConfigNumber = sc.Input [12];
	SCInputRef DrawEdsTargets = sc.Input [13];
	SCInputRef DrawMuralisTargets = sc.Input [14];
	SCInputRef DrawHiToLo = sc.Input [15];
	SCInputRef UsePivotPoint = sc.Input [16];
	SCInputRef DaysBack = sc.Input[17];
	SCInputRef PersistentDrawing = sc.Input[18];
	int i;
	s_UseTool Tool;

	// sc.AddMessageToLog("Startup", 1);
	if (sc.SetDefaults)
	{
		sc.GraphName = "Ed's Daily Targets v0.9";
		
		sc.ScaleRangeType = SCALE_SAMEASREGION;
		sc.GraphRegion = 0;
		sc.ValueFormat = VALUEFORMAT_INHERITED;

		//During development set this flag to 1, so the DLL can be modified. When development is completed, set it to 0 to improve performance.
		sc.FreeDLL = 1;
		sc.AutoLoop = 1;  //Auto looping is enabled. 

		sc.Subgraph[EC_HLC].Name= "Target High";
		sc.Subgraph[EC_HLC].DrawStyle = DRAWSTYLE_DASH;
		sc.Subgraph[EC_HLC].PrimaryColor = RGB(0,255,255);
		sc.Subgraph[EC_HLC].LineWidth = 3;
		sc.Subgraph[EC_HLC].DrawZeros = false;
		sc.Subgraph[EC_HLC].LineLabel = LL_DISPLAY_NAME | LL_NAME_ALIGN_CENTER | LL_NAME_ALIGN_RIGHT;

		sc.Subgraph[EC_HL].Name= "Target Low";
		sc.Subgraph[EC_HL].DrawStyle = DRAWSTYLE_DASH;
		sc.Subgraph[EC_HL].PrimaryColor = RGB(0,255,255);
		sc.Subgraph[EC_HL].LineWidth = 3;
		sc.Subgraph[EC_HL].DrawZeros = false;
		sc.Subgraph[EC_HL].LineLabel = LL_DISPLAY_NAME | LL_NAME_ALIGN_CENTER | LL_NAME_ALIGN_RIGHT;
		
		sc.Subgraph[EC_OPEN].Name = "Today's Open";
		sc.Subgraph[EC_OPEN].DrawStyle = DRAWSTYLE_DASH;
		sc.Subgraph[EC_OPEN].PrimaryColor = RGB(255,255,255);
		sc.Subgraph[EC_OPEN].LineWidth = 2;
		sc.Subgraph[EC_OPEN].DrawZeros = false;
		sc.Subgraph[EC_OPEN].LineLabel = LL_DISPLAY_NAME | LL_NAME_ALIGN_CENTER | LL_NAME_ALIGN_RIGHT;

		sc.Subgraph[EC_HIGH].Name = "Yesterday's High";
		sc.Subgraph[EC_HIGH].DrawStyle = DRAWSTYLE_DASH;
		sc.Subgraph[EC_HIGH].PrimaryColor = RGB(0,255,0);
		sc.Subgraph[EC_HIGH].LineWidth = 2;
		sc.Subgraph[EC_HIGH].DrawZeros = false;
		sc.Subgraph[EC_HIGH].LineLabel = LL_DISPLAY_NAME | LL_NAME_ALIGN_CENTER | LL_NAME_ALIGN_RIGHT;

		sc.Subgraph[EC_LOW].Name = "Yesterday's Low";
		sc.Subgraph[EC_LOW].DrawStyle = DRAWSTYLE_DASH;
		sc.Subgraph[EC_LOW].PrimaryColor = RGB(255,0,0);
		sc.Subgraph[EC_LOW].LineWidth = 2;
		sc.Subgraph[EC_LOW].DrawZeros = false;
		sc.Subgraph[EC_LOW].LineLabel = LL_DISPLAY_NAME | LL_NAME_ALIGN_CENTER | LL_NAME_ALIGN_RIGHT;

		sc.Subgraph[EC_LAST].Name = "Yesterday's Close";
		sc.Subgraph[EC_LAST].DrawStyle = DRAWSTYLE_DASH;
		sc.Subgraph[EC_LAST].PrimaryColor = RGB(255,128,0);
		sc.Subgraph[EC_LAST].LineWidth = 2;
		sc.Subgraph[EC_LAST].DrawZeros = false;
		sc.Subgraph[EC_LAST].LineLabel = LL_DISPLAY_NAME | LL_NAME_ALIGN_CENTER | LL_NAME_ALIGN_RIGHT;
		
		sc.Subgraph[EC_BE].Name = "In-Trend Buy Entry";
		sc.Subgraph[EC_BE].DrawStyle = DRAWSTYLE_DASH;
		sc.Subgraph[EC_BE].LineStyle = LINESTYLE_DASH;
		sc.Subgraph[EC_BE].PrimaryColor = RGB(0,255,0);
		sc.Subgraph[EC_BE].LineWidth = 2;
		sc.Subgraph[EC_BE].DrawZeros = false;
		sc.Subgraph[EC_BE].LineLabel = LL_DISPLAY_NAME | LL_NAME_ALIGN_CENTER | LL_NAME_ALIGN_RIGHT;
		
		sc.Subgraph[EC_SE].Name = "In-Trend Sell Entry";
		sc.Subgraph[EC_SE].DrawStyle = DRAWSTYLE_DASH;
		sc.Subgraph[EC_BE].LineStyle = LINESTYLE_DASH;
		sc.Subgraph[EC_SE].PrimaryColor = RGB(255,0,0);
		sc.Subgraph[EC_SE].LineWidth = 2;
		sc.Subgraph[EC_SE].DrawZeros = false;
		sc.Subgraph[EC_SE].LineLabel = LL_DISPLAY_NAME | LL_NAME_ALIGN_CENTER | LL_NAME_ALIGN_RIGHT;

		sc.Subgraph[EC_PROJHI].Name = "Projected High";
		sc.Subgraph[EC_PROJHI].DrawStyle = DRAWSTYLE_DASH;
		sc.Subgraph[EC_PROJHI].PrimaryColor = RGB(0,255,0);
		sc.Subgraph[EC_PROJHI].LineWidth = 2;
		sc.Subgraph[EC_PROJHI].DrawZeros = false;
		sc.Subgraph[EC_PROJHI].LineLabel = LL_DISPLAY_NAME | LL_NAME_ALIGN_CENTER | LL_NAME_ALIGN_RIGHT;

		sc.Subgraph[EC_PROJLO].Name = "Projected Low";
		sc.Subgraph[EC_PROJLO].DrawStyle = DRAWSTYLE_DASH;
		sc.Subgraph[EC_PROJLO].PrimaryColor = RGB(255,0,0);
		sc.Subgraph[EC_PROJLO].LineWidth = 2;
		sc.Subgraph[EC_PROJLO].DrawZeros = false;
		sc.Subgraph[EC_PROJLO].LineLabel = LL_DISPLAY_NAME | LL_NAME_ALIGN_CENTER | LL_NAME_ALIGN_RIGHT;
		
		/*
		sc.Subgraph[EC_OHLC].Name = "OHLC Avg";
		sc.Subgraph[EC_OHLC].DrawStyle = DRAWSTYLE_HIDDEN;
		sc.Subgraph[EC_OHLC].PrimaryColor = RGB(128,128,128);
		sc.Subgraph[EC_OHLC].DrawZeros = false;
		*/

		for (i=0; i<6; i++)
		{
			sc.Subgraph[MH5+i].Name= ML[i];
			sc.Subgraph[MH5+i].DrawStyle = DRAWSTYLE_DASH;
			sc.Subgraph[MH5+i].PrimaryColor = RGB(0,255,0);
			sc.Subgraph[MH5+i].LineWidth = 3;
			sc.Subgraph[MH5+i].DrawZeros = false;
			sc.Subgraph[MH5+i].LineLabel = LL_DISPLAY_NAME | LL_NAME_ALIGN_CENTER | LL_NAME_ALIGN_RIGHT;
		}

		sc.Input[0].Name = "Use This Intraday Chart";
		sc.Input[0].SetYesNo(1);

		sc.Input[1].Name = "Daily Chart #";
		sc.Input[1].SetChartNumber(1);
		
		sc.Input[4].Name = "Use Saturday Data";
		sc.Input[4].SetYesNo(0);
		
		sc.Input[6].Name = "Round to Tick Size";
		sc.Input[6].SetYesNo(1);

		UseDaySessionOnly2.Name = "Use Day Session For Yesterday's High/Low";
		UseDaySessionOnly2.SetYesNo(0);

		DisplayOnDaySessionOnly.  Name = "Display on Day Session Only";
		DisplayOnDaySessionOnly.  SetYesNo(0);
		 
		DrawRetracement.Name = "Draw Fibonacci Price Retracement On Chart";
		DrawRetracement.SetYesNo(1);
		
		FibConfigNumber.Name = "Fibonacci Price Retracement Configuration Number";
		FibConfigNumber.SetInt(2);
		FibConfigNumber.SetIntLimits(1,8);

		DrawEdsTargets.Name = "Draw Ed's Targets On Chart";
		DrawEdsTargets.SetYesNo(1);
		
		DrawMuralisTargets.Name = "Draw Murali's Levels On Chart";
		DrawMuralisTargets.SetYesNo(1);
		
		DrawHiToLo.Name = "Draw Fib High to Low";
		DrawHiToLo.SetYesNo(1);
		
		UsePivotPoint.Name = "Use Pivot Point Instead of Midpoint (unused)";
		UsePivotPoint.SetYesNo(0);
		
		DaysBack.Name = "Number of lookback days to calculate high/low (unused)";
		DaysBack.SetInt(3);
		
		PersistentDrawing.Name = "Persist Fib Drawing";
		PersistentDrawing.SetYesNo(0);
		
		// sc.AddMessageToLog("Defaults set", 1);

		return;
	}

	if (sc.HideStudy == 1)
		return;
	
	int InUseThisIntradayChart = sc.Input[0].GetYesNo();
	int InDailyChartNumber = sc.Input[1].GetInt();
	int InNumberOfDaysBack = 1; // sc.Input[2].GetInt();
	int InGraphHLHistorically = 0; // sc.Input[3].GetYesNo();
	int InUseSaturdayData = 0; /* sc.Input[4].GetYesNo(); */
	int InNumberOfDaysToCalculate = 1; // sc.Input[5].GetInt();
	int InRoundToTickSize = sc.Input[6].GetYesNo();
	int InDrawRetracement = sc.Input[11].GetYesNo();
	int InFibConfigNumber = sc.Input[12].GetInt();
	int InDrawEdsTargets = sc.Input[13].GetInt();
	int InDrawMuralisTargets = sc.Input[14].GetInt();
	int InDrawHiToLo = sc.Input[15].GetYesNo();
	int InUsePivotPoint = sc.Input[16].GetYesNo();
	int inPersistDrawing = sc.Input[18].GetYesNo();
	long LineNumber;
	

	float Open = 0.0f, High = 0.0f, Low = 0.0f, Close = 0.0f;
	float YOpen = 0.0f, YHigh = 0.0f, YLow = 0.0f, YClose = 0.0f;
	float TargetHigh = 0.0f, TargetLow = 0.0f;
	float BuyEntry = 0.0f, SellEntry = 0.0f;
	float PivotPoint = 0.0f;
	float Offset = 0.0f;
	float Midpoint = 0.0f;
	float Volume = 0.0f;
	float LastOpen = 0.0f;
	int IntradayChartDate = 0;
	int IsValid = 1;
	float AvgHi = 0.0f, AvgLo = 0.0f, AvgCl = 0.0f;
	float ProjHi = 0.0f, ProjLo = 0.0f;
	int DaysLookBack = sc.Input[17].GetInt();
	char scratchmsg[255];

	// we get chart data only once for speed
	SCGraphData DailyChartData;
	SCDateTimeArray DailyChartDateTimes;
	LineNumber = 76521;
	sc.DataStartIndex = 0;
	// sc.AddMessageToLog("InUseThisIntradayChart", 1);
	if (!InUseThisIntradayChart)
	{
		sc.GetChartBaseData(InDailyChartNumber, DailyChartData);
		sc.GetChartDateTimeArray(InDailyChartNumber, DailyChartDateTimes);
	}
	
	float HighestHigh = -FLT_MAX, LowestLow = FLT_MAX;
	
	int StartIndex = sc.UpdateStartIndex;
	if (StartIndex != 0)
	{
		SCDateTime TradingDayStartDateTime = sc.GetTradingDayStartDateTimeOfBar(sc.BaseDateTimeIn[StartIndex]);
		StartIndex = sc.GetFirstIndexForDate(sc.ChartNumber, TradingDayStartDateTime.GetDate());

	}

	//This is used by the Study/Price Overlay study and Spreadsheet study.
	sc.EarliestUpdateSubgraphDataArrayIndex = StartIndex;

	// sc.AddMessageToLog("CalculateDailyOHLC Start", 0);

		int Index = sc.ArraySize - 1;

		SCDateTime BarDateTime = sc.BaseDateTimeIn[Index];
		
		int IntradayChartTradingDayDate = sc.GetTradingDayDate(sc.BaseDateTimeIn[Index]);
		
		if (IntradayChartDate != IntradayChartTradingDayDate)
		{
			IntradayChartDate = IntradayChartTradingDayDate;
		}

			/*
			IsValid = 
				CalculateDailyOHLC(
					sc,
					IntradayChartDate,
					0, // InNumberOfDaysBack - just need today's open
					InNumberOfDaysToCalculate,
					InUseSaturdayData,
					InUseThisIntradayChart,
					InDailyChartNumber,
					DailyChartData,
					DailyChartDateTimes,
					0, // UseDaySessionOnly = False
					MidnightOpen,
					High,
					Low,
					Close
				);
				*/
			
			IsValid = 
				CalculateDailyOHLC(
					sc,
					IntradayChartDate,
					0, // InNumberOfDaysBack - just need today's open
					InNumberOfDaysToCalculate,
					InUseSaturdayData,
					InUseThisIntradayChart,
					InDailyChartNumber,
					DailyChartData,
					DailyChartDateTimes,
					1, // UseDaySessionOnly = True
					Open,
					High,
					Low,
					Close,
					Volume,
					0,
					0
				);
			
				YHigh = 0.0f;
				YLow = 0.0f;
				YClose = 0.0f;
				AvgHi = 0.0f;
				AvgLo = 0.0f;
				AvgCl = 0.0f;
				for (i=0; i<DaysLookBack; i++)
					{
					IsValid = 
					CalculateDailyOHLC(
						sc,
						IntradayChartDate,
						i+1, /* need yesterday's high and low */
						/* InNumberOfDaysBack, */
							InNumberOfDaysToCalculate,
							InUseSaturdayData,
							InUseThisIntradayChart,
							InDailyChartNumber,
							DailyChartData,
							DailyChartDateTimes,
							UseDaySessionOnly2.GetYesNo(),
							YOpen,
							YHigh,
							YLow,
							YClose,
							Volume,
							1,
							0
							);
							AvgHi = AvgHi + YHigh;
							AvgLo = AvgLo + YLow;
							AvgCl = AvgCl + YClose;
					};
				ProjHi = ((((AvgHi/DaysLookBack)+(AvgLo/DaysLookBack)+(AvgCl/DaysLookBack))/DaysLookBack)*2)-(AvgLo/DaysLookBack);
				ProjLo = ((((AvgHi/DaysLookBack)+(AvgLo/DaysLookBack)+(AvgCl/DaysLookBack))/DaysLookBack)*2)-(AvgHi/DaysLookBack);
				/*
				sprintf(scratchmsg, "AH=%f, AL=%f, AC=%f, DB=%d, ProjHi=%f, ProjLo=%f", AvgHi, AvgLo, AvgCl, DaysBack, ProjHi, ProjLo);
				sc.AddMessageToLog(scratchmsg, 1);
				*/
				
			IsValid = 
				CalculateDailyOHLC(
					sc,
					IntradayChartDate,
					1, /* need yesterday's high and low */
					/* InNumberOfDaysBack, */
					InNumberOfDaysToCalculate,
					InUseSaturdayData,
					InUseThisIntradayChart,
					InDailyChartNumber,
					DailyChartData,
					DailyChartDateTimes,
					UseDaySessionOnly2.GetYesNo(),
					YOpen,
					YHigh,
					YLow,
					YClose,
					Volume,
					0,
					0
				);
				
			HighestHigh = -FLT_MAX;//sc.High[Index];
			LowestLow = FLT_MAX;//sc.Low[Index];
		
		// sc.AddMessageToLog("CalculateDailyOHLC End", 0);

		// if (!IsValid)
// 			continue;

		float SubgraphOpen = Open;
		float SubgraphHigh = YHigh;
		float SubgraphLow = YLow;
		float SubgraphClose = YClose;
		float SubgraphTargetHigh = TargetHigh;
		float SubgraphTargetLow = TargetLow;
		float SubgraphBuyEntry = BuyEntry;
		float SubgraphSellEntry = SellEntry;
		float SubgraphProjHi = ProjHi;
		float SubgraphProjLo = ProjLo;
		
		/*
		Midpoint = ((H(m) - L(m)) / 2) + L(m)
        Offset = TodayOpen(m) - Midpoint
        HITGT(m) = Format(H(m) + (Offset), MKTMask(m))
'        Fib2(m) = Format(((H(m) - L(m)) * 0.736) + L(m) + Offset, MKTMask(m))
'        Fib3(m) = Format(((H(m) - L(m)) * 0.618) + L(m) + Offset, MKTMask(m))
'        Fib4(m) = Format(((H(m) - L(m)) * 0.5) + L(m) + Offset, MKTMask(m))
'        Fib5(m) = Format(((H(m) - L(m)) * 0.382) + L(m) + Offset, MKTMask(m))
'        Fib6(m) = Format(((H(m) - L(m)) * 0.236) + L(m) + Offset, MKTMask(m))
        LOTGT(m) = Format(L(m) + (Offset), MKTMask(m))
        HITGT50(m) = ((HITGT(m) - TodayOpen(m)) / 2) + TodayOpen(m)
        LOTGT50(m) = ((TodayOpen(m) - LOTGT(m)) / 2) + LOTGT(m)
		*/

		if (YHigh > 0 && YLow > 0 && Open > 0)
		{
			Midpoint = ((YHigh - YLow) / 2) + YLow;
			PivotPoint = (YHigh + YLow + YClose) / 3; /* From CalculatePivotPoints() in scstudyfunctions.cpp */
			Offset = Open - Midpoint;
			TargetHigh = (YHigh + Offset);
			TargetLow = (YLow + Offset);
			BuyEntry = ((TargetHigh - TargetLow) * .618) + TargetLow;
			SellEntry = ((TargetHigh - TargetLow) * .382) + TargetLow;
			/* Extended targets */
			// TargetHigh = Midpoint + (((YHigh - YLow) / 2) * 1.236) + Offset;
			// TargetLow = Midpoint - (((YHigh - YLow) / 2) * 1.236) + Offset;
			/* Short targets */
			TargetHigh = Midpoint + (((YHigh - YLow) / 2) * .82) + Offset;
			TargetLow = Midpoint - (((YHigh - YLow) / 2) * .82) + Offset;

			/*
			sprintf(scratchmsg, "OP=%f, YHI=%f, YLO=%f, Mid=%f, Off=%f, Diff=%f, TH=%f, TL=%f", Open, YHigh, YLow, Midpoint, Offset, (YHigh - YLow) / 2, TargetHigh, TargetLow);
			sc.AddMessageToLog(scratchmsg, 1);
			*/
			
		}
			
		if (InRoundToTickSize)
		{
			SubgraphOpen = (float)sc.RoundToTickSize(Open, sc.TickSize);
			SubgraphHigh = (float)sc.RoundToTickSize(YHigh, sc.TickSize);
			SubgraphLow = (float)sc.RoundToTickSize(YLow, sc.TickSize);
			SubgraphClose = (float)sc.RoundToTickSize(YClose, sc.TickSize);
			SubgraphTargetHigh = (float)sc.RoundToTickSize(TargetHigh, sc.TickSize);
			SubgraphTargetLow = (float)sc.RoundToTickSize(TargetLow, sc.TickSize);
			SubgraphBuyEntry = (float)sc.RoundToTickSize(BuyEntry, sc.TickSize);
			SubgraphSellEntry = (float)sc.RoundToTickSize(SellEntry, sc.TickSize);
			SubgraphProjHi = (float)sc.RoundToTickSize(ProjHi, sc.TickSize);
			SubgraphProjLo = (float)sc.RoundToTickSize(ProjLo, sc.TickSize);
		}
		else
		{
			SubgraphOpen = Open;
			SubgraphHigh = YHigh;
			SubgraphLow = YLow;
			SubgraphClose = YClose;
			SubgraphTargetHigh = TargetHigh;
			SubgraphTargetLow = TargetLow;
			SubgraphBuyEntry = BuyEntry;
			SubgraphSellEntry = SellEntry;
			SubgraphProjHi = ProjHi;
			SubgraphProjLo = ProjLo;
		}


		if (  DisplayOnDaySessionOnly.GetYesNo() && !sc.IsDateTimeInDaySession (BarDateTime) ) // Display yesterday's HL, open and targets only if in day session
		{
			SubgraphOpen = 0;
			SubgraphHigh = 0;
			SubgraphLow = 0;
			SubgraphClose = 0;
			SubgraphTargetHigh = 0;
			SubgraphTargetLow = 0;
			SubgraphBuyEntry = 0;
			SubgraphSellEntry = 0;
			SubgraphProjHi = 0;
			SubgraphProjLo = 0;
		}
		
		if (!sc.IsDateTimeInDaySession (BarDateTime))
		{	
			SubgraphOpen = 0; // Don't display open if not in session
			SubgraphTargetHigh = 0;
			SubgraphTargetLow = 0;
			SubgraphBuyEntry = 0;
			SubgraphSellEntry = 0;
			SubgraphProjHi = 0;
			SubgraphProjLo = 0;
		}

		if (InDrawEdsTargets)
		{
			sc.Subgraph[EC_OPEN][Index] = SubgraphOpen;
			sc.Subgraph[EC_HIGH][Index] = SubgraphHigh;
			sc.Subgraph[EC_LOW][Index] = SubgraphLow;
			sc.Subgraph[EC_LAST][Index] = SubgraphClose;
			sc.Subgraph[EC_HLC][Index] = SubgraphTargetHigh;
			sc.Subgraph[EC_HL][Index] = SubgraphTargetLow;
			sc.Subgraph[EC_BE][Index] = SubgraphBuyEntry;
			sc.Subgraph[EC_SE][Index] = SubgraphSellEntry;
			sc.Subgraph[EC_PROJHI][Index] = SubgraphProjHi;
			sc.Subgraph[EC_PROJLO][Index] = SubgraphProjLo;
		}
		
		/*
		        L3(m) = Format(C(m) - (1.1 * (H(m) - L(m)) / 4), MKTMask(m))
                L4(m) = Format(C(m) - (1.1 * (H(m) - L(m)) / 2), MKTMask(m))
                L5(m) = Format(C(m) - ((H(m) / L(m) * C(m)) - C(m)), MKTMask(m))
                H3(m) = Format((1.1 * (H(m) - L(m)) / 4) + C(m), MKTMask(m))
                H4(m) = Format((1.1 * (H(m) - L(m)) / 2) + C(m), MKTMask(m))
                H5(m) = Format((H(m) / L(m)) * C(m), MKTMask(m))
		*/
		if (InDrawMuralisTargets)
		{
			sc.Subgraph[MH5][Index] = (float)sc.RoundToTickSize((YHigh / YLow) * YClose, sc.TickSize); /* H5 */
			sc.Subgraph[MH4][Index] = (float)sc.RoundToTickSize((1.1 * (YHigh - YLow) / 2) + YClose, sc.TickSize); /* H4 */
			sc.Subgraph[MH3][Index] = (float)sc.RoundToTickSize((1.1 * (YHigh - YLow) / 4) + YClose, sc.TickSize); /* H3 */
			sc.Subgraph[ML3][Index] = (float)sc.RoundToTickSize(YClose - (1.1 * (YHigh - YLow) / 4), sc.TickSize); /* L3 */
			sc.Subgraph[ML4][Index] = (float)sc.RoundToTickSize(YClose - (1.1 * (YHigh - YLow) / 2), sc.TickSize); /* L4 */
			sc.Subgraph[ML5][Index] = (float)sc.RoundToTickSize(YClose - ((YHigh / YLow * YClose) - YClose), sc.TickSize); /* L5 */
		}
		/*
		sc.Subgraph[EC_LAST][Index] = SubgraphClose;


		sc.CalculateOHLCAverages( Index);
		*/
		
	// sc.AddMessageToLog("Before Retracement", 1);
	TargetHigh = (float)sc.RoundToTickSize(TargetHigh, sc.TickSize);
	TargetLow = (float)sc.RoundToTickSize(TargetLow, sc.TickSize);
	// if (InDrawRetracement && SubgraphTargetHigh > 0 && SubgraphTargetLow > 0 && !sc.ChartDrawingExists(sc.ChartNumber, LineNumber) && sc.HideStudy == 0 && SubgraphOpen > 0)
	if (InDrawRetracement && SubgraphTargetHigh > 0 && SubgraphTargetLow > 0 && sc.HideStudy == 0 && SubgraphOpen > 0)
		{
			// int BarIndex;

			Tool.Clear();
			Tool.ChartNumber = sc.ChartNumber;
			Tool.DrawingType = DRAWING_RETRACEMENT;
			Tool.LineNumber = LineNumber;
			Tool.AddMethod = UTAM_ADD_OR_ADJUST;
			// BarIndex = max(BarIndex,0);
			Tool.EndDateTime = sc.BaseDateTimeIn[1];
			Tool.BeginDateTime = sc.BaseDateTimeIn[sc.ArraySize - 1];
			if (inPersistDrawing)
				Tool.AddAsUserDrawnDrawing = 1;

			else
				Tool.AddAsUserDrawnDrawing = 0;
			/*
			if (InDrawHiToLo)
			{
				if (InUsePivotPoint)
					Tool.BeginValue = PivotPoint;
				else
					Tool.BeginValue = Midpoint;
				Tool.EndValue = YHigh;
			}
			else
			{
				Tool.BeginValue = YLow;
				if (InUsePivotPoint)
					Tool.EndValue = PivotPoint;
				else
					Tool.EndValue = Midpoint;
			}
			*/
			if (!InDrawHiToLo)
			{
				Tool.BeginValue = TargetHigh;
				Tool.EndValue = TargetLow;
			}
			else
			{
				Tool.BeginValue = TargetLow;
				Tool.EndValue = TargetHigh;
			}
			Tool.UseToolConfigNum = InFibConfigNumber;
			if (InRoundToTickSize)
				Tool.RoundToTickSize = 1;
			// sc.AddMessageToLog("Pre-Retrace", 1);
			sc.UseTool(Tool);
			// sc.AddMessageToLog("Post-Retrace", 1);

			/*
			sprintf(scratchmsg, "Retrace: AS=%d, LO=%f, OP=%f, YHI=%f, YLO=%f, Mid=%f, Off=%f, TH=%f, TL=%f", sc.ArraySize, LastOpen, Open, YHigh, YLow, Midpoint, Offset, TargetHigh, TargetLow);
			sc.AddMessageToLog(scratchmsg, 1);
			*/
			
		}
		else
		{
			if (sc.ChartDrawingExists(sc.ChartNumber, LineNumber) && !inPersistDrawing)
				sc.DeleteACSChartDrawing(sc.ChartNumber, DRAWING_RETRACEMENT, LineNumber);
		}
}
