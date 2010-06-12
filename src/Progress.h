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

#ifndef INCLUDED_PROGRESS
#define INCLUDED_PROGRESS

#include <string>
#include <time.h>

class Progress
{
public:
  Progress ();
  Progress (const std::string&, int, int, int, bool = true, bool = true);
  ~Progress ();

  void setStyle (const std::string&);
  void setLabel (const std::string&);
  void setWidth (int);
  void setMin (int);
  void setMax (int);
  void showPercentage (bool);
  void removeAfter (bool);
  void setStart (time_t);
  void showEstimate (bool);
  void showElapsed (bool);

  void update (int);
  void done ();

private:
  std::string formatTime (time_t);

  void renderStyleDefault ();
  void renderStyleText ();

private:
  std::string style;
  std::string label;
  int width;
  int minimum;
  int maximum;
  int current;
  bool percentage;
  bool remove;
  time_t start;
  bool estimate;
  bool elapsed;
};

#endif INCLUDED_PROGRESS

////////////////////////////////////////////////////////////////////////////////
