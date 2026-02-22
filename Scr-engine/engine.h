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
#include<thread>
#include<chrono>
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
    FOREVER, // حلقه ای برای تکرار تا زمانی که دستور توقف صادر شود
    BROADCAST, //برای ارسال پیام به اسکرپت ها
    STOP_ALL, // برای اعلان توقف به همه اسکرپت ها
};

enum trigger_type{
    NONE,
    WHEN_GREEN_FLAG,
    WHEN_KEY,
    WHEN_CLICKED,
    WHEN_BROADCAST,
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
    std::string trigger_value;
    trigger_type trigger;
};

inline bool evaluateCondition(const std::string& cond, const RuntimeState& state){
    std::istringstream iss(cond);
    std::string vari, op;
    double num; //num=number & vari=variable
    iss >> vari >> op >> num;
    double left=0;
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
struct Active_script{
    const Script* script;
    size_t current_index = 0;
};

struct EngineData{
    std::vector<Script>scripts; // همه اسکریپت ها
    std::vector<Active_script>running_scripts; // اسکریپت های در حال اچرا
    RuntimeState state; // وضعیت اجرا به صورت کلی
    bool is_pro_running = false;  //بررسی اجرا یا عدم اجرا
    bool greenFlag = false; // برای برررسی وضعیت پرچم سبز
};
// اجرای اسکریپت بر اساس رویدادها
inline void dispatch_Event(EngineData& engine,trigger_type type, const std::string& value) {
    for (const auto &s: engine.scripts) {
        if (s.trigger == type) {
            if (type == WHEN_KEY && s.trigger_value == value) {
                engine.running_scripts.push_back({&s, 0});
            } else if (type == WHEN_BROADCAST && s.trigger_value == value) {
                engine.running_scripts.push_back({&s, 0});
            } else if (type == WHEN_GREEN_FLAG) {
                engine.running_scripts.push_back({&s, 0});
            } else if (type == WHEN_CLICKED) {
                engine.running_scripts.push_back({&s, 0});
            }
        }
    }
}

// run a block
inline void executeBlock(const Block& b, RuntimeState& state,EngineData* engine = nullptr){
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
        case WHILE_LOOP: {
            int safe_position = state.max_loop;
            while (evaluateCondition(b.condition, state) && safe_position--) {
                for (const Block &inner: b.inner_blocks) {
                    executeBlock(inner, state);
                }
            }
            if (safe_position <= 0){
                std::cout<<"the loop is stopped";
            }
            break;
        }

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
            std:: cout<< "wait for "<< b.value <<"seconds\n";
            std::this_thread::sleep_for(std::chrono::seconds(b.value));
            break;

       case FOREVER: {
           while (true) {
               for (const Block &inner: b.inner_blocks) {
                   executeBlock(inner, state, engine);
                   if(engine && engine -> running_scripts.empty()){
                       return;
                   }
               }
           }
       }
       case BROADCAST:
            std::cout << "Broadcast: "<< b.text <<"\n";
            if(engine){
                dispatch_Event(*engine, trigger_type:: WHEN_BROADCAST, b.text);
            }
           break;

        case STOP_ALL:
            std::cout << "all the scrips are stopped\n";
            if(engine){
                engine -> running_scripts.clear();
            }
            break;
    }
}

// اجرای اسکرپت های فعال همزمان
inline void tick(EngineData& engine){
    for(auto i = engine.running_scripts.begin(); i != engine.running_scripts.end(); ){
        if(i-> current_index < i-> script -> blocks.size()){
            executeBlock(i-> script -> blocks[i -> current_index], engine.state, &engine);
            i -> current_index++;
            i++;
        }
        else {
            i = engine.running_scripts.erase(i);
        }
    }
}

// run the hole script
inline void runScript(const Script& script, RuntimeState& state,EngineData* engine =nullptr) {
    for (const Block &b: script.blocks) {
        executeBlock(b, state, engine);
    }
}
//تابع شروع به کمک دستور پرچم سبز
inline void start_greenFlag(EngineData& engine){
    engine.is_pro_running = true;
    engine.greenFlag = true;
    engine.state.is_running = true;
    dispatch_Event(engine, WHEN_GREEN_FLAG,"");
}
//اجرای همزمان اسکریپت ها
inline void tick(EngineData& engine){
    if (!engine.state.is_running) {
        engine.running_scripts.clear();
        return;
    }

    for(auto i = engine.running_scripts.begin(); i != engine.running_scripts.end(); ){
        if(i-> current_index < i-> script -> blocks.size()){
            executeBlock(i-> script -> blocks[i -> current_index], engine.state, &engine);
            i -> current_index++;
            ++i;
        }
        else {
            i = engine.running_scripts.erase(i);
        }
    }
}
// نمایش وضعیت
inline void Status(const EngineData& engine){
    std::cout << "\nEngine Status\n";
    std::cout << "Position: " << engine.state.x << ", " << engine.state.y << "\n";
    std::cout << "Direction: " << engine.state.direction << " degrees\n";
    std::cout << "Running scripts: " << engine.running_scripts.size() << "\n";
    std::cout << "Program running: " << (engine.is_pro_running ? "Yes" : "No") << "\n";
    std::cout << "Variables:\n";
    for (const auto& var : engine.state.variables) {
        std::cout << "  " << var.first << " = " << var.second << "\n";
    }
    std::cout << "\n";

}
// decomposer(to be more likely to simple scratch_engine)
#endif //SCR_ENGINE_ENGINE_H
