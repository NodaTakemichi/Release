#include <DxLib.h>
#include "Measure.h"

Measure* Measure::instance_ = nullptr;

void Measure::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Measure();
	}
	instance_->Init();
}

Measure& Measure::GetInstance(void)
{
	return *instance_;
}

void Measure::Init(void)
{
	initTime_ = std::chrono::system_clock::now();
	startTime_ = std::chrono::system_clock::now();
	preFrameTime_ = std::chrono::system_clock::now();
	cntTotalFrame_ = 0;
}

void Measure::Start(void)
{
	startTime_ = std::chrono::system_clock::now();
	totalTime_ =
		std::chrono::duration_cast<std::chrono::nanoseconds>(startTime_ - initTime_).count() / 1000000000.0;
	cntTotalFrame_++;

	auto deltaTime =
		std::chrono::duration_cast<std::chrono::nanoseconds>(startTime_ - preFrameTime_).count() / 1000000000.0;
	preFrameTime_ = startTime_;

	if (average_.size() >= AVG_FRAME)
	{
		average_.erase(average_.begin());
	}
	average_.push_back(deltaTime);

}

void Measure::Watch(std::string name)
{

	if (watchTimes_.count(name) == 0)
	{
		watchTimes_.emplace(name, 0.0);
	}
	else
	{
		auto nowTime = std::chrono::system_clock::now();
		auto deltaTime = 
			std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - startTime_).count() / 1000000000.0;
		watchTimes_[name] += deltaTime;
		startTime_ = nowTime;
	}

}

void Measure::Draw(void)
{

#ifdef _DEBUG

	if (IS_DISABLE)
	{
		return;
	}

	const int HEIGHT = 20;
	int y = 0;

	y += HEIGHT;
	DrawFormatString(0, y, 0xffffff, "%-15s : %.3f", "é¿çséûä‘", totalTime_);

	y += HEIGHT;
	DrawFormatString(0, y, 0xffffff, "%-15s : %d", "é¿çsÉtÉåÅ[ÉÄ", cntTotalFrame_);

	double avg = 0.0;
	for (auto d : average_)
	{
		avg += d;
	}
	double fps = 1.0 / (avg / static_cast<double>(AVG_FRAME));

	y += HEIGHT;
	DrawFormatString(0, y, 0xffffff, "%-15s : %.3f", "ïΩãœÇeÇoÇr", fps);

	for (const auto& watch : watchTimes_)
	{
		y += HEIGHT;
		DrawFormatString(0, y, 0xffffff, "%-15s : %.3f", watch.first.c_str(), watch.second);
	}

#endif // _DEBUG

}

void Measure::Reset(void)
{
	watchTimes_.clear();
}

Measure::Measure(void)
{
	initTime_ = std::chrono::system_clock::now();
	cntTotalFrame_ = 0;
	totalTime_ = 0.0;
}

Measure::~Measure(void)
{
	delete instance_;
}
