//آزمایش نحوه اجرا از طریق پرسش از کاربر
#include"engine.h"
using namespace std;
int main(){
    RuntimeState state;
    Script tentative;
    int move_v, turn_v, repeat_num, var_v;
    string say_t, var_n;
    //بلوک حرکتی
    cout << "move value: ";
    cin >> move_v;
    tentative.blocks.push_back(Block{MOVE, move_v, "",{},""});
    //بلوک زاویه چرخشی
    cout << "turn value : ";
    cin >> turn_v;
    tentative.blocks.push_back(Block{TURN, turn_v, "",{},""});
    //بلوک متن
    cin.ignore();
    cout << "text: ";
    getline(cin, say_t);
    tentative.blocks.push_back(Block{SAY, 0, say_t,{},""});
    // (نام و مقدار) بلوک تعیین متغیر
    cout << " variable name: " ;
    getline(cin, var_n);
    cout << "variable value: ";
    cin >> var_v;
    tentative.blocks.push_back(Block{SET_VAR, var_v, "",{},""});
    // بلوک تکرار
    cout << "repetition: ";
    cin>> repeat_num;
    Block repeat_block;
    repeat_block.type= REPEAT;
    repeat_block.value = repeat_num;
    repeat_block.inner_blocks.push_back(Block{MOVE,move_v, "", {}, ""});
    repeat_block.inner_blocks.push_back(Block{TURN,turn_v, "", {}, ""});
    tentative.blocks.push_back(repeat_block);

    runScript(tentative, state);
    return 0;
}





//#include"engine.h"
//int main(){
//
//    RuntimeState state;
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
