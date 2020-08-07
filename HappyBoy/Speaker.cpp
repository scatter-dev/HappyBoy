#include "Speaker.h"

#include <thread>
#include <iostream>

void audio_callback(void* userdata, Uint8* stream, int len)
{
	Speaker* s = (Speaker*)userdata;
	if (!s->apu->NR52.allsound_on) {
		return;
	}
	double channel1_freq = 0;
	if (!channel1.initial && s->apu->NR14.initial) {
		channel1.playing = true;
		channel1.start_time = s->global_time;
		channel1.length = s->apu->NR11.sound_length;
	}
	channel1.l_volume = s->apu->NR51.sound1_to_so1 ? s->apu->NR50.so1_volume : 0;
	channel1.r_volume = s->apu->NR51.sound1_to_so2 ? s->apu->NR50.so2_volume : 0;
	channel1.initial = s->apu->NR14.initial;
	channel1.constant = !s->apu->NR14.counter;
	channel1.freq = 131072.0 / (2048 - (((s->apu->NR14.reg & 0x07) << 8) + s->apu->NR13));

	channel2.l_volume = s->apu->NR51.sound2_to_so1 ? s->apu->NR50.so1_volume : 0;
	channel2.r_volume = s->apu->NR51.sound2_to_so2 ? s->apu->NR50.so2_volume : 0;
	channel2.initial = s->apu->NR24.initial;
	channel2.constant = !s->apu->NR24.counter;
	channel2.freq = 131072.0 / (2048 - (((s->apu->NR24.reg & 0x07) << 8) + s->apu->NR23));

	for (int i = 0; i < len / 2; i++) {
		((Uint16*)stream)[i] = channel1.get_sample(s->global_time) + channel2.get_sample(s->global_time);
		s->global_time += s->time_step;
	}
}



Speaker::Speaker(std::shared_ptr<APU> apu) : apu(apu)
{
	std::thread speakerThread(&Speaker::Start, this);
	speakerThread.detach();
}


void Speaker::Start() {
	SDL_Init(SDL_INIT_AUDIO);
	SDL_AudioDeviceID audio_device;
	SDL_AudioSpec audio_spec;
	SDL_zero(audio_spec);
	audio_spec.freq = 44100;
	audio_spec.format = AUDIO_S16SYS;
	audio_spec.channels = 1;
	audio_spec.samples = 1024;
	audio_spec.callback = audio_callback;
	audio_spec.userdata = this;
	time_step = 1.0 / audio_spec.freq;
	global_time = 0;

	audio_device = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);

	SDL_PauseAudioDevice(audio_device, 0);
}


