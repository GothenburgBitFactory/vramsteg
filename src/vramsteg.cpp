////////////////////////////////////////////////////////////////////////////////
// Vramsteg - Utility for displaying progress bars in shell scripts.
//
// Copyright 2010, Paul Beckingham, Federico Hernandez.
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the
//
//     Free Software Foundation, Inc.,
//     51 Franklin Street, Fifth Floor,
//     Boston, MA
//     02110-1301
//     USA
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <time.h>
#include <Progress.h>

extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;

////////////////////////////////////////////////////////////////////////////////
void showUsage ()
{
  std::cout << "\n"
            << "Usage: vramsteg [options]\n"
            << "\n"
            << "  -l, --label <value>         Progress bar label\n"
            << "  -m, --min <value>           Equivalent to 0%\n"
            << "  -x, --max <value>           Equivalent to 100%\n"
            << "  -c, --current <value>       Current value of progress bar\n"
            << "  -p, --percentage            Show percentage\n"
            << "  -s, --start <value>         Start time epoch\n"
            << "  -w, --width <value>         Width of progress bar, default 80\n"
            << "  -n, --now                   Returns current time as epoch\n"
            << "  -r, --remove                Removes the progress bar\n"
            << "  -e, --elapsed               Show elapsed time (needs --start)\n"
            << "  -t, --estimate              Show estimated remaining time (needs --start)\n"
            << "  -d, --done <color>          Color of completed part\n"
            << "  -a, --remaining <color>     Color of incomplete part\n"
            << "  -v, --version               Show vramsteg version\n"
            << "  -h, --help                  Show command options\n"
            << "\n"
            << std::endl;

  exit (0);
}

////////////////////////////////////////////////////////////////////////////////
void showVersion ()
{
  std::cout << "\n"
            << "\033[1mvramsteg 1.0\033[0m\n"
            << "Copyright (C) 2010 P. Beckingham, F. Hernandez.\n"
            << "\n"
            << "Vramsteg may be copied only under the terms of the GNU "
            << "General Public License, which may be found in the task "
            << "source kit.\n"
            << "\n"
            << "Documentation for vramsteg can be found using 'man vramsteg', "
            << " or at http://vitapi.org\n\n"
            << std::flush;

  exit (0);
}

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  int         arg_current    = 0;
  std::string arg_done       = "";    // TODO Needs a color.
  bool        arg_elapsed    = false;
  bool        arg_estimate   = false;
  std::string arg_label;
  int         arg_max        = 0;
  int         arg_min        = 0;
  bool        arg_percentage = false;
  std::string arg_remaining  = "";    // TODO Needs a color.
  bool        arg_remove     = false;
  int         arg_start      = 0;
  int         arg_width      = 80;    // TODO Default to terminal width.

  static struct option longopts[] = {
    { "current",    required_argument, NULL, 'c' },
    { "done",       required_argument, NULL, 'd' },
    { "elapsed",    no_argument,       NULL, 'e' },
    { "estimate",   no_argument,       NULL, 't' },
    { "label",      required_argument, NULL, 'l' },
    { "max",        required_argument, NULL, 'x' },
    { "min",        required_argument, NULL, 'm' },
    { "now",        no_argument,       NULL, 'n' },
    { "percentage", no_argument,       NULL, 'p' },
    { "remaining",  required_argument, NULL, 'a' },
    { "remove",     no_argument,       NULL, 'r' },
    { "start",      required_argument, NULL, 's' },
    { "version",    no_argument,       NULL, 'v' },
    { "width",      required_argument, NULL, 'w' },
    { "help",       no_argument,       NULL, 'h' },
    { NULL,         0,                 NULL, 0   }
  };

  int ch;
  while ((ch = getopt_long (argc, argv, "c:d:etl:x:m:npa:rs:vw:h", longopts, NULL)) != -1)
  {
    switch (ch)
    {
    case 'c': arg_current    = atoi (optarg);        break;
    case 'd': arg_done       = optarg;               break;
    case 'e': arg_elapsed    = true;                 break;
    case 't': arg_estimate   = true;                 break;
    case 'l': arg_label      = optarg;               break;
    case 'x': arg_max        = atoi (optarg);        break;
    case 'm': arg_min        = atoi (optarg);        break;
    case 'n': std::cout << time (NULL) << std::endl; exit (0);
    case 'p': arg_percentage = true;                 break;
    case 'a': arg_remaining  = optarg;               break;
    case 'r': arg_remove     = true;                 break;
    case 's': arg_start      = atoi (optarg);        break;
    case 'v': showVersion ();                        break;
    case 'w': arg_width      = atoi (optarg);        break;
    case 'h': showUsage ();                          break;

    default:
      std::cout << "<default>" << std::endl;
      break;
    }
  }

  argc -= optind;
  argv += optind;

  // TODO Sanity check arguments.

  // TODO Sanity check all values.
  // TODO min < max
  // TODO min <= current <= max
  // TODO width > label.length + percentage.length + estimate.length + elapsed.length

  // Set up and render Progress object.
  Progress p (arg_label, arg_width, arg_min, arg_max, arg_percentage, arg_remove);
  p.update (arg_current);

  if (arg_remove)
    p.done ();

  return 0;
}

////////////////////////////////////////////////////////////////////////////////

