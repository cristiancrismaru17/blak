char infomsg1[] = \
"^9  /$$$$$$   /$$                                  ^w                      /$$\n\
^9 /$$__  $$ | $$                                  ^w                     | $$\n\
^9| $$  \\__//$$$$$$    /$$$$$$  /$$$$$$  /$$   /$$ ^w /$$$$$$$  /$$$$$$  /$$$$$$\n\
^9|  $$$$$$|_  $$_/   /$$__  $$|____  $$| $$  | $$ ^w/$$_____/ |____  $$|_  $$_/\n\
^9 \\____  $$ | $$    | $$  \\__/ /$$$$$$$| $$  | $$|^w $$        /$$$$$$$  | $$\n\
^9 /$$  \\ $$ | $$ /$$| $$      /$$__  $$| $$  | $$|^w $$       /$$__  $$  | $$ /$$\n\
^9|  $$$$$$/ |  $$$$/| $$     |  $$$$$$$|  $$$$$$$|^w  $$$$$$$|  $$$$$$$  |  $$$$/\n\
^9 \\______/   \\___/  |__/      \\_______/ \\____  $$ ^w\\_______/ \\_______/   \\___/\n\
^9                                       /$$  | $$\n\
^9                                      |  $$$$$$/\n\
^9                                       \\______/\n\n";

char infomsg2[] = \
"^1  /$$$$$$                      /$$ /$$       /$$ /$$    ^w    /$$$\n\
^1 /$$__  $$                    |__/| $$      |__/| $$    ^w   /$$ $$\n\
^1| $$  \\ $$ /$$$$$$$  /$$$$$$$  /$$| $$$$$$$  /$$| $$   ^w   |  $$$\n\
^1| $$$$$$$$| $$__  $$| $$__  $$| $$| $$__  $$| $$| $$    ^w   /$$ $$/$$\n\
^1| $$__  $$| $$  \\ $$| $$  \\ $$| $$| $$  \\ $$| $$| $$ ^w     | $$  $$_/\n\
^1| $$  | $$| $$  | $$| $$  | $$| $$| $$  | $$| $$| $$    ^w  | $$\\  $$ \n\
^1| $$  | $$| $$  | $$| $$  | $$| $$| $$  | $$| $$| $$    ^w  |  $$$$/$$\n\
^1|__/  |__/|__/  |__/|__/  |__/|__/|__/  |__/|__/|__/    ^w   \\____/\\_/\n\n";


char graffiti[] = \
"                _                        \n\
                \\`*-.                    \n\
                 )  _`-.                 \n\
                .  : `. .                \n\
                : _   \'  \\               \n\
                ; *` _.   `*-._          \n\
                `-.-\'          `-.       \n\
                  ;       `       `.     \n\
                  :.       .        \\    \n\
                  . \\  .   :   .-\'   .   \n\
                  \'  `+.;  ;  \'      :   \n\
                  :  \'  |    ;       ;-. \n\
                  ; \'   : :`-:     _.`* ;\n\
         [cat] .*\' /  .*\' ; .*`- +\'  `*\' \n\
               `*-*   `*-*  `*-*\'        \n\n";

/*typedef struct usercmd_s {
unsigned char	_pad[1];
int			angles[3];
int 		buttons;
signed char forwardmove, rightmove;
char		__pad[3];
} usercmd_t;

typedef struct {
int		cursor;
int		scroll;
int		widthInChars;
char	buffer[MAX_EDIT_LINE];
} field_t;*/

/*typedef struct {
char _pad[36];
float viewaxis[3][3];
char __pad[356];
} refdef_t;*/

/*typedef struct {
char		 _pad[10];
int			x, y, width, height;
float		fov_x, fov_y;
float		vieworg[3];
float		viewaxis[3][3];		// transformation matrix

// time in milliseconds for shader effects and other time dependent rendering issues
int			time;

int			rdflags;			// RDF_NOWORLDMODEL, etc

// 1 bits will prevent the associated area from rendering at all
byte		areamask[32];

// text messages for deform text shaders
char		text[MAX_RENDER_STRINGS][MAX_RENDER_STRING_LENGTH];
} refdef_t;*/

/*typedef struct playerState_s {
int			commandTime;	// cmd->serverTime of last executed command
int			pm_type;
int			bobCycle;		// for view bobbing and footstep generation
int			pm_flags;		// ducked, jump_held, etc
int			pm_time;

vec3_t		origin;
vec3_t		velocity;
int			weaponTime;
int			gravity;
int			speed;
int			delta_angles[3];	// add to command angles to get view direction
// changed by spawns, rotating objects, and teleporters

int			groundEntityNum;// ENTITYNUM_NONE = in air

int			legsTimer;		// don't change low priority animations until this runs out
int			legsAnim;		// mask off ANIM_TOGGLEBIT

int			torsoTimer;		// don't change low priority animations until this runs out
int			torsoAnim;		// mask off ANIM_TOGGLEBIT

int			movementDir;	// a number 0 to 7 that represents the reletive angle
// of movement to the view angle (axial and diagonals)
// when at rest, the value will remain unchanged
// used to twist the legs during strafing

vec3_t		grapplePoint;	// location of grapple to pull towards if PMF_GRAPPLE_PULL

int			eFlags;			// copied to entityState_t->eFlags

int			eventSequence;	// pmove generated events
int			events[2];
int			eventParms[2];

int			externalEvent;	// events set on player from another source
int			externalEventParm;
int			externalEventTime;

int			clientNum;		// ranges from 0 to MAX_CLIENTS-1
int			weapon;			// copied to entityState_t->weapon
int			weaponstate;

vec3_t		viewangles;		// for fixed views
int			viewheight;

// damage feedback
int			damageEvent;	// when it changes, latch the other parms
int			damageYaw;
int			damagePitch;
int			damageCount;

int			stats[16];
int			persistant[16];	// stats that aren't cleared on death
int			powerups[16];	// level.time that the powerup runs out
int			ammo[16];

int			generic1;
int			loopSound;
int			jumppad_ent;	// jumppad entity hit this frame

// not communicated over the net at all
int			ping;			// server to game info for scoreboard
int			pmove_framecount;	// FIXME: don't transmit over the network
int			jumppad_frame;
int			entityEventSequence;
} playerState_t;*/


/*else if (tjbot.smooth < 0)
{
if (cmd.rightmove > 0 || (cmd.forwardmove > 0 && tjbot.ronebeat))
{
if (*tjbot.mouse_x < right_angle)
{
*tjbot.mouse_x += (right_angle - (rand() + 50) % 40 - *tjbot.mouse_x);
}
else if (*tjbot.mouse_x > right_angle)
{
*tjbot.mouse_x -= (left_angle - right_angle) / 75;
}
}
else if (cmd.rightmove < 0 || (cmd.forwardmove < 0 && tjbot.lonebeat))
{
if (*tjbot.mouse_x > left_angle)
{
*tjbot.mouse_x += (left_angle + (rand() + 50) % 40 - *tjbot.mouse_x);
}
else if (*tjbot.mouse_x < left_angle)
{
*tjbot.mouse_x += (left_angle - right_angle) / 75;
}
}
}*/