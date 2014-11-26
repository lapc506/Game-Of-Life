#include <LifeSystem.h>

using namespace std;

int main(){
    LifeSystem* test = new LifeSystem();
    test->run();
    delete test;
    return 0;
}
