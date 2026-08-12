#pragma once

#include <yup_gui/yup_gui.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace ehl::ui
{

inline constexpr std::uint32_t ink = 0xff050505u;
inline constexpr std::uint32_t low = 0xff2a2a2au;
inline constexpr std::uint32_t mid = 0xff8a8a86u;
inline constexpr std::uint32_t paper = 0xfff2f2f0u;
inline constexpr std::uint32_t transparent = 0x00000000u;

inline constexpr yup::Size<int> preferredSize { 640, 360 };
inline constexpr float grid = 4.0f;
inline constexpr yup::Rectangle<float> shortLogoViewBox { 0.0f, 0.0f, 512.0f, 192.0f };
inline constexpr yup::Rectangle<float> headerLogoBounds { 528.0f, 12.0f, 96.0f, 36.0f };

// Canonical EHL short mark from assets/logos/white/logo-short.svg. Keeping the
// path in the header makes plugin rendering independent of bundle resource paths.
inline constexpr const char* shortLogoPathData =
    "M133.18,66.35 L133.18,40.94 L56.94,40.94 L56.94,66.35 Z "
    "M285.65,104.47 L285.65,118.32 L311.06,118.32 L311.06,104.47 L311.06,79.06 L285.65,79.06 L217.88,79.06 L217.88,24.00 L192.47,24.00 L192.47,79.06 L192.47,104.47 L192.47,142.59 L217.88,142.59 L217.88,111.55 L225.96,111.55 L225.96,107.06 L250.61,107.06 L250.61,104.47 Z "
    "M193.15,108.96 L193.15,101.76 L242.53,101.76 L242.53,104.35 L201.23,104.35 L201.23,108.96 Z "
    "M473.74,136.32 L473.74,142.59 L480.47,142.59 L480.47,117.18 L395.76,117.18 L395.76,53.52 L381.70,53.52 L381.70,49.20 L395.76,49.20 L395.76,24.00 L370.35,24.00 L370.35,117.18 L370.35,142.59 L395.76,142.59 L449.04,142.59 L449.04,136.32 Z "
    "M158.59,91.76 L158.59,66.35 L133.18,66.35 L133.18,69.36 L152.74,69.36 L152.74,73.68 L133.18,73.68 L133.18,91.76 L56.94,91.76 L56.94,73.68 L49.49,73.68 L49.49,69.36 L56.94,69.36 L56.94,66.35 L31.53,66.35 L31.53,91.76 L31.53,117.18 L31.53,142.59 L56.94,142.59 L56.94,117.18 L158.59,117.18 Z "
    "M285.65,142.59 L311.06,142.59 L311.06,122.64 L285.65,122.64 Z "
    "M158.59,142.59 L56.94,142.59 L56.94,168.00 L158.59,168.00 Z";

inline const yup::Path& shortLogoPath()
{
    static const auto path = []
    {
        yup::Path result;
        result.fromString (yup::String (shortLogoPathData));
        return result;
    }();

    return path;
}

inline void paintShortLogo (yup::Graphics& graphics,
                            const yup::Rectangle<float>& targetBounds,
                            std::uint32_t color = paper)
{
    if (targetBounds.isEmpty())
        return;

    const auto scale = std::min (targetBounds.getWidth() / shortLogoViewBox.getWidth(),
                                 targetBounds.getHeight() / shortLogoViewBox.getHeight());
    const auto width = shortLogoViewBox.getWidth() * scale;
    const auto height = shortLogoViewBox.getHeight() * scale;
    const auto x = targetBounds.getX() + (targetBounds.getWidth() - width) * 0.5f;
    const auto y = targetBounds.getY() + (targetBounds.getHeight() - height) * 0.5f;
    const auto transform = yup::AffineTransform::scaling (scale).translated (x, y);
    const auto savedState = graphics.saveState();

    graphics.setFillColor (yup::Color (color));
    graphics.addTransform (transform);
    graphics.fillPath (shortLogoPath());
}

struct IndicatorCell
{
    int x;
    int y;
};

inline constexpr std::array<IndicatorCell, 16> clockwiseIndicatorRing {
    IndicatorCell { 0, 4 },
    IndicatorCell { 0, 3 },
    IndicatorCell { 0, 2 },
    IndicatorCell { 0, 1 },
    IndicatorCell { 0, 0 },
    IndicatorCell { 1, 0 },
    IndicatorCell { 2, 0 },
    IndicatorCell { 3, 0 },
    IndicatorCell { 4, 0 },
    IndicatorCell { 4, 1 },
    IndicatorCell { 4, 2 },
    IndicatorCell { 4, 3 },
    IndicatorCell { 4, 4 },
    IndicatorCell { 3, 4 },
    IndicatorCell { 2, 4 },
    IndicatorCell { 1, 4 }
};

enum class TextRole
{
    primary,
    secondary
};

inline void styleLabel (yup::Label& label, TextRole role)
{
    label.setColor (yup::Label::Style::textFillColorId,
                    yup::Color (role == TextRole::primary ? paper : mid));
    label.setColor (yup::Label::Style::textStrokeColorId, yup::Color (transparent));
    label.setColor (yup::Label::Style::backgroundColorId, yup::Color (transparent));
    label.setColor (yup::Label::Style::outlineColorId, yup::Color (transparent));
}

class PixelSlider final : public yup::Slider
{
public:
    explicit PixelSlider (SliderType sliderType)
        : yup::Slider (sliderType)
    {
    }

    void paint (yup::Graphics& graphics) override
    {
        const auto enabled = isEnabled();
        const auto bounds = getLocalBounds().to<float>().reduced (grid);
        const auto cell = std::max (
            grid,
            std::floor (std::min (bounds.getWidth(), bounds.getHeight()) / (grid * 7.0f)) * grid);
        const auto side = cell * 7.0f;
        const auto frame = yup::Rectangle<float> {
            bounds.getCenterX() - side * 0.5f,
            bounds.getCenterY() - side * 0.5f,
            side,
            side
        };

        graphics.setFillColor (enabled ? low : ink);
        graphics.fillRect (frame);
        graphics.setStrokeColor (enabled ? (hasKeyboardFocus() || isMouseOver() ? paper : mid) : low);
        graphics.setStrokeWidth (enabled && hasKeyboardFocus() ? 2.0f : 1.0f);
        graphics.strokeRect (frame.reduced (1.0f));

        constexpr int segmentCount = static_cast<int> (clockwiseIndicatorRing.size());
        const auto activeSegments = std::clamp (
            static_cast<int> (std::round (getValueNormalised() * segmentCount)), 0, segmentCount);
        const auto originX = frame.getX() + cell;
        const auto originY = frame.getY() + cell;
        const auto block = std::max (2.0f, cell - 2.0f);

        for (int index = 0; index < segmentCount; ++index)
        {
            const auto indicatorCell = clockwiseIndicatorRing[static_cast<std::size_t> (index)];
            graphics.setFillColor (index < activeSegments ? (enabled ? paper : mid) : ink);
            graphics.fillRect (originX + indicatorCell.x * cell + 1.0f,
                               originY + indicatorCell.y * cell + 1.0f,
                               block,
                               block);
        }

        graphics.setFillColor (enabled ? (isCurrentlyBeingDragged() ? paper : mid) : low);
        graphics.fillRect (frame.getCenterX() - cell * 0.5f + 1.0f,
                           frame.getCenterY() - cell * 0.5f + 1.0f,
                           block,
                           block);
    }
};

class CommandButton final : public yup::TextButton
{
public:
    using yup::TextButton::TextButton;

    void setSelected (bool shouldBeSelected)
    {
        if (selected == shouldBeSelected)
            return;

        selected = shouldBeSelected;
        repaint();
    }

    void paintButton (yup::Graphics& graphics) override
    {
        const auto bounds = getLocalBounds().to<float>();
        const auto active = selected || isButtonDown();
        const auto over = isButtonOver();
        const auto enabled = isEnabled();

        graphics.setFillColor (active ? paper : (over ? mid : low));
        graphics.fillRect (bounds);
        graphics.setStrokeColor (enabled ? (hasKeyboardFocus() ? paper : mid) : low);
        graphics.setStrokeWidth (hasKeyboardFocus() ? 2.0f : 1.0f);
        graphics.strokeRect (bounds.reduced (1.0f));

        graphics.setFillColor (enabled ? (active || over ? ink : paper) : mid);
        graphics.fillFittedText (getStyledText(), getTextBounds());
    }

private:
    bool selected = false;
};

class StripMeter final : public yup::Component
{
public:
    explicit StripMeter (std::uint32_t activeColor)
        : color (activeColor)
    {
    }

    void setLevel (float newLevel)
    {
        level = std::clamp (newLevel, 0.0f, 1.0f);
        repaint();
    }

    void paint (yup::Graphics& graphics) override
    {
        const auto bounds = getLocalBounds().to<float>();
        graphics.setFillColor (ink);
        graphics.fillRect (bounds);

        constexpr int segmentCount = 24;
        const auto activeSegments = std::clamp (
            static_cast<int> (std::round (level * segmentCount)), 0, segmentCount);
        const auto segmentWidth = bounds.getWidth() / static_cast<float> (segmentCount);

        for (int index = 0; index < segmentCount; ++index)
        {
            graphics.setFillColor (index < activeSegments ? color : low);
            graphics.fillRect (bounds.getX() + index * segmentWidth,
                               bounds.getY(),
                               std::max (1.0f, segmentWidth - 2.0f),
                               bounds.getHeight());
        }
    }

private:
    std::uint32_t color = paper;
    float level = 0.0f;
};

inline void paintEditorBackground (yup::Graphics& graphics, float width, float height)
{
    graphics.setFillColor (ink);
    graphics.fillAll();

    graphics.setFillColor (low);
    for (int y = 64; y < static_cast<int> (height); y += 16)
        graphics.fillRect (0.0f, static_cast<float> (y), width, 1.0f);

    graphics.setFillColor (paper);
    graphics.fillRect (0.0f, 0.0f, width, grid);

    graphics.setFillColor (low);
    graphics.fillRect (0.0f, 64.0f, width, 48.0f);
    graphics.setFillColor (paper);
    graphics.fillRect (0.0f, 108.0f, width, grid);

    constexpr int columns = 7;
    constexpr float margin = 16.0f;
    constexpr float gap = 8.0f;
    constexpr float top = 128.0f;
    constexpr float bottom = 16.0f;
    const auto cellWidth = (width - 2.0f * margin - gap * (columns - 1)) / columns;
    const auto cellHeight = height - top - bottom;

    graphics.setStrokeColor (low);
    graphics.setStrokeWidth (1.0f);
    for (int column = 0; column < columns; ++column)
    {
        const auto x = margin + column * (cellWidth + gap);
        graphics.strokeRect (x, top, cellWidth, cellHeight);

        graphics.setFillColor (column % 2 == 0 ? mid : low);
        for (int bit = 0; bit <= column; ++bit)
            graphics.fillRect (x + grid + bit * (grid * 2.0f), height - 24.0f, grid, grid);
    }

    const auto scale = width / static_cast<float> (preferredSize.getWidth());
    paintShortLogo (graphics,
                    { headerLogoBounds.getX() * scale,
                      headerLogoBounds.getY() * scale,
                      headerLogoBounds.getWidth() * scale,
                      headerLogoBounds.getHeight() * scale });
}

} // namespace ehl::ui
