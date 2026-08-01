#include"ProcessPool.hpp"
int main()
{
   ProcessPool pp(gdefaultnum);
   pp.Create();
   int cnt=10;
   while(cnt--)
   {
    pp.Run();
    sleep(1);
   }

   pp.Stop();
  // sleep(1000);
    return 0;
}