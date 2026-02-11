#include"engine.h"
int main(){

   RuntimeState state ={0, 0, 0};
   Block moveblock {blockType::MOVE, 10 };

   Block saysth;
   saysth.type = blockType:: SAY;
   saysth.text = "The world isn't fair::meu meu";

   Block ifBlock;
   ifBlock.type = blockType::IF_THEN;
   ifBlock.condition ="x > 20";
   ifBlock.inner_blocks.push_back(saysth);

   Block repeatBlock;
   repeatBlock.type = blockType::REPEAT;
   repeatBlock.value =3;
   repeatBlock.inner_blocks.push_back(moveblock);

   Script script;
   script.blocks.push_back(repeatBlock);
   script.blocks.push_back(ifBlock);

   runScript(script, state);

   return 0;
}
