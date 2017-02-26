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

#ifndef INCLUDED_PROGRESS
#define INCLUDED_PROGRESS

#include <string>
#include <ctime>

class Progress
{
public:
  void update (long);
  void done () const;

private:
  std::string formatTime (time_t) const;
  void renderStyleDefault () const;
  void renderStyleMono () const;
  void renderStyleText () const;

public:
  std::string style {};
  std::string label {};
  int width         {80};
  long minimum      {0};
  long maximum      {0};
  bool percentage   {true};
  bool remove       {true};
  time_t start      {0};
  bool estimate     {false};
  bool elapsed      {false};

private:
  long _current     {-1};
};

#endif

////////////////////////////////////////////////////////////////////////////////
