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

#include <iio.h> /* libiio API */

/* module information */
MODULE_AUTHOR("Cameron McQuinn");
MODULE_DESCRIPTION("Driver for Xilinx Zynq XADC");
MODULE_LICENSE("GPL v2");

/* config string */
#define MAX_ADC_CHAN 16
char *channel_cfg[MAX_ADC_CHAN] = { '\0' };
RTAPI_MP_ARRAY_STRING(channels, MAX_ADC_CHAN, "Names of XADC channels to look for");

/***********************************************************************
*                STRUCTURES AND GLOBAL VARIABLES                       *
************************************************************************/

typedef struct {
  iio_channel* channel_ptr;
  uint32_t value;
  char *name;
} channel_t;

/* pointer to array of channel_t structs in shared memory, 1 per ADC channel found */
static channel_t *xadc_channel_array;

/* libiio object globals */
static struct iio_context *context;
static struct iio_device *xadc;
static struct iio_buffer *buf;

/* HAL globals */
static int comp_id;
static int num_channels = 0;

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
  /* Determine how many channels were passed in */
  while (channel_cfg[num_channels] != '\0' && num_channels <= MAX_ADC_CHAN)
  {
    num_channels++;
  }

  if (num_channels == 0)
  {
    rtapi_print_msg(RTAPI_MSG_ERR, "XADC: ERROR: Channel configuration param is empty\n");
    return -1;
  }

  /* STEP 1: initialise the driver */
  comp_id = hal_init("hal_xadc");
  if (comp_id < 0) {
    rtapi_print_msg(RTAPI_MSG_ERR, "XADC: ERROR: hal_init() failed\n");
    return -1;
  }

  /* STEP 2: allocate shared memory for channel data */
  xadc_channel_array = hal_malloc(num_channels * sizeof(channel_t));
  if (xadc_channel_array == 0) {
    rtapi_print_msg(RTAPI_MSG_ERR, "XADC: ERROR: hal_malloc() failed\n");
    hal_exit(comp_id);
    return -1;
  }

  /* STEP 3: Initialize libiio with channel information from the HAL file */
  context = iio_create_local_context();
  xadc = iio_context_find_device(context, "xadc");
  for (int i = 0; i < num_channels; i++)
  {
      xadc_channel_array[n].channel_ptr = iio_device_find_channel(xadc, channel_cfg[i], false);
      /* does doing the assignment this way actually set the name to the string, or just a pointer to channel_cfg[i] */
      xadc_channel_array[n].name = channel_cfg[i];
      if (xadc_channel_array[n].channel_ptr == nullptr) {
          rtapi_print_msg(RTAPI_MSG_ERR, "XADC: ERROR: failed to find XADC channel %s\n", channel_cfg[i]);
          hal_exit(comp_id);
          iio_context_destroy(context);
          return -1;
      }

      /* setup the channel for sampling, if necessary */
      if(iio_channel_is_scan_element(xadc_channel_array[n].channel_ptr))
      {
        iio_channel_enable(xadc_channel_array[n].channel_ptr);
      }
  }

  /* STEP 4: export the pin(s) */
  for (int i = 0; i < num_channels; i++)
  {
      retval = hal_pin_u32_newf(HAL_OUT, &(xadc_channel_array[i].value), comp_id, "xadc.0.ch.%s.out", xadc_channel_array[i].name);
      if (retval < 0) {
        rtapi_print_msg(RTAPI_MSG_ERR, "XADC: ERROR: channel %s export failed with err=%i\n", xadc_channel_array[i]->name, retval);
        iio_context_destroy(context);
        hal_exit(comp_id);
        return -1;
      }
  }

  /* STEP 5: export read function */
  rtapi_snprintf(name, sizeof(name), "skeleton.%d.write", n);
  retval = hal_export_funct(name, write_port, &(xadc_channel_array[n]), 0, 0, comp_id);
  if (retval < 0) {
    rtapi_print_msg(RTAPI_MSG_ERR, "XADC: ERROR: port %d read funct export failed\n", n);
    iio_context_destroy(context);
    hal_exit(comp_id);
    return -1;
  }

  rtapi_print_msg(RTAPI_MSG_INFO, "XADC: installed driver for %d ports\n", num_channels);
  hal_ready(comp_id);
  return 0;
}

void rtapi_app_exit(void)
{

}

/**************************************************************
* REALTIME CHANNEL READ FUNCTION                                *
**************************************************************/

static void read_adc(void *arg, long period)
{
    channel_t *channel;
    unsigned char outdata;
    channel = arg;

    outdata = *(port->data_out) & 0xFF;
    /* write it to the hardware */
    rtapi_outb(outdata, 0x378);
}
