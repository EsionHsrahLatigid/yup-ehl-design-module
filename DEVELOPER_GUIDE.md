# Developer guide

## Local verification

When a sibling YUP checkout exists at `../yup`:

```sh
cmake --preset module-release
cmake --build --preset module-release --parallel
ctest --preset module-release --output-on-failure
```

Without a sibling checkout, configuration fetches the pinned YUP revision used by the EHL plugin repositories.

## Consumer update workflow

1. Change the module and run its contract tests on macOS and Windows.
2. Commit and push the module.
3. Update each plugin's `external/yup-ehl-design-module` Gitlink to that exact commit.
4. Build and test every plugin's `plugin-release` preset.
5. Verify staged Standalone, VST3, and AU bundles on macOS and Standalone/VST3 bundles on Windows.
6. Commit each consumer update separately.

When the canonical logo changes, update both `assets/logos/white/logo-short.svg` and
`shortLogoPathData`. The contract test intentionally fails if their path data diverges.

Do not add a local wrapper or copy of `EhlPluginTheme.h` to a consumer. The Gitlink must remain the only source of shared UI implementation.
