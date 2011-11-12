/************************************************************************
 * Copyright(c) 2011, One Unified. All rights reserved.                 *
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

#pragma once

#include "FrameMain.h"
#include "PanelSimulationControl.h"
#include "PanelFinancialChart.h"

#include "Strategy.h"

class AppStrategy1:
  public wxApp
{
public:
protected:
private:

  Strategy* m_pStrategy;

  FrameMain* m_pFrameMain;
  PanelSimulationControl* m_pPanelSimulationControl;
  PanelFinancialChart* m_pPanelFinancialChart;

  wxWindow* m_winChart;

  virtual bool OnInit();
  virtual int OnExit();

  void HandleBtnSimulationStart( void );

};

// Implements MyApp& wxGetApp()
DECLARE_APP(AppStrategy1)

