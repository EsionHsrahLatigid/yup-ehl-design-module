# yup-ehl-design-module

Shared, header-only EsionHsrahLatigid monochrome 8-bit UI primitives for YUP audio plugins.

[日本語](./README_ja.md)

## Acknowledgements

This module builds on [YUP](https://github.com/kunitoki/yup). The visual contract follows the reusable EHL monochrome plugin direction maintained in `EHL / Plugins / 8-bit UI Template`.

## Features

- Fixed EHL four-level monochrome palette.
- Compact `640x360` editor contract on a 4 px grid.
- Square pixel slider with a tested clockwise parameter indicator.
- Reusable command button, segmented meter, label styling, and editor background.
- Header-only CMake target: `ehl::yup_plugin_ui`.
- Contract tests on macOS and Windows.

See [DESIGN_CONTRACT.md](./DESIGN_CONTRACT.md) for the locked visual and compatibility rules.

## Requirements

- CMake 3.31 or newer.
- C++20 compiler.
- A YUP revision that provides `yup::yup_gui`.

## Installation

Add the repository as a Git submodule:

```sh
git submodule add https://github.com/EsionHsrahLatigid/yup-ehl-design-module.git external/yup-ehl-design-module
git submodule update --init --recursive
```

After the consumer has created the `yup::yup_gui` target:

```cmake
add_subdirectory(external/yup-ehl-design-module)
target_link_libraries(your_plugin_shared INTERFACE ehl::yup_plugin_ui)
```

## Minimal API

```cpp
#include <ehl/yup_plugin_ui/EhlPluginTheme.h>

auto slider = std::make_unique<ehl::ui::PixelSlider> (yup::Slider::RotaryVerticalDrag);
auto meter = std::make_unique<ehl::ui::StripMeter> (ehl::ui::paper);
```

The stable surface is documented in [DESIGN_CONTRACT.md](./DESIGN_CONTRACT.md).

## Development

```sh
cmake --preset module-release
cmake --build --preset module-release --parallel
ctest --preset module-release --output-on-failure
```

See [DEVELOPER_GUIDE.md](./DEVELOPER_GUIDE.md) for the consumer update procedure.

## Dependencies and licenses

| Dependency | Purpose | License |
| --- | --- | --- |
| [YUP](https://github.com/kunitoki/yup) | GUI component and drawing API | ISC |

YUP is not vendored by this repository. Consumer plugins provide it before adding this module.

## License

This project is available under the [MIT License](./LICENSE).
