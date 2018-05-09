#include "tjbot.h"
#include <sys/timeb.h>


 // Help function
static void reset_keys() {
	*tjbot.moveleft = 0;
	*tjbot.moveright = 0;
	*tjbot.forward = 0;
	*tjbot.back = 0;
}

// functions taken from Q3
static const char *CG_Argv(int arg) {
	static char buffer[MAX_STRING_CHARS];
	tjbot.orig_syscall(CG_ARGV, arg, buffer, sizeof(buffer));
	return buffer;
}

int doesFileExist(const char *filename) {
	return GetFileAttributes(filename) != INVALID_FILE_ATTRIBUTES;
}

static char *CG_Args(void) {
	static char cmd_args[MAX_STRING_CHARS];

	syscall(CG_ARGS, cmd_args, sizeof(cmd_args));

	return cmd_args;
}

char *outinfo;
int sw = 0;

// hack functions
static void get_offsets() {

	unsigned int offsets[2];
	offsets[1] = CG_OFFSET_999;
	offsets[0] = CG_OFFSET_TWC;
	unsigned int predictedPS_address = offsets[tjbot.offsetno] + (unsigned int)tjbot.cgame_handle;
	tjbot.et_address = 0x400000;

	tjbot.orig_x = (float *) (predictedPS_address + ORIGIN_X);
    tjbot.orig_y = (float *) (predictedPS_address + ORIGIN_Y);
    tjbot.orig_z = (float *) (predictedPS_address + ORIGIN_Z);
    tjbot.vel_x = (float *) (predictedPS_address + VELOCITY_X);
    tjbot.vel_y = (float *) (predictedPS_address + VELOCITY_Y);
    tjbot.vel_z = (float *) (predictedPS_address + VELOCITY_Z);
    tjbot.view_x = (float *) (tjbot.et_address + VIEW_X);
    tjbot.view_y = (float *) (tjbot.et_address + VIEW_Y);
    tjbot.ground_ent_num = (int *) (predictedPS_address + GROUND_ENTITY_NUM);
	tjbot.forward = (unsigned int *)(tjbot.et_address + FORWARD_KEY);
	tjbot.back = (unsigned int *)(tjbot.et_address + BACK_KEY);
	tjbot.moveright = (unsigned int *)(tjbot.et_address + MOVERIGHT_KEY);
	tjbot.moveup = (unsigned int *)(tjbot.et_address + MOVEUP_KEY);
	tjbot.moveleft = (unsigned int *)(tjbot.et_address + MOVELEFT_KEY);
	tjbot.mouse_x = (float *)(tjbot.et_address + MOUSE_YAW);
}

float left_angle, right_angle;

int rang = .3; int lang = .3, isOnground = 0, downtime = 0;
float MOUSE_SAFE_PAD, velDelta, saved_gnd = 0.0f, saved_air = 0.0f, rch = 0.0f, lch = 0.0f;
float offsetX = 0.0f;
int active = 0;
float savedViewX[30];
float viewAccel;

static void jump_bot() {

	int cmd_num = tjbot.orig_syscall(CG_GETCURRENTCMDNUMBER);
	tjbot.orig_syscall(CG_GETUSERCMD, cmd_num, &cmd);

	float vel_size = (float)sqrt(*tjbot.vel_x * *tjbot.vel_x + *tjbot.vel_y * *tjbot.vel_y);

	if (vel_size < 380)
		return;

	char fps_c[256];
	tjbot.orig_syscall(6, "com_maxfps", fps_c, 256);
	float frame_rate = atof(fps_c);

	float accel_coef = isOnground ? (0.0f) : (2.0f);
	float per_angle = AngleNormalize180(rad2deg(acos((MIN_SPEED - MIN_SPEED / frame_rate * accel_coef) / vel_size * tjbot.offset)));
	float vel_angle = rad2deg(atan2(*tjbot.vel_y, *tjbot.vel_x));
	float accel_angle = rad2deg(atan2(-cmd.rightmove, cmd.forwardmove));

	left_angle = (float)(*tjbot.mouse_x - AngleDelta(*tjbot.view_x + accel_angle, vel_angle + per_angle));
	right_angle = (float)(*tjbot.mouse_x - AngleDelta(*tjbot.view_x + accel_angle, vel_angle - per_angle));
	float center_angle = (right_angle + left_angle) / 2.0f;

	if (cmd.rightmove > 0 || (cmd.forwardmove > 0 && *tjbot.mouse_x < center_angle))
	{
		if (*tjbot.mouse_x > right_angle)
		{
			float diff = *tjbot.mouse_x + (right_angle - *tjbot.mouse_x) - offsetX * 5;
			if (abs(diff - *tjbot.mouse_x) > 0.001745f)
			{
				return;
			}
			else
			{
				*tjbot.mouse_x = diff;
			}
		}
	}
	else if (cmd.rightmove < 0 || (cmd.forwardmove > 0 && *tjbot.mouse_x > center_angle))
	{
		if (*tjbot.mouse_x < left_angle)
		{
			float diff = *tjbot.mouse_x + (left_angle - *tjbot.mouse_x) + offsetX * 5;
			if (abs(diff - *tjbot.mouse_x) > 0.001745f)
			{
				return;
			}
			else if (*tjbot.mouse_x < left_angle)
			{
				*tjbot.mouse_x = diff;
			}
		}
	}
}

// Automatically jumps if not in the air
static void autojump() {
	if (*tjbot.ground_ent_num != ENTITYNUM_NONE)
	{
		*(int *)KEY_UP = qtrue;
		tjbot.delayedframes = 0;
	}
	else
	{
		tjbot.delayedframes++;
		if (tjbot.delayedframes > tjbot.jumpdelay)
			*(int *)KEY_UP = qfalse;
	}
}

char *va(char *format, ...) {
    static char buf[MAX_STRING_CHARS];
    va_list va;
    va_start(va, format);
    vsprintf(buf, format, va);
    va_end(va);
    return buf;
}

float viewang = 0;

// replaced functions
int syscall(int cmd, ...) {
    int args[11];
    va_list va;
    va_start(va, cmd);
    for (int i = 0; i < sizeof(args) / sizeof(args[0]); i++)
        args[i] = va_arg(va, int);
    va_end(va);

	if (cmd == CG_R_RENDERSCENE && tjbot.view_hack_enable) {
		// 180 degs rotation matrix
		float mat[3][3];
		SetRotationZ(mat, viewang);

		refdef_t *rd = (refdef_t *)args[0];
		float res[3][3];
		MatrixMultiply(rd->viewaxis, mat, res);
		memcpy(rd->viewaxis, res, sizeof(rd->viewaxis));
		return tjbot.orig_syscall(cmd, rd);
	}

    return tjbot.orig_syscall(cmd, args[0], args[1], args[2], args[3], args[4],
                              args[5], args[6], args[7], args[8], args[9], args[10]);
}

void cg_dllEntry(int(*syscallptr)(int arg, ...)) {
    tjbot.orig_syscall = syscallptr;
    tjbot.orig_cg_dllEntry(syscall);
}

float setang;
float sinpar = 0.0f;
float saved_view = 0.0f;

int cg_vmMain(int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8,
              int arg9, int arg10, int arg11) {
	switch (command) {
	case CG_INIT:
	{
		tjbot.orig_syscall(CG_ADDCOMMAND, AUTO_ON_COMMAND);
		tjbot.orig_syscall(CG_ADDCOMMAND, AUTO_OFF_COMMAND);
		tjbot.orig_syscall(CG_ADDCOMMAND, BOT_ON_COMMAND);
		tjbot.orig_syscall(CG_ADDCOMMAND, BOT_OFF_COMMAND);
		tjbot.orig_syscall(CG_ADDCOMMAND, JUMP_DELAY_COMMAND);
		tjbot.orig_syscall(CG_ADDCOMMAND, VIEW_HACK_ON_COMMAND);
		tjbot.orig_syscall(CG_ADDCOMMAND, VIEW_HACK_OFF_COMMAND);
		tjbot.orig_syscall(CG_ADDCOMMAND, "sc_setangle");
		tjbot.orig_syscall(CG_ADDCOMMAND, "sc_offset");

		tjbot.offset = 1.09f;
		tjbot.offsetno = 0;
		tjbot.jumpdelay = 8;

		get_offsets();

		break;
	}
	case CG_SHUTDOWN:
	{
		// Warning: file is closed for vid_restart too
	}
    case CG_CONSOLE_COMMAND: 
	{
		const char *cmd = CG_Argv(0);
		if (!strcmp(cmd, BOT_ON_COMMAND)) {
			tjbot.groundaccel = tjbot.airaccel = 0.1f;
			tjbot.max_gndaccel = tjbot.max_airaccel = 1.1f;
			saved_air = saved_gnd = 0.1f;
			tjbot.jumpbot_enable = 1;
			return qtrue;
		}
		else if (!strcmp(cmd, BOT_OFF_COMMAND)) {
			tjbot.jumpbot_enable = 0;
			//reset_keys();
			return qtrue;
		}
		else if (!strcmp(cmd, AUTO_ON_COMMAND)) {
			tjbot.autojump_enable = 1;
			return qtrue;
		}
		else if (!strcmp(cmd, AUTO_OFF_COMMAND)) {
			*(int *)KEY_UP = qfalse;
			tjbot.autojump_enable = 0;
			return qtrue;
		}
		else if (!strcmp(cmd, VIEW_HACK_ON_COMMAND)) {
			tjbot.view_hack_enable = 1;
			//tjbot.orig_syscall(CG_SENDCONSOLECOMMAND, "bind a \"+moveright\"; bind d \"+moveleft\"; bind w \"+back\"; bind s \"+forward\"; ");
			return qtrue;
		}
		else if (!strcmp(cmd, VIEW_HACK_OFF_COMMAND)) {
			tjbot.view_hack_enable = 0;
			//tjbot.orig_syscall(CG_SENDCONSOLECOMMAND, "bind a \"+moveleft\"; bind d \"+moveright\"; bind w \"+forward\"; bind s \"+back\"; ");
			return qtrue;
		}
		else if (!strcmp(cmd, "sc_setangle")) {
			setang = *tjbot.view_x;
			return qtrue;
		}
		else if (!strcmp(cmd, JUMP_DELAY_COMMAND))
		{
			if (strlen(CG_Argv(1)))
			{
				tjbot.jumpdelay = (int)(strtof(CG_Argv(1), NULL));
			}
			else
			{
				tjbot.orig_syscall(CG_PRINT, va("^wsc_jumpdelay: %i   [default = 10]\n", tjbot.jumpdelay));
				tjbot.orig_syscall(CG_PRINT, va("^wAmount of frames jump key is pressed\n"));
			}
			return qtrue;
		}
		else if (!strcmp(cmd, "sc_offset"))
		{
			if (strlen(CG_Argv(1)))
			{
				tjbot.offset = (float)(strtof(CG_Argv(1), NULL));
			}
			else
			{
				tjbot.orig_syscall(CG_PRINT, va("^wsc_offset: %f\n", tjbot.offset));
			}
			return qtrue;
		}
		else if (!strcmp(cmd, ANGLE_COMMAND))
		{
			if (strlen(CG_Argv(1)))
			{
				float a = strtof(CG_Argv(1), NULL);
				*tjbot.mouse_x += a - *tjbot.view_x;
			}
			else
				tjbot.orig_syscall(CG_PRINT, va("^9Rotates view angle by [amount] degrees\n"));
			return qtrue;
		}
		break;
	}
        default:
            break;
    }

    int res = tjbot.orig_cg_vmMain(command, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);

	switch (command) {
	case CG_DRAW_ACTIVE_FRAME:
	{
		;
	}

	if (sinpar >= 3.14f)
	{
		sinpar = 0.0f;
	}
	sinpar += 0.000001f;

	/*if (active == 0)
	{
		sinpar = 0;
	}*/

	offsetX = sinf(sinpar);

	viewang = setang - *tjbot.view_x;

	isOnground = *tjbot.ground_ent_num != ENTITYNUM_NONE ? 1 : 0;
	// max speed
	float speed = (float)sqrt(*tjbot.vel_x * *tjbot.vel_x + *tjbot.vel_y * *tjbot.vel_y);
	if (speed > tjbot.max_speed)
		tjbot.max_speed = speed;
	
	if (tjbot.jumpbot_enable)
		jump_bot();

	if (tjbot.autojump_enable)
		autojump();
	else
		tjbot.delayedframes = 0;

	if (tjbot.cj_info)
	{

		int cmd_num = tjbot.orig_syscall(CG_GETCURRENTCMDNUMBER);
		tjbot.orig_syscall(CG_GETUSERCMD, cmd_num, &cmd);

		if (cmd.upmove > 0)
		{
			if (downtime < 125)
				downtime++;
		}
		else if (cmd.upmove == 0)
		{
			if (downtime < 125 && downtime)
				tjbot.orig_syscall(CG_SENDCLIENTCOMMAND, va("say_buddy \"^1MoveUp ^wpressed for ^1%i ms", downtime * 1000 / 125));
			else if (downtime == 125)
				tjbot.orig_syscall(CG_SENDCLIENTCOMMAND, "say_buddy \"^1MoveUp ^wpressed for ^11000+ ms");
			downtime = 0;
		}
	}

	float sum = 0.0f;
	for (int i = 29; i > 0; i--)
	{
		savedViewX[i] = savedViewX[i - 1];
		sum += savedViewX[i];
	}
	savedViewX[0] = saved_view - *tjbot.view_x;
	sum += savedViewX[0];
	viewAccel = sum / 30.0f;
	if (viewAccel > 1.0e-5f)
	{
		viewAccel = 1.0e-5f;
	}
	if (viewAccel < -1.0e-5f)
	{
		viewAccel = -1.0e-5f;
	}

	saved_view = *tjbot.view_x;

	if (!*tjbot.orig_x && !*tjbot.orig_y && !*tjbot.orig_z)
	{
		tjbot.offsetno = !tjbot.offsetno;
		get_offsets();
	}

	break;
    default:
        break;
    }
    return res;
}

// hooked functions

#define SIZE 6
BYTE oldBytesLLA[SIZE] = { 0 };
BYTE JMPLLA[SIZE] = { 0 };
HMODULE WINAPI tjbot_LoadLibraryA(LPCTSTR lpFileName) {
    memcpy(LoadLibraryA, oldBytesLLA, SIZE);

    HINSTANCE hInst = LoadLibraryA(lpFileName); 
	if (lpFileName && hInst && strstr(lpFileName, "cgame_mp_x86.dll")) {
		strcpy(tjbot.cgame_filename, lpFileName);
		tjbot.cgame_handle = hInst;
	}

    memcpy(LoadLibraryA, JMPLLA, SIZE);
    return hInst;
}

BYTE oldBytesGPA[SIZE] = { 0 };
BYTE JMPGPA[SIZE] = { 0 };
FARPROC WINAPI tjbot_GetProcAddress(HMODULE hModule, LPCSTR lpProcName) {
    memcpy(GetProcAddress, oldBytesGPA, SIZE);

    FARPROC fRet = GetProcAddress(hModule, lpProcName);
    if (hModule == tjbot.cgame_handle) {
        if (!strcmp(lpProcName, "dllEntry")) {
            tjbot.orig_cg_dllEntry = (void(*)(int(*)(int, ...))) fRet;
            fRet = (FARPROC)cg_dllEntry;
        }
        else if (!strcmp(lpProcName, "vmMain")) {
            tjbot.orig_cg_vmMain = (int(*)(int, int, int, int, int, int, int, int, int, int, int, int, int)) fRet;
            fRet = (FARPROC)cg_vmMain;
        }
    }

    memcpy(GetProcAddress, JMPGPA, SIZE);
    return fRet;
}

void hijack(LPVOID newFunction, LPVOID origFunction, BYTE oldBytes[SIZE], BYTE JMP[SIZE]) {
    BYTE tempJMP[SIZE] = { ASM_JMP, 0x90, 0x90, 0x90, 0x90, ASM_RETN };
    memcpy(JMP, tempJMP, SIZE);
    DWORD JMPSize = ((DWORD)newFunction - (DWORD)origFunction - 5);
    DWORD dwProtect;
    VirtualProtect((LPVOID)origFunction, SIZE, PAGE_EXECUTE_READWRITE, &dwProtect);
    memcpy(oldBytes, origFunction, SIZE);
    memcpy(&JMP[1], &JMPSize, 4);
    memcpy(origFunction, JMP, SIZE);
}

BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, PVOID pvReserved) {
    if (dwReason == DLL_PROCESS_ATTACH) {
        hijack(tjbot_LoadLibraryA, LoadLibraryA, oldBytesLLA, JMPLLA);
        hijack(tjbot_GetProcAddress, GetProcAddress, oldBytesGPA, JMPGPA);
    }
    return 1;
}