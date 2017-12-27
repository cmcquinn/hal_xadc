/*
 *  Copyright (C) 2017 Cameron McQuinn
 *
 *  xadc_test.cpp
 *
 *  A simple test program I wrote to explore the functionality of libiio
 *
 */

#include <iostream>
#include <iio.h>

typedef struct {
  iio_channel* channel_ptr;
  uint32_t value;
  char *name;
} channel_t;

int main(void)
{
  int num_channels = 3;

  channel_t channel_array[num_channels];

  /* libiio objects */
  struct iio_context *context;
  struct iio_device *xadc;
  struct iio_buffer *buf;

  context = iio_create_local_context();
  xadc = iio_context_find_device(context, "xadc");

}
