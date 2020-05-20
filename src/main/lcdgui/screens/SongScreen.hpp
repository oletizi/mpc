#pragma once
#include <lcdgui/ScreenComponent.hpp>

#include <memory>

namespace mpc::lcdgui::screens
{
	class SongScreen
		: public mpc::lcdgui::ScreenComponent
	{

	private:
		int offset = -1;
		int selectedSongIndex = 0;
		std::string defaultSongName = "Song";
		bool loop = false;

		void setLoop(bool b);
		void setOffset(int i);
		void setSelectedSongIndex(int i);
		void setDefaultSongName(std::string s);

	public:
		void init() override;

	public:
		void up() override;
		void down() override;
		void left() override;
		void right() override;
		void openWindow() override;
		void turnWheel(int i) override;
		void function(int i) override;

		SongScreen(const int& layer);

		void open() override;
		void close() override;
		void update(moduru::observer::Observable*, nonstd::any message);

	private:
		void displayTempo();
		void displayLoop();
		void displaySteps();
		void displayTempoSource();
		void displayNow0();
		void displayNow1();
		void displayNow2();
		void displaySongName();

	};
}
