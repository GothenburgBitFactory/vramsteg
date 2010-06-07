
#ifndef INCLUDED_PROGRESS
#define INCLUDED_PROGRESS

#include <string>

class Progress
{
public:
  Progress ();
  Progress (const std::string&, int, int, int, bool = true, bool = true);
  ~Progress ();

  void setLabel (const std::string&);
  void setWidth (int);
  void setMin (int);
  void setMax (int);
  void showPercentage (bool);
  void removeAfter (bool);

  void update (int);
  void done ();

private:
  std::string label;
  int width;
  int minimum;
  int maximum;
  int current;
  bool percentage;
  bool remove;
};

#endif INCLUDED_PROGRESS
