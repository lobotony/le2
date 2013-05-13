/*
 * mt-evtest.c, small test application to put some stress on
 * multitouch driver buffering.
 *
 * written by Simon Budig <simon.budig@kernelconcepts.de>
 * placed in the public domain.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <error.h>
#include <poll.h>

#include <linux/input.h>

#define MAX_TOUCH_POINTS 32

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)    ((array[LONG(bit)] >> OFF(bit)) & 1)

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define ABS(a)     (((a) < 0) ? -(a) : (a))

typedef struct _touchpoint TouchPoint;
struct _touchpoint
{
  double x;
  double y;
  int    id;
};

typedef struct _touch_context TouchContext;
struct _touch_context
{
  char       *touch_filename;
  int         touch_fd;

  TouchPoint  min;
  TouchPoint  max;

  int         have_multitouch;
  int         have_track_id;
  int         next_numtouch;
  TouchPoint  next_motion[MAX_TOUCH_POINTS];
};


int
read_mt_input_events (TouchContext *context)
{
  static int num_touches = 0;
  static TouchPoint touches[MAX_TOUCH_POINTS];
  static int have_x = 0, have_y = 0, have_syn_mt = 0;

  struct input_event evts[64];
  struct input_event *ev;
  int    i, j, ret;
  int    update = 0;

  while (1)
    {
      ret = read (context->touch_fd, evts, sizeof (evts));
      if (ret < 0 && errno != EAGAIN)
        error (1, errno, "failed to read multi touch events");

      if (ret == 0 || (ret < 0 && errno == EAGAIN))
        break;

      if (ret % sizeof (struct input_event) != 0)
        fprintf (stderr, "discarding partial event read\n");

      ret /= sizeof (struct input_event);

      for (i = 0; i < ret; i++)
        {
          ev = &evts[i];
        fprintf (stderr, "evt: %d %d (%d) - %d %d %d\n",
                 ev->type, ev->code, ev->value, num_touches, have_x, have_y);

          if (ev->type == EV_SYN && ev->code == SYN_REPORT)
            {
              context->next_numtouch  = num_touches;
              update = 1;
              fprintf (stderr, "syn\n");
              for (j = 0; j < MAX_TOUCH_POINTS; j++)
                {
                  if (j < num_touches)
                    {
                      context->next_motion[j].x = touches[j].x;
                      context->next_motion[j].y = touches[j].y;
                      context->next_motion[j].id = MAX (touches[j].id, 0);
                      touches[j].id = -1;
                    }
                  else
                    {
                      context->next_motion[j].id = -1;
                    }
                }
              num_touches = 0;
              have_x = have_y = have_syn_mt = 0;
            }

          if (ev->type == EV_SYN && ev->code == SYN_MT_REPORT)
            {
              if (have_x && have_y && num_touches < MAX_TOUCH_POINTS)
                {
                  fprintf (stderr, "mt, ");
                  if (!context->have_track_id)
                    touches[num_touches].id = num_touches;
                  num_touches ++;
                  have_x = have_y = 0;
                }

              have_syn_mt = 1;
            }

          if (num_touches < MAX_TOUCH_POINTS)
            {
              if (ev->type == EV_ABS && ev->code == ABS_MT_POSITION_X)
                {
                  touches[num_touches].x = ev->value;
                  have_x = 1;
                }
              if (ev->type == EV_ABS && ev->code == ABS_MT_POSITION_Y)
                {
                  touches[num_touches].y = ev->value;
                  have_y = 1;
                }
              if (ev->type == EV_ABS && ev->code == ABS_MT_TRACKING_ID)
                {
                  touches[num_touches].id = ev->value;
                  fprintf (stderr, "%2d; ", ev->value);
                }
            }
        }

    }

  // fprintf (stderr, "processed %d events: %d\n", ret, update);
  return update;
}


int
open_input_devices (TouchContext *context, char *touch_filename)
{
  unsigned long bits[NBITS(KEY_MAX)];
  char name[256] = "Unknown";
  struct input_absinfo abs;
  int fd;

  context->touch_fd = fd = open (touch_filename, O_RDONLY | O_NONBLOCK);
  if (context->touch_fd < 0)
    perror ("could not open touchscreen device");


  ioctl (fd, EVIOCGNAME(sizeof(name)), name);
  fprintf (stderr, "Input device name: \"%s\"\n", name);

  ioctl (fd, EVIOCGBIT(0, EV_MAX), bits);
  if (!test_bit (EV_ABS, bits))
    {
      fprintf (stderr, "   does not provide ABS events\n");
      return 1;
    }

  ioctl (fd, EVIOCGBIT (EV_ABS, KEY_MAX), bits);
  if (!(test_bit (ABS_MT_POSITION_X, bits) &&
        test_bit (ABS_MT_POSITION_Y, bits)))
    {
      printf("absolute events\n");
      ioctl (fd, EVIOCGABS (ABS_X), &abs);
      context->min.x  = abs.minimum;
      context->max.x  = abs.maximum;
      ioctl (fd, EVIOCGABS (ABS_Y), &abs);
      context->max.y = abs.maximum;
      context->have_multitouch = 0;
      context->min.id = -1;
      context->max.id = -1;
    }
  else
    {
      printf("absolute multitouch events\n");
      ioctl (fd, EVIOCGABS (ABS_MT_POSITION_X), &abs);
      context->min.x = abs.minimum;
      context->max.x = abs.maximum;
      ioctl (fd, EVIOCGABS (ABS_MT_POSITION_Y), &abs);
      context->min.y = abs.minimum;
      context->max.y = abs.maximum;
      context->have_multitouch = 1;
      context->min.id = -1;
      context->max.id = -1;
    }

  context->have_track_id = test_bit (ABS_MT_TRACKING_ID, bits) ? 1 : 0;

  fprintf (stderr, "   does %sprovide multitouch events with%s tracking ID\n",
           context->have_multitouch ? "" : "not ",
           context->have_track_id ? "" : "out");
  fprintf (stderr, "   coordinate range X: %f - %f, Y: %f - %f\n",
           context->min.x, context->max.x,
           context->min.y, context->max.y);

  return 0;
}



int
main (int argc, char *argv[])
{
  TouchContext *context;
  struct pollfd pollfds[2];
  int pollcount, i;

  if (argc < 2)
    exit (1);

  context = calloc (sizeof (TouchContext), 1);

  open_input_devices (context, argv[1]);

  pollcount = 0;

  if (context->touch_fd >= 0)
    {
      pollfds[pollcount].fd = context->touch_fd;
      pollfds[pollcount].events = POLLIN;
      pollcount++;
    }

  while (poll (pollfds, pollcount, 25) >= 0)
    {
      for (i = 0; i < pollcount; i++)
        {
          if (pollfds[i].fd == context->touch_fd &&
              pollfds[i].revents & POLLIN)
            {
              read_mt_input_events (context);
              usleep (20000);
            }
        }
    }

  return 0;
}
