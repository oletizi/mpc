#pragma once
#include <lcdgui/ScreenComponent.hpp>

#include <functional>

namespace mpc::lcdgui::screens
{
	class CancelOkScreen
		: public mpc::lcdgui::ScreenComponent
	{
	public:
		CancelOkScreen(mpc::Mpc& mpc);
        void close() override;
        void function(int) override;

        void setCancelAction(std::function<void()>);
        void setOkAction(std::function<void()>);
        
    private:
        std::function<void()> cancelAction = [](){};
        std::function<void()> okAction = [](){};
	};
}
