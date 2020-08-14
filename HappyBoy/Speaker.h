#pragma once

#include <memory>
#include <algorithm>

#include "APU.h";
#include <SDL.h>

struct {
	double length = 0;
	double start_time;
	double freq;
	bool constant = false;
	bool initial = false;
	double l_volume = 0;
	double r_volume = 0;
	bool playing = false;

	uint8_t envelope_initial_volume = 0;
	uint8_t envelope_volume = 0;
	uint8_t envelope_step = 0;
	bool envelope_increase = false;

	int16_t get_sample(double time) {
		if (!playing) {
			return 0;
		}
		if (time < start_time + length || constant) {
			double volume_mod = envelope_initial_volume + (envelope_increase ? 1 : -1) * (time - start_time) / (envelope_step / 64.0);
			volume_mod /= 15.0;
			if (volume_mod < 0) {
				volume_mod = 0;
			}
			return (sin(time * freq * 2 * M_PI) > 0 ? 1 : -1) * ((l_volume + r_volume) / 14.0) * 1000 * volume_mod;
		}
		else {
			return 0;
		}
	}
} channel1;

struct {
	double length = 0;
	double start_time;
	double freq;
	bool constant = false;
	bool initial = false;
	double l_volume = 0;
	double r_volume = 0;
	bool playing = false;

	uint8_t envelope_initial_volume = 0;
	uint8_t envelope_volume = 0;
	uint8_t envelope_step = 0;
	bool envelope_increase = false;

	int16_t get_sample(double time) {
		if (!playing) {
			return 0;
		}
		if (time < start_time + length || constant) {
			double volume_mod = envelope_initial_volume + (envelope_increase ? 1 : -1) * (time - start_time) / (envelope_step / 64.0);
			volume_mod /= 15.0;
			if (volume_mod < 0) {
				volume_mod = 0;
			}
			return (sin(time * freq * 2 * M_PI) > 0 ? 1 : -1) * ((l_volume + r_volume) / 14.0) * 1000 * volume_mod;
		}
		else {
			return 0;
		}
	}
} channel2;

struct {
	double length = 0;
	double start_time;
	double freq;
	bool constant = false;
	bool initial = false;
	double l_volume = 0;
	double r_volume = 0;
	uint8_t output_level = 0;

	int16_t get_sample(double time, uint8_t* waveram) {
		if (output_level == 0) {
			return 0;
		}
		uint8_t wavebyte = (uint8_t)(time * freq * 32 / 2) % 32;
		uint8_t sample = (wavebyte % 2 == 0) ? waveram[wavebyte/2] >> 4 : waveram[wavebyte/2] & 0x0F;
		sample >>= (output_level - 1);
		return (sample) * ((l_volume + r_volume) / 14.0) * 1000 / 16.0;
	}
} channel3;

struct {
	double length = 0;
	double start_time;
	double freq;
	double period;
	bool constant = false;
	bool initial = false;
	double l_volume = 0;
	double r_volume = 0;
	bool playing = false;

	uint8_t envelope_initial_volume = 0;
	uint8_t envelope_volume = 0;
	uint8_t envelope_step = 0;
	bool envelope_increase = false;

	int16_t get_sample(double time) {
		if (!playing) {
			return 0;
		}
		if (time < start_time + length || constant) {
			double volume_mod = envelope_initial_volume + (envelope_increase ? 1 : -1) * (time - start_time) / (envelope_step / 64.0);
			volume_mod /= 15.0;
			if (volume_mod < 0) {
				volume_mod = 0;
			}
			srand(fmod(time - start_time, 1.0/freq));
			return (rand() > 0.5) * 1000;
		}
		else {
			return 0;
		}
	}
} channel4;

class Speaker
{
public:
	Speaker(std::shared_ptr<APU> apu);
	std::shared_ptr<APU> apu;
	double global_time;
	double time_step;

private:
	void Start();
};

