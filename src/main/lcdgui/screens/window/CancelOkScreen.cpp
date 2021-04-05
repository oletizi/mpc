#include "CancelOkScreen.hpp"

using namespace mpc::lcdgui::screens::window;
using namespace std;

CancelOkScreen::CancelOkScreen(mpc::Mpc& mpc)
	: ScreenComponent(mpc, "cancel-ok", 1)
{
    vector<vector<string>> functionLabels{vector<string>{ "", "", "", "CANCEL", "OK", "" }};
    vector<vector<int>> functionTypes{vector<int>{ -1, -1, -1, 0, 1, -1 }};
    addChild(make_shared<FunctionKeys>(mpc, "function-keys", functionLabels, functionTypes));
}

void CancelOkScreen::close()
{
    cancelAction = [](){};
    okAction = [](){};
}

void CancelOkScreen::function(int i)
{
    switch (i) {
        case 3:
            cancelAction();
            break;
        case 4:
            okAction();
            break;
    }
}

void CancelOkScreen::setCancelAction(std::function<void()> f)
{
    cancelAction = f;
}

void CancelOkScreen::setOkAction(std::function<void()> f)
{
    okAction = f;
}
