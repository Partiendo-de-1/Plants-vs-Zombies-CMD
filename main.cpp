#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<functional>
#include<cstdlib>
#include<ctime>
#include<map>
#include<limits>
#include<chrono>
#include<thread>
#include<irrKlang.h>

#include "rlutil.h"

using namespace irrklang;
using namespace rlutil;

int WHIDTH = 133;
int HEIGHT = 50;

struct Buffer{
    std::vector<int> sp;
    int size_x;
    int size_y;
    int x;
    int y;
    int z = 0;
    int pos_bff = 0;
};

struct Text{
    std::string txt;
    std::vector<int> txtc;
    
    int x;
    int y;
    
    int rows;
    int cols;
};

struct Position{
    int x = 0;
    int y = 0;
};

struct Timer{
    int init = 0;
    int act = 0;
    int end = 0;
    bool is_plant = false;
    bool is_zombie = false;
    struct Plant* plant;
    struct Zombie* zombie;
    std::function<void(struct Timer* t)> call;
    std::function<void(struct Timer* t)> reset;
};
struct ItemInteract{
    bool is_q = false;
    bool is_e = false;
    std::function<void()> Q;
    std::function<void()> E;
};

const int NULL_ACTION = 0;
const int SEEDS = 1;
const int SHOVEL = 2;

const int PLANT_NO_PLANT = 0;
const int PLANT_PEASHOTER = 1;
const int PLANT_SUNFLOWER = 2;
const int PLANT_NUT = 3;

const int LIFE_PLANTS = 25;
const int NUT_LIFE = 80;
const int LIFE_ZOMBIES = 150;
const int LGIS_DMG = 60;
const int NORMAL_ZOMBIE_DMG = 10;

const int TIME_TIMER_CONTINUE_ZOMBIES_MAX = 3500;
int TIME_TIMER_CONTINUE_ZOMBIES = TIME_TIMER_CONTINUE_ZOMBIES_MAX;
int TIME_TIMER_INIT_ZOMBIES = 4000;

struct Plant{
    struct Buffer* bff;
    int x = 0;
    int y = 0;
    int life = LIFE_PLANTS;
    bool is_process = false;
    bool is_timer = false;
    int type = PLANT_NO_PLANT;
    int dmg = LGIS_DMG;
    struct Timer* time;
    std::function<void(int,int,int)> process;
};

const int PEASHOOT_TIME = 800;
const int PEASHOOT_TIME_MOVE = 50;
const int NUT_TIME = 50;

struct Zombie{
  struct Buffer* bff;
  struct Timer* t_move;
  int x = 0;
  int y = 0;
  int dmg = NORMAL_ZOMBIE_DMG;
  int life = LIFE_ZOMBIES;
};
const int TIME_TIMER_ZOMBIE_MOVE = 800;

int PLANT_SEED_ACT = PLANT_NO_PLANT;

int Selected_from_action = NULL_ACTION;

int MAX_SAMETIME_ZOMBIES = 3;

struct Timer* SELECT_TIMER;
struct Position* SIZE_MARKS;
struct Position* SELECTED;
struct Buffer* SELECT_IMG;
struct Buffer* SUN_IMG;

int TIME_TIMER_MARK = 200;

struct Buffer* DISP_BUFFER;
struct Buffer* ANT_BUFFER;
struct Text* DISP_TEXT;
struct Text* ANT_TEXT;

bool act_asset_mark = false;
bool PAUSE = false;
bool GAME_OVER = false;
bool WAIT_RESIZE = false;

std::vector<struct Buffer*> list;
std::vector<int> list_z;
std::map<int, struct Buffer*> map_bff;
std::vector<struct ItemInteract*> selectableItems;
std::vector<struct Timer*> timers;
std::vector<struct Plant*> crops;
std::vector<struct Text*> texts;

std::vector<int> MARKER_ASSET_1;
std::vector<int> MARKER_ASSET_2;

bool app_exit = false;
std::vector<int> readAsset(std::string fname);
void display(Buffer b);
void re_display();
void draw_loop();
void time_loop();
struct Buffer* start_buffer(struct Buffer *b, int size_x, int size_y, int c);
void draw(struct Buffer * b);
void start_selectable_grass();
void start_selectable_items();
void action_q();
void push_plant();
void move_mark(int x, int y);
int get_timer_pos(struct Timer* t);

void re_text_display();
void text_loop();
struct Text* start_text(struct Text *b, int size_x, int size_y, char c);
void draw_text(struct Text * b);

void start_seeds();
void start_grass();

void lost();
bool exsist_in_list(std::vector<int> list, int ex);

void callback_select_mark(struct Timer* t);
void reset_select_mark(struct Timer* t);
void callback_soles_spawn(struct Timer* t);
void reset_soloes_spawn(struct Timer* t);
void girasol_timer(struct Timer* t);
void girasol_timer_reset(struct Timer* t);
void callback_zombie_move(struct Timer* t);
void zombie_move_timer_reset(struct Timer* t);
void get_seeds();
void callback_init_zombies(struct Timer* t);
void reset_init_zombies(struct Timer* t);
void callback_continue_zombies(struct Timer* t);
void reset_continue_zombies(struct Timer* t);
void peashooter_timer_callback(struct Timer* t);
void peashooter_timer_reset(struct Timer* t);
void peashoot_timer_callback(struct Timer* t);
void peashoot_timer_reset(struct Timer* t);
void nut_timer(struct Timer* t);
void nut_timer_reset(struct Timer* t);

void reset();
void Create_mark();
void load_assets();
void load_flowerpots();
void load_interface();
void load_init_zombie_timer();

struct Zombie* create_zombie(int type, int y);
bool is_zombie_in_this(int x, int y);

int push_map_bff(struct Buffer* bf);
struct Position* get_cell_pos(int x, int y);

std::vector<int> marker;
std::vector<int> peashoter;
std::vector<int> sunflower;
std::vector<int> nut;
std::vector<int> nut_1;
std::vector<int> nut_2;
std::vector<int> nut_3;
std::vector<int> nut_4;
std::vector<int> nut_5;
std::vector<int> nut_6;
std::vector<int> nut_7;
std::vector<int> nut_8;

std::vector<int> grass;
std::vector<int> asphalt;
std::vector<int> sun;
std::vector<int> zombie;
std::vector<int> zombie_cone;
std::vector<int> zombie_bucket;
std::vector<int> shovel;

std::vector<bool> suns_in_grass;
std::vector<int> suns_in_grass_pos;
std::vector<struct Buffer*> suns_in_grass_bff;


std::vector<struct Zombie*> zombies;
int SUNS = 250;//girasol 50, lanzagisantes 100, nuez 50, "sol" 25 

struct Timer* SUNS_TIMER;
struct Buffer* ACT_PLANT_IMG = nullptr;
int pos_act_plant_img = -2;
int TIME_TIMER_SOLOES = 0;//range(min_range_spw_sol, max_range_spw_sol)

int min_range_spw_sol = 1;
int max_range_spw_sol = 5;

int min_range_spw_sunflower = 300;
int max_range_spw_sunflower = 500;

int time_1_range_spw_sol = 250;
int time_2_range_spw_sol = 5;//TIME_TIMER_SOLOES*time_1_range_spw_sol*time_2_range_spw_sol

int idx_map_bff = -1;

struct Timer* init_timer_for_zombies;
int itfz_pos_in_list = 0;
struct Timer* continue_timer_for_zombies;

const int MIN_TIME_TIMER = 500000;

const int MIN_TIME_ZOMBIE_SUBSTRACT = 100;

struct Text* SUNFLOWERTEXT;
struct Text* PEASHOOTERTEXT;
struct Text* SUNTEXT;
struct Text* NUTTEXT;

bool special_zombies = false;

ISoundEngine* audio;

irrklang::ISoundSource* shootSound;
irrklang::ISoundSource* crashSound;
irrklang::ISoundSource* musicSound;

int main(){
    DISP_BUFFER = new struct Buffer;
    ANT_BUFFER = new struct Buffer;

    DISP_BUFFER = start_buffer(DISP_BUFFER, WHIDTH, HEIGHT, BLUE);
    ANT_BUFFER = start_buffer(ANT_BUFFER, WHIDTH, HEIGHT, BLACK);
    audio = createIrrKlangDevice();
   if (!audio)
      return 0;
    
    cls();
    load_assets();
    Create_mark();
    
    load_flowerpots();
    load_interface();
    load_init_zombie_timer();

    start_selectable_items();
 
    start_seeds();
    start_grass();
    
	srand((unsigned) time(NULL));
    
    bool stop = false;
    bool is_draw_scroll_msg = false;
    audio->play2D(musicSound, true);
    while(!app_exit){

        fflush(stdout);
        if(trows() < HEIGHT || tcols() < WHIDTH){
            WAIT_RESIZE = true;
            PAUSE = true;
            if(!is_draw_scroll_msg){
                cls();
                setColor(WHITE);
                setBackgroundColor(BLACK);
                std::cout<<"Redimensiona con control + rueda del raton / Resize with control + mouse wheel";
                is_draw_scroll_msg = true;
            }
            continue;
        }else{
            PAUSE = false;
            is_draw_scroll_msg = false;
        }
        if(WAIT_RESIZE){
            WAIT_RESIZE = false;
            ANT_BUFFER->sp.clear();
            ANT_BUFFER = start_buffer(ANT_BUFFER, WHIDTH, HEIGHT, BLACK);
            ANT_TEXT->txt.clear();
            ANT_TEXT->txtc.clear();
            ANT_TEXT = start_text(ANT_TEXT, WHIDTH, HEIGHT, ' ');
        }
        if(GAME_OVER){
            if(kbhit()){
                char k = getch();
                if(k == ' ' || k == 13){
                    GAME_OVER = false;
                    reset();
                }
            }
            continue;
        }
        if(stop){
            if(kbhit()){
                char k = getch();
                if(k == 27){
                    stop = !stop;
                }
            }
            continue;
        }
        time_loop();
        DISP_BUFFER->sp.clear();
        DISP_BUFFER = start_buffer(DISP_BUFFER, WHIDTH, HEIGHT, BLUE);
        DISP_TEXT->txt.clear();
        DISP_TEXT->txtc.clear();
        DISP_TEXT = start_text(DISP_TEXT, WHIDTH, HEIGHT, ' ');
        SUNTEXT->txt = std::to_string(SUNS);
        SUNTEXT->rows = SUNTEXT->txt.size();
        
        if(kbhit()){
            char k = getch();
            if(k == 27){
                stop = !stop;
            }
            if(k == 'g' || k == 'G'){
                app_exit = true;
            }

            if(k == 'a' || k == 'A'){
                move_mark(-1,0);
            }
            if(k == 's' || k == 'S'){
                move_mark(0,1);
            }
            if(k == 'w' || k == 'W'){
                move_mark(0,-1);
            }
            if(k == 'd' || k == 'D'){
               move_mark(1,0);
            }
            if(k == 'q' || k == 'Q'){
                action_q();
            }
            if(k == 'f' || k == 'F'){
                PLANT_SEED_ACT = PLANT_NO_PLANT;
                Selected_from_action = NULL_ACTION;
                ACT_PLANT_IMG->sp = MARKER_ASSET_1;
                ACT_PLANT_IMG->size_x = 12;
                ACT_PLANT_IMG->size_y = 7;
            }
        }
        draw_loop();
        re_display();
        text_loop();
        re_text_display();
    }
    audio->drop(); 
    setColor(WHITE);
    setBackgroundColor(BLACK);
    cls();
    std::cout<<"END OF PROGRAM / FIN DEL PROGRAMA";
    getch();
    return 0;
}

//Game managment functions


void action_q(){
    if((SIZE_MARKS->x*SELECTED->y)+SELECTED->x < selectableItems.size()-1){
        
        if(selectableItems[(SIZE_MARKS->x*SELECTED->y)+SELECTED->x]->is_q){
           
            selectableItems[(SIZE_MARKS->x*SELECTED->y)+SELECTED->x]->Q();
        }
    }
}

void move_mark(int x, int y){
    if( !(SELECTED->x+x >= SIZE_MARKS->x) &&  SELECTED->x+x >= 0 ){
        SELECTED->x += x;
        SELECT_IMG->x = SELECTED->x*12;
    }
    if( !(SELECTED->y+y >= SIZE_MARKS->y) &&  SELECTED->y+y >= 0 ){
        SELECTED->y += y;
        SELECT_IMG->y = SELECTED->y*7;
    }
}

struct Position* get_cell_pos(int x, int y){
    struct Position* p = new struct Position;
    int x_act = -1;
    while(x > 0){
        x-=12;
        x_act++;
    }
    int y_act = -1;
    while(y > 0){
        y-=7;
        y_act++;
    }
    p->x = x_act == -1 ? 0 : x_act;
    p->y = y_act == -1 ? 0 : y_act;
    return p;
}

bool exsist_in_list(std::vector<int> list, int ex){
    for(int i=0;i<list.size();i++){
        if(list[i] == ex){
            return true;
        }
    }
    return false;
}

void lost(){
    std::vector<int> perdida = readAsset("Asets/lost.txt");
    int x = ((int)(WHIDTH/3)+12);
    int y = ((int)(HEIGHT/2)-4);
    for(int i=0;i<perdida.size();i++){
        x++;
        
        if(perdida[i] != -1){
            locate(x,y);
            setBackgroundColor(perdida[i]);
            std::cout<<" ";
        }
        if(x >= (WHIDTH/3)+12+12){
            x = ((int)(WHIDTH/3)+12);
            y ++;
        }
    }
    x = ((int)(WHIDTH/3)+12);
    y = ((int)(HEIGHT/2)-4);
    setBackgroundColor(LIGHTRED);
    locate(x+2,y);
    setColor(WHITE);
    std::cout<<"Game  Over";
    GAME_OVER = true;
}



void reset(){
    cls();
    for(int i=0;i<crops.size();i++){
        delete crops[i];
    }
    crops.clear();
    for(auto const& i : map_bff){
        delete i.second;
    }
    map_bff.clear();
    suns_in_grass.clear();
    suns_in_grass_bff.clear();
    suns_in_grass_pos.clear();
    for(int i=0;i<zombies.size();i++){
        delete zombies[i];
    }
    zombies.clear();
    for(int i=0;i<texts.size();i++){
        delete texts[i];
    }
    
    texts.clear();
    for(int i=0;i<timers.size();i++){
        delete timers[i];
    }
    
    timers.clear();

    for(int i=0;i<selectableItems.size();i++){
        delete selectableItems[i];
    }
    selectableItems.clear();
    Selected_from_action = NULL_ACTION;
    PLANT_SEED_ACT = PLANT_NO_PLANT;
    TIME_TIMER_CONTINUE_ZOMBIES = TIME_TIMER_CONTINUE_ZOMBIES_MAX;
    idx_map_bff = -1;
    delete SIZE_MARKS;
    delete SELECTED;

    ANT_BUFFER->sp.clear();
    ANT_BUFFER = start_buffer(ANT_BUFFER, WHIDTH, HEIGHT, BLACK);
    DISP_BUFFER->sp.clear();
    DISP_BUFFER = start_buffer(DISP_BUFFER, WHIDTH, HEIGHT, BLUE);
    DISP_TEXT = start_text(DISP_TEXT, WHIDTH, HEIGHT, ' ');
    PAUSE = false;
    GAME_OVER = false;
    
    ANT_TEXT->txt.clear();
    ANT_TEXT->txtc.clear();
    ANT_TEXT = start_text(ANT_TEXT, WHIDTH, HEIGHT, ' ');
    Create_mark();
    
    load_flowerpots();
    load_interface();
    load_init_zombie_timer();
    start_selectable_items();
 
    start_seeds();
    start_grass();

    SUNS = 250;
}
//End Game managment functions

//ItemGrid functions

void start_selectable_items(){
    for(int i=0;i<SIZE_MARKS->y;i++){

        for(int j=0;j<SIZE_MARKS->x;j++){
            struct ItemInteract* iit = new struct ItemInteract;
            if(j <= 3 && j >= 0 && i == 0){
                iit->Q = get_seeds;
                iit->is_q = true;
            }else if(i > 0 && j < SIZE_MARKS->x-1){
                iit->is_q = true;
                iit->Q = push_plant;
            }
            selectableItems.push_back(iit);
        }
    }
}

//End ItemGrid functions

//zombie managment functions

bool is_zombie_in_this(int x, int y){
    for(int i=0;i<zombies.size();i++){
        if(zombies[i]->x == x && zombies[i]->y == y){
            return true;
        }
    }
    return false;
}

struct Zombie* get_zombie_by_pos(int x, int y){
    for(int i=0;i<zombies.size();i++){
        if(zombies[i]->y == y && zombies[i]->x == x){
            return zombies[i];
        }
    }
    return nullptr;
}
//End zombie managment functions

//Plant functions

void get_seeds(){

    if(SELECTED->x == 0){//peashoter
        if(PLANT_SEED_ACT == PLANT_PEASHOTER){
            PLANT_SEED_ACT = PLANT_NO_PLANT;
            Selected_from_action = NULL_ACTION;
            ACT_PLANT_IMG->sp = MARKER_ASSET_1;
            ACT_PLANT_IMG->size_x = 12;
            ACT_PLANT_IMG->size_y = 7;
        }else if(SUNS >= 100){
            
            PLANT_SEED_ACT = PLANT_PEASHOTER;
            Selected_from_action = SEEDS;
            ACT_PLANT_IMG->sp = peashoter;
            ACT_PLANT_IMG->size_x = 5;
            ACT_PLANT_IMG->size_y = 6;
            
        }
    }else if(SELECTED->x == 1){//sunflower
        if(PLANT_SEED_ACT == PLANT_SUNFLOWER){
            PLANT_SEED_ACT = PLANT_NO_PLANT;
            Selected_from_action = NULL_ACTION;
            ACT_PLANT_IMG->sp = MARKER_ASSET_1;
            ACT_PLANT_IMG->size_x = 12;
            ACT_PLANT_IMG->size_y = 7;
        }else if(SUNS >= 50){
            PLANT_SEED_ACT = PLANT_SUNFLOWER;
            Selected_from_action = SEEDS;
            ACT_PLANT_IMG->sp = sunflower;
            ACT_PLANT_IMG->size_x = 12;
            ACT_PLANT_IMG->size_y = 7;
        }
    }else if(SELECTED->x == 2){//nut
        if(PLANT_SEED_ACT == PLANT_NUT){
            PLANT_SEED_ACT = PLANT_NO_PLANT;
            Selected_from_action = NULL_ACTION;
            ACT_PLANT_IMG->sp = MARKER_ASSET_1;
            ACT_PLANT_IMG->size_x = 12;
            ACT_PLANT_IMG->size_y = 7;
        }else if(SUNS >= 50){
            PLANT_SEED_ACT = PLANT_NUT;
            Selected_from_action = SEEDS;
            ACT_PLANT_IMG->sp = nut;
            ACT_PLANT_IMG->size_x = 12;
            ACT_PLANT_IMG->size_y = 7;
        }
    }else if(SELECTED->x == 3){//shovel
        if(Selected_from_action != SHOVEL){
            PLANT_SEED_ACT = PLANT_NO_PLANT;
            Selected_from_action = SHOVEL;
            ACT_PLANT_IMG->sp = shovel;
            ACT_PLANT_IMG->size_x = 12;
            ACT_PLANT_IMG->size_y = 7;
        }else{
            PLANT_SEED_ACT = PLANT_NO_PLANT;
            Selected_from_action = NULL_ACTION;
            ACT_PLANT_IMG->sp = MARKER_ASSET_1;
            ACT_PLANT_IMG->size_x = 12;
            ACT_PLANT_IMG->size_y = 7;
        }
    }
}
void push_plant(){
    bool del_sol = false;
    if(SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1)) <= suns_in_grass.size()-1){
        if(suns_in_grass[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))]){
            suns_in_grass[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))] = false;
            if(map_bff.find(suns_in_grass_bff[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))]->pos_bff) != map_bff.end()){
                delete map_bff[suns_in_grass_bff[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))]->pos_bff];
                map_bff.erase(suns_in_grass_bff[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))]->pos_bff);
                
            }else{
                delete suns_in_grass_bff[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))];
            }
            struct Buffer* b = new struct Buffer;
            suns_in_grass_bff[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))] = b;
            SUNS += 25;
            del_sol = true;
        }
    }
    if(Selected_from_action == SEEDS && !del_sol){
        if(PLANT_SEED_ACT != PLANT_NO_PLANT){
            if(crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))]->type == PLANT_NO_PLANT){
                
                if(PLANT_SEED_ACT == PLANT_PEASHOTER){
                    if(SUNS >= 100){
        
                        struct Plant* p = new struct Plant;
                        struct Buffer* b = new struct Buffer;
                        struct Timer* t = new struct Timer;
                        t->init = PEASHOOT_TIME;
                        t->act = PEASHOOT_TIME;
                        t->end = 0;
                        t->call = peashooter_timer_callback;
                        t->plant = p;
                        t->is_plant = true;
                        t->reset = peashooter_timer_reset;
                        timers.push_back(t);
                        b->sp = peashoter;
                        b->x = SELECTED->x*12;
                        b->y = SELECTED->y*7;
                        b->size_x = 5;
                        b->size_y = 6;
                        b->z = 25;

                        p->bff = b;
                        p->is_timer = true;
                        p->time = t;
                        p->type = PLANT_SEED_ACT;
                        p->x = SELECTED->x;
                        p->y = SELECTED->y;
                        int id = push_map_bff(b);
                        b->pos_bff = id;
                        delete crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))];
                        crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))] = p;
                        SUNS -= 100;
                    }
                }else if(PLANT_SEED_ACT == PLANT_SUNFLOWER){
                    if(SUNS >= 50){
                        struct Plant* p = new struct Plant;
                        struct Buffer* b = new struct Buffer;
                        struct Timer* t = new struct Timer;
                        
                        int range = max_range_spw_sunflower - min_range_spw_sunflower + 1;
                        int random = rand() % range + min_range_spw_sunflower;
                        t->init = random;
                        t->act = random;
                        t->end = 0;
                        t->call = girasol_timer;
                        t->plant = p;
                        t->is_plant = true;
                        t->reset = girasol_timer_reset;
                        timers.push_back(t);

                        b->sp = sunflower;
                        b->x = SELECTED->x*12;
                        b->y = SELECTED->y*7;
                        b->size_x = 12;
                        b->size_y = 7;
                        b->z = 25;

                        p->bff = b;
                        p->time = t;
                        p->is_timer = true;
                        p->type = PLANT_SEED_ACT;

                        p->x = SELECTED->x;
                        p->y = SELECTED->y;
                        int id = push_map_bff(b);
                        b->pos_bff = id;
                        delete crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))];
                        crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))] = p;
                        SUNS -= 50;
                    }
                }else if(PLANT_SEED_ACT == PLANT_NUT){
                    if(SUNS >= 50){
                        struct Plant* p = new struct Plant;
                        struct Buffer* b = new struct Buffer;
                        struct Timer* t = new struct Timer;
                        
                        t->init = NUT_TIME;
                        t->act = NUT_TIME;
                        t->end = 0;
                        t->call = nut_timer;
                        t->plant = p;
                        t->is_plant = true;
                        t->reset = nut_timer_reset;
                        timers.push_back(t);

                        b->sp = nut;
                        b->x = SELECTED->x*12;
                        b->y = SELECTED->y*7;
                        b->size_x = 12;
                        b->size_y = 7;
                        b->z = 25;

                        p->bff = b;
                        p->time = t;
                        p->is_timer = true;
                        p->type = PLANT_SEED_ACT;
                        p->life = NUT_LIFE;
                        p->x = SELECTED->x;
                        p->y = SELECTED->y;
                        int id = push_map_bff(b);
                        b->pos_bff = id;
                        delete crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))];
                        crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))] = p;
                        SUNS -= 50;
                    }
                }
            }
        }
    }else if(Selected_from_action == SHOVEL && !del_sol){
        if(crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))]->type != PLANT_NO_PLANT){
            map_bff.erase(crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))]->bff->pos_bff);
            delete crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))]->bff;

            if(crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))]->is_timer){
                int tp = get_timer_pos(crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))]->time);
                timers.erase(timers.begin()+tp);
                delete crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))]->time;
            }
            delete crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))];
            struct Plant* plantx = new struct Plant;
            crops[SELECTED->x + ((SELECTED->y-1)*(SIZE_MARKS->x-1))] = plantx;
        }
    }

}

//End Plant functions


//Draw functions

void draw_loop(){
    std::vector<struct Buffer*> list_z;
    int previous = 0;
    int minor = 0;
    int major = 0;
    bool init = false;
    for(auto const& i : map_bff){
        list_z.push_back(i.second);
    }
    for(int i=0;i<list_z.size();i++){
        if(list_z[i]->z < previous){
            struct Buffer* b = list_z[i];
            list_z.erase(list_z.begin()+i);
            list_z.insert(list_z.begin()+i-1, b);
            i = 0;
        }
        previous = list_z[i]->z;
    }
    for(int i=0;i<list_z.size();i++){
        draw(list_z[i]);
    }
}

struct Buffer* start_buffer(struct Buffer *b, int size_x, int size_y, int c){
    for(int i=0;i<size_x*size_y;i++){
        b->sp.push_back(c);
    }
    b->size_x = size_x;
    b->size_y = size_y;
    return b;
}

void draw(struct Buffer *b){
    int pxamount = 0;
    int x = 0;
    int y = 0;
    
    
    for(int i=((DISP_BUFFER->size_x-1)*b->y)+b->x;i<DISP_BUFFER->sp.size()-1;i++){
        if(pxamount > b->sp.size()-1){
            i = DISP_BUFFER->sp.size();
            continue;
        }
        if(x >= b->size_x){

            i += DISP_BUFFER->size_x-b->size_x-1;
            x = 0;
        }
        if(b->sp[pxamount] != -1 && i < DISP_BUFFER->sp.size()){
            DISP_BUFFER->sp[i] = b->sp[pxamount];
        }
        pxamount += 1;
        x += 1;
    }
}

void re_display(){
        int quantity = 0;
        for(int y = 0;y < DISP_BUFFER->size_y-1;y++){
            for(int x = 0;x < DISP_BUFFER->size_x-1;x++){
                if(DISP_BUFFER->sp[quantity] != ANT_BUFFER->sp[quantity]){ 
                    locate(x+1,y+1);
                    setBackgroundColor(DISP_BUFFER->sp[quantity]);
                    

                    std::cout<<" ";
                    ANT_BUFFER->sp[quantity] = DISP_BUFFER->sp[quantity];
                    ANT_TEXT->txt[quantity] = ' ';
                }
                quantity++;
                   
            }
            
        }

    setBackgroundColor(BLACK);
}

void display(Buffer b){//para el buffer
    if(b.size_x*b.size_y > b.sp.size()){
        //error
    }else{
        int count = 0;
        for(int y = 0;y < b.size_y;y++){
            for(int x = 0;x < b.size_x;x++){
                setBackgroundColor(b.sp[count]);
                std::cout<<" ";
                count++;
            }
            std::cout<<std::endl;
        }
    }
    setBackgroundColor(BLACK);
}

//End Draw functions
//Text Draw functions

void text_loop(){
    for(int i=0;i<texts.size();i++){
        
        draw_text(texts[i]);
    }
}

struct Text* start_text(struct Text *b, int size_x, int size_y, char c){
    for(int i=0;i<size_x*size_y;i++){
        b->txt.push_back(c);
        b->txtc.push_back(WHITE);
    }
    b->rows = size_x;
    b->cols = size_y;
    
    return b;
}
void draw_text(struct Text *b){
    int pxcant = 0;
    int x = 0;
    int y = 0;
    
    
    for(int i=((DISP_TEXT->rows-1)*b->y)+b->x;i<DISP_TEXT->txt.size()-1;i++){
        if(pxcant > b->txt.size()-1){
            i = DISP_TEXT->txt.size();
            continue;
        }
        if(x >= b->rows){

            i += DISP_TEXT->rows-b->rows-1;
            x = 0;
        }
        if(b->txt[pxcant] != -1){
            DISP_TEXT->txt[i] = b->txt[pxcant];
            if(i < DISP_TEXT->txtc.size() && b->txtc.size() != 0){
                setColor(GREEN);
                DISP_TEXT->txtc[i] = b->txtc[0];
            }
        }
        pxcant += 1;
        x += 1;
    }
}

void re_text_display(){
    int count = 0;
    for(int y = 0;y < DISP_TEXT->cols-1;y++){
        for(int x = 0;x < DISP_TEXT->rows-1;x++){
            if(DISP_TEXT->txt[count] != ANT_TEXT->txt[count]){ 
                locate(x+1,y+1);
                
                setBackgroundColor(DISP_BUFFER->sp[count]);
                setColor(DISP_TEXT->txtc[count]);
                std::cout<<DISP_TEXT->txt[count];
                ANT_TEXT->txt[count] = DISP_TEXT->txt[count];
            }
            count++;
               
        }
        
    }

setBackgroundColor(BLACK);
}
//End Text Draw functions

//Time functions

int get_timer_pos(struct Timer* t){
    
    int p = -1;
    for(int i=0;i<timers.size();i++){
        if(timers[i] == t){
            p = i;
            i = timers.size();
        }
    }
    return p;
}

void time_loop(){
    for(int i=0;i<timers.size();i++){
        timers[i]->act -= 1;
        if(timers[i]->act <= timers[i]->end){
            timers[i]->call(timers[i]);
            
            timers[i]->reset(timers[i]);
            
        }
    }
    std::this_thread::sleep_for(std::chrono::nanoseconds(MIN_TIME_TIMER));
}

//End Time functions


//callback timer functions

void callback_zombie_move(struct Timer* tmove){

    int pos_crop = (tmove->zombie->x-1)+((tmove->zombie->y-1)*(SIZE_MARKS->x-1));
    if(pos_crop < 0 || pos_crop > crops.size()-1){
    }else if(crops[pos_crop]->type != PLANT_NO_PLANT){
        crops[pos_crop]->life -= tmove->zombie->dmg;
        if(crops[pos_crop]->life <= 0){
            map_bff.erase(crops[pos_crop]->bff->pos_bff);
            delete crops[pos_crop]->bff;

            if(crops[pos_crop]->is_timer){
                int tp = get_timer_pos(crops[pos_crop]->time);
                timers.erase(timers.begin()+tp);
                delete crops[pos_crop]->time;
            }
            delete crops[pos_crop];
            struct Plant* plantx = new struct Plant;
            crops[pos_crop] = plantx;
        }
    }else{
        if(!is_zombie_in_this(tmove->zombie->x-1, tmove->zombie->y)){
            
            if(tmove->zombie->x-1 <= 0){
                lost();
            }
            tmove->zombie->x -= 1;
            tmove->zombie->bff->x -= 12;
        }
    }
}

void zombie_move_timer_reset(struct Timer* tmove){
    tmove->init = TIME_TIMER_ZOMBIE_MOVE;
    tmove->act =  TIME_TIMER_ZOMBIE_MOVE;
    tmove->end = 0;

}

void callback_soles_spawn(struct Timer* t){

    

    int rangeX = (SIZE_MARKS->x-2) - 0 ;
    int numX = rand() % rangeX + 0;
    int rangeY = (SIZE_MARKS->y-1) - 1 + 1;
    int numY = rand() % rangeY + 1;

    struct Buffer* solx = new struct Buffer;
    solx->sp = sun;
    solx->z = 90;
    solx->size_x = 12;
    solx->size_y = 7;
    while(suns_in_grass[numX+(numY-1)*(SIZE_MARKS->x-1)]){
        rangeX = (SIZE_MARKS->x-2) - 0 ;
        numX = rand() % rangeX + 0;
        rangeY = (SIZE_MARKS->y-1) - 1 + 1;
        numY = rand() % rangeY + 1;
    }

    solx->x = numX*12;
    solx->y = numY*7;
    suns_in_grass[numX+((numY-1)*(SIZE_MARKS->x-1))] = true;

    delete suns_in_grass_bff[(numX)+((numY-1)*(SIZE_MARKS->x-1))];
    suns_in_grass_bff[numX+((numY-1)*(SIZE_MARKS->x-1))] = solx;
    int id = push_map_bff(solx);
    solx->pos_bff = id;
    suns_in_grass_pos[numX+((numY-1)*(SIZE_MARKS->x-1))] = id;
}

void reset_soloes_spawn(struct Timer* t){
    int range = max_range_spw_sol - min_range_spw_sol + 1;
    TIME_TIMER_SOLOES = rand() % range + min_range_spw_sol;

    SUNS_TIMER->init = TIME_TIMER_SOLOES*time_1_range_spw_sol*time_2_range_spw_sol;
    SUNS_TIMER->act =  TIME_TIMER_SOLOES*time_1_range_spw_sol*time_2_range_spw_sol;
    SUNS_TIMER->end = 0;
}

void girasol_timer(struct Timer* t){
    int range = 10 - 0 + 1;
    int random = rand() % range + 0;
    if(random == 0 && !suns_in_grass[t->plant->x+((t->plant->y-1)*(SIZE_MARKS->x-1))]){
        struct Buffer* solx = new struct Buffer;
        solx->sp = sun;
        solx->x = t->plant->x*12;
        solx->y = (t->plant->y*7)+2;
        solx->z = 90;
        solx->size_x = 12;
        solx->size_y = 7;
        int id = push_map_bff(solx);
        solx->pos_bff = id;
        suns_in_grass[t->plant->x+((t->plant->y-1)*(SIZE_MARKS->x-1))] = true;
        delete suns_in_grass_bff[t->plant->x+((t->plant->y-1)*(SIZE_MARKS->x-1))];
        suns_in_grass_bff[t->plant->x+((t->plant->y-1)*(SIZE_MARKS->x-1))] = solx;
        suns_in_grass_pos[t->plant->x+((t->plant->y-1)*(SIZE_MARKS->x-1))] = solx->pos_bff;
    }
    
}
void girasol_timer_reset(struct Timer* t){
    if(t != nullptr){
    
    int range = max_range_spw_sunflower - min_range_spw_sunflower + 1;
    int random = rand() % range + min_range_spw_sunflower;
    t->init = random;
    t->act = random;
    t->end = 0;
    }    
}

void callback_init_zombies(struct Timer* t){

    continue_timer_for_zombies = new struct Timer;
    continue_timer_for_zombies->init = TIME_TIMER_CONTINUE_ZOMBIES;
    continue_timer_for_zombies->act =  TIME_TIMER_CONTINUE_ZOMBIES;
    continue_timer_for_zombies->end = 0;
    continue_timer_for_zombies->call = callback_continue_zombies;
    continue_timer_for_zombies->reset = reset_continue_zombies;
    timers.push_back(continue_timer_for_zombies);
}
void reset_init_zombies(struct Timer* t){
    if(t != nullptr){
        delete init_timer_for_zombies;
        timers.erase(timers.begin()+itfz_pos_in_list);
    }

}
void callback_continue_zombies(struct Timer* t){
    int rangeZ = MAX_SAMETIME_ZOMBIES - 1 ;
    int randomZ = rand() % rangeZ + 1;
    std::vector<int> list_exsist;
    for(int i=0;i<randomZ;i++){
        int range = SIZE_MARKS->y-1 - 0 ;
        int random = rand() % range + 0;
        bool is_all = true;

        for(int j=0;j<SIZE_MARKS->y-1;j++){
            if(!exsist_in_list(list_exsist, j)){
                is_all = false;
                j = SIZE_MARKS->y-1;
            }
        }
        while(exsist_in_list(list_exsist, random) && !is_all){
            range = SIZE_MARKS->y-1 - 0 ;
            random = rand() % range + 0;
        }
        if(special_zombies){
            int randomType = rand() % 100 + 0;
            create_zombie(randomType, random);
        }else{  
            create_zombie(0, random);
            special_zombies = true;
        }
        list_exsist.push_back(random);
    }
}

void reset_continue_zombies(struct Timer* t){
    if(t != nullptr){
    
    if(TIME_TIMER_CONTINUE_ZOMBIES > MIN_TIME_ZOMBIE_SUBSTRACT){
        TIME_TIMER_CONTINUE_ZOMBIES-=MIN_TIME_ZOMBIE_SUBSTRACT;
    }
    continue_timer_for_zombies->init = TIME_TIMER_CONTINUE_ZOMBIES;
    continue_timer_for_zombies->act =  TIME_TIMER_CONTINUE_ZOMBIES;
    continue_timer_for_zombies->end = 0;
    }
}
void peashooter_timer_callback(struct Timer* t){
    struct Position* ps = get_cell_pos(0,t->plant->bff->y);
    bool is = false;
    for(int i=t->plant->x;i<=WHIDTH;i++){
        if(is_zombie_in_this(i,ps->y+1)){
            is = true;
            i = WHIDTH+1;
        }
    }
    if(is){
    struct Plant* pea = new struct Plant;
    struct Buffer* bff = new struct Buffer;
    struct Timer* tp = new struct Timer;
    tp->init = PEASHOOT_TIME_MOVE;
    tp->act = PEASHOOT_TIME_MOVE;
    tp->end = 0;
    tp->call = peashoot_timer_callback;
    tp->plant = pea;
    tp->is_plant = true;
    tp->reset = peashoot_timer_reset;
    timers.push_back(tp);

    std::vector<int> pb = {5};

    bff->sp = pb;
    bff->size_x = 1;
    bff->size_y = 1;
    
    bff->x = t->plant->bff->x+6;
    bff->y = t->plant->bff->y+2;
    bff->pos_bff = push_map_bff(bff);

    pea->bff = bff;
    pea->time = tp;
    pea->dmg = LGIS_DMG;
    
    audio->play2D(shootSound);
    }

}
void peashooter_timer_reset(struct Timer* t){
    if(t != nullptr){
    
    t->init = PEASHOOT_TIME;
    t->act = PEASHOOT_TIME;
    t->end = 0;
    }
}

void peashoot_timer_callback(struct Timer* t){

    bool is_del = false;
    struct Position* ps = get_cell_pos(t->plant->bff->x,t->plant->bff->y);
    if(is_zombie_in_this(ps->x,ps->y) || is_zombie_in_this(ps->x-1,ps->y)){
       
        struct Zombie* z;
        if(is_zombie_in_this(ps->x-1,ps->y)){
            z = get_zombie_by_pos(ps->x-1,ps->y);
        }else{
            z = get_zombie_by_pos(ps->x,ps->y);
        }
        if(z != nullptr){
            z->life -= t->plant->dmg;
            if(z->life <= 0){
                map_bff.erase(z->bff->pos_bff);
                delete z->bff;
                int timpos = get_timer_pos(z->t_move);
                if(timpos != -1){
                    timers.erase(timers.begin()+timpos);
                    delete z->t_move;
                }
                int zx = -1;
                for(int i=0;i<zombies.size();i++){
                    if(z == zombies[i]){
                        zx = i;
                        i = zombies.size();
                    }
                }
                zombies.erase(zombies.begin()+zx);
                delete z;
            }
        }
        map_bff.erase(t->plant->bff->pos_bff);
        delete t->plant->bff;
        int timpos = get_timer_pos(t);
            if(timpos != -1){
                timers.erase(timers.begin()+timpos);
            }
        is_del = true;
        audio->play2D(crashSound);
        delete t->plant;
        delete t;
    }else{
        t->plant->bff->x += 4;
        if(t->plant->bff->x >= WHIDTH){
            map_bff.erase(t->plant->bff->pos_bff);
            delete t->plant->bff;
            int timpos = get_timer_pos(t);
            if(timpos != -1){
                timers.erase(timers.begin()+timpos);  
                delete t->plant;
                delete t;
                is_del = true;
            }
        }
    }
    if(!is_del){
        t->init = PEASHOOT_TIME_MOVE;
        t->act = PEASHOOT_TIME_MOVE;
        t->end = 0;

    }
    delete ps;
}

void nut_timer(struct Timer* t){
    if(t->plant->life <= 80 && t->plant->life >= 76){
        t->plant->bff->sp = nut;
    }else if(t->plant->life <= 75 && t->plant->life >= 70){
        t->plant->bff->sp = nut_1;
    }else if(t->plant->life <= 70 && t->plant->life >= 60){
        t->plant->bff->sp = nut_2;
    }else if(t->plant->life <= 60 && t->plant->life >= 50){
        t->plant->bff->sp = nut_3;
    }else if(t->plant->life <= 50 && t->plant->life >= 40){
        t->plant->bff->sp = nut_4;
    }else if(t->plant->life <= 40 && t->plant->life >= 30){
        t->plant->bff->sp = nut_5;
    }else if(t->plant->life <= 30 && t->plant->life >= 20){
        t->plant->bff->sp = nut_6;
    }else if(t->plant->life <= 20 && t->plant->life >= 10){
        t->plant->bff->sp = nut_7;
    }else if(t->plant->life <= 10 && t->plant->life >= 0){
        t->plant->bff->sp = nut_8;
    }
}

void nut_timer_reset(struct Timer* t){
    if(t != nullptr){
        t->init = NUT_TIME;
        t->act = NUT_TIME;
        t->end = 0;
    
    }
}

void peashoot_timer_reset(struct Timer* t){
}

void callback_select_mark(struct Timer* t){
    if(act_asset_mark){
        SELECT_IMG->sp = MARKER_ASSET_1;
        act_asset_mark = false;
    }else{
        SELECT_IMG->sp = MARKER_ASSET_2;
        act_asset_mark = true;
    }
}

void reset_select_mark(struct Timer* t){
    SELECT_TIMER->init = TIME_TIMER_MARK;
    SELECT_TIMER->act = TIME_TIMER_MARK;
    SELECT_TIMER->end = 0;
    
}
//End callback timer functions

//Add & create functions

struct Zombie* create_zombie(int type, int y){
    int x = SIZE_MARKS->x-1;

    int z_pos = 70+y;

    if(type <= 60 && !is_zombie_in_this(x,y+1)){

        struct Zombie* z = new struct Zombie;
        
        z->x = x;
        z->y = y+1;
                
        struct Buffer* b = new struct Buffer;
        b->sp = zombie;
        b->x = 12*x;
        b->y = (7*(y+1))-3;
        b->size_x = 12;
        b->size_y = 9;
        b->z = z_pos;
        b->pos_bff = push_map_bff(b);
        
        struct Timer* tmove = new struct Timer;
        tmove->init = TIME_TIMER_ZOMBIE_MOVE;
        tmove->act =  TIME_TIMER_ZOMBIE_MOVE;
        tmove->end = 0;
        tmove->call = callback_zombie_move;
        tmove->reset = zombie_move_timer_reset;
        tmove->is_zombie = true;
        tmove->zombie = z;

        timers.push_back(tmove);

        z->bff = b;
        z->t_move = tmove;

        zombies.push_back(z);
        return z;
    }
    if(type <= 90 && !is_zombie_in_this(x,y+1)){

        struct Zombie* z = new struct Zombie;
        
        z->x = x;
        z->y = y+1;
                
        struct Buffer* b = new struct Buffer;
        b->sp = zombie_cone;
        b->x = 12*x;
        b->y = (7*(y+1))-3;
        b->size_x = 12;
        b->size_y = 9;
        b->z = z_pos;
        b->pos_bff = push_map_bff(b);
        
        struct Timer* tmove = new struct Timer;
        tmove->init = TIME_TIMER_ZOMBIE_MOVE;
        tmove->act =  TIME_TIMER_ZOMBIE_MOVE;
        tmove->end = 0;
        tmove->call = callback_zombie_move;
        tmove->reset = zombie_move_timer_reset;
        tmove->is_zombie = true;
        tmove->zombie = z;

        timers.push_back(tmove);

        z->bff = b;
        z->t_move = tmove;
        z->life = LIFE_ZOMBIES+50;

        zombies.push_back(z);
        return z;
    }
    if(type <= 100 && !is_zombie_in_this(x,y+1)){

        struct Zombie* z = new struct Zombie;
        
        z->x = x;
        z->y = y+1;
                
        struct Buffer* b = new struct Buffer;
        b->sp = zombie_bucket;
        b->x = 12*x;
        b->y = (7*(y+1))-3;
        b->size_x = 12;
        b->size_y = 9;
        b->z = z_pos;
        b->pos_bff = push_map_bff(b);
        
        struct Timer* tmove = new struct Timer;
        tmove->init = TIME_TIMER_ZOMBIE_MOVE;
        tmove->act =  TIME_TIMER_ZOMBIE_MOVE;
        tmove->end = 0;
        tmove->call = callback_zombie_move;
        tmove->reset = zombie_move_timer_reset;
        tmove->is_zombie = true;
        tmove->zombie = z;

        timers.push_back(tmove);

        z->bff = b;
        z->t_move = tmove;
        z->life = LIFE_ZOMBIES+100;

        zombies.push_back(z);
        return z;
    }
}


int push_map_bff(struct Buffer* bf){
    
    idx_map_bff++;
    map_bff.insert(std::pair<int, struct Buffer*>(idx_map_bff,bf));
    return idx_map_bff;
}

void Create_mark(){
    SIZE_MARKS = new struct Position;
    SIZE_MARKS->x = 11;
    SIZE_MARKS->y = 7;

    SELECTED = new struct Position;
    SELECTED->x = 0;
    SELECTED->y = 0;

    SELECT_TIMER = new struct Timer;
    SELECT_TIMER->init = TIME_TIMER_MARK;
    SELECT_TIMER->act =  TIME_TIMER_MARK;
    SELECT_TIMER->end = 0;
    SELECT_TIMER->call = callback_select_mark;
    SELECT_TIMER->reset = reset_select_mark;

    timers.push_back(SELECT_TIMER);
    
    SELECT_IMG = new struct Buffer;
    SELECT_IMG->x = 0;
    SELECT_IMG->y = 0;
    SELECT_IMG->size_x = 12;
    SELECT_IMG->size_y = 7;
    SELECT_IMG->z = 100;
    
    SELECT_IMG->sp = MARKER_ASSET_1;

    push_map_bff(SELECT_IMG);
}
//End Add & create functions

//Load functions

void load_assets(){
    
    peashoter = readAsset("Asets/peashoter.txt");
    sun = readAsset("Asets/sun.txt");
    sunflower = readAsset("Asets/sunflower.txt");
    nut = readAsset("Asets/nut.txt");

    nut_1 = readAsset("Asets/nut_1.txt");
    nut_2 = readAsset("Asets/nut_2.txt");
    nut_3 = readAsset("Asets/nut_3.txt");
    nut_4 = readAsset("Asets/nut_4.txt");
    nut_5 = readAsset("Asets/nut_5.txt");
    nut_6 = readAsset("Asets/nut_6.txt");
    nut_7 = readAsset("Asets/nut_7.txt");
    nut_8 = readAsset("Asets/nut_8.txt");

    zombie = readAsset("Asets/zombie.txt");
    zombie_cone = readAsset("Asets/zombie cono.txt");
    zombie_bucket = readAsset("Asets/zombie bucket.txt");

    asphalt = readAsset("Asets/asphalt.txt");
    grass = readAsset("Asets/grass.txt");

    marker = readAsset("Asets/mazeta frame.txt");
    shovel = readAsset("Asets/shovel.txt");

    MARKER_ASSET_1 = readAsset("Asets/marker aset1.txt");
    MARKER_ASSET_2 = readAsset("Asets/marker aset2.txt");
    
    shootSound = audio->addSoundSourceFromFile("Asets/poppvz.wav"); 
    shootSound->setDefaultVolume(10000.f);
    crashSound = audio->addSoundSourceFromFile("Asets/crashpeapvz.wav"); 
    crashSound->setDefaultVolume(10000.f);
    musicSound = audio->addSoundSourceFromFile("Asets/20. Grasswalk IN-GAME.mp3"); 
    musicSound->setDefaultVolume(0.2f);
}

void load_flowerpots(){

    struct Buffer* PEASHOOTER_IMG = new struct Buffer;
    //xd = start_buffer(xd, 10,10, RED);
    PEASHOOTER_IMG->sp = peashoter;
    PEASHOOTER_IMG->size_x = 5;
    PEASHOOTER_IMG->size_y = 6;
    PEASHOOTER_IMG->x = 3;
    PEASHOOTER_IMG->y = 0;
    PEASHOOTER_IMG->z = 100;
    push_map_bff(PEASHOOTER_IMG);
    struct Buffer* SUNFLOWER_IMG = new struct Buffer;
    SUNFLOWER_IMG->sp = sunflower;
    SUNFLOWER_IMG->x = 12;
    SUNFLOWER_IMG->y = 0;
    SUNFLOWER_IMG->size_x = 12;
    SUNFLOWER_IMG->size_y = 7;
    SUNFLOWER_IMG->z = 10;
    
    push_map_bff(SUNFLOWER_IMG);

    struct Buffer* NUT_IMG = new struct Buffer;
    NUT_IMG->sp = nut;
    NUT_IMG->x = 12*2;
    NUT_IMG->y = 0;
    NUT_IMG->size_x = 12;
    NUT_IMG->size_y = 7;
    NUT_IMG->z = 10;
    
    push_map_bff(NUT_IMG);

}

void load_interface(){
    
    SUN_IMG = new struct Buffer;
    SUN_IMG->sp = sun;
    SUN_IMG->x = 12*5;
    SUN_IMG->y = 0;
    SUN_IMG->size_x = 12;
    SUN_IMG->size_y = 7;
    SUN_IMG->z = 50;
    push_map_bff(SUN_IMG);
    ACT_PLANT_IMG = new struct Buffer;
    ACT_PLANT_IMG->sp = MARKER_ASSET_1;
    ACT_PLANT_IMG->x = 12*4;
    ACT_PLANT_IMG->y = 0;
    ACT_PLANT_IMG->size_x = 12;
    ACT_PLANT_IMG->size_y = 7;
    ACT_PLANT_IMG->z = 10;
    push_map_bff(ACT_PLANT_IMG);
    struct Buffer* SHOVEL_IMG = new struct Buffer;
    SHOVEL_IMG->sp = shovel;
    SHOVEL_IMG->x = 12*3;
    SHOVEL_IMG->y = 0;
    SHOVEL_IMG->size_x = 12;
    SHOVEL_IMG->size_y = 7;
    SHOVEL_IMG->z = 10;
    push_map_bff(SHOVEL_IMG);

    PEASHOOTERTEXT = new struct Text;
    PEASHOOTERTEXT->x = 4;
    PEASHOOTERTEXT->y = 6;
    PEASHOOTERTEXT->rows = 3;
    PEASHOOTERTEXT->cols = 1;
    PEASHOOTERTEXT->txt = "100";
    PEASHOOTERTEXT->txtc.push_back(BLUE);
    texts.push_back(PEASHOOTERTEXT);
    DISP_TEXT = new struct Text;
    ANT_TEXT = new struct Text;

    DISP_TEXT = start_text(DISP_TEXT, WHIDTH, HEIGHT, ' ');
    ANT_TEXT = start_text(ANT_TEXT, WHIDTH, HEIGHT, ' ');

    SUNTEXT = new struct Text;
    SUNTEXT->x = (12*5)+3;
    SUNTEXT->y = 5;
    SUNTEXT->rows = 1;
    SUNTEXT->cols = 1;
    SUNTEXT->txt = "0";
    SUNTEXT->txtc.push_back(YELLOW);

    texts.push_back(SUNTEXT);
    int range = max_range_spw_sol - min_range_spw_sol + 1;
    TIME_TIMER_SOLOES = rand() % range + min_range_spw_sol;

    SUNS_TIMER = new struct Timer;
    SUNS_TIMER->init = TIME_TIMER_SOLOES*time_1_range_spw_sol*time_2_range_spw_sol;
    SUNS_TIMER->act =  TIME_TIMER_SOLOES*time_1_range_spw_sol*time_2_range_spw_sol;
    SUNS_TIMER->end = 0;
    SUNS_TIMER->call = callback_soles_spawn;
    SUNS_TIMER->reset = reset_soloes_spawn;

    timers.push_back(SUNS_TIMER);
    
    

    SUNFLOWERTEXT = new struct Text;
    SUNFLOWERTEXT->x = 17;
    SUNFLOWERTEXT->y = 6;
    SUNFLOWERTEXT->rows = 2;
    SUNFLOWERTEXT->cols = 1;
    SUNFLOWERTEXT->txt = "50";
    SUNFLOWERTEXT->txtc.push_back(BLUE);

    texts.push_back(SUNFLOWERTEXT);

    NUTTEXT = new struct Text;
    NUTTEXT->x = (12*2)+5;
    NUTTEXT->y = 6;
    NUTTEXT->rows = 2;
    NUTTEXT->cols = 1;
    NUTTEXT->txt = "50";
    NUTTEXT->txtc.push_back(BLUE);

    texts.push_back(NUTTEXT);

}

void load_init_zombie_timer(){


    init_timer_for_zombies = new struct Timer;
    init_timer_for_zombies->init = TIME_TIMER_INIT_ZOMBIES;
    init_timer_for_zombies->act =  TIME_TIMER_INIT_ZOMBIES;
    init_timer_for_zombies->end = 0;
    init_timer_for_zombies->call = callback_init_zombies;
    init_timer_for_zombies->reset = reset_init_zombies;
    timers.push_back(init_timer_for_zombies);
    itfz_pos_in_list = timers.size()-1;
}

void start_seeds(){
    


    for(int i=0;i<3;i++){
        
        struct Buffer *m = new struct Buffer;
        m->sp = marker;
        m->size_x = 12;
        m->size_y = 7;
        m->x = i*12;
        m->y = 0;
        m->z = 0;
        int id = push_map_bff(m);
        m->pos_bff = id;
    }

     

}
void start_grass(){

    for(int i=0;i<SIZE_MARKS->y-1;i++){
        
        for(int j=0;j<SIZE_MARKS->x;j++){
            if(j == SIZE_MARKS->x-1){
                struct Buffer *a = new struct Buffer;
                a->sp = asphalt;
                a->size_x = 12;
                a->size_y = 7;
                a->x = j*12;
                a->y = (i+1)*7;
                a->z = 0;
                int id = push_map_bff(a);
                a->pos_bff = id;

            }else{
                struct Buffer* solx = new struct Buffer;
                solx->sp = sun;
                solx->x = 0;
                solx->y = 0;
                solx->size_x = 0;
                solx->size_y = 0;
                suns_in_grass.push_back(false);
                suns_in_grass_bff.push_back(solx);
                suns_in_grass_pos.push_back(-1);

                struct Plant* plantx = new struct Plant;
                crops.push_back(plantx);
                struct Buffer *g = new struct Buffer;
                g->sp = grass;
                g->size_x = 12;
                g->size_y = 7;
                g->x = j*12;
                g->y = (i+1)*7;
                g->z = 0;
                int id = push_map_bff(g);
                g->pos_bff = id;
            }
        }
    }
 

}

std::vector<int> readAsset(std::string fname){
    std::ifstream f(fname.c_str(), std::ios::in);

    std::vector<int> fin;
    std::string str = "";

    if(!f.fail()){
        bool is = false;
        std::string ch = "";
        while(!f.eof()){
            std::getline(f, ch);
            str += ch;
            str += '\n';
        }
        f.close();
    }
    std::string actstr = "";
    bool pass = false;
    for(int i=0;i<str.size();i++){
        if(pass){
            pass = false;
            continue;
        }
        if(i+1 < str.size()-1){
            if(str[i] == 's' && str[i+1] == 'x'){
                int x = std::stoi(actstr);
                actstr = "";
                pass = true;
                continue;
        
            }
            if(str[i] == 's' && str[i+1] == 'y'){
            
                int x = std::stoi(actstr);
                
                actstr = "";
                pass = true;
                continue;
        
            }
        }
        if(str[i] == ' ' || str[i] == '\n'){
            if(actstr == ""){
                continue;
            }
            int x = std::stoi(actstr);
            fin.push_back(x);
            actstr = "";
            continue;
        }
        actstr += str[i];
    }
    return fin;
}

//End Load functions
