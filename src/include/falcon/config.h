/*
   FALCON - The Falcon Programming Language.
   FILE: config.h

   This file is partially generated before compile time
   by the configuration process, or manually finetuned.
   -------------------------------------------------------------------
   Author: Giancarlo Niccolai
   Begin: gio giu 7 2007

   -------------------------------------------------------------------
   (C) Copyright 2004: the FALCON developers (see list in AUTHORS file)

   See LICENSE file for licensing details.
*/

/** \file
   Falcon compile time configuration file.
   This file is partially generated before compile time by the configuration process,
   or manually finetuned.
*/

#ifndef FLC_CONFIG_H
#define FLC_CONFIG_H

//============================================
// Version informations
//
#define FALCON_VERSION_NUM          ( 0 <<24 | 9 <<16 | 6 << 8 | 5 )
#define FALCON_VERSION              "0.9.6.7"
#define FALCON_VERSION_NAME         "Chimera"

//============================================
// Version RCINFO
//

#define FALCON_VERSION_RCINFO 	    "0, 9, 6, 7"
#define FALCON_VERSION_RCINFO_N 	    0, 9, 6, 7

//============================================
// System informations
//

// Basic system. May be either
//   - FALCON_SYSTEM_UNIX
//   - FALCON_SYSTEM_WIN
//   - FALCON_SYSTEM_MAC

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#  define FALCON_SYSTEM_WIN
#  define FALCON_HOST_SYSTEM        "WINDOWS"
#else
#  define FALCON_SYSTEM_UNIX
#  define FALCON_HOST_SYSTEM        "LINUX"
#endif

#define FALCON_LITTLE_ENDIAN        1

// TODO: add support for apple and other weird platforms


//============================================
// Config informations
//
#ifdef FALCON_SYSTEM_WIN
   #define FALCON_DEFAULT_BIN          ""
   #define FALCON_DEFAULT_LIB          ""
   #define FALCON_DEFAULT_CONFIG       ""
   #define FALCON_DEFAULT_LOAD_PATH    ""
#else
   #define FALCON_DEFAULT_BIN          ""
   #define FALCON_DEFAULT_LIB          ""
   #define FALCON_DEFAULT_CONFIG       ""
   #define FALCON_DEFAULT_LOAD_PATH    ""
#endif

#endif

/* end of config.h */
