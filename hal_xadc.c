/*    Copyright (C) 2017 Cameron McQuinn
 *
 *    Based on hal_skeleton.c,
 *    by John Kasunich, Copyright (C) 2003
 *
 *
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

/* Machinekit includes */
#include "rtapi.h"		/* RTAPI realtime OS API */
#include "rtapi_app.h"		/* RTAPI realtime module decls */

#include "hal.h"		/* HAL public API decls */

/* Linux iio includes */
#include <iio/iio.h>
#include <iio.h>

/* module information */
MODULE_AUTHOR("Cameron McQuinn");
MODULE_DESCRIPTION("Driver for Xilinx Zynq XADC");
MODULE_LICENSE("GPL v2");

/***********************************************************************
*                STRUCTURES AND GLOBAL VARIABLES                       *
************************************************************************/

static iio_device* xadc;


/***********************************************************************
*                  LOCAL FUNCTION DECLARATIONS                         *
************************************************************************/
/* These is the functions that actually do the I/O
   everything else is just init code
*/

static void read_xadc(void *arg, long period);

/***********************************************************************
*                       INIT AND EXIT CODE                             *
************************************************************************/

int rtapi_app_main(void)
{
  /* pointers used in the initialization */
  struct iio_context* context = iio_create_local_context();

}

void rtapi_app_exit(void)
{

}
