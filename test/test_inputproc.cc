#include <stdio.h>

#include <check.h> // for unit testing, apt install check

#include "inputproc.h"

void print_event(UkKeyEvent &event);
bool compare_event(const UkKeyEvent &event, const UkKeyEvent &other);

UkInputProcessor kProcessor;

void Setup(void) {
    kProcessor.init(); // Called SetupInputClassifierTable() here
}

START_TEST(testKeyCodeToEvent) {
    struct _tcase {
        UkInputMethod   im;
        uint32_t            keycode;
        UkKeyEvent      event;
    };
    typedef struct _tcase tcase;

    tcase test_cases[] = {
        {UkTelex, 'z', {vneTone0, ukcVn, vnl_z, 'z', 0}},
        {UkTelex, 's', {vneTone1, ukcVn, vnl_s, 's', 1}},
        {UkTelex, 'f', {vneTone2, ukcNonVn, vnl_f, 'f', 2}},
        {UkTelex, 'r', {vneTone3, ukcVn, vnl_r, 'r', 3}},
        {UkTelex, 'x', {vneTone4, ukcVn, vnl_x, 'x', 4}},
        {UkTelex, 'j', {vneTone5, ukcNonVn, vnl_j, 'j', 5}},

        // mapping
        {UkTelex, '[', {vneMapChar, ukcVn, vnl_oh, '[', 5}}, // 5 from the last test

        {UkVni, '0', {vneTone0, ukcNonVn, vnl_nonVnChar, '0', 0}},
        {UkVni, '1', {vneTone1, ukcNonVn, vnl_nonVnChar, '1', 1}},
        {UkVni, '2', {vneTone2, ukcNonVn, vnl_nonVnChar, '2', 2}},
        {UkVni, '3', {vneTone3, ukcNonVn, vnl_nonVnChar, '3', 3}},
        {UkVni, '4', {vneTone4, ukcNonVn, vnl_nonVnChar, '4', 4}},
        {UkVni, '5', {vneTone5, ukcNonVn, vnl_nonVnChar, '5', 5}},
    };

    tcase emptycase;
    size_t num_cases = sizeof(test_cases) / sizeof(emptycase);

    UkInputMethod old_im = UkTelex;

    uint32_t keycode = 0;
    UkKeyEvent *want;
    UkKeyEvent event = {0};
    UkInputMethod im = UkTelex;

    for (size_t i = 0; i < num_cases; ++i) {
        im = test_cases[i].im;
        keycode = test_cases[i].keycode;
        want = &(test_cases[i].event);
        if (im != old_im) {
            kProcessor.setIM(im);
            old_im = im;
        }

        kProcessor.keyCodeToEvent(keycode, event);
        ck_assert(compare_event(event, *want));
    }
}
END_TEST

Suite *test_suit() {
    Suite *s = suite_create("Unikey Input Processor");
    /* Core test case */

    TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture(tc_core, NULL, NULL);

    tcase_add_checked_fixture(tc_core, Setup, NULL);

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

void print_event(UkKeyEvent &event) {
    printf("event: type %d, chType %d, lexi %d, code %d, tone %d\n",
            event.evType, (int)event.chType, (int)event.vnSym,
            event.keyCode, event.tone);
}

bool compare_event(const UkKeyEvent &event, const UkKeyEvent &other) {
    return event.evType == other.evType && event.chType == other.chType &&
           event.vnSym == other.vnSym && event.keyCode == other.keyCode &&
           event.tone == other.tone;
}
