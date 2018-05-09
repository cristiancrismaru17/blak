A TrickJump Bot for Wolfenstein: Enemy Territory 2.60b and ETJump

## Features / Commands
     +sc_autojump	|	-sc_autojump	automatic `+moveup` (jump) activation
     +sc_jumpbot	|	-sc_jumpbot		enable/disable jumpbot
     +sc_lonebeat	|	-sc_lonebeat	enable/disable left side one beat (W only)
     +sc_ronebeat	|	-sc_ronebeat	enable/disable left side one beat (W only)
     +sc_origin		|	-sc_origin		start/stop trying to move to desired XY origin
     +sc_ps_print	| 	-sc_ps_print 	enable/disable printing predicted playerstate view angles, origin and velocity every frame in the console
     +sc_viewhack	| 	-sc_viewhack 	rotates view camera by 180 degs for jumping backwards
	 sc_airaccel <value>				jumpbot air angle acceleration
     sc_angle <deg>						set new horizontal view angle to specified value
	 sc_coloured_chat <1|2|3>			coloured chat type. Usage: write your message in console, then press "\" key
	 sc_drawCgaz <0|1|2|3|4>			hide cgaz status. <only premium users>
	 sc_groundaccel <value>				jumpbot ground angle acceleration
	 sc_info							prints Hack info
	 sc_jump_delay <value>				jump delay for autojump
     sc_ms_print	| sc_ms_reset 		prints/resets horizontal maxspeed
     sc_originget						diplays current origin (similar to viewpos command)
     sc_originset <[x] [y]>				sets new desired [X][Y] origin or current origin
	 sc_password <your password>		enables premium features
	 sc_reset 							resets everything to default value
	 sc_safestart <0|1>					jumpbot smooth start (requires sc_smoothness > 0)
	 sc_smoothness <value>				smoothness degree of the bot. 0 = automated bot movements, other = smooth movement.
										the bigger the value, the smoother the movement.
	 sc_smoothness_auto <0|1>			sets sc_smoothness based on velocity value
     sc_startdump <filename> |  sc_stopdump		to dump view angles, origin and velocity every frame into a file
	 sc_styles <f|i|b> 					jumpbot allowed styles (default <fi>). f = fullbeat(strafe), i = invert, b = backwards

## Usage
- Enable +sprint to make sure to always sprint, the tjbot does not work while not sprinting

- Set your FPS to 125 and enable fixed player move
  com_maxfps 125
  set pmove_fixed 1

- TrickJump!

### Run
Run TJBOT.EXE AS ADMIN then open ET and connect to a compatible ETJump server

### Licence
GPLv2

### Credit
setup
Annihil
Straycat