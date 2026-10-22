#include "testlib.h"
#include <string>

int main(int argc, char* argv[])
{
    registerValidation(argc, argv);

    int n = inf.readInt(1, 100000, "n");
    inf.readEoln();
    std::string first = inf.readLine("[01]{1,100000}", "first row");
    std::string second = inf.readLine("[01]{1,100000}", "second row");

    ensuref((int)first.size() == n, "first row length must be n");
    ensuref((int)second.size() == n, "second row length must be n");
    ensuref(first[0] == '0', "start cell must be 0");
    ensuref(second[n - 1] == '0', "princess cell must be 0");

    inf.readEof();
    return 0;
}
