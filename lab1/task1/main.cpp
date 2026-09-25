#include "src/Duck/DecoyDuck.h"
#include "src/Duck/MallardDuck.h"
#include "src/Duck/ModelDuck.h"
#include "src/Duck/RedheadDuck.h"
#include "src/Duck/RubberDuck.h"
#include "src/DuckFunctions.h"
#include <cstdlib>

int main() {
    MallardDuck mallardDuck;
    PlayWithDuck(mallardDuck);

    RedheadDuck redheadDuck;
    PlayWithDuck(redheadDuck);

    RubberDuck rubberDuck;
    PlayWithDuck(rubberDuck);

    DecoyDuck decoyDuck;
    PlayWithDuck(decoyDuck);

    ModelDuck modelDuck;
    PlayWithDuck(modelDuck);

    modelDuck.SetFlyBehavior(std::make_unique<FlyWithWings>());
    PlayWithDuck(modelDuck);

    return EXIT_SUCCESS;
}