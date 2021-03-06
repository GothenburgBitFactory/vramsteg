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

#include <Progress.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
void Progress::update (long value)
{
  if (isatty (fileno (stdout)) && _current != value)
  {
    // Box the range.
    if (value < minimum) value = minimum;
    if (value > maximum) value = maximum;

    // Current value.
    _current = value;

    // Capable of supporting multiple styles.
         if (style == "")     renderStyleDefault ();
    else if (style == "mono") renderStyleMono ();
    else if (style == "text") renderStyleText ();
    else
      throw std::string ("Style '") + style + "' not supported.";
  }
}

////////////////////////////////////////////////////////////////////////////////
void Progress::done () const
{
  if (isatty (fileno (stdout)))
  {
    if (remove)
      std::cout << "\r"
                << std::setfill (' ')
                << std::setw (width)
                << ' ';

    std::cout << std::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
std::string Progress::formatTime (time_t t) const
{
  char buffer [128];

  int days    =  t          / 86400;
  int hours   = (t % 86400) / 3600;
  int minutes = (t %  3600) / 60;
  int seconds =  t % 60;

  if (days)
    snprintf (buffer, 128, "%dd %d:%02d:%02d", days, hours, minutes, seconds);
  else if (hours)
    snprintf (buffer, 128,     "%d:%02d:%02d",       hours, minutes, seconds);
  else
    snprintf (buffer, 128,        "%02d:%02d",              minutes, seconds);

  return std::string (buffer);
}

////////////////////////////////////////////////////////////////////////////////
// Default style looks like this:
//
// label GGGGGGGGRRRRRRRRRRRRRRRRR  34% 0:12 0:35
//
// ^^^^^                                            Label string
//       ^^^^^^^^                                   Completed bar (in green)
//               ^^^^^^^^^^^^^^^^^                  Incomplete bar (in red)
//                                 ^^^^             Percentage complete
//                                      ^^^^        Elapsed time
//                                           ^^^^   Remaining estimate
void Progress::renderStyleDefault () const
{
  // Fraction completed.
  double fraction = (1.0 * (_current - minimum)) / (maximum - minimum);

  // Elapsed time.
  auto now = time (nullptr);
  std::string elapsed_time;
  if (elapsed && start != 0)
    elapsed_time = formatTime (now - start);

  // Estimated remaining time.
  std::string estimate_time;
  if (estimate && start != 0)
  {
    if (fraction >= 1e-6)
      estimate_time = formatTime ((time_t) (int) (((now - start) * (1.0 - fraction)) / fraction));
    else
      estimate_time = formatTime (0);
  }

  // Calculate bar width.
  auto bar = width
           - (label.length () ? label.length () + 1         : 0)
           - (percentage      ? 5                           : 0)
           - (elapsed         ? elapsed_time.length () + 1  : 0)
           - (estimate        ? estimate_time.length () + 1 : 0);

  if (bar < 1)
    throw std::string ("The specified width is insufficient.");

  int visible = (int) (fraction * bar);

  // Render.
  if (label.length ())
    std::cout << label
              << ' ';

  if (visible > 0)
    std::cout << "\033[42m" // Green
              << std::setfill (' ')
              << std::setw (visible)
              << ' ';

  if (bar - visible > 0)
    std::cout << "\033[41m" // Red
              << std::setfill (' ')
              << std::setw (bar - visible)
              << ' ';

  std::cout << "\033[0m";

  if (percentage)
    std::cout << " "
              << std::setfill (' ')
              << std::setw (3)
              << (int) (fraction * 100)
              << "%";

  if (elapsed && start != 0)
    std::cout << " "
              << elapsed_time;

  if (estimate && start != 0 && fraction > 0.2)
    std::cout << " "
              << estimate_time;

  std::cout << "\r"
            << std::flush;
}

////////////////////////////////////////////////////////////////////////////////
// Mono style looks like this:
//
// label WWWWWWWWBBBBBBBBBBBBBBBBB  34% 0:12 0:35
//
// ^^^^^                                            Label string
//       ^^^^^^^^                                   Completed bar (in white)
//               ^^^^^^^^^^^^^^^^^                  Incomplete bar (in black)
//                                 ^^^^             Percentage complete
//                                      ^^^^        Elapsed time
//                                           ^^^^   Remaining estimate
void Progress::renderStyleMono () const
{
  // Fraction completed.
  double fraction = (1.0 * (_current - minimum)) / (maximum - minimum);

  // Elapsed time.
  auto now = time (nullptr);
  std::string elapsed_time;
  if (elapsed && start != 0)
    elapsed_time = formatTime (now - start);

  // Estimated remaining time.
  std::string estimate_time;
  if (estimate && start != 0)
  {
    if (fraction >= 1e-6)
      estimate_time = formatTime ((time_t) (int) (((now - start) * (1.0 - fraction)) / fraction));
    else
      estimate_time = formatTime (0);
  }

  // Calculate bar width.
  auto bar = width
           - (label.length () ? label.length () + 1         : 0)
           - (percentage      ? 5                           : 0)
           - (elapsed         ? elapsed_time.length () + 1  : 0)
           - (estimate        ? estimate_time.length () + 1 : 0);

  if (bar < 1)
    throw std::string ("The specified width is insufficient.");

  int visible = (int) (fraction * bar);

  // Render.
  if (label.length ())
    std::cout << label
              << ' ';

  if (visible > 0)
    std::cout << "\033[47m" // White
              << std::setfill (' ')
              << std::setw (visible)
              << ' ';

  if (bar - visible > 0)
    std::cout << "\033[40m" // Black
              << std::setfill (' ')
              << std::setw (bar - visible)
              << ' ';

  std::cout << "\033[0m";

  if (percentage)
    std::cout << " "
              << std::setfill (' ')
              << std::setw (3)
              << (int) (fraction * 100)
              << "%";

  if (elapsed && start != 0)
    std::cout << " "
              << elapsed_time;

  if (estimate && start != 0)
    std::cout << " "
              << estimate_time;

  std::cout << "\r"
            << std::flush;
}

////////////////////////////////////////////////////////////////////////////////
// Text style looks like this:
//
// label [********                ]  34% 0:12 0:35
//
// ^^^^^                                             Label string
//        ^^^^^^^^                                   Completed bar
//               ^^^^^^^^^^^^^^^^^                   Incomplete bar
//                                  ^^^^             Percentage complete
//                                       ^^^^        Elapsed time
//                                            ^^^^   Remaining estimate
void Progress::renderStyleText () const
{
  // Fraction completed.
  double fraction = (1.0 * (_current - minimum)) / (maximum - minimum);

  // Elapsed time.
  auto now = time (nullptr);
  std::string elapsed_time;
  if (elapsed && start != 0)
    elapsed_time = formatTime (now - start);

  // Estimated remaining time.
  std::string estimate_time;
  if (estimate && start != 0)
  {
    if (fraction >= 1e-6)
      estimate_time = formatTime ((time_t) (int) (((now - start) * (1.0 - fraction)) / fraction));
    else
      estimate_time = formatTime (0);
  }

  // Calculate bar width.
  auto bar = width
           - 2                                                    // The [ and ]
           - (label.length () ? label.length () + 1         : 0)
           - (percentage      ? 5                           : 0)
           - (elapsed         ? elapsed_time.length () + 1  : 0)
           - (estimate        ? estimate_time.length () + 1 : 0);

  if (bar < 1)
    throw std::string ("The specified width is insufficient.");

  int visible = (int) (fraction * bar);

  // Render.
  if (label.length ())
    std::cout << label
              << ' ';

  std::cout << '[';

  if (visible > 0)
    std::cout << std::setfill ('*')
              << std::setw (visible)
              << '*';

  if (bar - visible > 0)
    std::cout << std::setfill (' ')
              << std::setw (bar - visible)
              << ' ';

  std::cout << ']';

  if (percentage)
    std::cout << " "
              << std::setfill (' ')
              << std::setw (3)
              << (int) (fraction * 100)
              << "%";

  if (elapsed && start != 0)
    std::cout << " "
              << elapsed_time;

  if (estimate && start != 0)
    std::cout << " "
              << estimate_time;

  std::cout << "\r"
            << std::flush;
}

////////////////////////////////////////////////////////////////////////////////
