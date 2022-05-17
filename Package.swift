// swift-tools-version:5.5
// The swift-tools-version declares the minimum version of Swift required to build this package.
// https://developer.apple.com/documentation/xcode/creating_a_standalone_swift_package_with_xcode?language=objc

import PackageDescription

let BrightcovePlayerSDK = Package(
    name: "FreeWheel Plugin for Brightcove Player SDK",
    platforms: [
        .iOS(.v11), .tvOS(.v11)
    ],
    products: [
        .library(
            name: "BrightcoveFW",
            targets: ["BrightcoveFW"])
    ],
    targets: [
        .binaryTarget(
            name: "BrightcoveFW",
            path: "xcframework/BrightcoveFW.xcframework"
        )
    ]
)