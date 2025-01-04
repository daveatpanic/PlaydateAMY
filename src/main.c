//
//  main.c
//  Extension
//
//  Created by Dave Hayden on 7/30/14.
//  Copyright (c) 2014 Panic, Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "amy.h"


int AMYrender(void* context, int16_t* left, int16_t* right, int len)
{
	int16_t* block = amy_simple_fill_buffer();
	
	for ( int i = 0; i < len; ++i )
	{
		left[i] = block[2*i];
		right[i] = block[2*i+1];
	}
	
	return 1;
}

static int update(void* userdata);

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
		amy_start(/* cores= */ 1, /* reverb= */ 1, /* chorus= */ 1, /* echo= */ 1);
		amy_reset_oscs();
		
		pd->sound->addSource(AMYrender, NULL, 1);

		pd->system->setUpdateCallback(update, pd);
	}
	
	return 0;
}

//extern struct state amy_global;

int fprintf(FILE* restrict stream, const char * restrict format, ...)
{
	return 0;
}

// from examples.c:
void example_fm(uint32_t start) {
	// Direct construction of an FM tone, as in the documentation.
	struct event e;

	// Modulating oscillator (op 2)
	e = amy_default_event();
	e.time = start;
	e.osc = 9;
	e.wave = SINE;
	e.ratio = 1.0f;
	e.amp_coefs[COEF_CONST] = 1.0f;
	e.amp_coefs[COEF_VEL] = 0;
	e.amp_coefs[COEF_EG0] = 0;
	amy_add_event(e);

	// Output oscillator (op 1)
	e = amy_default_event();
	e.time = start;
	e.osc = 8;
	e.wave = SINE;
	e.ratio = 0.2f;
	e.amp_coefs[COEF_CONST] = 1.0f;
	e.amp_coefs[COEF_VEL] = 0;
	e.amp_coefs[COEF_EG0] = 1.0f;
	strcpy(e.bp0, "0,1,1000,0,0,0");
	amy_add_event(e);

	// ALGO control oscillator
	e = amy_default_event();
	e.time = start;
	e.osc = 7;
	e.wave = ALGO;
	e.algorithm = 1;  // algo 1 has op 2 driving op 1 driving output (plus a second chain for ops 6,5,4,3).
	strcpy(e.algo_source, ",,,,9,8");
	amy_add_event(e);

	// Add a note on event.
	e = amy_default_event();
	e.time = start + 100;
	e.osc = 7;
	e.midi_note = 60;
	e.velocity = 1.0f;
	amy_add_event(e);
}

// input goes here:
// extern int16_t amy_in_block[AMY_BLOCK_SIZE*AMY_NCHANS];

static int update(void* userdata)
{
	PlaydateAPI* pd = userdata;
	pd->graphics->clear(kColorWhite);
	
	PDButtons pressed;
	pd->system->getButtonState(NULL, &pressed, NULL);
	
	// amy_play_message((char*)data);
	
	if ( pressed & kButtonA )
	{
		amy_reset_oscs();

		struct event a = amy_default_event();
		a.osc = 1;
		a.wave = SINE;
		a.freq_coefs[0] = 0.25;

		a.amp_coefs[0] = 1;

		amy_add_event(a);

		struct event e = amy_default_event();
		e.osc = 0;
		e.wave = PULSE;
		e.freq_coefs[0] = 440;

		e.mod_source = 1;

		e.duty_coefs[5] = 1;
		e.velocity = 1;
		amy_add_event(e);
	}
	
	if ( pressed & kButtonB )
	{
		amy_reset_oscs();
		example_fm(0);
	}
	
	pd->system->drawFPS(0,0);
	return 1;
}

