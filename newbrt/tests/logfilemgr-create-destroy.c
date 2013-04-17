#include "test.h"
#include "logfilemgr.h"

int test_main(int argc, const char *argv[]) {
    int r;

    TOKULOGFILEMGR lfm = NULL;
    r = toku_logfilemgr_create(&lfm);
    assert(r == 0);

    r = toku_logfilemgr_destroy(&lfm);
    assert(r == 0);

    return 0;
}