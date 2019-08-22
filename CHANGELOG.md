# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [v1.4 - 2019-08-22 20:00]
### Added
- Support for the `bme680` environmental sensor.
- Support for the `bhi160` sensor fusion.
- simple_menu` module for creating simple menus in Python.
- Python power module to access the voltage and current measurements from the PMIC.

### Changed
- Refactored BLE card10 service.
- Improved BLE file-transfer.
- Replaced dynamic attribute creation with static attributes.
- Clock: Added multiple color themes for the main clock.
- Clock: Color themes are read from a json file, so people can customize their clock
- Clock: Last selected theme is saved in the clock.json so it's persistent


### Fixed
- Fixed menu listing files starting with `.`.
- Fixed `utime.set_time()` applying the timezone offset in the wrong direction.
- Fixed the PMIC driver not releasing some locks properly.

## [v1.3] - 2019-08-22 00:12

```text
e17e31cd feat(preload): update nick name preloads
65efd6bc Merge branch 'display_multiline_fix' into 'master'
d29f8f95 Merge branch 'ble-file-transfer-dir' into 'master'
00578959 BLE: FileTrans: Use LOG_ERR
2a542de3 BLE: FileTrans: Null terminate the string
7cef2115 BLE: FileTrans: Create a directory if needed
638ab1d5 docs: Update instructions for dependencies on Arch
d079e3df feat: Add requirements.txt
50eeecd9 style(ledfx): Reformat docs and comments
ab8ee07f docs: Fix personal_state documentation
5083858f docs: Fix os documentation
19adf1f3 feat(pycardium): Add os.urandom()
3a6507e3 fix(splashscreen): Adapt coding style
00108bca fix(splashscreen): More useful message
f97b1e53 fix(splashscreen): Print version correctly
1427ed31 fix(splashscreen): Reduce to version
13ad01e8 fix(splashscreen): Display useful version info
d65034fe feat(splashscreen): Add tag/commit to epicardium splashscreen
c3e34289 Added blacklist for write access for certain file names.
9a2e9263 Add requirements.txt file to install working version of python packages
c7494640 ble.py: Show MAC suffix
ec611574 fix(ble-uart): Prevent buffer overflow
f189fff0 I hate embedded string manipulation.
2766e2f1 During startup of the BLE, the advert name is pulled off the random bytes of the mac in mac.txt.
7df76031 fix(maxusb): Disable confusing message
2e547c66 fix(serial-task): increase stack size
7f938908 Update instructions for building on Arch
73876f4b Merge branch 'display_multiline_fix' of https://git.card10.badge.events.ccc.de/omniskop/firmware into display_multiline_fix
3e272a88 Merge remote-tracking branch 'original/master' into display_multiline_fix
4edea9c1 fixed code style
b0c748be fix(gfx): Break line before char is printed
e287bb4d fixed code style
52829e8b fix(gfx): Break line before char is printed
```

## [v1.2] - 2019-08-21 18:18

```text
8e8d8614 feat(apps): Add scope to preload
e1a7684a fix(cdcacm): Disable before printing error
4c74f061 fix(utime.c): set_time should operate in local tz
e0824843 feat(pmic): Switch off if battery is low
46ef3985 feat(pmic): Add API-call to read battery voltage
79e2fb15 feat(epicardium): Periodically check the battery voltage
5da9e074 feat(pmic): Implement AMUX reading
8c59935e py: timezone workaround
c7f59d3e fix(text_reader): Convert to unix line-endings
78a7a7f4 docs: Fix underlines in ble/card10
15649293 feat(app): Add some preloads
b12e4ef9 chore(docs): Fix utime module docs
3efbab13 feat(utime.c): add python functions to set time
38f83243 chore(docs): Fix color documentation
a966e221 chore(docs): Fix python-directives with double-module
66cd10d4 docs: Document os.reset()
5fe5fe31 docs: Document pride module
338132e5 apped apps folder to search module search path
cda91555 rename Main Clock to Home
c2935c8c fixed syntax
3017591a Rename preloaded apps to make use of hatchery folder structure
842e9ad8 feat(menu.py): support scrolling for long menu entries
fbf7c8c0 fix(menu.py) Refactored menu.py based on !138
8aa8c31f feat(ble): Store bondings
5e5c7a4f fix(menu.py): Fix color-mismatch of selector background
```

## [v1.1] - 2019-08-21 03:14
### Added
- Seed ``urandom`` PRNG with ``TRNG`` peripheral.
- Show linenumbers in MicroPython tracebacks.

### Fixed
- **buttons**: Acquire lock before accessing I2C.
- **rtc**: Fix RTC getting stuck because of improper initialization.
- Make lifecycle task more important than dispatcher.

## [v1.0] - 2019-08-21 00:50
Initial release.

[Unreleased]: https://git.card10.badge.events.ccc.de/card10/firmware/compare/v1.3...master
[v1.3]: https://git.card10.badge.events.ccc.de/card10/firmware/compare/v1.2...v1.3
[v1.2]: https://git.card10.badge.events.ccc.de/card10/firmware/compare/v1.1...v1.2
[v1.1]: https://git.card10.badge.events.ccc.de/card10/firmware/compare/v1.0...v1.1
[v1.0]: https://git.card10.badge.events.ccc.de/card10/firmware/-/tags/v1.0
