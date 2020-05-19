#pragma once
#include <observer/Observer.hpp>

#include <memory>

namespace mpc {
	
	namespace sequencer {
		class Sequencer;
	}

	namespace lcdgui {
		class Field;
		class Label;
	}
	
	namespace ui {
		namespace sequencer {

			class NextSeqObserver
				: public moduru::observer::Observer
			{

			private:
				std::weak_ptr<mpc::lcdgui::Field> sqField{};
				std::weak_ptr<mpc::lcdgui::Field> now0Field{};
				std::weak_ptr<mpc::lcdgui::Field> now1Field{};
				std::weak_ptr<mpc::lcdgui::Field> now2Field{};
				std::weak_ptr<mpc::lcdgui::Field> tempoField{};
				std::weak_ptr<mpc::lcdgui::Label> tempoLabel{};
				std::weak_ptr<mpc::lcdgui::Field> tempoSourceField{};
				std::weak_ptr<mpc::lcdgui::Field> timingField{};
				std::weak_ptr<mpc::lcdgui::Field> nextSqField{};
				std::weak_ptr<mpc::sequencer::Sequencer> sequencer{};
				
			public:
				void update(moduru::observer::Observable* o, nonstd::any arg) override;

				NextSeqObserver();
				~NextSeqObserver();

			};

		}
	}
}
