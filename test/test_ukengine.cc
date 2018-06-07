#include <stdio.h>

#include <check.h> // for unit testing, apt install check

#include "ukengine.h"

START_TEST(testKeyCodeToEvent) {
    UkEngine a;
}
END_TEST

Suite *test_suit() {
    Suite *s = suite_create("Unikey Engine");
    /* Core test case */

    TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture(tc_core, NULL, NULL);

    tcase_add_checked_fixture(tc_core, NULL, NULL);

    tcase_add_test(tc_core, testKeyCodeToEvent);

    suite_add_tcase(s, tc_core);
    return s;
}

int main() {
    // utf8proc_int32_t codepoint = 0x00E3;
    // char dst[4];
    // utf8proc_encode_char(codepoint, dst);
    int number_failed;
    Suite *s    = test_suit();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return number_failed;
}
