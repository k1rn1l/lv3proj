/*
   FALCON - The Falcon Programming Language.
   FILE: rampmode.cpp

   Ramp mode - progressive GC limits adjustment algoritmhs
   -------------------------------------------------------------------
   Author: Giancarlo Niccolai
   Begin: Thu, 19 Mar 2009 08:23:59 +0100

   -------------------------------------------------------------------
   (C) Copyright 2009: the FALCON developers (see list in AUTHORS file)

   See LICENSE file for licensing details.
*/

#include <falcon/rampmode.h>
#include <falcon/memory.h>
#include <falcon/mempool.h>

namespace Falcon {

RampMode::~RampMode()
{}

void RampMode::reset()
{
}

//=========================================================
// A void ramp mode.

RampNone::~RampNone()
{
}

void RampNone::onScanInit()
{
    m_active = memPool->thresholdActive();
    m_normal = memPool->thresholdNormal();
}

void RampNone::onScanComplete()
{
}

//=========================================================
//



RampStrict::~RampStrict()
{
}

void RampStrict::onScanInit()
{
}

void RampStrict::onScanComplete()
{
   m_active = gcMemAllocated();
   m_normal = m_active/2;
}


//=========================================================
//


RampLoose::~RampLoose()
{
}

void RampLoose::onScanInit()
{
   m_normal = gcMemAllocated();
   m_active = m_normal * 2;
}

void RampLoose::onScanComplete()
{
    m_normal = (m_normal + gcMemAllocated()) / 2;
}

//=========================================================
//

RampSmooth::RampSmooth( numeric factor ):
   RampMode(),
   m_pNormal(0),
   m_pActive(0),
   m_factor( factor )
{
   if ( m_factor < 1.0 )
      m_factor = 1.0;
}


RampSmooth::~RampSmooth()
{
}

void RampSmooth::reset()
{
   m_pNormal = 0;
}


void RampSmooth::onScanInit()
{
   // on the first loop, we setup the waiting loops.
   if ( m_pNormal == 0 )
   {
      m_pNormal = gcMemAllocated();
      m_normal = m_pNormal;
      m_active = (size_t)(m_normal * m_factor);
   }
}

void RampSmooth::onScanComplete()
{
   // size_t is usually unsigned.
   size_t allocated = gcMemAllocated();
   if( m_pNormal > allocated )
   {
       // we're getting smaller
       m_pNormal -= (size_t)((m_pNormal - allocated) / m_factor);
   }
   else {
       m_pNormal += size_t((allocated-m_pNormal) / m_factor);
   }

   m_normal = m_pNormal;
   m_active = (size_t)(m_normal * m_factor);
}

}

/* end of rampmode.cpp */
