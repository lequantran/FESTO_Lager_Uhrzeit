/* Gruppe E1, Lager
 * Andreas Gebel
 * Le Quan Tran
 */

#include <rtai_mbx.h>
#include <rtai_sched.h>

#include <sys/rtai_modbus.h>

MODULE_LICENSE("GPL");

const char lager[8][16] = {
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

static char speicher[8][16] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

//static char speicher[8][16] = {0};

static char soll_lager[8][16] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0},
	{0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0},
	{0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0},
	{0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0},
	{0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0}
};

const char numbers[10][5][3] = {
	{
		{1, 1, 1},
		{1, 0, 1},
		{1, 0, 1},
		{1, 0, 1},
		{1, 1, 1}
	},
	{
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1}
	},
	{
		{1, 1, 1},
		{0, 0, 1},
		{1, 1, 1},
		{1, 0, 0},
		{1, 1, 1}
	},
	{
		{1, 1, 1},
		{0, 0, 1},
		{1, 1, 1},
		{0, 0, 1},
		{1, 1, 1}
	},
	{
		{1, 0, 1},
		{1, 0, 1},
		{1, 1, 1},
		{0, 0, 1},
		{0, 0, 1}
	},
	{
		{1, 1, 1},
		{1, 0, 0},
		{1, 1, 1},
		{0, 0, 1},
		{1, 1, 1}
	},
	{
		{1, 1, 1},
		{1, 0, 0},
		{1, 1, 1},
		{1, 0, 1},
		{1, 1, 1}
	},
	{
		{1, 1, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1}
	},
	{
		{1, 1, 1},
		{1, 0, 1},
		{1, 1, 1},
		{1, 0, 1},
		{1, 1, 1}
	},
	{
		{1, 1, 1},
		{1, 0, 1},
		{1, 1, 1},
		{0, 0, 1},
		{1, 1, 1}
	}
};


#define SET 1
#define GRAB 2
#define X_START (1 << 4)
#define Y_START (1 << 9)
#define BEWEGUNGSFREIGABE (1 << 14)
#define GRABBER_UP (1 << 10)
#define GRABBER_DOWN (1 << 11)
#define GRABBER_OPEN (1 << 12)
#define GRABBER_CLOSE (1 << 13)
#define LIGHT (1 << 15)
#define S_X_ERLAUBT 0
#define S_X_ABGESCHLOSSEN 1
#define S_Y_ERLAUBT 2
#define S_Y_ABGESCHLOSSEN 3
#define S_GRABBER_UP 4
#define S_GRABBER_DOWN 5
#define S_GRABBER_OPEN 6
#define S_GRABBER_CLOSE 7
#define S_DOOR 9
#define S_ULTRASONIC 8
#define NO_MOVE 13
#define Y_UP (14 << 5)
#define Y_DOWN (15 << 5)
#define X_RIGHT 14
#define X_LEFT 15

static int fd_node;
static unsigned char counter = 0;
static int x_pos;
static int y_pos;

void speicher_print(void);
void speicher_write(int , int , char, int);
int lager_read(int);
int grabber_move(char, char, int, int);
void grabber_grab(char, int);
int check_Sensor(int , int);
void soll_print(void);
int grabber_move_absolut(char, char, int, int );
void set_time(int,int);
int grabber_move_idle(int);
void set_lager_time(int);
void speicher_clear(void);

static RT_TASK door_task;	//Tuer Task
static RT_TASK grabber_task; //Greifer Task
static RT_TASK clock_task;

static MBX mbx_door;
static MBX mbx_clock;
static MBX mbx_speicher;
static SEM sem_move;

static void clock(long x) {

	unsigned int hour, min;
	char msg;
	int new_time;
	unsigned long loc_time_raw;
	struct timeval curr_time_raw;
	struct tm curr_time;

	do_gettimeofday(&curr_time_raw);
	loc_time_raw = curr_time_raw.tv_sec;
	time_to_tm(loc_time_raw, 0, &curr_time);
	min = curr_time.tm_min;
	hour = curr_time.tm_hour + 1;

	hour = hour % 12;
	if(hour == 0)
		hour = 12;

	set_time(hour,min);

	rt_printk("clock: task start\n");

	while(1){

		do_gettimeofday(&curr_time_raw);
		loc_time_raw = curr_time_raw.tv_sec;
		time_to_tm(loc_time_raw, 0, &curr_time);
		min = curr_time.tm_min;
		hour = curr_time.tm_hour + 1;

		hour = hour % 12;
		if(hour == 0)
			hour = 12;

		new_time = rt_mbx_receive_if(&mbx_clock, &msg, sizeof(char));
		if(new_time == 0){
			rt_printk("Neue Uhrzeit: %d:%d\n" , hour, min);
			set_time(hour,min);
		}

		rt_sleep(1000 * nano2count(1000000));
	}
}

static void door(long x) {

	int bits;

	rt_printk("door: task start\n");

	if ((fd_node = rt_modbus_connect("MODBUS-NODE")) == -1) {
		rt_printk("door: cannot connect to modbus-node\n");
		rt_printk("door: task exited\n");
		return;
	}

	rt_printk("door: MODBUS communication opened %d\n" , fd_node);

	rt_task_resume(&clock_task);
	rt_task_resume(&grabber_task);

	while(1){

		
		 while(check_Sensor(S_DOOR, fd_node) == 0) {

			rt_printk("############suspend grabber############\n");
			bits = LIGHT;
			rt_modbus_set(fd_node, DIGITAL_OUT, 0, bits);	//Licht Bit setzen und andere Bits loeschen

			do{												//warten, bis die tuer geschlossen ist
				rt_sleep(100 * nano2count(1000000));
			}while(check_Sensor(S_DOOR, fd_node) != 1);

			bits = LIGHT | BEWEGUNGSFREIGABE;				//Bewegungsfreigabe und Licht Bits setzen
			rt_modbus_set(fd_node, DIGITAL_OUT, 0, bits);
			rt_sleep(100 * nano2count(1000000));

			grabber_move_absolut(x_pos, y_pos, fd_node, 0);	//Auf die vorherige Position zurückfahren, ohne in der Funktion auf die Tuer zu achten

			if(check_Sensor(S_DOOR, fd_node) == 1){			//Nachricht senden, dass die Bewegung wieder erlaubt ist
				rt_mbx_send_if(&mbx_door, "a", sizeof(char));
			}
			rt_printk("##########resume grabber##########\n");

		}
		 rt_sleep(50 * nano2count(1000000));
	}
}

static void grabber(long x) {
			
	int setzBits = 0;

	rt_printk("grabber: task start\n");

	//Bewegungsfreiheit und Beleuchtung setzen
	setzBits = LIGHT;
	rt_modbus_set(fd_node, DIGITAL_OUT, 0, setzBits);

	rt_sleep(1000 * nano2count(1000000));

	setzBits = LIGHT | BEWEGUNGSFREIGABE;
	rt_modbus_set(fd_node, DIGITAL_OUT, 0, setzBits);
	rt_sleep(5000 * nano2count(1000000));

	do{
		setzBits |=  GRABBER_UP;	//Grabber nach oben (10)
		rt_modbus_set(fd_node, DIGITAL_OUT, 0, setzBits);
		rt_sleep(600 * nano2count(1000000));
		rt_printk("GRABBER UP\n");
	}while(check_Sensor(S_GRABBER_UP, fd_node) != 1);

	grabber_move(0, 0, fd_node, 1);
	grabber_move(10, 10, fd_node, 1);
	grabber_grab(SET, fd_node);

	soll_print();
	
	//Pruefung, ob genug Magazine vorhanden sind oder ob zuviele Magazine vorhanden sind
	//Erlaubte Anzahl an Magazine im Lager: 42-43
	do{							
		counter = 0;
		lager_read(fd_node);
		if(counter < 42) {
			speicher_print();
			rt_printk("Fuege %d-%d Magazine hinzu.\n", 42-counter, 43-counter);
			speicher_clear();
			rt_sleep(20000 * nano2count(1000000));
		}
		else if(counter > 43) {
			speicher_print();
			rt_printk("Entferne %d-%d Magazine.\n", counter-43 , counter-42);
			speicher_clear();
			rt_sleep(20000 * nano2count(1000000));
		}

	}while(counter < 42 || counter > 43);

	speicher_print();

	while(1){
		set_lager_time(fd_node);
		rt_mbx_send_if(&mbx_clock, "a", sizeof(char));
		grabber_move_idle(fd_node);
	}

	rt_sleep(1000 * nano2count(1000000));


	rt_modbus_disconnect(fd_node);
		rt_printk("grabber: MODBUS communication successfully closed\n");
		rt_printk("grabber: task exit\n");

	return;

}

/*----------------------------------------------------------------------*/

/*-----------------Init()--------------------*/
static int __init
example_init(void) {
	rt_set_oneshot_mode();
	start_rt_timer(0);
	modbus_init();

	rt_mbx_init(&mbx_door, sizeof(char));
	rt_mbx_init(&mbx_clock, sizeof(char));
	rt_mbx_init(&mbx_speicher, sizeof(char));
	rt_typed_sem_init(&sem_move, 1, BIN_SEM);

	if (rt_task_init(&door_task, door, 0, 10240, 0, 0, NULL)) {
		printk("cannot initialize door task\n");
		goto fail;
	}


	if (rt_task_init(&grabber_task, grabber, 0, 10240, 10, 0, NULL)) {
		printk("cannot initialize control task\n");
		goto fail1;
	}

	if (rt_task_init(&clock_task, clock, 0, 102400, 5, 0, NULL)) {
		printk("cannot initialize control task\n");
		goto fail2;
	}

	rt_task_resume(&door_task);

	printk("rtai_example loaded\n");
	return (0);

fail2:
	rt_task_delete(&grabber_task);
fail1:
	rt_task_delete(&door_task);
fail:
	rt_mbx_delete(&mbx_door);
	rt_mbx_delete(&mbx_clock);
	rt_mbx_delete(&mbx_speicher);
	rt_sem_delete(&sem_move);
	stop_rt_timer();
	return (1);
}
/*----------------------------------------------------------------------*/

/* -----------------Exit() -----------------*/
static void __exit
example_exit(void)
{
	rt_task_delete(&door_task);
	rt_task_delete(&grabber_task);
	rt_task_delete(&clock_task);
	rt_mbx_delete(&mbx_door);
	rt_mbx_delete(&mbx_clock);
	rt_mbx_delete(&mbx_speicher);
	rt_sem_delete(&sem_move);

	stop_rt_timer();

	printk("rtai_example unloaded\n");
}

/*----------------------------------------------------------------------*/

/*-------------------------- Unterfunktionen ---------------------------*/

int grabber_move(char x, char y, int fd_node, int check_door) {


	int setzBits = LIGHT | BEWEGUNGSFREIGABE;
	int done;
	char msg;
	rt_printk("im grabber move\n");


	if(x < 16 && x >= 0 && y < 16 && y >= 0) {

			if(check_door == 1){
				if(check_Sensor(S_DOOR, fd_node) == 0){
					rt_printk("########STOP GRABBER MOVE#########\n");
					setzBits = LIGHT;
					rt_modbus_set(fd_node, DIGITAL_OUT, 0, setzBits);
					rt_mbx_receive(&mbx_door, &msg, sizeof(char));
					setzBits = LIGHT | BEWEGUNGSFREIGABE;
					rt_modbus_set(fd_node, DIGITAL_OUT, 0, setzBits);
					rt_sleep(100 * nano2count(1000000));
					rt_printk("########RESUME GRABBER MOVE#########\n");
				}
			}

			rt_sem_wait(&sem_move);
			rt_printk("x_Pos: %d    ,  y_Pos: %d \n" , x_pos , y_pos);
			rt_printk("Bewege Greifer\n");

			do{
				setzBits |= x | (y << 5); // Position Greifer
				rt_modbus_set(fd_node, DIGITAL_OUT, 0, setzBits);
				rt_sleep(10 * nano2count(1000000));
				done = 0;
				done = check_Sensor(S_X_ERLAUBT, fd_node);
				done += check_Sensor(S_Y_ERLAUBT, fd_node);
				rt_printk("########BEWEGUNG ERLAUBT#########\n");
				if(check_Sensor(S_DOOR, fd_node) == 0)
					break;
			}while(done != 2);

			do{
				setzBits |= X_START | Y_START; // Starte Greifer in X und Y Richtung
				rt_modbus_set(fd_node, DIGITAL_OUT, 0, setzBits);
				rt_sleep(300 * nano2count(1000000)); // minimum time to wait after starting xy
				rt_printk("########BEWEGUNG STARTEN#########\n");
				done = 0;
				done = check_Sensor(S_X_ABGESCHLOSSEN, fd_node);
				done += check_Sensor(S_Y_ABGESCHLOSSEN, fd_node);
				if(check_Sensor(S_DOOR, fd_node) == 0)
					break;
			}while (done != 2);
			rt_printk("########BEWEGUNG ABGESCHLOSSEN#########\n");
			
			//neue Position speichern, wenn die Funktion vom Grabber-Task aufgerufen wurde
			if(check_door == 1){
				if((x <= 9 && x > 0) && (y <= 8 && y > 0)){
					x_pos = x;
					y_pos = y;
				}
				if(x == 14){		//bei Schrittbewegung / Relativbewegung die x_pos nur um 1 erhoehen
					x_pos++;
					if(x_pos == 0)	//
						x_pos = 1;

				}else if(x == 15){
					x_pos--;
					if(x_pos == 0)
						x_pos = -1;
				}
				if(y == 14){
					y_pos++;
				} else if (y == 15){
					y_pos--;
				}
			}

			rt_sem_signal(&sem_move);

			if(check_door == 1){
				if(check_Sensor(S_DOOR, fd_node) == 0){
					rt_printk("########STOP GRABBER MOVE#########\n");
					setzBits = LIGHT;
					rt_modbus_set(fd_node, DIGITAL_OUT, 0, setzBits);
					rt_mbx_receive(&mbx_door, &msg, sizeof(char));
					setzBits = LIGHT | BEWEGUNGSFREIGABE;
					rt_modbus_set(fd_node, DIGITAL_OUT, 0, setzBits);
					rt_sleep(100 * nano2count(1000000));
					rt_printk("########RESUME GRABBER MOVE#########\n");
				}
			}

	}else
		rt_printk("Ungueltige Position");

	return 0;
}

int grabber_move_absolut(char x, char y, int fd_node, int door) {


		switch(x){
			case -1:
				grabber_move(1,y,fd_node, door);
				grabber_move(X_LEFT, NO_MOVE, fd_node, door);
				break;

			case -2:
				grabber_move(1,y,fd_node, door);
				grabber_move(X_LEFT, NO_MOVE, fd_node, door);
				grabber_move(X_LEFT, NO_MOVE, fd_node, door);
				break;

			case -3:
				grabber_move(1,y,fd_node, door);
				grabber_move(X_LEFT, NO_MOVE, fd_node, door);
				grabber_move(X_LEFT, NO_MOVE, fd_node, door);
				grabber_move(X_LEFT, NO_MOVE, fd_node, door);
				break;

			case 10:
				grabber_move(9,y,fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				break;

			case 11:
				grabber_move(9,y,fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				break;

			case 12:
				grabber_move(9,y,fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				break;

			case 13:
				grabber_move(9,y,fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				break;

			case 14:
				grabber_move(9,y,fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				grabber_move(X_RIGHT, NO_MOVE, fd_node, door);
				break;

			default:
				grabber_move(x , y, fd_node, door);
		}

	return 0;

}

void grabber_grab(char set_grab, int fd_node){

	int grabber_state;
	int set_grabber;
	int init_state = 0xc000;
	char msg;

	rt_modbus_get(fd_node, DIGITAL_IN, 0, (unsigned short*) &grabber_state);

	if(((grabber_state >> 4) & 0b1) == 0){	//pruefe ob grabber oben ist
		set_grabber |= init_state | (1 << 10) | (1 << 13);	//Grabber nach oben (10)
		rt_modbus_set(fd_node, DIGITAL_OUT, 0, set_grabber);
		do{
			rt_modbus_get(fd_node, DIGITAL_IN, 0, (unsigned short*) &grabber_state);
		}while(((grabber_state >> 4) & 0b1) != 1);
	}

	if(check_Sensor(S_DOOR, fd_node) == 0){
		rt_printk("########STOP GRABBER GRAB#########\n");
		set_grabber = LIGHT;
		rt_modbus_set(fd_node, DIGITAL_OUT, 0, set_grabber);
		rt_mbx_receive(&mbx_door, &msg, sizeof(char));
		rt_printk("########RESUME GRABBER GRAB#########\n");
	}
		rt_sem_wait(&sem_move);

		if(set_grab == GRAB){


			do{
				set_grabber = init_state | GRABBER_OPEN; // Grabber nach unten (11) und oeffnen (12)
				rt_modbus_set(fd_node, DIGITAL_OUT, 0, set_grabber);
				rt_sleep(50 * nano2count(1000000));
				rt_printk("GRABBER OPEN\n");
			}while(check_Sensor(S_GRABBER_OPEN, fd_node) != 1);	//pruefen ob grabber unten und offen ist*/

			do{
				set_grabber = init_state | GRABBER_DOWN; // Grabber nach unten (11) und oeffnen (12)
				rt_modbus_set(fd_node, DIGITAL_OUT, 0, set_grabber);
				rt_sleep(50 * nano2count(1000000));
				rt_printk("GRABBER DOWN\n");
			}while(check_Sensor(S_GRABBER_DOWN, fd_node) != 1);	//pruefen ob grabber unten und offen ist*/


			do{
				set_grabber = init_state | GRABBER_CLOSE;	//Grabber schliessen(13)
				rt_modbus_set(fd_node, DIGITAL_OUT, 0, set_grabber);
					rt_sleep(50 * nano2count(1000000));
					rt_printk("GRABBER CLOSE\n");
			}while(check_Sensor(S_GRABBER_CLOSE, fd_node) != 1);



			do{
				set_grabber = init_state | GRABBER_UP;	//Grabber nach oben (10)
				rt_modbus_set(fd_node, DIGITAL_OUT, 0, set_grabber);
					rt_sleep(50 * nano2count(1000000));
					rt_printk("GRABBER UP\n");
			}while(check_Sensor(S_GRABBER_UP, fd_node) != 1);


		}
		else if (set_grab == SET){

			do{
					set_grabber = init_state | GRABBER_DOWN; // Grabber nach unten (11)
					rt_modbus_set(fd_node, DIGITAL_OUT, 0, set_grabber);
					rt_sleep(50 * nano2count(1000000));
					rt_printk("GRABBER DOWN\n");
			}while(check_Sensor(S_GRABBER_DOWN, fd_node)!= 1);

			do{
					set_grabber = init_state | GRABBER_OPEN;	//Grabber oeffnen
					rt_modbus_set(fd_node, DIGITAL_OUT, 0, set_grabber);
					rt_sleep(50 * nano2count(1000000));
					rt_printk("GRABBER OPEN\n");
			}while(check_Sensor(S_GRABBER_OPEN, fd_node)!= 1);


			do{
					set_grabber = init_state | GRABBER_UP;	//Grabber nach oben (10)
					rt_modbus_set(fd_node, DIGITAL_OUT, 0, set_grabber);
					rt_sleep(50 * nano2count(1000000));
					rt_printk("GRABBER UP\n");
			}while(check_Sensor(S_GRABBER_UP, fd_node)!= 1);
		}

		rt_sem_signal(&sem_move);

		if(check_Sensor(S_DOOR, fd_node) == 0){
			rt_printk("########STOP GRABBER GRAB#########\n");
			set_grabber = LIGHT;
			rt_modbus_set(fd_node, DIGITAL_OUT, 0, set_grabber);
			rt_mbx_receive(&mbx_door, &msg, sizeof(char));
			rt_printk("########RESUME GRABBER GRAB#########\n");
		}
}

void soll_print(void) {
    int i, j;
   rt_printk("Lager (soll): \n");
    for (i = 0; i < 8; i++) {
    	rt_printk("%d  ", 8-i);
        for (j = 0; j < 16; j++) {
            if (!((i == 0 && j == 0) | (i == 1 && j == 0) | (i == 0 && j == 1) | (i == 1 && j == 1) | (i == 2 && j == 0) | (i == 2 && j == 1) | (i == 0 && j == 11) | (i == 0 && j == 12) | (i == 0 && j == 13) | (i == 0 && j == 14) | (i == 0 && j == 15) | (i == 1 && j == 11) | (i == 1 && j == 12) | (i == 1 && j == 13) | (i == 1 && j == 14) | (i == 1 && j == 15) | (i == 2 && j == 14) | (i == 2 && j == 15))) {
                if (soll_lager[i][j] == 1)
                	rt_printk("(O)");
                else
                	rt_printk("( )");
            }
            else
            	rt_printk("   ");
        }
        rt_printk("\n");
    }
    rt_printk("          1  2  3  4  5  6  7  8  9\n\n");
    rt_printk("Anzahl Magazine: %d\n", counter);
}

void speicher_print(void) {
    int i, j;
   rt_printk("Speicher: \n");
    for (i = 0; i < 8; i++) {
    	rt_printk("%d  ", 8-i);
        for (j = 0; j < 16; j++) {
            if (!((i == 0 && j == 0) | (i == 1 && j == 0) | (i == 0 && j == 1) | (i == 1 && j == 1) | (i == 2 && j == 0) | (i == 2 && j == 1) | (i == 0 && j == 11) | (i == 0 && j == 12) | (i == 0 && j == 13) | (i == 0 && j == 14) | (i == 0 && j == 15) | (i == 1 && j == 11) | (i == 1 && j == 12) | (i == 1 && j == 13) | (i == 1 && j == 14) | (i == 1 && j == 15) | (i == 2 && j == 14) | (i == 2 && j == 15))) {
                if (speicher[i][j] == 1)
                	rt_printk("(O)");
                else
                	rt_printk("( )");
            }
            else
            	rt_printk("   ");
        }
        rt_printk("\n");
    }
    rt_printk("          1  2  3  4  5  6  7  8  9\n\n");
    rt_printk("Anzahl Magazine: %d\n", counter);
}

void speicher_write(int x, int y, char set_magazin, int fd_node) {

	char msg;

		if(check_Sensor(S_DOOR, fd_node) == 0){
			rt_printk("########STOP GRABBER MOVE#########\n");
			rt_mbx_receive(&mbx_door, &msg, sizeof(char));
			rt_sleep(100 * nano2count(1000000));
			rt_printk("########RESUME GRABBER MOVE#########\n");
		}


	if(x <= -1){
		speicher[8-y][x+2] = set_magazin;
		rt_printk("schreibe auf %d:%d\n", x, y);
		counter++;
	}
	else if(x > 0){
		speicher[8-y][x+1] = set_magazin;
		rt_printk("schreibe auf %d:%d\n", x, y);
		counter++;
	}

	speicher_print();
}

void speicher_clear(void){

	int i,j;

	for (i = 0; i < 16; i++){
		for(j = 0; j < 8; j++){
			speicher[j][i] = 0;
		}
	}
}

int check_Sensor(int sensor, int fd_node){

	int sensor_state = 0;

	rt_modbus_get(fd_node, DIGITAL_IN, 0, (unsigned short*) &sensor_state);

	switch(sensor){

		case S_X_ERLAUBT:
			sensor_state = (sensor_state >> S_X_ERLAUBT)  & 0b1;
			break;

		case S_Y_ERLAUBT:
			sensor_state = (sensor_state >> S_Y_ERLAUBT)  & 0b1;
			break;

		case S_X_ABGESCHLOSSEN:
			sensor_state = (sensor_state >> S_X_ABGESCHLOSSEN)  & 0b1;
			break;

		case S_Y_ABGESCHLOSSEN:
			sensor_state = (sensor_state >> S_Y_ABGESCHLOSSEN)  & 0b1;
			break;

		case S_GRABBER_UP:
			sensor_state = (sensor_state >> S_GRABBER_UP)  & 0b1;
			break;

		case S_GRABBER_DOWN:
			sensor_state = (sensor_state >> S_GRABBER_DOWN)  & 0b1;
			break;

		case S_GRABBER_OPEN:
			sensor_state = (sensor_state >> S_GRABBER_OPEN)  & 0b1;
			break;

		case S_GRABBER_CLOSE:
			sensor_state = (sensor_state >> S_GRABBER_CLOSE)  & 0b1;
			break;

		case S_DOOR:
			sensor_state = (sensor_state >> S_DOOR)  & 0b1;
			break;

		case S_ULTRASONIC:
			sensor_state = (sensor_state >> S_ULTRASONIC)  & 0b1;
			break;

		default: rt_printk("Ungueltiger Sensor: %d \n", sensor);
				sensor_state = 0;
	}
	return sensor_state;

}

//x = {-2,-1,1-14) und y = {1-8)
int grabber_move_idle(int fd_node) {

	static int x = 1;

	grabber_move_absolut(x, 8, fd_node, 1);
	rt_sleep(500 * nano2count(1000000));

	x++;
	if(x == 10)
		x = 1;

	return 0;
}

int lager_read(int fd_node) {
	int x;
	int y;
    int i;
    int j;

    //grabber_move(0, 0, fd_node, 1);
    x = -3;
    y = 1;
    //Bewege auf Initial-Position
    grabber_move_absolut(x, y, fd_node, 1);

    //Suche ab hier das ganze Feld ab
    for (i = 0; i < 16; i++) {
    	if (i % 2 == 0) {	//gerade Spalten
    		rt_printk("--------------- pruefe spalte %d ----------------\n", i);
    		for (j = 7; j > -1; j--) {
					if (check_Sensor(S_ULTRASONIC, fd_node) == 1) {
						if(x == -1)
							speicher_write(x+2, y, 1, fd_node);
						else
							speicher_write(x+1, y, 1, fd_node);
					}
    				if (j > 0) {
    					grabber_move(13, 14, fd_node, 1);
    					y++;
    				}

					if (j == 0) {
						grabber_move(14, 13, fd_node, 1); //Graber nach rechts
						x++;
						if(x == 0){
							x = 1;
						}
					}
    		}
    	}
    	else if (i % 2 == 1) {	//ungerade Spalten
    		rt_printk("--------------- pruefe spalte %d ----------------\n", i);
    		for (j = 0; j < 8; j++) {
					if (check_Sensor(S_ULTRASONIC, fd_node) == 1) {
						if(x == -1)
							speicher_write(x+2, y, 1, fd_node);
						else
							speicher_write(x+1, y, 1, fd_node);
					}
    				if (j < 7) {
    					grabber_move(13, 15, fd_node, 1);	//Graber nach unten
    					y--;
    				}
					if (j == 7) {
						grabber_move(14, 13, fd_node, 1); //Graber nach rechts
						x++;
						if(x_pos == 0){
							y = 1;
							}

						}
    			}
    	}
    }

    speicher_write(13, 8 , 0, fd_node);
    counter -= 2;
    return 0;
}

void set_time(int hour, int min){

	int x,y;
	int hour1, hour2;
	int min1, min2;

	hour1 = hour / 10;
	hour2 = hour % 10;

	min1 = min / 10;
	min2 = min % 10;

	switch(hour1){

		case 0:
			for(y = 3; y < 8; y++){
					soll_lager[y][0] = 0;
				}
			break;

		case 1:
			for(x = 0; x < 1; x++){
				for(y = 3; y < 8; y++){
					soll_lager[y][0] = numbers[1][y-3][2];
				}
			}
	}

	for(x = 2; x < 5 ; x++){
		for(y = 3; y < 8; y++){
			soll_lager[y][x] = numbers[hour2][y-3][x-2];
		}
	}

	for(x = 6; x < 9 ; x++){
		for(y = 3; y < 8; y++){
			soll_lager[y][x] = numbers[min1][y-3][x-6];
		}
	}

	for(x = 10; x < 13 ; x++){
		for(y = 3; y < 8; y++){
			soll_lager[y][x] = numbers[min2][y-3][x-10];
		}
	}



}

void set_lager_time(int fd_node) {

	int i, j, k, l, m, n;
	int magazin_grabbed, magazin_found;

	for (i = 0; i < 14; i++) {
		for (j = 2; j < 8; j++) {
			if (soll_lager[j][i] != speicher[j][i]) {
				if (soll_lager[j][i] == 1) {			//wenn magazin fehlt
					magazin_found = 0;
					for (k = 0; k < 14; k++) {				// wo ist das nächste Freie Magazin im Uhrzeit Feld
						for (l = 2; l < 8; l++) {
							if (soll_lager[l][k] != speicher[l][k]) {
								if (soll_lager[l][k] == 0) {
									if (k < 2) {
										grabber_move_absolut(k-2, 8-l, fd_node, 1);
										speicher_write(k-2, 8-l, 0, fd_node);
										rt_printk("x: %d   und y: %d  Magazin loeschen\n", k-2, 8-l);
									}
									else {
										grabber_move_absolut(k-1, 8-l, fd_node, 1);
										speicher_write(k-1, 8-l, 0, fd_node);
										rt_printk("x: %d   und y: %d  Magazin loeschen\n", k-1, 8-l);
									}
									grabber_grab(GRAB, fd_node);
									magazin_found = 1;
									speicher_print();
									soll_print();
									k = 13;
									l = 7;
								}
							}
						}
					}
					if(magazin_found == 0){
						for (k = 15; k > 13; k--) {				// wo ist in Lager 2
							for (l = 7; l > 2; l--) {
								if (soll_lager[l][k] != speicher[l][k]) {
									if (soll_lager[l][k] == 0) {
										if (k < 2) {
											grabber_move_absolut(k-2, 8-l, fd_node, 1);
											speicher_write(k-2, 8-l, 0, fd_node);
											rt_printk("x: %d   und y: %d  Magazin loeschen\n", k-2, 8-l);
										}
										else {
											grabber_move_absolut(k-1, 8-l, fd_node, 1);
											speicher_write(k-1, 8-l, 0, fd_node);
											rt_printk("x: %d   und y: %d  Magazin loeschen\n", k-1, 8-l);
										}
										grabber_grab(GRAB, fd_node);
										magazin_found = 1;
										speicher_print();
										soll_print();
										k = 14;
										l = 3;
									}
								}
							}
						}
					}
					if(magazin_found == 0){
						for (k = 10; k > 1; k--) {				// wo ist in Lager 1
							for (l = 1; l > -1; l--) {
								if (soll_lager[l][k] != speicher[l][k]) {
									if (soll_lager[l][k] == 0) {
										if (k < 2) {
											grabber_move_absolut(k-2, 8-l, fd_node, 1);
											speicher_write(k-2, 8-l, 0, fd_node);
											rt_printk("x: %d   und y: %d  Magazin loeschen\n", k-2, 8-l);
										}
										else {
											grabber_move_absolut(k-1, 8-l, fd_node, 1);
											speicher_write(k-1, 8-l, 0, fd_node);
											rt_printk("x: %d   und y: %d  Magazin loeschen\n", k-1, 8-l);
										}
										grabber_grab(GRAB, fd_node);
										magazin_found = 1;
										speicher_print();
										soll_print();
										k = 2;
										l = 0;
									}
								}
							}
						}
					}

					if (i < 2) {
						grabber_move_absolut(i-2, 8-j, fd_node, 1);
						speicher_write(i-2, 8-j, 1, fd_node);
						rt_printk("x: %d   und y: %d  Magazin setzen\n", i-2, 8-j);
					}
					else {
						grabber_move_absolut(i-1, 8-j, fd_node, 1);
						speicher_write(i-1, 8-j, 1, fd_node);
						rt_printk("x: %d   und y: %d  Magazin setzen\n", i-1, 8-j);
					}
					grabber_grab(SET, fd_node);
					speicher_print();
					soll_print();
				}
				else if(soll_lager[j][i] == 0) {				// wenn magazin zu viel
					if (i < 2) {
						grabber_move_absolut(i-2, 8-j, fd_node, 1);
						speicher_write(i-2, 8-j, 0, fd_node);
						rt_printk("x: %d   und y: %d  Magazin loeschen\n", i-2, 8-j);
					}
					else {
						grabber_move_absolut(i-1, 8-j, fd_node, 1);
						speicher_write(i-1, 8-j, 0, fd_node);
						rt_printk("x: %d   und y: %d  Magazin loeschen\n", i-1, 8-j);
					}

					grabber_grab(GRAB, fd_node);
					magazin_grabbed = 1;
					// bewege nun zu einer freien stelle Lager 1: x: 1-9, y: 7-8
					for (k = 2; k < 11; k++) {
						for (l = 0; l < 2; l++) {
							if(speicher[l][k] == 0) {
								grabber_move_absolut(k-1, 8-l, fd_node, 1);
								speicher_write(k-1, 8-l, 1, fd_node);
								rt_printk("x: %d   und y: %d  Magazin loeschen\n", k-1, 8-l);
								grabber_grab(SET, fd_node);
								k = 10;
								l = 1;
								magazin_grabbed = 0;
							}
						}
					}
					// bewege nun zu einer freien stelle Lager 2, wenn Lager 1 voll ist: x: 13-14, y: 1-5
					if (magazin_grabbed == 1){
						for (m = 14; m < 16; m++){
							for (n = 3; n < 8; n++){
								if(speicher[n][m] == 0) {
									grabber_move_absolut(m-1, 8-n, fd_node, 1);
									speicher_write(m-1, 8-n, 1, fd_node);
									rt_printk("x: %d   und y: %d  Magazin loeschen\n", m-1, 8-n);
									grabber_grab(SET, fd_node);
									m = 15;
									n = 7;
									magazin_grabbed = 0;
								}
							}
						}
					}
					//falls Lager1/2 voll, auf x = 10-12, y = 1-5 lagern
					if(magazin_grabbed == 1){
						for(m = 11; m < 14 ; m++){
							for (n = 3; n < 8; n++){
								if(speicher[n][m] == 0) {
									grabber_move_absolut(m-1, 8-n, fd_node, 1);
									speicher_write(m-1, 8-n, 1, fd_node);
									rt_printk("x: %d   und y: %d  Magazin loeschen\n", m-1, 8-n);
									grabber_grab(SET, fd_node);
									m = 13;
									n = 7;
									magazin_grabbed = 0;
								}
							}
						}
					}
				}
			}
		}
	}
}

/*----------------------------------------------------------------------*/
module_exit(example_exit)
module_init(example_init)
