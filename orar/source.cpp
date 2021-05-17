#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>

#include "SDL.h"
#include "SDL_Image.h"
#include "SDL_ttf.h"
#include "UI_engine.h"

using namespace UI_engine;

std::vector<std::vector<std::string>> program = {
	//luni
		{
		"info",
			"info",
			"info",
			"franca",
			"fizica",
			"info",
			"info",
			"info",
		},
		//marti
	{
		"fizica",
		"bio",
		"chimie",
		"franca",
		"romana",
		"engleza",
		"religie",
	},
	//miercuri
	{
		"mate",
		"mate",
		"info",
		"bio",
		"romana",
		"fizica",
		"isto",
	},
	//joi
	{
		"sport",
		"mate",
		"geogra",
		"muzica/desen",
		"tic",
		"fizica",
		"atp",
	},
	//vineri
	{
		"chimie",
		"romana",
		"engleza",
		"mate",
		"mate",
		"psiho",
	},

};

struct point {
	float x, y;
}origin;

int hour_index = 0;
int week_index = 0;

int hour = 7, minute = 50;

float divisor = 3.710f;
float cell_width = 0.0f, cell_height = 0.0f;
float offset_width = 5.0f, offset_height = 5.0f;

const std::string ziua[] = { "duminica","luni","marti","miercuri","joi","vineri","sambata" };
const int translated_day[] = { 7,1,2,3,4,5,6 };

void updateTime(UI_window* window) {
	if (((UI_text*)window->object("currentTime"))->text() != std::to_string((time(0) / 3600 % 24 + 3) % 24) + ":" + (((time(0) % 3600) / 60 < 10) ? "0" : "") + std::to_string((time(0) % 3600) / 60)) {
		((UI_text*)window->object("currentTime"))->text(std::to_string((time(0) / 3600 % 24 + 3) % 24) + ":" + (((time(0) % 3600) / 60 < 10) ? "0" : "") + std::to_string((time(0) % 3600) / 60));
	}
	window->addEvent(updateTime);
}

void updateTTNH(UI_window* window) {
	if (((hour - ((time(0) / 3600 % 24 + 3) % 24)) * 60 + (minute - ((time(0) % 3600) / 60))) < 0) {
		minute += 50;
		hour += minute / 60;
		minute %= 60;
		++hour_index;
	}
	if (((UI_text*)window->object("timeTillNextHour"))->text() != (((hour - ((time(0) / 3600 % 24 + 3) % 24)) * 60 + ((minute - ((time(0) % 3600) / 60)))) < 10 ? "0" : "") + std::to_string((hour - ((time(0) / 3600 % 24 + 3) % 24)) * 60 + (minute - ((time(0) % 3600) / 60)))) {
		((UI_text*)window->object("timeTillNextHour"))->text((((hour - ((time(0) / 3600 % 24 + 3) % 24)) * 60 + ((minute - ((time(0) % 3600) / 60)))) < 10 ? "0" : "") + std::to_string((hour - ((time(0) / 3600 % 24 + 3) % 24)) * 60 + (minute - ((time(0) % 3600) / 60))));
	}

	if (hour_index > program[week_index].size()) {
		window->addEvent(updateTTNH);
	}
}

void waitForHoursToStart(UI_window* window) {
	if ((time(0) / 3600 % 24 + 3) % 24 > 8) {
		window->object("waiting_text")->destroy();

		SDL_Surface* currentDaySurface = IMG_Load("assets/current_day.png");
		SDL_Surface* daySurface = IMG_Load("assets/day.png");

		cell_width = daySurface->w / divisor, cell_height = daySurface->h / divisor;

		UI_frame* hourThingsBackground = new UI_frame(0.0f, 0.0f, 205.0f, window->height(), 1.0f, { 243, 189, 161 });

		UI_text* timeTillNextHour = new UI_text(25.0f, 15.0f, 1.0f, "tTNH");

		timeTillNextHour->font("C:/Windows/Fonts/Arial.ttf", 205);
		timeTillNextHour->width(timeTillNextHour->width() * 1.25f);
		timeTillNextHour->height(timeTillNextHour->height() * 1.35f);

		UI_text* currentTime = new UI_text(33.0f, window->height() - 85.0f, 1.0f, std::to_string((time(0) / 3600 % 24 + 3) % 24) + ":" + std::to_string((time(0) % 3600) / 60));

		window->add(hourThingsBackground);
		window->add(timeTillNextHour);
		window->add(currentTime);

		window->addEvent(updateTime);

		for (minute; (hour * 60 + minute) < ((((time(0) / 3600 % 24 + 3) % 24)) * 60 + ((time(0) % 3600) / 60)); minute += 50) {
			hour += minute / 60;
			minute %= 60;
			++hour_index;
		}
		--hour_index;

		std::cout << "---hour_index " << hour_index << '\n';

		window->addEvent(updateTTNH);

		std::cout << (time(0) / 3600 % 24 + 3) % 24 << ':' << (time(0) % 3600) / 60 << '\n';

		for (int wd = 1; wd <= 5; ++wd) {
			for (int h = 0; h < program[wd - 1].size(); ++h) {
				std::cout << program[wd - 1][h] << '\n';
				UI_image* day = new UI_image((origin.x + (cell_width + offset_width) * (wd - 1)), (origin.y + (cell_height + offset_height) * h), cell_width, cell_height, 1.0f);
				UI_text* dayName = new UI_text(day->x(), day->y(), 1.0f, program[wd - 1][h]);

				dayName->width(dayName->width() / 3.0f);
				dayName->height(dayName->height() / 3.0f);

				dayName->x(day->x() + day->width() / 2 - dayName->width() / 2);
				dayName->y(day->y() + day->height() / 2 - dayName->height() / 2);

				day->source((wd == week_index) ? (currentDaySurface) : (daySurface));
				day->setRenderSectionFull();

				window->addWithCustomName(ziua[wd] + ".hour(" + std::to_string(h + 1) + ")", day);
				window->addWithCustomName(ziua[wd] + ".text(" + std::to_string(h + 1) + ")", dayName);
			}
			std::cout << '\n';
		}

	}
	else {
		window->addEvent(waitForHoursToStart);
	}
}

int main(int argc, char* argv[]) {
	UI_window window("HSO",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1050, 750, SDL_WINDOW_SHOWN, SDL_RENDERER_ACCELERATED,
		75.0);
	window.background({ 104, 121, 128,255 });

	origin = { 210.0f,45.0f };

	time_t rawtime;
	tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	int wday = timeinfo->tm_wday;

	week_index = translated_day[wday];

	//if (week_index >= 1 && week_index < 6)
	window.addEvent(waitForHoursToStart);

	UI_text waiting_text(window.width() / 2.0f, window.height() / 2.0f, 1.0f, "waiting for hours to start");

	waiting_text.x(waiting_text.x() - waiting_text.width() / 2.0f);
	waiting_text.y(waiting_text.y() - waiting_text.height() / 2.0f);

	window.add(&waiting_text);

	processEvents(&window);
	return 0;
}