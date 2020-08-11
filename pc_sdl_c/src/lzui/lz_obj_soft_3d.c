//
// Created by LHB on 2020/8/10.
//

#include <stdlib.h>
#include "lzui/lz_obj_soft_3d.h"
#include "lzui/renderer.h"
#include "lzui/vector.h"

#include "lzui/lz_draw.h"
#include <lzui/lz_tick.h>
#include "lzui/lz_color.h"

#define SCREEN_WIDTH 1290	//window height
#define SCREEN_HEIGHT 720	//window width
#define FIREWORKS 10		//number of fireworks
#define PARTICALS 100		//number of particles a firework explodes into
#define SCALE .015			//adjust how high the fireworks will go to suit your screen resolution
#define TRAIL 3				//trail the particles leave behind

struct partical {

    struct vector2d pos;			//position
    struct vector2d vel;			//velocity
    struct vector2d trail[TRAIL];	//array of previous positions
    float alpha;					//currrent alpha value
    float alpha_rate;				//rate at which the alpha drops per frame
};

struct firework {

    struct partical property;				//contains fireworks position,vel and accel property's
    struct partical particles[PARTICALS];	//particles firework will explode into
    uint8_t r;								//firework colour
    uint8_t g;
    uint8_t b;
};

struct pix_buff ball_pb;				//firework pixel buffer used to draw to
int width, height;						//used if fullscreen
struct firework fireworks[FIREWORKS];	//the array of fireworks !
struct vector2d gravity;				//gravity effecting the particles



void update(int i) {

    int j, k;

    //fireworks has reached its peak
    if (fireworks[i].property.vel.y >= 0 ) {

        fireworks[i].property.alpha = 0;

        //calculate particle physics
        for(j = 0; j < PARTICALS; j++) {

            if (fireworks[i].particles[j].alpha >= fireworks[i].particles[j].alpha_rate) {


                fireworks[i].particles[j].alpha -= fireworks[i].particles[j].alpha_rate;

                //particles have faded away
            } else {

                fireworks[i].particles[j].alpha = 0;
            }

            //save particle previous position
            float prev_pos_x = fireworks[i].particles[j].pos.x;
            float prev_pos_y = fireworks[i].particles[j].pos.y;

            //change position based on velocity
            add_vector(&fireworks[i].particles[j].pos, &fireworks[i].particles[j].vel);

            //change velocity based on acceleration
            add_vector(&fireworks[i].particles[j].vel, &gravity);

            //slow down particle after ever frame
            multiply_vector(&fireworks[i].particles[j].vel, 0.97);

            //keep track the fireworks particles previous positions
            for(k = 0; k < TRAIL; k++) {

                //store current cell position in a temp variable
                float temp_x = fireworks[i].particles[j].trail[k].x;
                float temp_y = fireworks[i].particles[j].trail[k].y;

                //update current cell with the last known prev position
                fireworks[i].particles[j].trail[k].x = prev_pos_x;
                fireworks[i].particles[j].trail[k].y = prev_pos_y;

                //update the prev position for the next iteration of the loop
                prev_pos_x = temp_x;
                prev_pos_y = temp_y;
            }
        }

        //particle 0 always has the shortest alpha_rate. so is alpha is 0, the all particles have a 0 alpha. so reset the firework.
        if(fireworks[i].particles[0].alpha == 0) {
            init_firework(i);
        }

        //calculate firework physics
    } else {

        //save fireworks previous position
        float prev_pos_x = fireworks[i].property.pos.x;
        float prev_pos_y = fireworks[i].property.pos.y;

        //change position based on velocity
        add_vector(&fireworks[i].property.pos, &fireworks[i].property.vel);

        //change velocity based on acceleration
        add_vector(&fireworks[i].property.vel, &gravity);

        //make all particle follow the fire work
        for(j = 0; j < PARTICALS; j++) {

            fireworks[i].particles[j].pos.x = fireworks[i].property.pos.x;
            fireworks[i].particles[j].pos.y = fireworks[i].property.pos.y;
        }

        //keep track of fireworks previous positions
        for(k = 0; k < TRAIL; k++) {

            //store current cell position in a temp variable
            float temp_x = fireworks[i].property.trail[k].x;
            float temp_y = fireworks[i].property.trail[k].y;

            //update current cell with the last known prev position
            fireworks[i].property.trail[k].x = prev_pos_x;
            fireworks[i].property.trail[k].y = prev_pos_y;

            //update the prev position for the next iteration of the loop
            prev_pos_x = temp_x;
            prev_pos_y = temp_y;
        }
    }
}

void init_firework(int i) {

    int j, k;

    float vel_scale = SCALE * height;

    //set up firework property's
    fireworks[i].property.pos.x = rand() % width;
    fireworks[i].property.pos.y = height;
    fireworks[i].property.vel.x = sin(rand());
    fireworks[i].property.vel.y = (float) (rand() % 5 + vel_scale)  * -1;
    fireworks[i].property.alpha = 255;
    fireworks[i].r = rand() % 155 + 100;
    fireworks[i].g = rand() % 155 + 100;
    fireworks[i].b = rand() % 155 + 100;

    //record of previous positions for each firework
    for (k = 0; k < TRAIL; k++) {

        fireworks[i].property.trail[k].x = fireworks[i].property.pos.x;
        fireworks[i].property.trail[k].y = fireworks[i].property.pos.y;
    }

    for (j = 0; j < PARTICALS; j++) {

        int angle = rand();
        float vel = (float) rand() / RAND_MAX * 5; //random velocity for explosion
        float flip = (float) rand() / RAND_MAX;

        //set up firework's particle
        fireworks[i].particles[j].pos.x = fireworks[i].property.pos.x;
        fireworks[i].particles[j].pos.y = fireworks[i].property.pos.y;
        fireworks[i].particles[j].vel.x = sin(angle);	//will explode is a circular fashion
        fireworks[i].particles[j].vel.y = cos(angle);	//will explode is a circular fashion
        fireworks[i].particles[j].alpha = 255;
        fireworks[i].particles[j].alpha_rate = flip > .8 ? 4 : 6;
        multiply_vector(&fireworks[i].particles[j].vel, vel); //change to a random velocity so it wont explode in a perfect circle

        //record of previous positions for each particle
        for (k = 0; k < TRAIL; k++) {

            fireworks[i].particles[j].trail[k].x = fireworks[i].property.pos.x;
            fireworks[i].particles[j].trail[k].y = fireworks[i].property.pos.y;
        }
    }

    //always give the first paticle the shortest alpha rate to determine when all other particles have faded out.
    fireworks[i].particles[0].alpha_rate = 4;
}

int init(int width, int height, int argc, char *args[]) {

    int i;
    //create and allocate custom pixel buffer
    create_pixel_buffer(&ball_pb, 100, 100);
    if (ball_pb.pixels == NULL) {
        return 1;
    }
    return 0;
}

static void lui_design (lz_obj_t * obj, lz_point_t *point);

lz_obj_t * lz_create_soft_3d(int x,int y) {
    lz_soft_3d_t * pos = lz_malloc(sizeof(lz_soft_3d_t));
    lz_obj_t * obj = lz_create_obj(x,y,320,240, pos, lui_design);

    //create firework sprite
    draw_circle(&ball_pb, ball_pb.width / 2, ball_pb.height / 2, ball_pb.width / 2, 0xffffffff);

    //define the world gravity
    gravity.x = 0;
    gravity.y = 0.2;

    int i, j, k;

    //populate fireworks property's
    for (i = 0; i < FIREWORKS; i++) {

        init_firework(i);
    }
    int sleep = 0;
    int quit = 0;
//    uint32_t next_game_tick = SDL_GetTicks();

    return obj;
}

static void lui_design (lz_obj_t * obj, lz_point_t *point) {
//    int i, j, k;
//
//    //draw the fireworks
//    for (i = 0; i < FIREWORKS; i++) {
//        //draw firework
//        dest.x = fireworks[i].property.pos.x;
//        dest.y = fireworks[i].property.pos.y;
//        dest.w = 4;
//        dest.h = 4;
//        SDL_SetTextureColorMod(ball_t, fireworks[i].r, fireworks[i].g, fireworks[i].b);
//        SDL_SetTextureAlphaMod(ball_t, (uint8_t) fireworks[i].property.alpha);
//        float alpha = fireworks[i].property.alpha;
//        //draw fireworks if they have not yet exploded
//        if (alpha) {
//
//            SDL_RenderCopy(renderer, ball_t, NULL, &dest);
//        }
//
//        //draw firework trails
//        for(k = 0; k < TRAIL; k++) {
//
//            dest.x = fireworks[i].property.trail[k].x;
//            dest.y = fireworks[i].property.trail[k].y;
//            dest.w = 3;
//            dest.h = 3;
//
//            //change size of each trail
//            float size = (float) k / TRAIL;
//            size *= 3;
//
//            if (size  >= 1) {
//
//                dest.w = 3 - size;
//                dest.h = 3 - size;
//            }
//
//            SDL_RenderCopy(renderer, ball_t, NULL, &dest);
//        }
//
//        //draw particles
//        for (j = 0; j < PARTICALS; j++) {
//
//            dest.x = fireworks[i].particles[j].pos.x;
//            dest.y = fireworks[i].particles[j].pos.y;
//            dest.w = 4;
//            dest.h = 4;
//
//            //draw particles if the firework has exploded and has no alpha
//
//            if (!alpha) {
//
//                SDL_SetTextureAlphaMod(ball_t, (uint8_t) fireworks[i].particles[j].alpha);
//                SDL_RenderCopy(renderer, ball_t, NULL, &dest);
//
//                //draw particle trails
//                for(k = 0; k < TRAIL; k++) {
//
//                    dest.x = fireworks[i].particles[j].trail[k].x;
//                    dest.y = fireworks[i].particles[j].trail[k].y;
//                    dest.w = 3;
//                    dest.h = 3;
//
//                    //change size of each trail
//                    float size = (float) k / TRAIL;
//                    size *= 3;
//
//                    if (size  >= 1) {
//
//                        dest.w = 3 - size;
//                        dest.h = 3 - size;
//                    }
//
//                    SDL_RenderCopy(renderer, ball_t, NULL, &dest);
//                }
//            }
//        }
//
//        //update physics
//        update(i);
//    }
}
