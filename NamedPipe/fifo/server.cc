#include "comm.hpp"

int main()
{
    // 创建管道文件
    NamedFifo fifo(PATH, FILENAME);

    // 文件操作了
    FileOper readerfile(PATH, FILENAME);
    readerfile.OpenForRead();
    readerfile.Read();
    readerfile.Close();

    return 0;
}