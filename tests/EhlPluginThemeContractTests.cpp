#include <ehl/yup_plugin_ui/EhlPluginTheme.h>

#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

namespace
{

int failures = 0;

void expect (bool condition, const char* message)
{
    if (condition)
        return;

    std::cerr << "FAILED: " << message << '\n';
    ++failures;
}

} // namespace

int main()
{
    expect (ehl::ui::ink == 0xff050505u, "ink token");
    expect (ehl::ui::low == 0xff2a2a2au, "low token");
    expect (ehl::ui::mid == 0xff8a8a86u, "mid token");
    expect (ehl::ui::paper == 0xfff2f2f0u, "paper token");
    expect (ehl::ui::transparent == 0x00000000u, "transparent token");
    expect (ehl::ui::preferredSize.getWidth() == 640, "preferred width");
    expect (ehl::ui::preferredSize.getHeight() == 360, "preferred height");
    expect (ehl::ui::grid == 4.0f, "base grid");
    expect (ehl::ui::shortLogoViewBox.getWidth() == 512.0f, "short logo viewBox width");
    expect (ehl::ui::shortLogoViewBox.getHeight() == 192.0f, "short logo viewBox height");
    expect (ehl::ui::headerLogoBounds.getWidth() == 96.0f, "header logo width");
    expect (ehl::ui::headerLogoBounds.getHeight() == 36.0f, "header logo height");

    const auto logoBounds = ehl::ui::shortLogoPath().getBounds();
    expect (! logoBounds.isEmpty(), "short logo path parses");
    expect (logoBounds.getX() >= 0.0f && logoBounds.getRight() <= 512.0f,
            "short logo path stays inside horizontal viewBox");
    expect (logoBounds.getY() >= 0.0f && logoBounds.getBottom() <= 192.0f,
            "short logo path stays inside vertical viewBox");

    std::ifstream logoFile { std::string { EHL_YUP_PLUGIN_UI_SOURCE_DIR }
                             + "/assets/logos/white/logo-short.svg" };
    std::ostringstream logoBuffer;
    logoBuffer << logoFile.rdbuf();
    const auto logoSvg = logoBuffer.str();
    expect (logoFile.good() || logoFile.eof(), "canonical short logo SVG is readable");
    expect (logoSvg.find (ehl::ui::shortLogoPathData) != std::string::npos,
            "embedded path matches canonical short logo SVG");
    expect (logoSvg.find ("<text") == std::string::npos, "canonical short logo contains no text element");
    expect (logoSvg.find ("<image") == std::string::npos, "canonical short logo contains no raster image");
    expect (logoSvg.find ("<filter") == std::string::npos, "canonical short logo contains no filter");

    constexpr std::array<ehl::ui::IndicatorCell, 16> expectedRing {
        ehl::ui::IndicatorCell { 0, 4 },
        ehl::ui::IndicatorCell { 0, 3 },
        ehl::ui::IndicatorCell { 0, 2 },
        ehl::ui::IndicatorCell { 0, 1 },
        ehl::ui::IndicatorCell { 0, 0 },
        ehl::ui::IndicatorCell { 1, 0 },
        ehl::ui::IndicatorCell { 2, 0 },
        ehl::ui::IndicatorCell { 3, 0 },
        ehl::ui::IndicatorCell { 4, 0 },
        ehl::ui::IndicatorCell { 4, 1 },
        ehl::ui::IndicatorCell { 4, 2 },
        ehl::ui::IndicatorCell { 4, 3 },
        ehl::ui::IndicatorCell { 4, 4 },
        ehl::ui::IndicatorCell { 3, 4 },
        ehl::ui::IndicatorCell { 2, 4 },
        ehl::ui::IndicatorCell { 1, 4 }
    };

    for (std::size_t index = 0; index < expectedRing.size(); ++index)
    {
        expect (ehl::ui::clockwiseIndicatorRing[index].x == expectedRing[index].x,
                "clockwise indicator x coordinate");
        expect (ehl::ui::clockwiseIndicatorRing[index].y == expectedRing[index].y,
                "clockwise indicator y coordinate");
    }

    static_assert (std::is_base_of_v<yup::Slider, ehl::ui::PixelSlider>);
    static_assert (std::is_base_of_v<yup::TextButton, ehl::ui::CommandButton>);
    static_assert (std::is_base_of_v<yup::Component, ehl::ui::StripMeter>);

    if (failures != 0)
        return 1;

    std::cout << "EHL YUP plugin UI contract passed\n";
    return 0;
}
