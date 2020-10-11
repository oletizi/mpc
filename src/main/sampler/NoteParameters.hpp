#pragma once
#include <mpc/MpcNoteParameters.hpp>

#include <mpc/MpcStereoMixerChannel.hpp>
#include <mpc/MpcIndivFxMixerChannel.hpp>

namespace mpc::sampler
{
	class Sampler;

	class NoteParameters
		: public virtual ctoot::mpc::MpcNoteParameters
	{
	public:
		int getSoundIndex() override;
		int getSoundGenerationMode() override;
		int getOptionalNoteA() override;
		int getOptionalNoteB() override;
		int getMuteAssignA() override;
		int getMuteAssignB() override;
		int getTune() override;
		int getVelocityToStart() override;
		int getAttack() override;
		int getDecay() override;
		int getVelocityToAttack() override;
		int getDecayMode() override;
		int getVeloToLevel() override;
		int getFilterFrequency() override;
		int getVelocityToFilterFrequency() override;
		int getFilterAttack() override;
		int getFilterDecay() override;
		int getFilterResonance() override;
		int getFilterEnvelopeAmount() override;
		int getVoiceOverlap() override;

		std::weak_ptr<ctoot::mpc::MpcStereoMixerChannel> getStereoMixerChannel();
		std::weak_ptr<ctoot::mpc::MpcIndivFxMixerChannel> getIndivFxMixerChannel();

	private:
		std::shared_ptr<ctoot::mpc::MpcStereoMixerChannel> stereoMixerChannel = std::make_shared<ctoot::mpc::MpcStereoMixerChannel>();
		std::shared_ptr<ctoot::mpc::MpcIndivFxMixerChannel> indivFxMixerChannel = std::make_shared<ctoot::mpc::MpcIndivFxMixerChannel>();
		int soundIndex = -1;
		int soundGenerationMode = 0;
		int velocityRangeLower = 0;
		int optionalNoteA = 0;
		int velocityRangeUpper = 0;
		int optionalNoteB = 0;
		int voiceOverlap = 0;
		int muteAssignA = 0;
		int muteAssignB = 0;
		int tune = 0;
		int attack = 0;
		int decay = 0;
		int decayMode = 0;
		int filterFrequency = 0;
		int filterResonance = 0;
		int filterAttack = 0;
		int filterDecay = 0;
		int filterEnvelopeAmount = 0;
		int velocityToLevel = 0;
		int velocityToAttack = 0;
		int velocityToStart = 0;
		int velocityToFilterFrequency = 0;
		int sliderParameterNumber = 0;
		int velocityToPitch = 0;
		int index = 0;

	public:
		void setSoundNumberNoLimit(int i);
		void setSoundIndex(int i);
		void setSoundGenMode(int i);
		void setVeloRangeLower(int i);
		int getVelocityRangeLower();
		void setOptNoteA(int i);
		void setVeloRangeUpper(int i);
		int getVelocityRangeUpper();
		void setOptionalNoteB(int i);
		void setVoiceOverlap(int i);
		void setMuteAssignA(int i);
		void setMuteAssignB(int i);
		void setTune(int i);
		void setAttack(int i);
		void setDecay(int i);
		void setDecayMode(int i);
		void setFilterFrequency(int i);
		void setFilterResonance(int i);
		void setFilterAttack(int i);
		void setFilterDecay(int i);
		void setFilterEnvelopeAmount(int i);
		void setVeloToLevel(int i);
		void setVelocityToAttack(int i);
		void setVelocityToStart(int i);
		void setVelocityToFilterFrequency(int i);
		void setSliderParameterNumber(int i);
		int getSliderParameterNumber();
		void setVelocityToPitch(int i);
		int getVelocityToPitch();
		NoteParameters* clone(const int newIndex);
		int getNumber();

		NoteParameters(int index);
	};
}
