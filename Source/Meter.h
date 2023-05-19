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
#define RT        0.5f

#define BORDER_WIDTH 1.5f
#define SCALE_TO_KNOB_RATIO 0.85f

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
	MyLookAndFeel()
	{
		
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

		const juce::Colour brightColour = juce::Colour(0xff808080);
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

class MyIncDecSlider : public juce::LookAndFeel_V4
{
public:
	MyIncDecSlider()
	{
		
	}

	void drawButtonBackground(juce::Graphics& g, juce::Button& b, const juce::Colour&, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
	{
		auto buttonArea = b.getLocalBounds();
		juce::Colour backColour = juce::Colour(0xff9400d3);
		juce::Rectangle<float> r = buttonArea.toFloat();


		g.setColour(juce::Colours::whitesmoke);
		juce::Rectangle<float> round = r.toFloat().reduced(0.8f);
		g.drawRoundedRectangle(round, 5.0f, 1);

		if (shouldDrawButtonAsDown) 
		{ 
			g.setColour(backColour.brighter(0.2));
			g.fillRoundedRectangle(r, 5.0f);

		}
		else if (shouldDrawButtonAsHighlighted) 
		{ 
			g.setColour(backColour.brighter(0.4));
			g.fillRoundedRectangle(r, 5.0f);
		}
		else 
		{
			g.setColour(backColour.brighter(0.6));
			g.fillRoundedRectangle(r, 5.0f);
		}
	}
};