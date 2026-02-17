// renovation the header:(
#ifndef SCR_ENGINE_ENGINE_H
#define SCR_ENGINE_ENGINE_H

#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<cmath>
#include<map>
#include<algorithm>
//kinds of blocks
enum blockType{
    MOVE, //حرکت
    TURN, //چرخش
    SAY, // متن دریافتی از کاربر
    REPEAT, //تکرار فرایندها
    IF_THEN,// شرط گذاری برای اعمال فرایندها
    IF_ELSE, //بررسی شروط
    SET_VAR, // ایجاد متغیر و مقدار دهی آن
    CHANGE_VAR, // مورد استفاده هنگام نیاز به تغییر مقدار
    WHILE_LOOP, // حلقه شرطی برای تکرار، بر اساس بررسی
    WAIT, // دستور توقف برای ابزه زمانی معین
//    FREQUENT, // حلقه ای برای تکرار تا زمانی که دستور توقف صادر شود

};
// execution status
struct RuntimeState{

    int x=0, y=0, direction=0;
    std::map<std::string,int> variables;
    int max_loop=1000; // مقدار پیش فرض و قایل تغییر در صورت نیاز (برای جلوگیری ماندن در حلقه بی نهایت)
};
// blocks
struct Block{
    blockType type;
    int value=0;
    std::string text;
    std::vector<Block> inner_blocks;
    std::string condition;
    std::vector<Block> else_blocks;
};
//script
struct Script{
    std::vector<Block>blocks;
};

inline bool evaluateCondition(const std::string& cond, const RuntimeState& state){
    std::istringstream iss(cond);
    std::string vari, op;
    double num; //num=number & vari=variable
    iss >> vari >> op >> num;
    int left=0;
    if (vari == "x") left = state.x;
    else if (vari=="y") left = state.y;
    else if (vari=="direction") left = state.direction;
    else if (state.variables.count(vari)) left = state.variables.at(vari);
    else
        return false;

    if (op == ">") return left > num;
    if (op == "<") return left < num;
    if (op == "==") return left == num;
    if (op == "<=") return left <= num;
    if (op == ">=") return left >= num;
    if (op == "!=") return left != num;
    return false;
}


// run a block
inline void executeBlock(const Block& b, RuntimeState& state){
    switch (b.type) {
        case blockType::MOVE: {
            double rad = state.direction * M_PI / 180.0;
            state.x += static_cast<int>(b.value * std::cos(rad));
            state.y += static_cast<int>(b.value * std::sin(rad));
            std::cout << "Move to (" << state.x << ", " << state.y << ")\n";
            break;
        }

        case blockType::TURN:
            state.direction = (state.direction + b.value) % 360;
            if (state.direction < 0){
                state.direction += 360;
            }
            std::cout << "Turn to " << state.direction << " degrees\n";
            break;

        case blockType::SAY:
            std::cout << "Say : " << b.text << "\n";
            break;

        case blockType::SET_VAR:
            state.variables[b.text] = b.value;
            std:: cout << "Set " << b.text << " = " << b.value << "\n";
            break;

        case CHANGE_VAR:
            if (!state.variables.count(b.text)) state.variables[b.text] =0;
            state.variables[b.text] += b.value;
            std:: cout << "Change " << b.text <<" by " << b.value <<" -> " << state.variables[b.text] << "\n";
            break;

        case blockType::REPEAT:
            for (int i = 0; i < b.value; i++) {
                for (const Block &inner: b.inner_blocks) {
                    executeBlock(inner, state);
                }
            }
            break;
        case WHILE_LOOP:
            while (evaluateCondition(b.condition, state)){
                for (const Block& inner: b.inner_blocks){
                    executeBlock(inner, state);
                }
            }
            break;

        case blockType::IF_THEN:
            if (evaluateCondition(b.condition, state)) {
                for (const Block &inner: b.inner_blocks) {
                    executeBlock(inner, state);
                }
            }
            break;
        case IF_ELSE:
            if (evaluateCondition(b.condition, state)) {
                for (const Block &inner: b.inner_blocks) {
                    executeBlock(inner, state);
                }
            }
            else {
                for (const Block &inner: b.else_blocks) {
                    executeBlock(inner, state);
                }
            }
            break;

        case WAIT:
            std:: cout<< "wait for "<< b.value <<"\n";
            break;

//       case FREQUENT:
//           while (true){
//               for (const Block& inner : b.inner_blocks){
//                   executeBlock(inner, state);
//                   if (b.value >0) break;
//               }
//           }
//           break;
    }
}
// run the hole script
inline void runScript(const Script& script, RuntimeState& state) {
    for (const Block &b: script.blocks) {
        executeBlock(b, state);
    }
}
// decomposer(to be more likely to simple scratch_engine)
#endif //SCR_ENGINE_ENGINE_H
