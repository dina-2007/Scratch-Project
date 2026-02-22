//#include"engine.h"
//using namespace std;
//int main(){
//    RuntimeState state;
//    Script tentative;
//    int move_v, turn_v, repeat_num, var_v;
//    string say_t, var_n;
//    //بلوک حرکتی
//    cout << "move value: ";
//    cin >> move_v;
//    tentative.blocks.push_back(Block{MOVE, move_v, "",{},""});
//    //بلوک زاویه چرخشی
//    cout << "turn value : ";
//    cin >> turn_v;
//    tentative.blocks.push_back(Block{TURN, turn_v, "",{},""});
//    //بلوک متن
//    cin.ignore();
//    cout << "text: ";
//    getline(cin, say_t);
//    tentative.blocks.push_back(Block{SAY, 0, say_t,{},""});
//    // (نام و مقدار) بلوک تعیین متغیر
//    cout << " variable name: " ;
//    getline(cin, var_n);
//    cout << "variable value: ";
//    cin >> var_v;
//    tentative.blocks.push_back(Block{SET_VAR, var_v, var_n,{},"",{}});
//    //while_loop یلوک تعیین تعداد تکرار های حلقه
//    cout << "max_loop iterations:";
//    cin >> state.max_loop;
//    // بلوک تکرار
//    cout << "repetition: ";
//    cin>> repeat_num;
//    Block repeat_block;
//    repeat_block.type= REPEAT;
//    repeat_block.value = repeat_num;
//    repeat_block.inner_blocks.push_back(Block{MOVE,move_v, "", {}, ""});
//    repeat_block.inner_blocks.push_back(Block{TURN,turn_v, "", {}, ""});
//    tentative.blocks.push_back(repeat_block);
//
//    runScript(tentative, state);
//    return 0;
//}
#include "engine.h"

int main() {

    EngineData engine;
    // SCRIPT 1 : WHEN GREEN FLAG
    Script greenScript;
    greenScript.trigger = WHEN_GREEN_FLAG;

    Block sayHello;
    sayHello.type = SAY;
    sayHello.text = "Hello!";

    Block moveBlock;
    moveBlock.type = MOVE;
    moveBlock.value = 10;

    Block turnBlock;
    turnBlock.type = TURN;
    turnBlock.value = 45;

    greenScript.blocks.push_back(sayHello);
    greenScript.blocks.push_back(moveBlock);
    greenScript.blocks.push_back(turnBlock);

    engine.scripts.push_back(greenScript);

    // SCRIPT 2 : WHEN KEY (space)
    Script keyScript;
    keyScript.trigger = WHEN_KEY;
    keyScript.trigger_value = "space";

    Block sayKey;
    sayKey.type = SAY;
    sayKey.text = "Space Pressed!";

    Block changeVar;
    changeVar.type = CHANGE_VAR;
    changeVar.text = "score";
    changeVar.value = 1;

    keyScript.blocks.push_back(sayKey);
    keyScript.blocks.push_back(changeVar);

    engine.scripts.push_back(keyScript);

    // SCRIPT 3 : WHEN CLICKED
    Script clickScript;
    clickScript.trigger = WHEN_CLICKED;

    Block sayClick;
    sayClick.type = SAY;
    sayClick.text = "Sprite Clicked!";

    clickScript.blocks.push_back(sayClick);

    engine.scripts.push_back(clickScript);

    // MENU
    int choice;

    while (true) {

        std::cout << "\n.... Scratch Engine Menu ....\n";
        std::cout << "1. Green Flag\n";
        std::cout << "2. Press Key (space)\n";
        std::cout << "3. Click Sprite\n";
        std::cout << "4. Tick (Run one step)\n";
        std::cout << "5. Show Status\n";
        std::cout << "6. Exit\n";
        std::cout << "Choose: ";
        std::cin >> choice;

        if (choice == 1) {
            start_greenFlag(engine);
        }
        else if (choice == 2) {
            engine.is_pro_running = true;
            dispatch_Event(engine, WHEN_KEY, "space");
        }
        else if (choice == 3) {
            engine.is_pro_running = true;
            dispatch_Event(engine, WHEN_CLICKED, "");
        }
        else if (choice == 4) {
            tick(engine);
        }
        else if (choice == 5) {
            Status(engine);
        }
        else if (choice == 6) {
            break;
        }
    }

    return 0;
}
