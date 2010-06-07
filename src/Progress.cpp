#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <unistd.h>
#include <Progress.h>

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

Progress::~Progress ()
{
}

void Progress::setLabel (const std::string& value)
{
  label = value;
}

void Progress::setWidth (int value)
{
  width = value;
}

void Progress::setMin (int value)
{
  minimum = value;
}

void Progress::setMax (int value)
{
  maximum = value;
}

void Progress::showPercentage (bool value)
{
  percentage = value;
}

void Progress::removeAfter (bool value)
{
  remove = value;
}

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

/*
    std::cout << "w" << width
              << " l" << label.length ()
              << " b" << bar
              << " f" << fraction
              << " v" << visible
              << std::endl;
*/

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

