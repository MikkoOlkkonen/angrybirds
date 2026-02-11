#include "test_objects.hpp"
#include "test_leveldata.hpp"
#include "test_states.hpp"


int main () {
    testTakeDamage();
    testInvalidLevelFile();
    testValidLevelFile();
    testMenuButtonInit();
    // testMenuButtonClickRelease();
    // testMenuButtonHover();
    return 0;
}
