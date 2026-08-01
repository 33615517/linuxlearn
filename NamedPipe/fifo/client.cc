#include "comm.hpp"

int main()
{
    FileOper writerfile(PATH, FILENAME);
    writerfile.OpenForWrite();
    writerfile.Write();
    writerfile.Close();
    return 0;
}