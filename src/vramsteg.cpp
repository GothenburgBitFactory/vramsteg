////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2010 - 2017, Paul Beckingham, Federico Hernandez.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sys/ioctl.h>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <unistd.h>
#include <ctime>
#include <csignal>
#include <Progress.h>
#include <cmake.h>

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
            << "  -w, --width <value>         Width of progress bar, default full width\n"
            << "  -n, --now                   Returns current time as epoch\n"
            << "  -r, --remove                Removes the progress bar\n"
            << "  -e, --elapsed               Show elapsed time (needs --start)\n"
            << "  -t, --estimate              Show estimated remaining time (needs --start)\n"
            << "  -v, --version               Show vramsteg version\n"
            << "  -h, --help                  Show command options\n"
            << "\n"
            << "Supported styles are:\n"
            << "  (default)     label \033[42m        \033[41m            \033[0m 40%\n"
            << "  mono          label \033[47m    \033[40m                \033[0m 20%\n"
            << "  text          label [************      ] 60%\n"
            << "\n";

  exit (0);
}

////////////////////////////////////////////////////////////////////////////////
void showVersion ()
{
  std::cout << "\n"
            << "\033[1m" << PACKAGE_STRING << "\033[0m\n"
            << "Copyright (C) 2010 - 2017, Göteborg Bit Factory\n"
            << "Copyright (C) 2010 - 2017, P. Beckingham, F. Hernandez.\n"
            << "\n"
            << "Vramsteg may be copied only under the terms of the MIT license, "
            << "which may be found in the taskwarrior source kit.\n"
            << "\n"
            << "Documentation for vramsteg can be found using 'man vramsteg', or "
            << "at http://tasktools.org.\n"
            << "\n";

  exit (0);
}

////////////////////////////////////////////////////////////////////////////////
int main (int argc, char** argv)
{
  try
  {
    // We don't want any signals causing the program to quit in mid output, as
    // this would lead to odd colors persisting in the terminal.
    signal (SIGHUP,    SIG_IGN);
    signal (SIGINT,    SIG_IGN);
    signal (SIGPIPE,   SIG_IGN);
    signal (SIGTERM,   SIG_IGN);
    signal (SIGUSR1,   SIG_IGN);
    signal (SIGUSR2,   SIG_IGN);

    long        arg_current    {0};
    bool        arg_elapsed    {false};
    bool        arg_estimate   {false};
    std::string arg_label      {};
    long        arg_max        {0};
    long        arg_min        {0};
    bool        arg_percentage {false};
    bool        arg_remove     {false};
    time_t      arg_start      {0};
    int         arg_width      {80};
    std::string arg_style      {};

    // Dynamically determine terminal width.
    unsigned short buff[4];
    if (ioctl (fileno(stdout), TIOCGWINSZ, &buff) != -1)
      arg_width = buff[1];

    static struct option longopts[] = {
      { "current",    required_argument, nullptr, 'c' },
      { "elapsed",    no_argument,       nullptr, 'e' },
      { "estimate",   no_argument,       nullptr, 't' },
      { "label",      required_argument, nullptr, 'l' },
      { "max",        required_argument, nullptr, 'x' },
      { "min",        required_argument, nullptr, 'm' },
      { "now",        no_argument,       nullptr, 'n' },
      { "percentage", no_argument,       nullptr, 'p' },
      { "remove",     no_argument,       nullptr, 'r' },
      { "start",      required_argument, nullptr, 's' },
      { "version",    no_argument,       nullptr, 'v' },
      { "width",      required_argument, nullptr, 'w' },
      { "style",      required_argument, nullptr, 'y' },
      { "help",       no_argument,       nullptr, 'h' },
      { nullptr,      0,                 nullptr, 0   }
    };

    int ch;
    while ((ch = getopt_long (argc, argv, "c:etl:x:m:nprs:vw:h", longopts, nullptr)) != -1)
    {
      switch (ch)
      {
      case 'c': arg_current    = atol (optarg);        break;
      case 'e': arg_elapsed    = true;                 break;
      case 't': arg_estimate   = true;                 break;
      case 'l': arg_label      = optarg;               break;
      case 'x': arg_max        = atol (optarg);        break;
      case 'm': arg_min        = atol (optarg);        break;
      case 'n': std::cout << time (nullptr) << std::endl; exit (0);
      case 'p': arg_percentage = true;                 break;
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
      if (arg_label.length () >= static_cast <unsigned int> (arg_width))
        throw std::string ("The --label string is longer than the allowed --width value.");

    if (! arg_remove && ! (arg_min || arg_current || arg_max))
      showUsage ();

    if (arg_elapsed && arg_start == 0)
      throw std::string ("To use the --elapsed feature, --start must be provided.");

    if (arg_estimate && arg_start == 0)
      throw std::string ("To use the --estimate feature, --start must be provided.");

    // Disallow signals from stopping the program while it is displaying color codes
    // Set up and render Progress object.
    Progress p;
    p.style      = arg_style;
    p.label      = arg_label;
    p.width      = arg_width;
    p.minimum    = arg_min;
    p.maximum    = arg_max;
    p.percentage = arg_percentage;
    p.remove     = arg_remove;
    p.start      = arg_start;
    p.elapsed    = arg_elapsed;
    p.estimate   = arg_estimate;
    p.remove     = arg_remove;
    p.update (arg_current);

    if (p.remove)
      p.done ();
  }

  catch (const std::string& e) { std::cerr << "Error: " << e << std::endl; }
  catch (...)                  { std::cerr << "Unknown error occurred - please report." << std::endl; }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////

