#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cstdlib>
// Create Class for Character(s)
class Character {
    public :
        int hp; // Health point
        int x; // koordinat x robot
        int y; // koordinat y robot
        int dv; // damage value
        int ar; // attack range
};

/*
Atribut punya kecoa sama robot sama aja, bedanya:
- Koordinat x,y robot diupdate, tapi kecoa nggak (kecoanya diem)
- HP initial Robot > HP initial Kecoa (ini asumsi aja)
- DV Robot > DV Kecoa (ini juga cuma asumsi)
*/

int getRandomInt() {
    // Use current time as
	// seed for random generator
	srand(time(0));
    // Misalkan range koordinat : x[0..10], y[0..10]
	int num = (rand() % 11);
	return num;
}




void generateKecoa(Character& robot, Character& kecoa){
    printf("New mutant just appeared!\n\n");
    kecoa.x = getRandomInt();
    kecoa.y = getRandomInt();
    // Validating and repeating coordinate-generating process if the generated coordinate is same as the robot's current coordinate
    while (kecoa.x == robot.x && kecoa.y == robot.y) {
        kecoa.x = getRandomInt();
        kecoa.y = getRandomInt();
    }
    // Now the coordinate of the mutant is different with the robot's
    // Generating Mutant's health point
    kecoa.hp = 10;
    kecoa.dv = 2;
    kecoa.ar = 2;
}

bool IsPossibleMove(char dir, int x, int y, Character& kecoa) { //checks whether robot is out of map or in the same position as kecoa
    if (x<0 || y<0 || x>10 || y>10) {
        return false;
    }
    else if (x==kecoa.x && y==kecoa.y) {
        return false;
    }
    else {// Robot tidak di luar map dan tidak menabrak kecoa
        return true;
    }
}

void move(Character& robot, Character& kecoa) {
    /* misalkan:
    'U' = up
    'D' = down
    'L' = left
    'R' = right
    */
    char dir; // arah
    const int baris = 11;
    const int  kolom = 11;
    char area[baris][kolom] = {}; // membuat 2d Array untuk visualisasi perjalanan robot

    printf("Pilih arah gerak robot (U/D/L/R): ");
    scanf("%c", &dir);
    printf("\n");

    if (dir=='U' || dir=='D' || dir=='L' || dir=='R') {
        int temp_x = robot.x; // temporary x value for robot (not checked)
        int temp_y = robot.y; // temporary y value for robot (not checked)
        if (dir=='L') {
            temp_x = robot.x - 1;
        }
        else if (dir=='R') {
            temp_x = robot.x + 1;
        }
        else if (dir=='D') {
            temp_y = robot.y + 1;
        }
        else { // dir=='U'
            temp_y = robot.y - 1;
        }

        if (IsPossibleMove(dir, temp_x, temp_y, kecoa)) { // Updating robot's condition
            robot.x = temp_x;
            robot.y = temp_y; // for loop untuk memperlihatkan 2d array perjalanan robot
            for (int i=0; i<baris; ++i) {
                for (int j=0; j<kolom; ++j){
                    if (i == temp_y && j == temp_x){
                        printf_s("[R]");
                    }
                    else if(i ==kecoa.y && j == kecoa.x){
                        printf_s("[K]");
                    }
                    else{
                        printf_s("[.]");
                    }
                }
                printf("\n");

            }
        }
        else {
            printf("Tidak dapat bergerak\n\n");
            move(robot, kecoa);
        }
    }
    else {
        printf("Arah tidak dikenali\n");
        move(robot, kecoa);
    }
    }


int distance(Character robot, Character kecoa) {
    return round(sqrt(pow(robot.x-kecoa.x, 2) + pow(robot.y-kecoa.y, 2)));
}

void attack(int distance, Character& robot, Character& kecoa){
    if (distance > robot.ar) {
        printf("Tidak dapat menembak, kecoa di luar range\n");
    }
    else { // Kecoa berhasil tertembak
        kecoa.hp -= robot.dv;
        printf("Attack launched successfully\n\n");
    }
}

void decide(int distance, Character& robot, Character& kecoa) {
    char aom;
    printf("Attack or Move?\nType A to attack and M to move\n");
    scanf("%char", &aom); // aom : attack or move input
    printf("\n");
    if (aom=='A') {
        attack(distance, robot, kecoa);
    }
    else if (aom=='M') {
        move(robot, kecoa);
    }
    else {
        printf("Input tidak dikenali, ulangi masukan\n");
        decide(distance, robot, kecoa);
    }
}

void lanjut(Character& robot, Character& kecoa, int killed) { // fungsi untuk melanjutkan game atau tidak
    char answer; // jawaban user

    printf("Do you want to continue(Y/N):");
    scanf("%char", &answer);
    printf("\n");
    if (answer == 'N'){ //  tidak lanjut permainan
        printf("\n\n Selamat! Kamu membunuh %i kecoak\n\n", killed); // total kecoa yang dibunuh pada saat game diakhiri
        printf("\n---Sampai jumpa di petualangan selanjutnya!---\n\n");
        exit(0);
    }
    else if (answer == 'Y'){ // lanjut permainan
        generateKecoa(robot, kecoa);
    }
    else {
        printf("Input tidak dikenali, ulangi masukan\n"); // salah memasukkan input
        lanjut(robot, kecoa, killed);
    }
}

void showcond(int killed, int dist, Character& robot, Character& kecoa) {
    printf("My coordinate : %d , %d \n", robot.x, robot.y);
    printf("HP : %d \n", robot.hp);
    printf("Killed mutants : %d \n", killed);
    printf("Enemy's coordinate : %d, %d \n", kecoa.x, kecoa.y);
    printf("Enemy HP : %d \n\n", kecoa.hp);
}

bool isKecoaDead (Character kecoa) {

    if (kecoa.hp <= 0) {
        printf("You killed a mutant!\n\n");
        return true;
    }
    else {
        return false;
    }
}

void kecoaAttack(int distance, Character& kecoa, Character& robot) {
    if (distance <= kecoa.ar) {
        robot.hp -= kecoa.dv;
        printf("You're attacked! (-2 HP)\n\n");
    }
}

int main() {

    // Create the robot
    Character robot;
    // Assigning values to robot
    robot.hp = 20;
    robot.x = 0;
    robot.y = 0;
    robot.dv = 3;
    robot.ar = 3;
    // Now create the first Kecoa
    Character kecoa;
    generateKecoa(robot, kecoa);
    // Getting inside the loop
    int killed = 0; // Initializing the amount of kecoa killed
    int dist;
    printf("selamat datang wahai pejuang. Berikut adalah atributmu: \n\n");
    while (robot.hp>0) {
        dist = distance (robot, kecoa); // Updating distance
        showcond(killed, dist, robot, kecoa);
        // Showing the condition of the robot (robot's position, hp, num of kecoa killed, distance to kecoa, kecoa's position, kecoa's hp)
        decide(dist, robot, kecoa);
        // attack or move
        // If the kecoa is still alive, it can attack robot
        if (isKecoaDead(kecoa)) {
            killed += 1; // menambah total kecoak yang dibunuh
            lanjut(robot, kecoa, killed);
            }
        else {
            kecoaAttack(dist, kecoa, robot);

        }
    }
    printf("\n\nYah, kamu mati! Lebih hati-hati lagi yaa di lain kesempatan!!!\n"); // total kecoa yang dibunuh pada saat game diakhiri
    printf("Kamu telah membunuh %i kecoa\n\n <<<-Game Over->>>", killed);
    exit(0);
}
