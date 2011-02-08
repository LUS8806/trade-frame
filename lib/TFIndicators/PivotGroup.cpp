/************************************************************************
 * Copyright(c) 2010, One Unified. All rights reserved.                 *
 *                                                                      *
 * This file is provided as is WITHOUT ANY WARRANTY                     *
 *  without even the implied warranty of                                *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                *
 *                                                                      *
 * This software may not be used nor distributed without proper license *
 * agreement.                                                           *
 *                                                                      *
 * See the file LICENSE.txt for redistribution information.             *
 ************************************************************************/

#include "StdAfx.h"

#include "PivotGroup.h"

namespace ou { // One Unified
namespace tf { // TradeFrame

CPivotGroup::CPivotGroup(void) 
 {
}

CPivotGroup::CPivotGroup( CBars *pBars ) {
  CalculatePivotSets( pBars );
}

CPivotGroup::~CPivotGroup(void) {
  m_mmPivots.clear();
}

void CPivotGroup::AddToMap( CPivotSet &set ) {
  for ( unsigned short ix = 0; ix < CPivotSet::PivotCount; ++ix ) {
    m_mmPivots.insert( set[ ix ] );
  }
}

void CPivotGroup::CalculatePivotSets(CBars *pBars) {

  CBar *pBar = pBars->Last();

  //stringstream ss;
  date dtThisDay = pBar->DateTime().date();
  //ss << "This Day: " << dtThisDay;
  date dtPrevDay = ( boost::date_time::Monday == dtThisDay.day_of_week() ) 
    ? dtThisDay - days( 3 )
    : dtThisDay - days( 1 );
  //ss << ", Prev Day: " << dtPrevDay;
  date dtPrevMonday = dtThisDay + days( boost::date_time::Monday - dtThisDay.day_of_week() ) - days( 7 );
  //ss << ", Prev Monday: " << dtPrevMonday;
  date dtPrevWeekFriday = dtPrevMonday + days( 4 );
  //ss << ", Prev Friday: " << dtPrevWeekFriday;
  date dtPrevMonthEnd = dtThisDay - days( dtThisDay.day().as_number() );
  //ss << ", Prev Month End: " << dtPrevMonthEnd;
  date dtPrevMonth = dtPrevMonthEnd - days( dtPrevMonthEnd.day().as_number() - 1 );
  //ss << ", Prev Month: " << dtPrevMonth;
  date dtMonthAgo = dtThisDay - days( 30 );
  //ss << ", Month ago: " << dtMonthAgo;
  date dtSixMonthsAgo = dtThisDay - months( 6 );
  //ss << ", Six Months ago: " << dtSixMonthsAgo;
  //date dt200DaysAgo = dtThisDay - days( 42 * 7 - 2 );
  date dt200BarsAgo = dtThisDay - days( ( ( 200 /*bars wanted*/ / 5 /*days/wk*=#wks*/ ) * 7 /*days/wk*/ ) + 10 /*trade holidays*/ ); // ensures 200 bars is within reasonable time frame
  //ss << ", 200 Days ago: " << dt200DaysAgo;

  CPivotSet Pivot1Day( "pv1Dy", pBar->High(), pBar->Low(), pBar->Close() );
  AddToMap( Pivot1Day );

  double day3hi = pBar->High();
  double day3lo = pBar->Low();
  double day3cl = pBar->Close();

  if ( pBars->Size() >= 3 ) {
    pBar = pBars->Ago( 1 );
    day3hi = max( day3hi, pBar->High() );
    day3lo = min( day3lo, pBar->Low() );
    pBar = pBars->Ago( 2 );
    day3hi = max( day3hi, pBar->High() );
    day3lo = min( day3lo, pBar->Low() );
    CPivotSet Pivot3Day( "pv3Dy", day3hi, day3lo, day3cl );
    AddToMap( Pivot3Day );
  }

  CBars *pBarsForPeriod;

  if ( pBars->Size() >= 10 ) {
    pBarsForPeriod = pBars->Subset( ptime( dtPrevMonday ), 5 );
    CPivotSet PivotWeek( "pvWk", pBarsForPeriod );
    AddToMap( PivotWeek );
    delete pBarsForPeriod;
  }
  if ( pBars->Size() >= 20 ) {
    pBarsForPeriod = pBars->Subset( ptime( dtMonthAgo ), 20 );
    CPivotSet Pivot20Bars( "pv20B", pBarsForPeriod );
    AddToMap( Pivot20Bars );
    delete pBarsForPeriod;
  }
  if ( pBars->Size() >= 42 ) {
    pBarsForPeriod = pBars->Subset( ptime( dtPrevMonth ), 20 ); 
    CPivotSet PivotMonth( "pvMn", pBarsForPeriod );
    AddToMap( PivotMonth );
    delete pBarsForPeriod;
  }
  if ( pBars->Size() >= 200 ) {
    pBarsForPeriod = pBars->Subset( ptime( dt200BarsAgo ), 200 );
    CPivotSet Pivot200Bars( "pv200B", pBarsForPeriod );
    AddToMap( Pivot200Bars );
    delete pBarsForPeriod;
  }

}

} // namespace tf
} // namespace ou