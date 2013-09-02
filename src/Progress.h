////////////////////////////////////////////////////////////////////////////////
// Vramsteg - Utility for displaying progress bars in shell scripts.
//
// Copyright 2010 - 2013, Paul Beckingham, Federico Hernandez.
// All rights reserved.
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
  void renderStyleMono ();
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

#endif

////////////////////////////////////////////////////////////////////////////////
