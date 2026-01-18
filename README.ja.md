# ビルド可能なLibinfo

🇬🇧 [English](README.md)

これはAppleがオープンソースソフトウェアとして公開している [Darwin](https://opensource.apple.com/releases/) の一部である [Libinfo](https://github.com/apple-oss-distributions/Libinfo)です。AppleのOSSは内部SDKと呼ばれる非公開のプラットホーム上でビルドされることを前提としているため、多くの場合そのままでは通常のmacOS環境でビルドできません。ビルドするためには欠けているヘッダファイルをかき集めたり、ソースコードを修正する必要があります。

このリポジトリはそれらの作業を行い、通常のmacOS上でビルド可能に改変したものです。

## バージョン

使用しているソフトウェアのバージョンは macOS 26.2 に準拠しています。

```
Libinfo-repo/
├── Libinfo-600/            # https://github.com/apple-oss-distributions/Libinfo/archive/Libinfo-600.tar.gz
│   ├── AdditionalHeaders/  # 追加のスタブヘッダー
│   ├── Libinfo.xcodeproj
│   ├── dns.subproj/
│   ├── gen.subproj/
│   ├── lookup.subproj/
│   ├── membership.subproj/
│   ├── nis.subproj/
│   ├── rpc.subproj/
│   ├── util.subproj/
│   └── xcodescripts/
│
└── Supplements/           # 依存関係解決のためのソース
    ├── apple_internal_sdk/      # https://github.com/Torrekie/apple_internal_sdk
    ├── CoreOSMakefiles/         # https://github.com/apple-oss-distributions/CoreOSMakefiles/tree/CoreOSMakefiles-79
    ├── Libc-1725.40.4/          # https://github.com/apple-oss-distributions/Libc/archive/Libc-1725.40.4.tar.gz
    ├── Libsystem-1356/          # https://github.com/apple-oss-distributions/Libsystem/archive/Libsystem-1356.tar.gz
    ├── configd-1385.60.3/       # https://github.com/apple-oss-distributions/configd/archive/configd-1385.60.3.tar.gz
    ├── launchd-main/            # https://github.com/apple-oss-distributions/launchd
    ├── libdispatch-1542.0.4/    # https://github.com/apple-oss-distributions/libdispatch/archive/libdispatch-1542.0.4.tar.gz
    ├── mDNSResponder-2881.60.4/ # https://github.com/apple-oss-distributions/mDNSResponder/archive/mDNSResponder-2881.60.4.tar.gz
    └── xnu-12377.61.12/         # https://github.com/apple-oss-distributions/xnu/archive/xnu-12377.61.12.tar.gz
```

## ビルド

### ビルド環境

- **macOS** + **Xcode (Command Line Tools)**

ビルド成功を確認しているのは当方の環境(macOS 15.7.3/Xcode 26.2)でのみですが、相当古いバージョンの macOS/Xcode でもビルド可能だと思います。

### ビルド方法

#### コマンドラインでビルド

```bash
cd Libinfo-600
xcodebuild -project Libinfo.xcodeproj -configuration Debug -target Libinfo clean build
```

### ビルド成果物

ビルドに成功すると、build/Debugに以下のバリアントが生成されます:

- libsystem_info.dylib
- libsystem_info_debug.dylib
- libsystem_info_profile.dylib

## デバッグ方法

基本的には環境変数 `DYLD_LIBRARY_PATH` を設定し、正規の libsystem_info.dylib の代わりにビルドしたものをダイナミックリンクさせる方法で行います。

### コマンドラインでデバッグ

Libinfoをリンクしたバイナリを用意して以下の要領で行います。

```bash
DYLD_LIBRARY_PATH=./build/Debug lldb ./testbinary
```

### GUIでデバッグ

- Xcode で macOS Command Line Toolプロジェクトを作成します。
- [上部メニューバー] - [Product] - [Scheme] - [Edit Scheme] を開きます(または ⌘ <)。
  - 表示された設定ウインドウの左側メニューから [Run] を選択します。
  - 設定ウインドウ右側上部から [Arguments] を選択します。
  - [Environmental Variables] 欄の Name に `DYLD_LIBRARY_PATH` 、Value に `build/Debugへのフルパス` を設定します。
  - Close ボタンをクリックします。
- プロジェクトにデバッグ用コードを追加します。
- ビルド(⌘ B)が成功したら、適当な場所にブレークポイントを設定して Run(⌘ R) します。

## 技術的な解決策

### プライベートヘッダーの問題

Libinfoのソースコード中でインクルードしている「存在しないプライベートヘッダ」について以下の対応を行っています。

- os/feature_private.h: Torrekie/apple_internal_sdk から拝借。
- opendirectory/od_private_hmr.h: 当方にて作成。
- 他のほとんどは他のApple OSSから同名のファイルをかき集めました。
- これらを参照するように Lixinfo.xcconfig を修正しました。
- どうしても見つからないヘッダファイルはソースコード中でコメントアウトしました。
  - それに起因するビルドエラーはソースコードを改変して対処しました。

## 注意事項

⚠️ **重要**: このライブラリはシステムの `libsystem_info.dylib` を置き換えることを意図していません。

- 学習・研究・デバッグ目的でのみ使用してください
- スタブヘッダーは完全な実装ではなく、一部の機能は動作しない可能性があります
- 実際の macOS システムでは Apple 純正の libinfo が使用されます

## 既知の問題
- `-Wdeprecated-declarations` によりいくつかの関数にワーニングが出ます。気にしないでください。
- xcodebuildする度に `xcodescripts/install_files.sh` が毎回実行されます。このため毎回 /tmp/Libinfo.dst/ にヘッダファイルがコピーされます。実害はないので放置しています。
- 前項の問題と同じ原因で、`xcodebuild clean` 時に /tmp/Libinfo.dst/ が消せずにエラーとなります。その他のビルド生成物は削除され、実害はないので放置しています。

## ライセンス

- Apple OSSは `Apple Public Source License(APSL)` でライセンスされています。
  - APSLにはバージョン 1.0, 1.1, 1.2, 2.0とあり、内容がまあまあ違います。
  - ソフトウェアごとにどのバージョンを使用しているのか異なります。
- apple_internal_sdk のライセンスは明記されていません。

## 関連リソース

- [Apple Open Source](https://opensource.apple.com/source/)
- [Apple OSS Distributions](https://github.com/apple-oss-distributions)
- [Torrekie/apple_internal_sdk](https://github.com/Torrekie/apple_internal_sdk)
- [showxu/objc4](https://github.com/showxu/objc4)
