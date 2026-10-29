#include "testlib.h"

int main(int argc, char* argv[])
{
    registerValidation(argc, argv);

    int t = inf.readInt(1, 200000, "t");
    inf.readEoln();
    for (int i = 0; i < t; i++)
    {
        inf.readInt(1, 10000000, "h");
        inf.readEoln();
    }
    inf.readEof();
    return 0;
}
