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
            << "  -y, --style <name>          Style of bar rendering\n"
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
#ifdef WAITING_FOR_VITAPI
            << "  -d, --done <color>          Color of completed part\n"
            << "  -a, --remaining <color>     Color of incomplete part\n"
#endif
            << "  -v, --version               Show vramsteg version\n"
            << "  -h, --help                  Show command options\n"
            << std::endl;

  exit (0);
}

////////////////////////////////////////////////////////////////////////////////
void showVersion ()
{
  std::cout << "\n"
            << "\033[1m" << "vramsteg 1.0" << "\033[0m\n"
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
  try
  {
    int         arg_current    = 0;
#ifdef WAITING_FOR_VITAPI
    std::string arg_done       = "";
#endif
    bool        arg_elapsed    = false;
    bool        arg_estimate   = false;
    std::string arg_label;
    int         arg_max        = 0;
    int         arg_min        = 0;
    bool        arg_percentage = false;
#ifdef WAITING_FOR_VITAPI
    std::string arg_remaining  = "";
#endif
    bool        arg_remove     = false;
    time_t      arg_start      = 0;
    int         arg_width      = 80;
    std::string arg_style      = "";

    static struct option longopts[] = {
      { "current",    required_argument, NULL, 'c' },
#ifdef WAITING_FOR_VITAPI
      { "done",       required_argument, NULL, 'd' },
#endif
      { "elapsed",    no_argument,       NULL, 'e' },
      { "estimate",   no_argument,       NULL, 't' },
      { "label",      required_argument, NULL, 'l' },
      { "max",        required_argument, NULL, 'x' },
      { "min",        required_argument, NULL, 'm' },
      { "now",        no_argument,       NULL, 'n' },
      { "percentage", no_argument,       NULL, 'p' },
#ifdef WAITING_FOR_VITAPI
      { "remaining",  required_argument, NULL, 'a' },
#endif
      { "remove",     no_argument,       NULL, 'r' },
      { "start",      required_argument, NULL, 's' },
      { "version",    no_argument,       NULL, 'v' },
      { "width",      required_argument, NULL, 'w' },
      { "style",      required_argument, NULL, 'y' },
      { "help",       no_argument,       NULL, 'h' },
      { NULL,         0,                 NULL, 0   }
    };

    int ch;
#ifdef WAITING_FOR_VITAPI
    while ((ch = getopt_long (argc, argv, "c:d:etl:x:m:npa:rs:vw:h", longopts, NULL)) != -1)
#else
    while ((ch = getopt_long (argc, argv, "c:etl:x:m:nprs:vw:h", longopts, NULL)) != -1)
#endif
    {
      switch (ch)
      {
      case 'c': arg_current    = atoi (optarg);        break;
#ifdef WAITING_FOR_VITAPI
      case 'd': arg_done       = optarg;               break;
#endif
      case 'e': arg_elapsed    = true;                 break;
      case 't': arg_estimate   = true;                 break;
      case 'l': arg_label      = optarg;               break;
      case 'x': arg_max        = atoi (optarg);        break;
      case 'm': arg_min        = atoi (optarg);        break;
      case 'n': std::cout << time (NULL) << std::endl; exit (0);
      case 'p': arg_percentage = true;                 break;
#ifdef WAITING_FOR_VITAPI
      case 'a': arg_remaining  = optarg;               break;
#endif
      case 'r': arg_remove     = true;                 break;
      case 's': arg_start      = atoi (optarg);        break;
      case 'v': showVersion ();                        break;
      case 'w': arg_width      = atoi (optarg);        break;
      case 'y': arg_style      = optarg;               break;
      case 'h': showUsage ();                          break;

      default:
        std::cout << "<default>" << std::endl;
        break;
      }
    }

    argc -= optind;
    argv += optind;

    // Sanity check arguments.
    if (arg_min || arg_max)
      if (arg_min > arg_max)
        throw std::string ("The --max value must not be less than the --min value.");

    if (arg_min || arg_max || arg_current)
      if (arg_min > arg_current || arg_current > arg_max)
        throw std::string ("The --current value must not lie outside the --min/--max range.");

    if (arg_width && arg_label.length ())
      if (arg_label.length () >= arg_width)
        throw std::string ("The --label string is longer than the allowed --width value.");

    if (! arg_remove && ! (arg_min || arg_current || arg_max))
      showUsage ();

    if (arg_elapsed && arg_start == 0)
      throw std::string ("To use the --elapsed feature, --start must be provided.");

    if (arg_estimate && arg_start == 0)
      throw std::string ("To use the --estimate feature, --start must be provided.");

    // Disallow signals from stopping the program while it is displaying color codes
    // Set up and render Progress object.
    Progress p (arg_label, arg_width, arg_min, arg_max, arg_percentage, arg_remove);
    p.setStyle (arg_style);
    p.setStart (arg_start);
    p.showElapsed (arg_elapsed);
    p.showEstimate (arg_estimate);
    p.removeAfter (arg_remove);
    p.update (arg_current);

    if (arg_remove)
      p.done ();
  }

  catch (const std::string& e) { std::cerr << "Error: " << e << std::endl; }
  catch (...)                  { std::cerr << "Unknown error occurred - please report." << std::endl; }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////

