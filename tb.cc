#include <verilated.h>
#include "obj_dir/Vvideo.h"

#include <tb.h>

int main(int argc, char** argv)
{
    Verilated::commandArgs(argc, argv);
    TB* tb = new TB(argc, argv);
    while (tb->main());
    return tb->destroy();
}
