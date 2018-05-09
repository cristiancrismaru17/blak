/*
 * TJBot
 * Copyright (c) 2016 Annihil
 * github.com/Annihil/TJBot
 */

#ifndef __TJBOT_H__
#define __TJBOT_H__

#include <windows.h>
#define _USE_MATH_DEFINES
#pragma warning(disable:4996)

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include "cgame\cg_local.h"	
#include "game\g_local.h"
#include "sc_info.h"

#define MIN_SPEED 320
#define varName(var)  #var

int syscall(int cmd, ...);

vec_t VectorLength(const vec3_t v) {
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

float VectorDistance(vec3_t v1, vec3_t v2) {
	vec3_t dir;

	VectorSubtract(v2, v1, dir);
	return VectorLength(dir);
}

 //void trap_UI_Popup( const char *arg0) {
void trap_UI_Popup(int arg0) {
	syscall(162, arg0);
}

static float AngleNormalize360(float angle) {
	return (float)((360.0 / 65536) * ((int)(angle * (65536 / 360.0)) & 65535));
}

static float AngleNormalize180(float angle) {
	angle = AngleNormalize360(angle);
	if (angle > 180.0)
		angle -= 360.0;
	return angle;
}

static float AngleDelta(float angle1, float angle2) {
	return AngleNormalize180(angle1 - angle2);
}

static inline float rad2deg(double a) {
	return (float)((a * 180.0f) / M_PI);
}

void SetRotationZ(float m[3][3], float angle)
{
	angle = angle * 3.1415926535f / 180.0f;
	float s = sinf(angle);
	float c = cosf(angle);
	m[0][0] = c; m[0][1] = s; m[0][2] = 0.0f;
	m[1][0] = -s; m[1][1] = c; m[1][2] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
}

vec3_t	vec3_origin = { 0,0,0 };
vec3_t	axisDefault[3] = { { 1, 0, 0 },{ 0, 1, 0 },{ 0, 0, 1 } };

void AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up) {
	float		angle;
	static float		sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

	angle = (float)(angles[1] * (M_PI * 2 / 360));
	sy = (float)sin(angle);
	cy = (float)cos(angle);
	angle = (float)(angles[0] * (M_PI * 2 / 360));
	sp = (float)sin(angle);
	cp = (float)cos(angle);
	angle = (float)(angles[2] * (M_PI * 2 / 360));
	sr = (float)sin(angle);
	cr = (float)cos(angle);

	if (forward)
	{
		forward[0] = cp*cy;
		forward[1] = cp*sy;
		forward[2] = -sp;
	}
	if (right)
	{
		right[0] = (-1 * sr*sp*cy + -1 * cr*-sy);
		right[1] = (-1 * sr*sp*sy + -1 * cr*cy);
		right[2] = -1 * sr*cp;
	}
	if (up)
	{
		up[0] = (cr*sp*cy + -sr*-sy);
		up[1] = (cr*sp*sy + -sr*cy);
		up[2] = cr*cp;
	}
}

void AnglesToAxis(const vec3_t angles, vec3_t axis[3]) {
	vec3_t	right;

	// angle vectors returns "right" instead of "y axis"
	AngleVectors(angles, axis[0], right, axis[2]);
	VectorSubtract(vec3_origin, right, axis[1]);
}

static void MatrixMultiply(float in1[3][3], float in2[3][3], float out[3][3]) {
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] + in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] + in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] + in1[0][2] * in2[2][2];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] + in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] + in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] + in1[1][2] * in2[2][2];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] + in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] + in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] + in1[2][2] * in2[2][2];
}

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];

#define AUTO_ON_COMMAND "+sc_autojump"
#define AUTO_OFF_COMMAND "-sc_autojump"
#define BOT_ON_COMMAND "+sc_jumpbot"
#define BOT_OFF_COMMAND "-sc_jumpbot"
#define LONEBEAT_ON_COMMAND "+sc_lonebeat"
#define LONEBEAT_OFF_COMMAND "-sc_lonebeat"
#define RONEBEAT_ON_COMMAND "+sc_ronebeat"
#define RONEBEAT_OFF_COMMAND "-sc_ronebeat"
#define VIEW_HACK_ON_COMMAND "+sc_viewhack"
#define VIEW_HACK_OFF_COMMAND "-sc_viewhack"
#define ANGLE_COMMAND "sc_angle"
#define SAFE_START_COMMAND "sc_safestart"
#define SMOOTH_COMMAND "sc_smoothness"
#define AUTOSMOOTH_COMMAND "sc_smoothness_auto"
#define STYLES_COMMAND "sc_styles"
#define CJ_COMMAND "sc_cj_info"
#define JUMP_DELAY_COMMAND "sc_jump_delay"
#define SMOOTHNESS_STYLE "sc_smoothness_style"
#define SMOOTHNESS_AGGRESSIVE "sc_smoothness_agressiveness"
#define SAY_BUFFER "cg_messageText"

#define ASM_JMP 0xe9
#define ASM_RETN 0xc3

//hooked struct calls//
level_locals_t level;
typedef struct gentity_s gentity_t;
struct gclient_s   client;
gentity_t ent, g_entities[MAX_GENTITIES];
usercmd_t cmd;
playerState_t ps;
//////////////////////

// offsets
// client = 2.60b

//	etjump myserv + 999 0x01df0920   
//                 twc  0x1E33BD4
#define CG_OFFSET_999 0x01E39540
#define CG_OFFSET_TWC 0x01E33B20

//#define ETJUMP_OFFSET 0x01DF0978


#define CGAZ_ARG 0x003DDAE0
#define CGAZ_999 0x003DDADC
#define CGAZ_TWC 0x0388C76C
#define ARG1_UINFO_999 0x003BB138
#define ARG1_UINFO_TWC 0x001B0E110
#define MOUSE_X 0x013EEA8C
#define MOUSE_YAW 0x00FEEA8C
#define KEY_FORWARD (0x00835A08 + 4 * 4)
#define KEY_BACK (0x00835A20 + 4 * 4)
#define KEY_MOVELEFT (0x00835A50 + 4 * 4)
#define KEY_MOVERIGHT (0x00835A68 + 4 * 4)
#define KEY_UP (0x00835AC8 + 4 * 4)
#define CONSOLE 0x128FB4C //client console chat line
#define CHCOUNT 0x128FB40 //client console nr of chars
#define SERVERCHAT 0x2A4542 //last console server non-voicechat line
#define CHAT999 0x5C97DC
#define VOICECHAT 0xE8B8BB // last voicechat command line (starts with [hh:mm:ss])
#define ENTER_KEY 0x128FD30
#define PL_MOVEMENT_999 0x1DF0A70 //1x if jumping, 2x if ducking x = left,right...         1E15054      1E39638   HEX
#define PL_MOVEMENT_TWC 0x1DF0A70
#define PAD_HEIGHT 0x1CA6F0 //height of the pad you are on
#define CLIENT_NUM 0x616064 // et.exe +

#define FORWARD_KEY 0x435A18
#define BACK_KEY 0x435A30
#define MOVERIGHT_KEY 0x435A90
#define MOVELEFT_KEY 0x435A78
#define MOVEUP_KEY 0x435AD8

// cgame = etjump and other mods generaly use those values
//#define PREDICTED_PS_OFFSET 0x48C20
#define ORIGIN_X 5 * 4
#define ORIGIN_Y 6 * 4
#define ORIGIN_Z 7 * 4
#define VELOCITY_X 8 * 4
#define VELOCITY_Y 9 * 4
#define VELOCITY_Z 10 * 4
#define VIEW_X 0xFE8D94
#define VIEW_Y 0xFE8D90
#define GROUND_ENTITY_NUM 20 * 4
#define MAX_STRING_CHARS 1024
#define	MAX_RENDER_STRINGS			8
#define	MAX_RENDER_STRING_LENGTH	32


struct tjbot_s {
    void *cgame_handle;
	unsigned int et_address;
    char cgame_filename[1024];
	char styles[16];
    void (*orig_cg_dllEntry)(int (*)(int, ...));
    int (*orig_cg_vmMain)(int, int, int, int, int, int, int, int, int, int, int, int, int);
    int (*orig_syscall)(int, ...);
    float *mouse_x;
    float *orig_x;
    float *orig_y;
    float *orig_z;
    float *vel_x;
    float *vel_y;
    float *vel_z;
    float *view_x;
    float *view_y;
	float aggressiveness;
	float groundaccel, airaccel;
	float max_gndaccel, max_airaccel;
	float smooth;
	float origin_x;
	float origin_y;
	float max_speed;
	float cj_info;
	int movement;
	int *forward;
	int *back;
	int *moveleft;
	int *moveright;
	int *moveup;
	int *ground_ent_num;
	int jumpdelay;
	int autosmooth;
	int safestart;
	int smoothstyle;
	int fb;
	int inv;
	int bw;
	int offsetno;
	int ronebeat;
	int lonebeat;
    int jumpbot_enable;
    int autojump_enable;
    int origin_enable;
    int view_hack_enable;
	int delayedframes;
	float offset;
	FILE *dump, *settings_file;
};
static struct tjbot_s tjbot;

typedef struct {
	char    *cmd;
	char	*res;
} vsay_cmd;

#define MAX_VA_STRING       32000

char* CG_TranslateString(const char *string) {
	static char staticbuf[2][MAX_VA_STRING];
	static int bufcount = 0;
	char *buf;

	// some code expects this to return a copy always, even
	// if none is needed for translation, so always supply
	// another buffer

	buf = staticbuf[bufcount++ % 2];

	tjbot.orig_syscall(CG_TRANSLATE_STRING, string, buf);

	return buf;
}

#define SETUP_MOUNTEDGUN_STATUS( ps )							\
	switch ( ps->persistant[PERS_HWEAPON_USE] ) {				 \
	case 1:													\
		ps->eFlags |= EF_MG42_ACTIVE;						\
		ps->eFlags &= ~EF_AAGUN_ACTIVE;						\
		ps->powerups[PW_OPS_DISGUISED] = 0;					\
		break;												\
	case 2:													\
		ps->eFlags |= EF_AAGUN_ACTIVE;						\
		ps->eFlags &= ~EF_MG42_ACTIVE;						\
		ps->powerups[PW_OPS_DISGUISED] = 0;					\
		break;												\
	default:												\
		ps->eFlags &= ~EF_MG42_ACTIVE;						\
		ps->eFlags &= ~EF_AAGUN_ACTIVE;						\
		break;												\
	}

void BG_PlayerStateToEntityState(playerState_t *ps, entityState_t *s, qboolean snap) {
	int i;

	if (ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPECTATOR) { // || ps->pm_flags & PMF_LIMBO ) { // JPW NERVE limbo
		s->eType = ET_INVISIBLE;
	}
	else if (ps->stats[STAT_HEALTH] <= GIB_HEALTH) {
		s->eType = ET_INVISIBLE;
	}
	else {
		s->eType = ET_PLAYER;
	}

	s->number = ps->clientNum;

	s->pos.trType = TR_INTERPOLATE;
	VectorCopy(ps->origin, s->pos.trBase);
	if (snap) {
		SnapVector(s->pos.trBase);
	}

	s->apos.trType = TR_INTERPOLATE;
	VectorCopy(ps->viewangles, s->apos.trBase);
	if (snap) {
		SnapVector(s->apos.trBase);
	}

	if (ps->movementDir > 128) {
		s->angles2[YAW] = (float)ps->movementDir - 256;
	}
	else {
		s->angles2[YAW] = ps->movementDir;
	}

	s->legsAnim = ps->legsAnim;
	s->torsoAnim = ps->torsoAnim;
	s->clientNum = ps->clientNum;    // ET_PLAYER looks here instead of at number
									 // so corpses can also reference the proper config
									 // Ridah, let clients know if this person is using a mounted weapon
									 // so they don't show any client muzzle flashes

	if (ps->eFlags & EF_MOUNTEDTANK) {
		ps->eFlags &= ~EF_MG42_ACTIVE;
		ps->eFlags &= ~EF_AAGUN_ACTIVE;
	}
	else {
		SETUP_MOUNTEDGUN_STATUS(ps);
	}

	s->eFlags = ps->eFlags;

	if (ps->stats[STAT_HEALTH] <= 0) {
		s->eFlags |= EF_DEAD;
	}
	else {
		s->eFlags &= ~EF_DEAD;
	}

	// from MP
	if (ps->externalEvent) {
		s->event = ps->externalEvent;
		s->eventParm = ps->externalEventParm;
	}
	else if (ps->entityEventSequence < ps->eventSequence) {
		int seq;

		if (ps->entityEventSequence < ps->eventSequence - MAX_EVENTS) {
			ps->entityEventSequence = ps->eventSequence - MAX_EVENTS;
		}
		seq = ps->entityEventSequence & (MAX_EVENTS - 1);
		s->event = ps->events[seq] | ((ps->entityEventSequence & 3) << 8);
		s->eventParm = ps->eventParms[seq];
		ps->entityEventSequence++;
	}
	// end
	// Ridah, now using a circular list of events for all entities
	// add any new events that have been added to the playerState_t
	// (possibly overwriting entityState_t events)
	for (i = ps->oldEventSequence; i != ps->eventSequence; i++) {
		s->events[s->eventSequence & (MAX_EVENTS - 1)] = ps->events[i & (MAX_EVENTS - 1)];
		s->eventParms[s->eventSequence & (MAX_EVENTS - 1)] = ps->eventParms[i & (MAX_EVENTS - 1)];
		s->eventSequence++;
	}
	ps->oldEventSequence = ps->eventSequence;

	s->weapon = ps->weapon;
	s->groundEntityNum = ps->groundEntityNum;

	s->powerups = 0;
	for (i = 0; i < MAX_POWERUPS; i++) {
		if (ps->powerups[i]) {
			s->powerups |= 1 << i;
		}
	}

	s->nextWeapon = ps->nextWeapon; // Ridah
									//	s->loopSound = ps->loopSound;
	s->teamNum = ps->teamNum;
	s->aiState = ps->aiState;       // xkan, 1/10/2003
}

#endif /* __TJBOT_H__ */
