// Simple test program.  Will be replaced by new main function below.

#include <unistd.h>
#include <Progress.h>

int main (int argc, char** argv)
{
  // One
  Progress p1 ("label", 80, 1, 100, true, true);

  for (int i = 1; i <= 100; ++i)
  {
    p1.update (i);
    usleep (100000);
  }

  usleep (1000000);
  p1.done ();

  // Two
  Progress p2;
  p2.setLabel ("");
  p2.setWidth (80);
  p2.setMin (101);
  p2.setMax (201);
  p2.removeAfter (false);

  for (int i = 101; i <= 201; ++i)
  {
    p2.update (i);
    usleep (100000);
  }

  usleep (1000000);
  p2.done ();

  // Three
  Progress p3 ("label", 80, 1, 100);

  for (int i = 1; i <= 100; ++i)
  {
    p3.update (i);
    usleep (100000);
  }

  usleep (1000000);
  p3.done ();

  return 0;
}

#ifdef NOT_YET

#include <iostream>
#include <getopt.h>
#include <Progress.h>

extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;

int main (int argc, char** argv)
{
  // TODO Process command line.
/*
  pb --min n
     --max x
     --width w
     --current c
     --label l
     --percentage
     --start s
     --elapsed
     --estimate
     --done color-done
     --remaining color-remaining
     --now
*/


  int percentage = 0;
  int elapsed = 0;
  int estimate = 0;
  int remove = 0;
  int now = 0;

  static struct option longopts[] = {
    { "min",        required_argument,      NULL,           'm' },
    { "max",        required_argument,      NULL,           'x' },
    { "width",      required_argument,      NULL,           'w' },
    { "current",    required_argument,      NULL,           'c' },
    { "label",      required_argument,      NULL,           'l' },
    { "percentage", no_argument,            &percentage,    'p' },
    { "elapsed",    no_argument,            &elapsed,       'e' },
    { "estimate",   no_argument,            &estimate,      't' },
    { "remove",     no_argument,            &remove,        'r' },
    { "now",        no_argument,            &now,           'n' },
    { "done",       required_argument,      NULL,           'd' },
    { "remaining",  required_argument,      NULL,           'a' },
    { "start",      required_argument,      NULL,           's' },
    { NULL,         0,                      NULL,           0   }
  };

  int ch;
  while ((ch = getopt_long (argc, argv, "mxwclpetrsdna:", longopts, NULL)) != -1)
  {
    switch (ch)
    {
    case 'm':      std::cout << "m" << std::endl; break;
    case 'x':      std::cout << "x" << std::endl; break;
    case 'w':      std::cout << "w" << std::endl; break;
    case 'c':      std::cout << "c" << std::endl; break;
    case 'l':      std::cout << "l" << std::endl; break;
    case 'p':      std::cout << "p" << std::endl; break;
    case 'e':      std::cout << "e" << std::endl; break;
    case 't':      std::cout << "t" << std::endl; break;
    case 's':      std::cout << "s" << std::endl; break;
    case 'd':      std::cout << "d" << std::endl; break;

    // TODO remove bar.
    case 'r':      std::cout << "r" << std::endl; break;

    // TODO immediately display time() to STDOUT.
    case 'n':      std::cout << "n" << std::endl; break;

    case 'a':      std::cout << "a" << std::endl; break;
    default:       std::cout << "usage!" << std::endl; break;
/*
    case 'b':
      bflag = 1;
      break;

    case 'f':
      if ((fd = open(optarg, O_RDONLY, 0)) == -1)
        err(1, "unable to open %s", optarg);
      break;

    case 0:
      if (daggerset)
        fprintf (stderr, "Buffy will use her dagger to apply fluoride to dracula's teeth\n");
      break;

    default:
      usage ();
*/
    }
  }

  argc -= optind;
  argv += optind;


  // TODO Set up Progress object.

  // TODO Render Progress bar.

  return 0;
}

#endif

