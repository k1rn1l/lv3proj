/*
   FALCON - The Falcon Programming Language.
   FILE: sys_unix.cpp

   System specific (unix) support for VM.
   -------------------------------------------------------------------
   Author: Giancarlo Niccolai
   Begin: mar nov 9 2004

   -------------------------------------------------------------------
   (C) Copyright 2004: the FALCON developers (see list in AUTHORS file)

   See LICENSE file for licensing details.
*/

/** \file
   Short description
*/

#ifdef __APPLE__
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#else
extern "C"
{
   extern char **environ;
}
#endif

#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <falcon/setup.h>
#include <falcon/types.h>
#include <falcon/memory.h>
#include <falcon/sys.h>
#include <falcon/string.h>
#include <falcon/transcoding.h>

namespace Falcon {
namespace Sys {

void _dummy_ctrl_c_handler()
{
}

numeric _seconds()
{
   struct timeval time;
   gettimeofday( &time, 0 );
   return time.tv_sec + (time.tv_usec / 1000000.0 );
}


numeric _localSeconds()
{
   struct timeval current;
   struct tm date_local, date_gm;
   time_t t;

   gettimeofday( &current, 0 );
   time( &t );
   localtime_r( &t, &date_local );
   gmtime_r( &t, &date_gm );
   time_t leap = mktime( &date_local) - mktime( &date_gm );

   return leap + current.tv_sec + (current.tv_usec / 1000000.0 );
}

uint32 _milliseconds()
{
#if POSIX_TIMERS > 0
   struct timespec time;
   clock_gettime( CLOCK_REALTIME, &time );
   return time.tv_sec * 1000 + time.tv_nsec / 1000000;
#else
   struct timeval time;
   gettimeofday( &time, 0 );
   return time.tv_sec * 1000 + time.tv_usec / 1000;
#endif
}

int64 _epoch()
{
   return (int64) time(0);
}

void _tempName( String &res )
{
   static bool first = true;
   const char *temp_dir;
   struct stat st;

   if( first ) {
      first = false;
      time_t t;
      srand( (unsigned int ) time( &t ) );
   }

   temp_dir = getenv( "TMP" );

   if ( temp_dir == 0 )
      temp_dir = getenv( "TMPDIR" );

   if ( temp_dir == 0 ) {
      temp_dir = DEFAULT_TEMP_DIR;
   }

   if ( stat( temp_dir, &st ) == -1 || ! S_ISDIR( st.st_mode ) ) {
      temp_dir = ".";
   }

   res = temp_dir;
   res.append( "/falcon_tmp_" );
   res.writeNumber( (int64) getpid() );
   res.append("_");
   res.writeNumber( (int64) rand() );
   res.bufferize();
   // force buffering
}

bool _describeError( int64 eid, String &target )
{
   const char *error = strerror( eid );
   if( error != 0 ) {
      target.bufferize( error );
      return true;
   }

   return false;
}

int64 _lastError()
{
   return (int64) errno;
}

bool _getEnv( const String &var, String &result )
{
   static char convertBuf[512]; // system var names larger than 512 are crazy.
   // in unix system, we have at worst UTF-8 var names.
   if ( var.toCString( convertBuf, 512 ) >= 0 )
   {
      char *value = getenv( convertBuf );
      if ( value != 0 )
      {
         TranscodeFromString( value, "utf-8", result );
         return true;
      }
   }

   return false;
}

bool _setEnv( const String &var, const String &value )
{
   // in unix system, we have at worst UTF-8 var names.
   uint32 varLen = var.length() * 4 + 2;
   uint32 valueLen = value.length() * 4 + 2;
   char *varBuf = (char *) memAlloc( varLen );
   char *valueBuf = (char *) memAlloc( valueLen );

   var.toCString( varBuf, varLen );
   value.toCString( valueBuf, valueLen );

   bool result = setenv( varBuf, valueBuf, 1 ) == 0;
   memFree( varBuf );
   memFree( valueBuf );
   return result;
}

bool _unsetEnv( const String &var )
{
   // in unix system, we have at worst UTF-8 var names.
   uint32 varLen = var.length() * 4 + 2;
   char *varBuf = (char *) memAlloc( varLen );

   var.toCString( varBuf, varLen );

   /* currently unsetenv does not return in darwin;
      we need sometime to find a solution
   bool result = unsetenv( varBuf ) == 0;
   */
   bool result = true;
   unsetenv( varBuf );
   memFree( varBuf );
   return result;
}

void _enumerateEnvironment( EnvStringCallback cb, void* cbData )
{
   // do we know which encoding are we using?
   String enc;
   bool bTranscode = GetSystemEncoding( enc ) && enc != "C";

   char** env = environ;
   while( *env != 0 )
   {
      String temp;
      if( bTranscode )
      {
         if( ! TranscodeFromString( *env, enc, temp ) )
         {
            bTranscode = false;
            temp = *env;
         }
      }
      else
         temp = *env;

      uint32 pos;
      if ( (pos = temp.find( "=" )) != String::npos )
      {
         cb( temp.subString(0,pos), temp.subString(pos+1), cbData );
      }

      ++env;
   }
}

}
}

/* end of sys_unix.cpp */
