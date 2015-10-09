# 1.1.0
### Breaking Changes
* **Important: Please read "Playing and Pausing" section of README.md**
* We now wait to call the ad Context policy until the duration of the AVPlayer is known. Previous to this change, we would call the block as soon as we could, which may have included a valid duration. Although documented, this was confusing, and probably not what you would expect. Although we do not recommend it, if you need the old behavior, please see the README.md on how to disable `waitForAVPlayerDuration`.

### Additions and Improvements
* Updated README.md for more information about plugin customization.
* Add the ability to preload slots. See README.md for more information.
* Added `session.providerExtentionisPausedOnFreewheelsRequest`, `-[session.providerExtention fw_pause]` and `-[session.providerExtention fw_play]`. See README.md for more information.

# 1.0.4
### Additions and Improvements
* Internal Improvements.
* You must use the Brightcove Player SDK for iOS version 4.4.0+.

# 1.0.3
### Breaking Changes
* iOS 6 is still deprecated in this release. We have not removed support yet.  iOS 6.x currently accounts for ~2% of global SDK traffic.
* This release has been built with Xcode 6. In Xcode 6, Apple removed armv7s from the list of standard architectures. This release no longer includes an armv7s architecture slice.

### Additions and Improvements
* Internal Improvements

# 1.0.2

### Breaking Changes
* Use of ReactiveCocoa in public APIs within the BCOVW Plugin has been removed in this release.

### Additions and Improvements
* This release is required for compatibility with Brightcove Player SDK for iOS v4.2.0, but contains no outward-facing functionality changes or bug fixes.

# 1.0.1

### Breaking Changes
* Use of ReactiveCocoa in public APIs within the BCOVW Plugin is deprecated in this release. Version 1.1.0 will not require clients to install any version of ReactiveCocoa.

### Additions and Improvements
* This release is required for compatibility with Brightcove Player SDK for iOS v4.0.8, but contains no outward-facing functionality changes or bug fixes.
