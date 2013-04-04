/**
   @file Main.cxx
   @author Kuba Podgórski <k_u_b_a@users.sourceforge.net>
*/

#include <Base.hxx>

int main(int argc, char *argv[])
{
#ifdef SHOW_HEADER
  system("cat HEADER");
#endif
  TBase App(argc, argv);
  
  App.Run();
  
  return 0;
}
