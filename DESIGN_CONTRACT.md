# EHL YUP Plugin UI Design Contract

This repository is the single source of truth for reusable EsionHsrahLatigid YUP plugin UI primitives.

## Locked profile

- Profile: `yup-plugin`.
- Logical editor canvas: `640x360`.
- Base grid: 4 logical pixels; major group spacing: 8 logical pixels.
- Palette:
  - `ink`: `#050505`
  - `low`: `#2A2A2A`
  - `mid`: `#8A8A86`
  - `paper`: `#F2F2F0`
- Controls use square, integer-aligned, quantized geometry.
- Operational text must remain clean and legible.
- No chromatic accents, gradients, glow, RGB split, fake hardware, rounded panels, or decorative noise over operational content.

## Logo contract

- The compact editor header uses the canonical one-color `ehl` short mark, not replacement text or a waveform motif.
- The source asset is `assets/logos/white/logo-short.svg`; its outlined path is embedded for resource-independent rendering in Standalone, VST3, and AU bundles.
- The mark occupies the `96x36` logical-pixel area at `(528, 12)` on the `640x360` canvas and scales with the editor width.
- The logo is rendered in `paper` on `ink`. Product titles and operational labels remain separate and undamaged.

## Parameter indicator direction

Active segments advance clockwise in screen coordinates. The path starts at the lower-left cell, rises along the left edge, moves right along the top edge, descends along the right edge, and returns left along the bottom edge.

The exact path is exposed as `ehl::ui::clockwiseIndicatorRing` and protected by contract tests.

## Public compatibility surface

The following names form the supported API:

- Palette tokens: `ink`, `low`, `mid`, `paper`, `transparent`.
- Layout tokens: `preferredSize`, `grid`, `clockwiseIndicatorRing`, `shortLogoViewBox`, `headerLogoBounds`.
- Types: `IndicatorCell`, `TextRole`, `PixelSlider`, `CommandButton`, `StripMeter`.
- Functions: `styleLabel`, `shortLogoPath`, `paintShortLogo`, `paintEditorBackground`.
- Namespace: `ehl::ui`.
- CMake target: `ehl::yup_plugin_ui`.

Changing the palette, editor footprint, logo geometry/placement, indicator direction, namespace, target name, or public component names requires a coordinated consumer migration and a new major version.

## Product boundaries

Product identity, warning copy, parameter semantics, DSP behavior, and product-specific editor layout remain in each plugin repository. This module owns only reusable visual tokens and primitives.

The canonical reusable Canva direction is `EHL / Plugins / 8-bit UI Template` (`DAHSB2E1xQE`). Canva is a direction source; the tested C++ module is the production implementation source.
