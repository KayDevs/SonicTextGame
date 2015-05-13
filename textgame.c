#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

/**SONIC '86 THE TEXT ADVENTURE**/


/*TYPES, CONSTANTS*/

float max_health = 100.0f;
char *verbs[] = {"run", "avoid", "jump", "talk", "attack", "help", "approach"};
char *modifiers[] = {"fast"};
char *intermediates[] = {"to", "at", "from", "of", "with", "by", "and", "for", "the", "up"};

typedef
struct
{
	char *verb;
	char *modifier;
	char *object;
}
player_input_t;

typedef
enum
{
	ACT_GOOD = 0,
	ACT_NEUTRAL = 1,
	ACT_BAD = 2,
	ACT_TALK = 3,
	ACT_NULL = 4,
	ACT_APPROACH = 5
}
player_action_t;

typedef 
enum
{
	BAD = 0,
	MEAN = 1,
	NEUTRAL = 2,
	NICE = 4,
	GOOD = 5
}
morality_t;


typedef
struct gamecharacter
{
	morality_t morality;
	float strength;
	const char *name;
	void (*action)(struct gamecharacter *doer, player_action_t player_action); /*this will interpret a player's actions and do the thing.*/
}
gamecharacter_t;

gamecharacter_t *gamecharacters[256] = {NULL};

/*returns a reference to the object itself*/
gamecharacter_t *find_character_by_name(const char *name)
{
	gamecharacter_t *found = NULL;
	int i;
	for(i = 0; i < sizeof(gamecharacters)/sizeof(gamecharacters[0]); ++i) {
		if(gamecharacters[i] != NULL) {
			if(strcmp(name, gamecharacters[i]->name) == 0) {
				found = gamecharacters[i];
				break; /*Returns first occurence.*/
			}
		}
	}
	return found;
}

/*returns index in the gamecharacters array*/
int find_character_index_by_name(const char *name)
{
	int found = -1;
	int i;
	for(i = 0; i < sizeof(gamecharacters)/sizeof(gamecharacters[0]); ++i) {
		if(gamecharacters[i] != NULL) {
			if(strcmp(name, gamecharacters[i]->name) == 0) {
				found = i;
				break; /*Returns first occurence.*/
			}
		}
	}
	return found;
}

int gamecharacters_contains_name(const char *name)
{
	int found = 0;
	int i;
	for(i = 0; i < sizeof(gamecharacters)/sizeof(gamecharacters[0]); ++i) {
		if(gamecharacters[i] != NULL) {
			if(strcmp(name, gamecharacters[i]->name) == 0) {
				found = 1;
				break; /*Returns first occurence.*/
			}
		}
	}
	return found;
}

void survey() 
{
	int i;
	for(i = 0; i < sizeof(gamecharacters)/sizeof(gamecharacters[0]); ++i) {
		if(gamecharacters[i] != NULL) {
			printf("There is a %s about %i feet away from you.\n", gamecharacters[i]->name, 10); /*change gamecharacters[i]->distance*/
			switch(gamecharacters[i]->morality) {
				case BAD:
					printf("They seem bad.\n");
					break;
				case GOOD:
					printf("They seem good.\n");
					break;
				case NICE:
					printf("They seem nice.\n");
					break;
				case MEAN:
					printf("They seem mean.\n");
					break;
				case NEUTRAL:
					printf("It's hard to tell their demeanor.\n");
					break;
			}
		}
	}
	printf("\n");
}
/*END TYPES, CONSTANTS*/


/*SONIC*/

struct
{
	morality_t morality;
	float health;
	float damage_resistance;
} sonic;

void subtract_morality()
{
	if(sonic.morality > 0) {
		sonic.morality--;
	}
}
void add_morality()
{
	sonic.morality++;
}


void die()
{
	printf("Sonic is dead.\n");
	printf("Sonic died with a morality of %i.\n\n", sonic.morality);
	exit(0);
}

void request_death(gamecharacter_t *offender) 
{
	printf("%s attempts to kill Sonic!!\n", offender->name);
	
	if(sonic.health < offender->strength / sonic.damage_resistance) {
		printf("%s has killed Sonic!!\n\n", offender->name);
		die();
	} else {
		char *status = "";
		
		sonic.health -= offender->strength / sonic.damage_resistance;
		
		if(sonic.health <= max_health / 10.0f) {
			status = " barely";
		}
		if(sonic.health > max_health - (max_health / 10.0f)) {
			status = ", in good health,";
		}
		if(offender->strength / sonic.damage_resistance < (max_health / 20.0f)) {
			status = ", with godlike resistance,";
		}
		if(sonic.health <= 0.0f) {
			printf("%s has killed Sonic!!\n", offender->name);
			die();
		} else {
			printf("%s has attacked Sonic, but Sonic%s survives!\n", offender->name, status);
			printf("Sonic is left with %.2f/%.2f health.\n\n", sonic.health, max_health);
		}
		
	}
}

/*END SONIC*/


/*SHIA LABEOUF*/

void ShiaLabeouf_action(gamecharacter_t *doer, player_action_t player_action) {
	if(player_action == ACT_GOOD) {
		if(sonic.morality >= NEUTRAL) {
			printf("Shia appreciates Sonic's good will.\n\n");
			add_morality();
		} else {
			printf("Shia feels uncomfortable. Shia does not trust Sonic. He accepts anyway.\n\n");
			add_morality();
		}
	}
	else if(player_action == ACT_NEUTRAL) {
		if(sonic.morality >= NEUTRAL)
			printf("Shia is malcontented with Sonic's disconcern.\n\n");
		else
			printf("Shia feels worrysome.\n\n");
	}
	else if(player_action == ACT_BAD) {
		if(sonic.morality < NEUTRAL) {
			printf("Shia doubles his fists.\n\n");
			request_death(doer);
			subtract_morality();
		}
		else if(sonic.morality == NEUTRAL) {
			printf("Shia is hurt. He wonders why Sonic would do such a thing.\n\n");
			subtract_morality();
		}
		else if(sonic.morality > NEUTRAL) {
			printf("Shia is shocked and appalled. Why would you do such a thing? You are a backstabber.\n\n");
			sonic.morality = BAD; /*you're a fuck, and there is no way this can be good.*/
		}
	}
	else if(player_action == ACT_TALK) {
		if(gamecharacters_contains_name("robot")) {
			printf("I'm worried about the robot.\n\n");
		} else {
			if(sonic.morality > NEUTRAL) {
				printf("Good day!\n\n");
			} else {
				printf("I don't have anything to talk about with you.\n\n");
			}
		}
	}
	else if(player_action == ACT_APPROACH) {
		if(sonic.morality >= NEUTRAL) {
			printf("Shia sees Sonic. Shia is intrigued.\n\n");
		}
		if(sonic.morality < NEUTRAL) {
			printf("Shia is afraid. Shia is ready to fight.\n\n");
		}
	}
}

gamecharacter_t *new_ShiaLabeouf() {
	gamecharacter_t *ShiaLabeouf = malloc(sizeof(gamecharacter_t));
	ShiaLabeouf->morality = NICE;
	ShiaLabeouf->strength = 10.0f;
	ShiaLabeouf->name = "shia"; 
	ShiaLabeouf->action = &ShiaLabeouf_action;
	return ShiaLabeouf;
}

/*END SHIA LABEOUF*/


/*EGGMAN BOT*/

void Robot_action(gamecharacter_t *doer, player_action_t player_action) {
	if(player_action == ACT_GOOD) {
		printf("The robot, lacking a legitimate sense of feeling, ignores sonic's good will.\n\n");
		request_death(doer);
	}
	else if(player_action == ACT_NEUTRAL) {
		printf("The robot eyes sonic disparagingly.\n\n");
	}
	else if(player_action == ACT_BAD) {
			int globalindex;
			printf("Sonic successfully dismantles the robot.\n\n");
			globalindex = find_character_index_by_name(doer->name); /*have to remove the object from the global array if it's dead.*/
			free(gamecharacters[globalindex]); /*same as free(doer)*/
			gamecharacters[globalindex] = NULL; /*KILL IT*/
			add_morality();
	}
	else if(player_action == ACT_TALK) {
		printf("SONIC MUST DIE.\n\n");
		request_death(doer);
	}
	else if(player_action == ACT_APPROACH) {
		printf("The robot is ready to fight.\n\n");
		request_death(doer);
	}
}


gamecharacter_t *new_Robot() {
	gamecharacter_t *Robot = malloc(sizeof(gamecharacter_t));
	Robot->morality = BAD;
	Robot->strength = 50.0f;
	Robot->name = "robot"; 
	Robot->action = &Robot_action;
	return Robot;
}

/*END EGGMAN BOT*/


/*PLAYER INPUT INTERPRETATION*/

void interpret(player_input_t input)
{
	player_action_t action;
	gamecharacter_t *receiver;
	
	action = ACT_NULL;
	if((strcmp(input.verb, verbs[0]) == 0)
	|| (strcmp(input.verb, verbs[1]) == 0)) {
		action = ACT_NEUTRAL;
	}
	if(strcmp(input.verb, verbs[3]) == 0) {
		action = ACT_TALK;
	}
	if(strcmp(input.verb, verbs[4]) == 0) {
		action = ACT_BAD;
	}
	if(strcmp(input.verb, verbs[5]) == 0) {
		action = ACT_GOOD;
	}
	if(strcmp(input.verb, verbs[6]) == 0) {
		action = ACT_APPROACH;
	}
	
	if(input.object != NULL) {
		receiver = find_character_by_name(input.object);
		if(receiver == NULL) {
			printf("That person doesn't exist.\n\n");
		} else {
			receiver->action(receiver, action);
		}
	}
}

/*END PLAYER INPUT INTERPRETATION*/




int main(int argc, char **argv) 
{
	char *input_raw;
	int i;
	player_input_t input;
	gamecharacter_t *shia, *robot;
	
	input_raw = calloc(64, sizeof(char)); /*sixty. four. bits*/
	
	sonic.health = max_health;
	sonic.damage_resistance = 1.0f; /*Modify this at game start for difficulty control.*/
	sonic.morality = NEUTRAL;
	
	shia = new_ShiaLabeouf();
	gamecharacters[0] = shia;
	
	robot = new_Robot();
	gamecharacters[1] = robot;
	
	printf("Sonic is in a room with one of Eggman's Robots, and Shia Labeouf.\n");
	printf("type \'instructions\' for a guide.\n\n");
	
	do
	{
		assert(fgets(input_raw, 64, stdin) != NULL); /*it's 64 bits anyway*/
		for(i = 0; i < 64; ++i) {
			input_raw[i] = tolower(input_raw[i]); /*make it all lowercase*/
		}
		if( input_raw[strlen(input_raw) - 1] == '\n' ) {
    	input_raw[strlen(input_raw) - 1] = 0; /*remove the trailing newline*/
		}
    
    /*convert phrases to their respective valid verbs - 
    since there is only one verb per sentence, no for loop is required*/	
    {
    	char *buffer = "";
    	char *new_string = calloc(64, sizeof(char)); /*strlen(input_raw) is padded to 64*/
    	buffer = strtok(input_raw, " ");
    	while(buffer) 
    	{
				
				/*run to/from -> approach/avoid*/
				if(strcmp(buffer, "run") == 0) 
				{
					buffer = strtok(NULL, " ");
					if(strcmp(buffer, "to") == 0) {
						/*printf("VERB translated: run to -> approach\n");*/
						strcat(new_string, "approach ");
					} else if(strcmp(buffer, "from") == 0) {
						/*printf("VERB translated: run from -> avoid\n");*/
						strcat(new_string, "avoid ");
					} else {
						strcat(new_string, "run ");
					}
				}
				
				/*fuck with/up -> approach/attack*/
				if(strcmp(buffer, "fuck") == 0) 
				{
					buffer = strtok(NULL, " ");
					if(strcmp(buffer, "with") == 0) {
						/*printf("VERB translated: fuck with -> approach\n");*/
						strcat(new_string, "approach ");
					} else if(strcmp(buffer, "up") == 0) {
						/*printf("VERB translated: fuck up -> attack\n");*/
						strcat(new_string, "attack ");
					} else {
						strcat(new_string, "fuck ");
					}
				}
				
				/*kill -> attack*/
				if(strcmp(buffer, "kill") == 0) 
				{
					buffer = strtok(NULL, " ");
					strcat(new_string, "attack ");
				}
			
				/*printf("Next string: %s\n", buffer);*/
				strcat(new_string, buffer);
				strcat(new_string, " ");
				buffer = strtok(NULL, " ");
    	}
    	
			free(input_raw);
			input_raw = new_string;
    }
    
    
		/*index through intermediates and remove them*/
		for(i = 0; i < sizeof(intermediates)/sizeof(intermediates[0]); ++i)
		{
			char *buffer = "";
			char *new_string = calloc(64, sizeof(char)); /*strlen(input_raw) - bit align to 64*/
			buffer = strtok(input_raw, " ");
			while(buffer) {
				if(strcmp(buffer, intermediates[i]) == 0) {
					/*printf("INTERMEDIATE removed: %s\n", buffer);*/
				} else {
					strcat(new_string, buffer);
					strcat(new_string, " ");
				}
				buffer = strtok(NULL, " ");
			}
			free(input_raw);
			input_raw = new_string;
		}
		
		/*printf("RAW: %s\n", input_raw);*/

		if(strstr(input_raw, "morality") != NULL) {
			printf("Sonic's current morality is %i.\n\n", sonic.morality);
		}
		if(strstr(input_raw, "health") != NULL) {
			printf("Sonic's current health is %.2f/%.2f.\n\n", sonic.health, max_health);
		}
		if(strstr(input_raw, "survey") != NULL) {
			survey();
		}
		if(strstr(input_raw, "instructions") != NULL) {
			printf("\nYou play this game by typing in Sonic's actions.\n");
			printf("Please use simple [VERB] [OBJECT] format.\n");
			printf("Keep in mind only certain actions will have results, \nand most actins don't actually do anything.\n\n");
			printf("type \'instructions\' for a guide.\n");
			printf("type \'morality\' for Sonic's current morality level \n(this influences how characters will interact with you)\n");
			printf("type \'health\' for Sonic's health.\n");
			printf("type \'survey\' to check your surroundings.\n\n");
		}
		if(strstr(input_raw, "exit") != NULL) {
			break;
		}
		
		input.verb = strtok(input_raw, " ");
		/*printf("VERB: %s\n", input.verb);*/
		/*input.modifier = strtok(input_raw, " ");*/ /*modifiers not implemented yet (they wouldn't do anything anyway)*/
		input.object = strtok(NULL, " ");
		/*printf("OBJECT: %s\n", input.object);*/
		
		interpret(input);
	}
	while(strcmp(input_raw, "exit") != 0);
	
	/* clean up */
	free(input_raw);
	
	for(i = 0; i < sizeof(gamecharacters)/sizeof(gamecharacters[0]); ++i) {
		if(gamecharacters[i] != NULL)
			free(gamecharacters[i]);
	}
	
	return 0;
}