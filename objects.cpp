#include "objects.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include <cmath>




Deathsprite::Deathsprite(double x, double y){
    this->x = x;
    this->y = y;
    this->lifetime = 10;
    deathspriteTexture.loadFromFile("images/death.png");
    deathsprite.setTexture(deathspriteTexture);
}

Deathsprite::~Deathsprite(){}

void Deathsprite::draw(sf::RenderWindow &window){
    deathsprite.setPosition(x, y);
    window.draw(deathsprite);
}

Bomb::Bomb(double x, double y){
    this->x = x;
    this->y = y;
    bombTexture.loadFromFile("images/bomb.png");
    bomb.setTexture(bombTexture);
}

Bomb::~Bomb(){}

void Bomb::draw(sf::RenderWindow &window){
    bomb.setPosition(x, y);
    window.draw(bomb);
}

bool checkBombKey(){
    if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        return true;
    return false;
}

bool collision(sf::FloatRect sprite1, sf::FloatRect sprite2)
{
    return sprite1.intersects(sprite2);
}

// _______________template code for collision detection_______________
// .global check_collision

// .section .text
// check_collision:
//     @ Input parameters (passed from C)
//     @ r0: rect1_x, r1: rect1_y, r2: rect2_x, r3: rect2_y

//     @ Additional input parameters (passed from C)
//     @ r4: rect1_width, r5: rect1_height, r6: rect2_width, r7: rect2_height

//     @ Output
//     @ r0: Result (1 if collision, 0 if no collision)

//     cmp r0, r2          @ Compare rect1_x with rect2_x
//     bgt no_collision    @ If rect1 is to the right of rect2, no collision
//     add r8, r0, r4     @ Calculate rect1_x + rect1_width
//     cmp r8, r2          @ Compare rect1_x + rect1_width with rect2_x
//     blt no_collision    @ If rect1 is to the left of rect2, no collision
//     cmp r1, r3          @ Compare rect1_y with rect2_y
//     bgt no_collision    @ If rect1 is below rect2, no collision
//     add r8, r1, r5     @ Calculate rect1_y + rect1_height
//     cmp r8, r3          @ Compare rect1_y + rect1_height with rect2_y
//     blt no_collision    @ If rect1 is above rect2, no collision

//     @ If none of the above conditions were met, there's a collision
//     mov r0, #1          @ Set the result to 1 (collision)
//     bx lr

// no_collision:
//     mov r0, #0          @ Set the result to 0 (no collision)
//     bx lr


// how to use collision detection

//  result = check_collision(rect1_x, rect1_y, rect2_x, rect2_y, rect1_width, rect1_height, rect2_width, rect2_height);

//     if (result) {
//         printf("Collision detected.\n");
//     } else {
//         printf("No collision.\n");
//     }



// _______________template code for random spawn point (pure ass-embly)_______________
// .global random_coordinates

// .section .bss
// seed:   .int 0

// .section .text
// random_coordinates:
//     @ Load the current system time as a seed
//     ldr r0, =seed
//     bl get_seed

//     @ Generate random x and y coordinates
//     bl generate_random_x
//     bl generate_random_y

//     bx lr

// get_seed:
//     @ Call the gettimeofday system call to get the current time
//     mov r7, #78         @ gettimeofday syscall number
//     mov r1, #0
//     mov r2, r0
//     ldr r3, [r0, #4]   @ r3 will hold the time in microseconds
//     svc 0

//     @ Use the time in microseconds as the seed
//     ldr r1, [r0, #4]
//     mov r0, r1
//     bx lr

// generate_random_x:
//     @ Generate a random x coordinate in the range [0, 460]
//     ldr r0, =460
//     bl generate_random
//     bx lr

// generate_random_y:
//     @ Generate a random y coordinate in the range [-300, 0]
//     ldr r0, =300
//     bl generate_random
//     neg r0, r0     @ Make the y coordinate negative
//     bx lr

// generate_random:
//     @ Generate a random number between 0 and r0 (inclusive)
//     @ r0: upper bound
//     @ Returns: random number in r0

//     @ Load the current seed
//     ldr r1, =seed

//     @ Ensure that the seed is not 0 (0 is not a good seed)
//     cmp r0, #0
//     ite eq
//     mov r0, #1
//     strne r0, [r1]

//     @ Linear congruential generator
//     ldr r0, [r1]
//     mov r1, #1664525
//     mov r2, #1013904223
//     muls r0, r0, r1
//     adds r0, r0, r2
//     str r0, [r1]

//     @ Calculate the random number in the range [0, r0]
//     umull r0, r1, r0, r0  @ r0:r1 = r0 * r0
//     mov r1, r0, asr #31    @ r1 = (r0 >> 31)
//     adds r0, r1, r0

//     bx lr




// _______________template code for random spawn point (with c library)_______________
// .global random_coordinates

// .section .text
// .extern rand

// random_coordinates:
//     @ Call the C library's rand function to generate random x and y coordinates
//     bl get_random_x
//     bl get_random_y

//     bx lr

// get_random_x:
//     @ Generate a random x coordinate in the range [0, 460]
//     ldr r0, =461
//     bl call_rand
//     bx lr

// get_random_y:
//     @ Generate a random y coordinate in the range [-300, 0]
//     ldr r0, =301
//     bl call_rand
//     neg r0, r0  @ Make the y coordinate negative
//     bx lr

// call_rand:
//     @ Call the C library's rand function
//     bl rand
//     bx lr


// how to use random spawn point

// extern void random_coordinates(int *x, int *y);

// int main() {
//     int x, y;
//     random_coordinates(&x, &y);
//     printf("Random Coordinates: x = %d, y = %d\n", x, y);
//     return 0;
// }

double degToRad(double deg){
    return deg * pI / 180;
}

double radToDeg(double rad){
    return rad * 180 / pI;
}

// _______________template code for degToRad,radToDeg_______________
// .section .data
// pI: .double 3.14159265358979323846

// .section .bss

// .section .text
// .global degToRad
// .global radToDeg

// degToRad:
//     @ Function prologue
//     @ Save registers
//     push {r4, lr}

//     @ Load function argument into a register
//     ldr d0, [sp]   @ Load the degree value from the stack into d0

//     @ Load the constant pi (pI) into d1
//     vldr d1, =pI

//     @ Calculate deg * (pi / 180)
//     vdiv.f64 d0, d0, d1  @ Divide degree by pi
//     vmov.f64 d2, #180.0  @ Load 180.0 into d2
//     vdiv.f64 d0, d0, d2  @ Divide by 180.0

//     @ Store the result in d0 to the stack
//     vstr d0, [sp]

//     @ Function epilogue
//     @ Restore registers and return
//     pop {r4, pc}

// radToDeg:
//     @ Function prologue
//     @ Save registers
//     push {r4, lr}

//     @ Load function argument into a register
//     ldr d0, [sp]   @ Load the radian value from the stack into d0

//     @ Load the constant pi (pI) into d1
//     vldr d1, =pI

//     @ Calculate rad * (180 / pi)
//     vmul.f64 d0, d0, d1  @ Multiply radian by pi
//     vmov.f64 d2, #180.0  @ Load 180.0 into d2
//     vdiv.f64 d0, d0, d2  @ Divide by 180.0

//     @ Store the result in d0 to the stack
//     vstr d0, [sp]

//     @ Function epilogue
//     @ Restore registers and return
//     pop {r4, pc}


// how to use degToRad,radToDeg

// #include <stdio.h>

// extern double degToRad(double deg);
// extern double radToDeg(double rad);

// int main() {
//     double degree = 45.0;
//     double radian = degToRad(degree);
//     printf("%f degrees is %f radians\n", degree, radian);

//     double radian2 = 1.0;
//     double degree2 = radToDeg(radian2);
//     printf("%f radians is %f degrees\n", radian2, degree2);

//     return 0;
// }


double homing(double x1, double y1, double x2, double y2){
    bool reached = false;
    double dx = x1 - x2;
    double dy = y1 - y2;

    if (dx <= 0){
        return atan(dy/dx);
    }
    else if (dx>0){
        return atan(dy/dx) + 3.14159265358979323846;
    }
}


// _______________template code for homing_______________
// .section .data
// .section .bss

// .section .text
// .global homing

// homing:
//     @ Function prologue
//     @ Save registers
//     push {r4, r5, r6, lr}

//     @ Load function arguments into registers
//     ldrd r0, r1, [sp]     @ Load x1 and y1
//     ldrd r2, r3, [sp, #8] @ Load x2 and y2

//     @ Calculate dx and dy
//     subs r4, r0, r2   @ dx = x1 - x2
//     subs r5, r1, r3   @ dy = y1 - y2

//     @ Check if dx is less than or equal to 0
//     cmp r4, #0
//     ble is_less_than_or_equal

//     @ Calculate atan(dy/dx) and add pi
//     vdiv.f64 d0, d1, d2   @ Calculate dy/dx using VFP
//     vatan.f64 d0, d0      @ Calculate atan(dy/dx)
//     vldr d1, =3.14159265358979323846  @ Load pi into d1
//     vadd.f64 d0, d0, d1   @ Add pi to atan result
//     b done

// is_less_than_or_equal:
//     @ Calculate atan(dy/dx)
//     vdiv.f64 d0, d1, d2   @ Calculate dy/dx using VFP
//     vatan.f64 d0, d0      @ Calculate atan(dy/dx)

// done:
//     @ Store the result in d0 to the stack
//     vstr d0, [sp]

//     @ Function epilogue
//     @ Restore registers and return
//     pop {r4, r5, r6, pc}


// how to use homing

// #include <stdio.h>
// extern double homing(double x1, double y1, double x2, double y2);

// int main() {
//     double x1 = 1.0;
//     double y1 = 2.0;
//     double x2 = 3.0;
//     double y2 = 4.0;

//     double result = homing(x1, y1, x2, y2);

//     printf("Result: %f\n", result);

//     return 0;
// }
