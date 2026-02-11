//#include"engine.h"
//int main(){
//
//    RuntimeState state ={0, 0, 0};
//    Block moveblock {blockType::MOVE, 10 };
//
//    Block saysth;
//    saysth.type = blockType:: SAY;
//    saysth.text = "The world isn't fair::meu meu";
//
//    Block ifBlock;
//    ifBlock.type = blockType::IF_THEN;
//    ifBlock.condition ="x > 20";
//    ifBlock.inner_blocks.push_back(saysth);
//
//    Block repeatBlock;
//    repeatBlock.type = blockType::REPEAT;
//    repeatBlock.value =3;
//    repeatBlock.inner_blocks.push_back(moveblock);
//
//    Script script;
//    script.blocks.push_back(repeatBlock);
//    script.blocks.push_back(ifBlock);
//
//    runScript(script, state);
//
//    return 0;
//}
#include "engine.h"

int main() {

    RuntimeState state;

    // set counter = 2
    Block setCounter;
    setCounter.type = SET_VAR;
    setCounter.text = "counter";
    setCounter.value = 2;

    // say Positive
    Block sayPositive;
    sayPositive.type = SAY;
    sayPositive.text = "Positive!";

    // say Zero or Negative
    Block sayNegative;
    sayNegative.type = SAY;
    sayNegative.text = "Zero or Negative!";

    // IF_ELSE counter > 0
    Block ifElseBlock;
    ifElseBlock.type = IF_ELSE;
    ifElseBlock.condition = "counter > 0";
    ifElseBlock.inner_blocks = { sayPositive };
    ifElseBlock.else_blocks = { sayNegative };

    Script script;
    script.blocks = { setCounter, ifElseBlock };

    runScript(script, state);

    return 0;
}