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
#include <iomanip>
#include <stdio.h>
#include <unistd.h>
#include <Progress.h>

////////////////////////////////////////////////////////////////////////////////
Progress::Progress ()
: label ("")
, width (0)
, minimum (0)
, maximum (0)
, current (0)
, percentage (true)
, remove (true)
{
}

////////////////////////////////////////////////////////////////////////////////
Progress::Progress (const std::string& l, int w, int n, int x, bool p /* = true */, bool r /* = true */)
: label (l)
, width (w)
, minimum (n)
, maximum (x)
, percentage (p)
, remove (r)
{
  current = minimum;
}

////////////////////////////////////////////////////////////////////////////////
Progress::~Progress ()
{
}

////////////////////////////////////////////////////////////////////////////////
void Progress::setLabel (const std::string& value)
{
  label = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::setWidth (int value)
{
  width = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::setMin (int value)
{
  minimum = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::setMax (int value)
{
  maximum = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::showPercentage (bool value)
{
  percentage = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::removeAfter (bool value)
{
  remove = value;
}

////////////////////////////////////////////////////////////////////////////////
void Progress::update (int value)
{
  if (isatty (fileno (stdout)) && current != value)
  {
    int bar = width
            - (label.length () ? label.length () + 1 : 0)
            - (percentage ? 5 : 0);

    if (bar < 1)
      throw std::string ("Progress: specified width is insufficient.");

    current = value;
    float fraction = 1.0 * (current - minimum + 1) / (maximum - minimum + 1);
    int visible = (int) (fraction * bar);

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

    std::cout << "\r"
              << std::flush;
  }
}

////////////////////////////////////////////////////////////////////////////////
void Progress::done ()
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
