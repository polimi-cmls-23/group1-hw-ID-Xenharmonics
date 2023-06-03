/*
  ==============================================================================

    Meter.h
    Created: 27 Apr 2022 11:23:21am
    Author:  PC

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#define FPS       25
#define DB_FLOOR -48.0f
#define RT         0.5f

class Meter : public Component, public Timer
{
public:
    Meter()
    {
        alpha = exp(-1.0f / (FPS * RT));
        startTimerHz(FPS);
    }

    ~Meter()
    {
        stopTimer();
    }

    void paint(Graphics& g) override
    {
        auto W = getWidth();
        auto H = getHeight();

        g.fillAll(Colours::black);
        g.setColour(Colours::grey);
        g.drawRect(0, 0, W, H, 1);

        if (observedEnvelope != nullptr)
        {
            auto envelopeSnapshot = observedEnvelope->get();
            observedEnvelope->set(envelopeSnapshot * alpha);

            auto peak = Decibels::gainToDecibels(envelopeSnapshot);

            auto barHeight = jmap(peak, DB_FLOOR, 0.0f, 0.0f, H - 2.0f);
            barHeight = jlimit(0.0f, H - 2.0f, barHeight);

			auto topColour = peak >= 0.0f ? juce::Colour(0xfff9ac53) : juce::Colour(0xfff95e53);
            ColourGradient filler = ColourGradient(juce::Colour(0xffcb6ce6), 0, H, topColour, 0, 0, false);
            filler.addColour(0.8f, juce::Colour(0xFF5271FF));

            g.setGradientFill(filler);

            g.fillRect(1.0f, H - 1.0f - barHeight, W - 2.0f, barHeight);
        }

    }

    void resized() override {}

    void connectTo(Atomic<float>& targetVariable)
    {
        observedEnvelope = &targetVariable;
    }

private:

    void timerCallback() override
    {
        repaint();
    }

    float alpha = 0.0f;

    Atomic<float>* observedEnvelope = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Meter)
};


#define BORDER_WIDTH 1.5f
#define SCALE_TO_KNOB_RATIO 0.85f

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
	MyLookAndFeel()
	{

	}

	void drawTickBox(Graphics& g, Component& component,
		float x, float y, float w, float h,
		const bool ticked,
		const bool isEnabled,
		const bool shouldDrawButtonAsHighlighted,
		const bool shouldDrawButtonAsDown) override
	{
		ignoreUnused(isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

		Rectangle<float> tickBounds(x, y, w, h);

		g.setColour(juce::Colour(0xfff9ac53));
		g.drawRoundedRectangle(tickBounds, 4.0f, 1.6f);

		if (ticked)
		{
			g.setColour(juce::Colour(0xffcb6ce6));
			auto tick = getTickShape(0.75f);
			g.fillPath(tick, tick.getTransformToScaleToFit(tickBounds.reduced(4, 5).toFloat(), false));
		}
	}

	void drawRotarySlider(juce::Graphics& g,
		int x, int y, int width, int height,
		float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
		juce::Slider& slider) override
	{
		// Useful variables
		const auto radius = juce::jmin(width, height) * 0.5f * SCALE_TO_KNOB_RATIO - BORDER_WIDTH * 0.5f;
		const auto centreX = x + width * 0.5f;
		const auto centreY = y + height * 0.5f;
		const auto rx = centreX - radius;
		const auto ry = centreY - radius;
		const auto rw = radius * 2.0f;

		const juce::Colour brightColour = juce::Colour(0xfff9ac53);
		const juce::Colour darkColour = juce::Colour(0xff9400d3);

		// Draw knob body
		g.setGradientFill(juce::ColourGradient(brightColour, rx, ry, darkColour.brighter(0.05), rx, ry + rw, false));
		g.fillEllipse(rx, ry, rw, rw);

		// Draw outline
		g.setGradientFill(juce::ColourGradient(brightColour.brighter(0.9), rx, ry, darkColour.darker(0.9), rx, ry + rw, false));
		g.drawEllipse(rx, ry, rw, rw, BORDER_WIDTH);

		// Prepare pointer shape
		juce::Path p;
		const auto pointerLength = radius * 0.33f;
		const auto pointerThickness = 2.0f;
		p.addRectangle(pointerThickness * -0.5f, -radius, pointerThickness, pointerLength);

		// Draw pointer shape
		const auto angle = juce::jmap(sliderPosProportional, rotaryStartAngle, rotaryEndAngle);
		p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
		g.setGradientFill(juce::ColourGradient(juce::Colours::white, rx, ry, juce::Colours::white.darker(0.2f), rx, ry + rw, false));
		g.fillPath(p);

		// Draw ticks
		juce::Path originalTickShape, t;
		auto tickThickness = 2.0f;
		originalTickShape.addEllipse(tickThickness * -0.5f, radius / -SCALE_TO_KNOB_RATIO, tickThickness, tickThickness);
		g.setColour(juce::Colours::grey);

		for (int i = 0; i < numTicks; ++i)
		{
			t = originalTickShape;
			auto normValue = pow((float)i / (numTicks - 1), (float)slider.getSkewFactor());
			float angle = juce::jmap(normValue, rotaryStartAngle, rotaryEndAngle);
			t.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
			g.fillPath(t);
		}

	}

	void setNumTicks(int newValue)
	{
		numTicks = newValue;
	}

private:

	int numTicks = 21;
};


class MyLinearSlider : public LookAndFeel_V4
{
public:
	MyLinearSlider()
	{
		//setColour(Slider::thumbColourId, Colours::red);
	}


	void drawLinearSlider(Graphics& g,
		int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const juce::Slider::SliderStyle, Slider& slider) override
	{
		g.setColour(juce::Colour(0xfff9ac53));
		Rectangle<int> rint(x, y, width, height);

		Rectangle<float> r = rint.toFloat().reduced(0.5f);
		g.drawRoundedRectangle(r, 5, 1);

		g.setColour(juce::Colour(0xffcb6ce6));
		Rectangle<int> pint(x + 3.9f, y + 3.f, width - 7.f, height - 7.f);

		Rectangle<float> p = pint.toFloat().reduced(0.5f);
		g.drawRoundedRectangle(p, 5, 1);

		g.setColour(juce::Colour(0xffcb6ce6));

		float radius = (r.getWidth() - 1) / 4.f;

		g.drawLine(r.getX() + r.getWidth() / 2.f,
			r.getY() + radius,
			r.getX() + r.getWidth() / 2.f,
			r.getHeight() - radius*0.5f);

		auto l = (r.getHeight() - radius) - (r.getY() + radius);
		float originalPos = sliderPos / (float)height;

		sliderPos = l * originalPos; 
		g.setColour(juce::Colour(0xfff9ac53));

		g.fillEllipse(x + width * 0.5f - radius*0.25f,
			sliderPos + radius + 4.f,
			radius * 0.5f,
			radius * 0.5f);

	}

};

class MyHorizontalSlider : public LookAndFeel_V4
{
public:
	MyHorizontalSlider()
	{
		//setColour(Slider::thumbColourId, Colours::red);
	}


	void drawLinearSlider(Graphics& g,
		int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const juce::Slider::SliderStyle, Slider& slider) override
	{

		g.setColour(juce::Colour(0xfff9ac53));
		Rectangle<int> rint(x, y, width, height - 2.f);

		Rectangle<float> r = rint.toFloat().reduced(0.5f);
		g.drawRoundedRectangle(r, 5, 1);

		g.setColour(juce::Colour(0xffcb6ce6));
		Rectangle<int> pint(x+2.5f, y+3.f, width-5.f, height - 8.f);

		Rectangle<float> p = pint.toFloat().reduced(0.5f);
		g.drawRoundedRectangle(p, 5, 1);

		g.setColour(juce::Colour(0xffcb6ce6));

		float radius = (r.getWidth() - 1) / 4.f;
		float border = r.getWidth() / 10.f;

		g.drawLine(r.getX() + border,
			r.getY() + r.getHeight() / 2.f,
			r.getX() + r.getWidth() - border,
			r.getY() + r.getHeight() / 2.f);

		auto l = (r.getWidth() - 2.f * border) ;
		float originalPos = sliderPos / (float)r.getWidth();

		sliderPos = l * originalPos; 
		g.setColour(juce::Colour(0xfff9ac53));

		g.fillEllipse(sliderPos + radius * 0.25f,
			r.getY() + r.getHeight() / 2.f - radius*0.5f/2.f,
			radius * 0.5f,
			radius * 0.5f);

	}

};


class CustomButton : public LookAndFeel_V4
{
public:
	CustomButton()
	{
		//setColour(Slider::thumbColourId, Colours::red);
	}

	void drawLinearSlider(Graphics& g,
		int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const juce::Slider::SliderStyle, Slider& slider) override
	{

		g.setColour(Colours::whitesmoke);
		Rectangle<int> rint(x, y, width, height - 2.f);

		Rectangle<float> r = rint.toFloat().reduced(0.5f);
		g.drawRoundedRectangle(r, 5, 1);

		g.setColour(Colours::white);

		float radius = (r.getWidth() - 1) / 4.f;
		float border = r.getWidth() / 10.f;

		g.drawLine(r.getX() + border,
			r.getY() + r.getHeight() / 2.f,
			r.getX() + r.getWidth() - border,
			r.getY() + r.getHeight() / 2.f);

		auto l = (r.getWidth() - 2.f * border);
		float originalPos = sliderPos / (float)r.getWidth();

		sliderPos = l * originalPos; 
		g.setColour(Colours::whitesmoke);

		g.fillEllipse(sliderPos + radius * 0.25f,
			r.getY() + r.getHeight() / 2.f - radius * 0.5f / 2.f,
			radius * 0.5f,
			radius * 0.5f);

	}

};


class MyIncDecSlider : public juce::LookAndFeel_V4
{
public:
	MyIncDecSlider()
	{

	}

	void drawButtonBackground(juce::Graphics& g, juce::Button& b, const juce::Colour&, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
	{
		auto buttonArea = b.getLocalBounds();
		juce::Rectangle<float> r = buttonArea.toFloat();

		g.setColour(juce::Colours::whitesmoke);
		juce::Rectangle<float> round = r.toFloat().reduced(0.8f);
		g.drawRoundedRectangle(round, 5.0f, 1);

		const juce::Colour brightColour = juce::Colour(0xfff9ac53);
		const juce::Colour darkColour = juce::Colour(0xff9400d3);

		if (shouldDrawButtonAsDown)
		{
			g.setGradientFill(juce::ColourGradient(brightColour, 150, 0, darkColour.brighter(0.02), 50, 50, false));
			g.fillRoundedRectangle(r, 5.0f);

		}
		else if (shouldDrawButtonAsHighlighted)
		{
			g.setGradientFill(juce::ColourGradient(brightColour, 60, 0, darkColour.brighter(0.04), 50, 50, false));
			g.fillRoundedRectangle(r, 5.0f);
		}
		else
		{
			g.setGradientFill(juce::ColourGradient(brightColour, 70, 0, darkColour.brighter(0.06), 50, 50, false));
			g.fillRoundedRectangle(r, 5.0f);
		}
	}
};