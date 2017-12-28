/*
 *  Copyright (C) 2017 Cameron McQuinn
 *
 *  iio_dev.cpp
 *
 *  Program to list the names of iio channels found
 *
 */

#include <stdio.h>
#include <iio.h>

/* libiio object globals */
static struct iio_context *context;
static struct iio_device *xadc;
static struct iio_buffer *buf;

const char *name_at_index(int index)
{
  struct iio_channel *chn = iio_device_get_channel(xadc, index);
  return iio_channel_get_name(chn);
}

int main(void)
{
  /* setup libiio */
  context = iio_create_local_context();
  xadc = iio_context_find_device(context, "xadc");

  int channel_count = iio_device_get_channels_count(xadc);

  for (int i = 0; i < channel_count; i++)
  {
    printf("Found channel %s\n", name_at_index(i));
  }

  return 0;
}
