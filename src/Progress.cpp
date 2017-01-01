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
void Progress::setStyle (const std::string& value)
{
  _style = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::setLabel (const std::string& value)
{
  _label = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::setWidth (int value)
{
  _width = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::setMin (long value)
{
  _minimum = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::setMax (long value)
{
  _maximum = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::showPercentage (bool value)
{
  _percentage = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::removeAfter (bool value)
{
  _remove = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::setStart (time_t value)
{
  _start = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::showEstimate (bool value)
{
  _estimate = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::showElapsed (bool value)
{
  _elapsed = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::update (long value)
{
  if (isatty (fileno (stdout)) && _current != value)
  {
    // Box the range.
    if (value < _minimum) value = _minimum;
    if (value > _maximum) value = _maximum;

    // Current value.
    _current = value;

    // Capable of supporting multiple styles.
         if (_style == "")     renderStyleDefault ();
    else if (_style == "mono") renderStyleMono ();
    else if (_style == "text") renderStyleText ();
    else
      throw std::string ("Style '") + _style + "' not supported.";
  }
}

////////////////////////////////////////////////////////////////////////////////
void Progress::done () const
{
  if (isatty (fileno (stdout)))
  {
    if (_remove)
      std::cout << "\r"
                << std::setfill (' ')
                << std::setw (_width)
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
  double fraction = (1.0 * (_current - _minimum)) / (_maximum - _minimum);

  // Elapsed time.
  time_t now = time (nullptr);
  std::string elapsed_time;
  if (_elapsed && _start != 0)
    elapsed_time = formatTime (now - _start);

  // Estimated remaining time.
  std::string estimate_time;
  if (_estimate && _start != 0)
  {
    if (fraction >= 1e-6)
      estimate_time = formatTime ((time_t) (int) (((now - _start) * (1.0 - fraction)) / fraction));
    else
      estimate_time = formatTime (0);
  }

  // Calculate bar width.
  int bar = _width
          - (_label.length () ? _label.length () + 1        : 0)
          - (_percentage      ? 5                           : 0)
          - (_elapsed         ? elapsed_time.length () + 1  : 0)
          - (_estimate        ? estimate_time.length () + 1 : 0);

  if (bar < 1)
    throw std::string ("The specified width is insufficient.");

  int visible = (int) (fraction * bar);

  // Render.
  if (_label.length ())
    std::cout << _label
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

  if (_percentage)
    std::cout << " "
              << std::setfill (' ')
              << std::setw (3)
              << (int) (fraction * 100)
              << "%";

  if (_elapsed && _start != 0)
    std::cout << " "
              << elapsed_time;

  if (_estimate && _start != 0 && fraction > 0.2)
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
  double fraction = (1.0 * (_current - _minimum)) / (_maximum - _minimum);

  // Elapsed time.
  time_t now = time (nullptr);
  std::string elapsed_time;
  if (_elapsed && _start != 0)
    elapsed_time = formatTime (now - _start);

  // Estimated remaining time.
  std::string estimate_time;
  if (_estimate && _start != 0)
  {
    if (fraction >= 1e-6)
      estimate_time = formatTime ((time_t) (int) (((now - _start) * (1.0 - fraction)) / fraction));
    else
      estimate_time = formatTime (0);
  }

  // Calculate bar width.
  int bar = _width
          - (_label.length () ? _label.length () + 1        : 0)
          - (_percentage      ? 5                           : 0)
          - (_elapsed         ? elapsed_time.length () + 1  : 0)
          - (_estimate        ? estimate_time.length () + 1 : 0);

  if (bar < 1)
    throw std::string ("The specified width is insufficient.");

  int visible = (int) (fraction * bar);

  // Render.
  if (_label.length ())
    std::cout << _label
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

  if (_percentage)
    std::cout << " "
              << std::setfill (' ')
              << std::setw (3)
              << (int) (fraction * 100)
              << "%";

  if (_elapsed && _start != 0)
    std::cout << " "
              << elapsed_time;

  if (_estimate && _start != 0)
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
  double fraction = (1.0 * (_current - _minimum)) / (_maximum - _minimum);

  // Elapsed time.
  time_t now = time (nullptr);
  std::string elapsed_time;
  if (_elapsed && _start != 0)
    elapsed_time = formatTime (now - _start);

  // Estimated remaining time.
  std::string estimate_time;
  if (_estimate && _start != 0)
  {
    if (fraction >= 1e-6)
      estimate_time = formatTime ((time_t) (int) (((now - _start) * (1.0 - fraction)) / fraction));
    else
      estimate_time = formatTime (0);
  }

  // Calculate bar width.
  int bar = _width
          - 2                                                    // The [ and ]
          - (_label.length () ? _label.length () + 1        : 0)
          - (_percentage      ? 5                           : 0)
          - (_elapsed         ? elapsed_time.length () + 1  : 0)
          - (_estimate        ? estimate_time.length () + 1 : 0);

  if (bar < 1)
    throw std::string ("The specified width is insufficient.");

  int visible = (int) (fraction * bar);

  // Render.
  if (_label.length ())
    std::cout << _label
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

  if (_percentage)
    std::cout << " "
              << std::setfill (' ')
              << std::setw (3)
              << (int) (fraction * 100)
              << "%";

  if (_elapsed && _start != 0)
    std::cout << " "
              << elapsed_time;

  if (_estimate && _start != 0)
    std::cout << " "
              << estimate_time;

  std::cout << "\r"
            << std::flush;
}

////////////////////////////////////////////////////////////////////////////////
