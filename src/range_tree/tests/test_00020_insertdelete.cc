/* -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*- */
// vim: ft=cpp:expandtab:ts=8:sw=4:softtabstop=4:
#ident "$Id$"
#ident "Copyright (c) 2007-2012 Tokutek Inc.  All rights reserved."
#ident "The technology is licensed by the Massachusetts Institute of Technology, Rutgers State University of New Jersey, and the Research Foundation of State University of New York at Stony Brook under United States of America Serial No. 11/760379 and to the patents and/or patent applications resulting from it."
/* We are going to test whether we can insert and delete. */

#include "test.h"

int main(int argc, const char *argv[]) {
    int r;
    toku_range_tree *tree;
    toku_range range;
    toku_point nums[8] = {{0},{1},{2},{3},{4},{5},{6},{7}};
    char letters[2] = {'A','B'};


    parse_args(argc, argv);

    /* Test overlap case */
    /*
        1   2   3   4   5   6   7
        |-------A-------|
            |-------A-------|
                |-------A-------|
            |-------B-------|
    */
#ifndef TOKU_RT_NOOVERLAPS
    r = toku_rt_create(&tree, int_cmp, char_cmp, true, test_incr_memory_size, test_decr_memory_size, NULL);
    CKERR(r);

    /* Verify we can insert a trivial range and lose it. */
    range.ends.left =  (toku_point*)&nums[1];
    range.ends.right = (toku_point*)&nums[1];
    range.data = (TXNID)letters[0];
    r = toku_rt_insert(tree, &range);   CKERR(r);
    size_t num_in_range = toku_rt_get_size(tree);
    assert(num_in_range == 1);
    r = toku_rt_delete(tree, &range);   CKERR(r);

    range.ends.left = (toku_point*)&nums[1];
    range.ends.right = (toku_point*)&nums[5];
    range.data = (TXNID)letters[0];
    r = toku_rt_insert(tree, &range);   CKERR(r);

    /* Try and fail to insert exact same thing. */
    r = toku_rt_insert(tree, &range);   CKERR2(r,EDOM);

    /* Try and succeed to insert (and delete) similar yet different things */
    range.ends.right = (toku_point*)&nums[6];
    r = toku_rt_insert(tree, &range);   CKERR(r);
    r = toku_rt_delete(tree, &range);   CKERR(r);
    range.ends.right = (toku_point*)&nums[5];
    range.data = (TXNID)letters[1];
    r = toku_rt_insert(tree, &range);   CKERR(r);
    r = toku_rt_delete(tree, &range);   CKERR(r);
    range.data = (TXNID)letters[0];

    range.ends.left = (toku_point*)&nums[2];
    range.ends.right = (toku_point*)&nums[6];
    range.data = (TXNID)letters[0];
    r = toku_rt_insert(tree, &range);   CKERR(r);

    range.ends.left = (toku_point*)&nums[3];
    range.ends.right = (toku_point*)&nums[7];
    range.data = (TXNID)letters[0];
    r = toku_rt_insert(tree, &range);   CKERR(r);

    range.ends.left = (toku_point*)&nums[2];
    range.ends.right = (toku_point*)&nums[6];
    range.data = (TXNID)letters[1];
    r = toku_rt_insert(tree, &range);   CKERR(r);

    range.ends.left = (toku_point*)&nums[2];
    range.ends.right = (toku_point*)&nums[6];
    range.data = (TXNID)letters[0];
    r = toku_rt_delete(tree, &range);   CKERR(r);

    /* Try to delete again, make sure it fails. (Not there anymore) */
    r = toku_rt_delete(tree, &range);   CKERR2(r,EDOM);

    range.ends.left = (toku_point*)&nums[2];
    range.ends.right = (toku_point*)&nums[6];
    range.data = (TXNID)letters[0];
    r = toku_rt_insert(tree, &range);   CKERR(r);

    range.ends.left = (toku_point*)&nums[2];
    range.ends.right = (toku_point*)&nums[6];
    range.data = (TXNID)letters[1];
    r = toku_rt_delete(tree, &range);   CKERR(r);

    /* Clean up. */
    range.ends.left = (toku_point*)&nums[1];
    range.ends.right = (toku_point*)&nums[5];
    range.data = (TXNID)letters[0];
    r = toku_rt_delete(tree, &range);   CKERR(r);

    range.ends.left = (toku_point*)&nums[2];
    range.ends.right = (toku_point*)&nums[6];
    range.data = (TXNID)letters[0];
    r = toku_rt_delete(tree, &range);   CKERR(r);

    range.ends.left = (toku_point*)&nums[3];
    range.ends.right = (toku_point*)&nums[7];
    range.data = (TXNID)letters[0];
    r = toku_rt_delete(tree, &range);   CKERR(r);
    /* Done */

    r = toku_rt_close(tree);            CKERR(r);

    tree = NULL;
#endif
    /* Test no overlap case. */
    /*
        1   2   3   4   5   6   7
        |---A---|
                    |---B---|
    */
    r = toku_rt_create(&tree, int_cmp, char_cmp, false, test_incr_memory_size, test_decr_memory_size, NULL);
    CKERR(r);

    /* Verify we can insert a trivial range and lose it. */
    range.ends.left = (toku_point*)&nums[1];
    range.ends.right = (toku_point*)&nums[1];
    range.data = (TXNID)letters[0];
    r = toku_rt_insert(tree, &range);   CKERR(r);
    r = toku_rt_delete(tree, &range);   CKERR(r);

    range.ends.left = (toku_point*)&nums[1];
    range.ends.right = (toku_point*)&nums[3];
    range.data = (TXNID)letters[0];
    r = toku_rt_insert(tree, &range);   CKERR(r);

    /* Try and fail to insert exact same thing. */
    r = toku_rt_insert(tree, &range);   CKERR2(r,EDOM);

    /* Try (and fail) to insert an overlapping range in a nooverlap tree. */
    range.ends.left = (toku_point*)&nums[0];
    range.ends.right = (toku_point*)&nums[4];
    range.data = (TXNID)letters[0];
    r = toku_rt_insert(tree, &range);   CKERR2(r,EDOM);
    
    /* Try (and fail) to insert an overlapping range (different data) in a
       nooverlap tree. */
    range.ends.left = (toku_point*)&nums[0];
    range.ends.right = (toku_point*)&nums[4];
    range.data = (TXNID)letters[1];
    r = toku_rt_insert(tree, &range);   CKERR2(r,EDOM);
    
    range.ends.left = (toku_point*)&nums[4];
    range.ends.right = (toku_point*)&nums[6];
    range.data = (TXNID)letters[1];
    r = toku_rt_insert(tree, &range);   CKERR(r);

    range.ends.left = (toku_point*)&nums[4];
    range.ends.right = (toku_point*)&nums[6];
    range.data = (TXNID)letters[1];
    r = toku_rt_delete(tree, &range);   CKERR(r);

    /* Try to delete again, make sure it fails. (Not there anymore) */
    r = toku_rt_delete(tree, &range);   CKERR2(r,EDOM);

    range.ends.left = (toku_point*)&nums[4];
    range.ends.right = (toku_point*)&nums[6];
    range.data = (TXNID)letters[1];
    r = toku_rt_insert(tree, &range);   CKERR(r);

    range.ends.left = (toku_point*)&nums[1];
    range.ends.right = (toku_point*)&nums[3];
    range.data = (TXNID)letters[0];
    r = toku_rt_delete(tree, &range);   CKERR(r);

    /* Clean up. */
    range.ends.left = (toku_point*)&nums[4];
    range.ends.right = (toku_point*)&nums[6];
    range.data = (TXNID)letters[1];
    r = toku_rt_delete(tree, &range);   CKERR(r);
    /* Done */

    r = toku_rt_close(tree);            CKERR(r);

    tree = NULL;

    return 0;
}