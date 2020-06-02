#pragma once
#include <lcdgui/ScreenComponent.hpp>

namespace mpc::lcdgui::screens::dialog
{
	class FileAlreadyExistsScreen;
}

namespace mpc::lcdgui::screens::window
{
	class SaveASoundScreen
		: public mpc::lcdgui::ScreenComponent
	{

	public:
		void turnWheel(int i) override;
		void function(int i) override;

		SaveASoundScreen(const int layerIndex);
		void open() override;

	private:
		std::string saveName = "";
		void setFileType(int i);
		int fileType = 0;
		void displayFileType();
		void displayFile();

		friend class mpc::lcdgui::screens::dialog::FileAlreadyExistsScreen;

	};
}
