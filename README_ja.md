# yup-ehl-design-module

YUPオーディオプラグイン向けの、EsionHsrahLatigid共通モノクロ8-bit UIプリミティブです。ヘッダオンリーで利用できます。

[English](./README.md)

## 謝辞

本モジュールは[YUP](https://github.com/kunitoki/yup)を利用しています。ビジュアル契約は、`EHL / Plugins / 8-bit UI Template`で管理するEHL共通モノクロプラグイン方針に基づきます。

## 機能

- EHLの4段階モノクロパレットを固定。
- 4pxグリッド上のコンパクトな`640x360`エディタ契約。
- 検証済みpath geometryから直接描画する、1色の正式EHL short mark。
- 時計回りのパラメーターインジケータをテストする角形ピクセルスライダー。
- 共通のコマンドボタン、セグメントメーター、ラベルスタイル、エディタ背景。
- ヘッダオンリーCMakeターゲット: `ehl::yup_plugin_ui`。
- macOSとWindowsの契約テスト。

固定されたビジュアル規則と互換性規則は[DESIGN_CONTRACT.md](./DESIGN_CONTRACT.md)を参照してください。

## 必要環境

- CMake 3.31以降。
- C++20コンパイラ。
- `yup::yup_gui`を提供するYUPリビジョン。

## インストール

Git submoduleとして追加します。

```sh
git submodule add https://github.com/EsionHsrahLatigid/yup-ehl-design-module.git external/yup-ehl-design-module
git submodule update --init --recursive
```

consumer側で`yup::yup_gui`ターゲットを作成した後に追加します。

```cmake
add_subdirectory(external/yup-ehl-design-module)
target_link_libraries(your_plugin_shared INTERFACE ehl::yup_plugin_ui)
```

## 最小API

```cpp
#include <ehl/yup_plugin_ui/EhlPluginTheme.h>

auto slider = std::make_unique<ehl::ui::PixelSlider> (yup::Slider::RotaryVerticalDrag);
auto meter = std::make_unique<ehl::ui::StripMeter> (ehl::ui::paper);
ehl::ui::paintShortLogo (graphics, { 528.0f, 12.0f, 96.0f, 36.0f });
```

`paintEditorBackground`は、コンパクトなヘッダー内へEHL short markを標準描画します。

安定APIは[DESIGN_CONTRACT.md](./DESIGN_CONTRACT.md)に記載しています。

## 開発

```sh
cmake --preset module-release
cmake --build --preset module-release --parallel
ctest --preset module-release --output-on-failure
```

consumer更新手順は[DEVELOPER_GUIDE.md](./DEVELOPER_GUIDE.md)を参照してください。

## 依存関係とライセンス

| 依存関係 | 用途 | ライセンス |
| --- | --- | --- |
| [YUP](https://github.com/kunitoki/yup) | GUIコンポーネントと描画API | ISC |

YUPは本リポジトリにvendorしていません。consumerプラグインが、本モジュール追加前にYUPを提供します。

## ライセンス

本プロジェクトは[MIT License](./LICENSE)で公開します。
