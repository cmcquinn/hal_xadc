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

/* pointer to array of channel_t structs in shared memory, 1 per ADC channel found */
static channel_t *xadc_channel_array;

/* libiio object globals */
static struct iio_context *context;
static struct iio_device *xadc;
static struct iio_buffer *buf;


int main(void)
{
  context = iio_create_local_context();
  xadc = iio_context_find_device(context, "xadc");

}
